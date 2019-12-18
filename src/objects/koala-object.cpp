#include "koala-object.hpp"

using namespace Koala;

const std::string BaseObject::metaIgnore[] = {
    "uuid", "path", "name", "source", "tags", "parser", "size", "md5sum"};

std::shared_ptr<BaseObject> BaseObject::CreateObject(rapidjson::Pointer props) {
  if (!props.IsValid()) {
    return std::shared_ptr<BaseObject>();
  }

  std::shared_ptr<BaseObject> newObject = std::make_shared<BaseObject>();

  const size_t totalProps = props.GetTokenCount();
  const auto tokens = props.GetTokens();

  for (size_t currentProp = 0; currentProp < totalProps; currentProp++) {
    const auto token = tokens[currentProp];
    newObject->logger.Info("Token : %s", token.name);
  }
}

BaseObject::BaseObject(const std::string newUuid, const std::string newPath,
                       const std::string newName, const std::string newParser,
                       const size_t newSize, const std::string newMD5)
    : uuid(newUuid), path(newPath), name(newName), parser(newParser),
      size(newSize), md5(newMD5),
      logger("Object" + path + "/" + name, DebugLogger::DebugColor::COLOR_GREEN,
             false) {
  logger.Info("Created Object");
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
  }
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

[[nodiscard]] const uint8_t const *BaseObject::GetData() const noexcept {
  return static_cast<const uint8_t const *>(data);
}
