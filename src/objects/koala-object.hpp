#include "debuglogger.hpp"
#include <mutex>
#include <rapidjson/pointer.h>
#include <unordered_map>
#include <vector>

using namespace Magnum;

namespace Koala {

class BaseObject {
public:
  static std::shared_ptr<BaseObject> CreateObject(rapidjson::Pointer props);

  [[nodiscard]] const std::string GetParser() const noexcept;

  void AddTag(const std::string tag) noexcept;
  [[nodiscard]] bool DeleteTag(const std::string tag) noexcept;
  [[nodiscard]] bool HasTag(const std::string tag) const noexcept;

  void Load();
  void Unload();
  [[nodiscard]] bool IsLoaded() const noexcept;
  [[nodiscard]] const uint8_t const *GetData() const noexcept;

private:
  BaseObject();
  ~BaseObject();

private:
  static const std::string metaIgnore[];

  const std::string uuid;
  const std::string path;
  const std::string name;
  const std::string parser;
  const int32_t size;
  const std::string md5;

  std::mutex loadLock;
  bool loaded;
  uint8_t *data;

  std::mutex tagsLock;
  std::vector<std::string> tags;

  std::mutex metaObjectLock;
  std::unordered_map<std::string, std::string> metaObjects;
};
} // namespace Koala