#include "koala-object.hpp"
#include "glsl-frag-object.hpp"
#include "gltf-object.hpp"
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

using namespace Koala;

const std::string BaseObject::metaIgnore[] = {
    "uuid", "path", "name", "source", "tags", "parser", "size", "md5sum"};

std::shared_ptr<BaseObject> BaseObject::CreateObject(
    const std::string rootDir,
    rapidjson::GenericObject<false, rapidjson::Value::ValueType> props) {
  std::shared_ptr<BaseObject> newObject;

  const std::string parser = props["parser"].GetString();

  if (parser == "gltf") {
    newObject = std::dynamic_pointer_cast<BaseObject>(
        std::make_shared<GLTFObject>(props, rootDir));
  }
  if (parser == "glsl-frag") {
    newObject = std::dynamic_pointer_cast<BaseObject>(
        std::make_shared<GLSLFragObject>(props, rootDir));
  }

  if (newObject.operator bool()) {
    std::scoped_lock<std::mutex> lock(newObject->tagsLock);
    const std::string tagStringRaw = props["tags"].GetString();
    boost::split(newObject->tags, tagStringRaw,
                 [](char c) { return c == ' '; });
  }

  return newObject;
}

BaseObject::BaseObject(
    rapidjson::GenericObject<false, rapidjson::Value::ValueType> props,
    const std::string root)
    : uuid(props["uuid"].GetString()), path(props["path"].GetString()),
      name(props["name"].GetString()), parser(props["parser"].GetString()),
      size(props["size"].GetUint()), md5Sum(props["hash"].GetString()),
      rootDir(root), parsed(false), data(nullptr),
      logger("Object" + path + "/" + name, DebugLogger::DebugColor::COLOR_GREEN,
             false) {
  logger.Info("Created Object [%s] ==> [%s]", uuid.c_str(), parser.c_str());
}

BaseObject::~BaseObject() {
  logger.Info("Destoryed Object [%s] ==> [%s]", uuid.c_str(), parser.c_str());
}

[[nodiscard]] const std::string BaseObject::GetUUID() const noexcept {
  return uuid;
}
[[nodiscard]] const std::string BaseObject::GetPath() const noexcept {
  return path;
}
[[nodiscard]] const std::string BaseObject::GetName() const noexcept {
  return name;
}
[[nodiscard]] const std::string BaseObject::GetParser() const noexcept {
  return parser;
}

void BaseObject::AddTag(const std::string tag) noexcept {
  std::scoped_lock<std::mutex> lock(tagsLock);
  if (std::find(tags.begin(), tags.end(), tag) == tags.end()) {
    tags.push_back(tag);
  }
}

[[nodiscard]] bool BaseObject::DeleteTag(const std::string tag) noexcept {
  std::scoped_lock<std::mutex> lock(tagsLock);
  auto it = std::find(tags.begin(), tags.end(), tag);
  if (it != tags.end()) {
    tags.erase(it);
    return true;
  }
  return false;
}

[[nodiscard]] bool BaseObject::HasTag(const std::string tag) const noexcept {
  std::scoped_lock<std::mutex> lock(tagsLock);
  return std::find(tags.begin(), tags.end(), tag) != tags.end();
}

void BaseObject::Load() {
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
                 uuid.c_str(), size, strerror(errno));
    return;
  }

  const std::string fullPath = rootDir + "/" + uuid;

  auto fd = open(fullPath.c_str(), O_RDONLY);

  if (fd == -1) {
    logger.Error("Failed to load : Unable to open [%s] ==> [%s]", uuid.c_str(),
                 strerror(errno));
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

[[nodiscard]] bool BaseObject::IsParsed() const noexcept { return parsed; }

void BaseObject::Unload() {
  std::scoped_lock<std::mutex> lock(loadLock);
  if (data != nullptr) {
    free(data);
    data = nullptr;
    logger.Info("Unloaded!");
  } else {
    logger.Warning("Failed to unload : not loaded");
  }
}

[[nodiscard]] bool BaseObject::Parse() noexcept {
  auto start = std::chrono::system_clock::now();

  logger.Info("Parsing complete [%d] us",
              std::chrono::duration_cast<std::chrono::microseconds>(
                  std::chrono::system_clock::now() - start)
                  .count());
  return true;
}

[[nodiscard]] bool BaseObject::IsLoaded() const noexcept {
  std::scoped_lock<std::mutex> lock(loadLock);
  return data != nullptr;
}

[[nodiscard]] const uint8_t *BaseObject::GetData() const noexcept {
  return static_cast<const uint8_t *>(data);
}
