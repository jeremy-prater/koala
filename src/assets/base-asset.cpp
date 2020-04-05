#include "base-asset.hpp"
#include "engine/engine.hpp"
#include "glsl-asset.hpp"
#include "gltf-asset.hpp"
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

using namespace Koala::Assets;

std::shared_ptr<BaseAsset> BaseAsset::CreateAsset(
    const std::string &rootDir,
    rapidjson::GenericObject<false, rapidjson::Value::ValueType> props) {
  std::shared_ptr<BaseAsset> newAsset;

  const std::string parser = props["parser"].GetString();

  if (newAsset.operator bool()) {
    std::scoped_lock<std::mutex> lock(newAsset->tagsLock);
    const std::string tagStringRaw = props["tags"].GetString();
    boost::split(newAsset->tags, tagStringRaw, [](char c) { return c == ' '; });
  }

  if (parser == "gltf") {
    newAsset = std::dynamic_pointer_cast<BaseAsset>(
        std::make_shared<GLTFAsset>(props, rootDir));
  }

  if (parser == "glsl") {
    newAsset = std::dynamic_pointer_cast<BaseAsset>(
        std::make_shared<GLSLAsset>(props, rootDir));
  }

  return newAsset;
}

BaseAsset::BaseAsset(
    rapidjson::GenericObject<false, rapidjson::Value::ValueType> props,
    const std::string &root)
    : uuid(Koala::Engine::Engine::StringUUIDGenerator(
          props["uuid"].GetString())),
      path(props["path"].GetString()), name(props["name"].GetString()),
      fullPath(path + "/" + name), parser(props["parser"].GetString()),
      size(props["size"].GetUint()), md5Sum(props["hash"].GetString()),
      rootDir(root), parsed(false), data(nullptr),
      logger("Asset : " + fullPath, DebugLogger::DebugColor::COLOR_GREEN,
             false) {
  logger.Info("Created Asset [%s] ==> [%s]",

              boost::uuids::to_string(uuid).c_str(), parser.c_str());
  for (auto &value : props["metadata"].GetObject()) {
    std::string metaName = value.name.GetString();
    std::string metaValue = value.value.GetString();
    metaObjects[metaName] = metaValue;
    logger.Info("Setting asset metadata [%s] ==> [%s]", metaName.c_str(),
                metaValue.c_str());
  }
}

BaseAsset::~BaseAsset() {
  logger.Info("Destroyed Asset [%s] ==> [%s]",
              boost::uuids::to_string(uuid).c_str(), parser.c_str());
}

[[nodiscard]] const boost::uuids::uuid BaseAsset::GetUUID() const noexcept {
  return uuid;
}
[[nodiscard]] const std::string BaseAsset::GetPath() const noexcept {
  return path;
}
[[nodiscard]] const std::string BaseAsset::GetFullPath() const noexcept {
  return fullPath;
}
[[nodiscard]] const std::string BaseAsset::GetName() const noexcept {
  return name;
}
[[nodiscard]] const std::string BaseAsset::GetParser() const noexcept {
  return parser;
}

void BaseAsset::AddTag(const std::string &tag) noexcept {
  std::scoped_lock<std::mutex> lock(tagsLock);
  if (std::find(tags.begin(), tags.end(), tag) == tags.end()) {
    tags.push_back(tag);
  }
}

[[nodiscard]] bool BaseAsset::DeleteTag(const std::string &tag) noexcept {
  std::scoped_lock<std::mutex> lock(tagsLock);
  auto it = std::find(tags.begin(), tags.end(), tag);
  if (it != tags.end()) {
    tags.erase(it);
    return true;
  }
  return false;
}

[[nodiscard]] bool BaseAsset::HasTag(const std::string &tag) const noexcept {
  std::scoped_lock<std::mutex> lock(tagsLock);
  return std::find(tags.begin(), tags.end(), tag) != tags.end();
}

void BaseAsset::Load() {
  auto start = std::chrono::system_clock::now();

  std::scoped_lock<std::mutex> lock(loadLock);

  if (data == nullptr) {
    data = static_cast<uint8_t *>(malloc(size));
  } else {
    logger.Warning("Failed to load : already loaded");
    return;
  }

  if (data == nullptr) {
    logger.Error("Failed to load : Unable allocate memory [%s][%d] ==> [%s]",
                 boost::uuids::to_string(uuid).c_str(), size, strerror(errno));
    return;
  }

  const std::string fullPath = rootDir + "/" + boost::uuids::to_string(uuid);

  auto fd = open(fullPath.c_str(), O_RDONLY);

  if (fd == -1) {
    logger.Error("Failed to load : Unable to open [%s] ==> [%s]",
                 boost::uuids::to_string(uuid).c_str(), strerror(errno));
    free(data);
    data = nullptr;
    return;
  }

  auto readSize = read(fd, data, size);

  if (readSize == -1) {
    logger.Warning("Failed to load : Failed to read [%s] ==> [%s]",
                   fullPath.c_str(), strerror(errno));
  } else if (readSize != size) {
    logger.Warning("Failed to load : Read size mis-match [%d] != [%d]",
                   readSize, size);
  }

  close(fd);

  logger.Info("Loaded! [%d] us",
              std::chrono::duration_cast<std::chrono::microseconds>(
                  std::chrono::system_clock::now() - start)
                  .count());
}

[[nodiscard]] bool BaseAsset::IsParsed() const noexcept { return parsed; }

void BaseAsset::Unload() {
  std::scoped_lock<std::mutex> lock(loadLock);
  if (data != nullptr) {
    free(data);
    data = nullptr;
    logger.Info("Unloaded!");
  } else {
    logger.Warning("Failed to unload : not loaded");
  }
}

[[nodiscard]] bool BaseAsset::Parse() noexcept {
  auto start = std::chrono::system_clock::now();

  logger.Info("Parsing complete [%d] us",
              std::chrono::duration_cast<std::chrono::microseconds>(
                  std::chrono::system_clock::now() - start)
                  .count());
  return true;
}

[[nodiscard]] bool BaseAsset::IsLoaded() const noexcept {
  std::scoped_lock<std::mutex> lock(loadLock);
  return data != nullptr;
}

[[nodiscard]] const uint8_t *BaseAsset::GetData() const noexcept {
  return static_cast<const uint8_t *>(data);
}

[[nodiscard]] const std::string &
BaseAsset::GetMetaObject(const std::string &key) const noexcept {
  static const std::string emptyMetaObject = "";

  std::scoped_lock<std::mutex> lock(metaObjectLock);
  auto it = metaObjects.find(key);
  if (it == metaObjects.end()) {
    logger.Warning("No asset metadata key! [%s]", key.c_str());
    return emptyMetaObject;
  }
  return it->second;
}
void BaseAsset::SetMetaObject(const std::string &key,
                              const std::string &value) noexcept {
  std::scoped_lock<std::mutex> lock(metaObjectLock);
  metaObjects[key] = value;
}
