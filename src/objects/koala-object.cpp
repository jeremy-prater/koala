#include "koala-object.hpp"

using namespace Koala;

const std::string BaseObject::metaIgnore[] = {
    "uuid", "path", "name", "source", "tags", "parser", "size", "md5sum"};

std::shared_ptr<BaseObject> BaseObject::CreateObject(rapidjson::Pointer props) {
  if (!props.IsValid()) {
    return std::shared_ptr<BaseObject>();
  }

  const size_t totalProps = props.GetTokenCount();
  const auto props = props.GetTokens();

  for (size_t currentProp = 0; currentProp < totalProps; currentProp++) {
    const auto prop = props[currentProp];
  }
}

BaseObject::BaseObject() {}
BaseObject::~BaseObject() {}

[[nodiscard]] const std::string BaseObject::GetParser() const noexcept {}

void BaseObject::AddTag(const std::string tag) noexcept {}

[[nodiscard]] bool BaseObject::DeleteTag(const std::string tag) noexcept {}
[[nodiscard]] bool BaseObject::HasTag(const std::string tag) const noexcept {}

void BaseObject::Load() {}
void BaseObject::Unload() {}
[[nodiscard]] bool BaseObject::IsLoaded() const noexcept {}
[[nodiscard]] const uint8_t const *BaseObject::GetData() const noexcept {}
