#include "koala-object.hpp"
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

  DebugLogger logger("BaseObject::CreateObject",
                     DebugLogger::DebugColor::COLOR_RED, false);

  const std::string uuid = props["uuid"].GetString();
  const std::string path = props["path"].GetString();
  const std::string name = props["name"].GetString();
  const std::string parser = props["parser"].GetString();
  const size_t size = props["size"].GetUint();
  const std::string md5Sum = props["hash"].GetString();

  std::shared_ptr<BaseObject> newObject = std::make_shared<BaseObject>(
      uuid, path, name, parser, size, md5Sum, rootDir);

  {
    std::scoped_lock<std::mutex> lock(newObject->tagsLock);
    const std::string tagStringRaw = props["tags"].GetString();
    boost::split(newObject->tags, tagStringRaw,
                 [](char c) { return c == ' '; });
  }

  return newObject;
}

BaseObject::BaseObject(const std::string newUuid, const std::string newPath,
                       const std::string newName, const std::string newParser,
                       const size_t newSize, const std::string newMD5,
                       const std::string newRootDir)
    : uuid(newUuid), path(newPath), name(newName), parser(newParser),
      size(newSize), md5(newMD5), rootDir(newRootDir),
      logger("Object" + path + "/" + name, DebugLogger::DebugColor::COLOR_GREEN,
             false) {
  logger.Info("Created Object [%s] ==> [%s]", uuid.c_str(), parser.c_str());
}

BaseObject::~BaseObject() {}

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
  std::scoped_lock<std::mutex> lock(loadLock);
  if (data == nullptr) {
    data = static_cast<uint8_t *>(malloc(size));
  }

  if (data == nullptr) {
    logger.Error("Unable allocate memory [%s][%d] ==> [%s]", uuid.c_str(), size,
                 strerror(errno));
    return;
  }

  const std::string fullPath = rootDir + "/" + uuid;

  auto fd = open(fullPath.c_str(), O_RDONLY);

  logger.Info("[%s] opened ==> %d", fullPath.c_str(), fd);

  if (fd == -1) {
    logger.Error("Unable to open [%s] ==> [%s]", uuid.c_str(), strerror(errno));
    free(data);
    data = nullptr;
    return;
  }

  auto readSize = read(fd, data, size);

  if (readSize == -1) {
    logger.Warning("Failed to read [%s] ==> [%s]", fullPath.c_str(),
                   strerror(errno));
  } else if (readSize != size) {
    logger.Warning("Read size mis-match [%d] != [%d]", readSize, size);
  }

  close(fd);
}

void BaseObject::Unload() {
  std::scoped_lock<std::mutex> lock(loadLock);
  if (data != nullptr) {
    free(data);
    data = nullptr;
  }
}

[[nodiscard]] bool BaseObject::IsLoaded() const noexcept {
  std::scoped_lock<std::mutex> lock(loadLock);
  return data != nullptr;
}

[[nodiscard]] const uint8_t *const BaseObject::GetData() const noexcept {
  return static_cast<const uint8_t *const>(data);
}
