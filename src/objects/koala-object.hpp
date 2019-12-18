#include "debuglogger.hpp"
#include <memory>
#include <mutex>
#include <rapidjson/pointer.h>
#include <unordered_map>
#include <vector>

namespace Koala {

class BaseObject {
public:
  static std::shared_ptr<BaseObject> CreateObject(rapidjson::Pointer props);

  [[nodiscard]] const std::string GetUUID() const noexcept;
  [[nodiscard]] const std::string GetPath() const noexcept;
  [[nodiscard]] const std::string GetName() const noexcept;
  [[nodiscard]] const std::string GetParser() const noexcept;

  void AddTag(const std::string tag) noexcept;
  [[nodiscard]] bool DeleteTag(const std::string tag) noexcept;
  [[nodiscard]] bool HasTag(const std::string tag) const noexcept;

  void Load();
  void Unload();
  [[nodiscard]] bool IsLoaded() const noexcept;
  [[nodiscard]] const uint8_t *const GetData() const noexcept;

  BaseObject(const std::string newUuid, const std::string newPath,
             const std::string newName, const std::string newParser,
             const size_t newSize, const std::string newMD5);
  ~BaseObject();

private:
  static const std::string metaIgnore[];

  const std::string uuid;
  const std::string path;
  const std::string name;
  const std::string parser;
  const size_t size;
  const std::string md5;

  mutable std::mutex loadLock;
  uint8_t *data;

  mutable std::mutex tagsLock;
  std::vector<std::string> tags;

  mutable std::mutex metaObjectLock;
  std::unordered_map<std::string, std::string> metaObjects;

  DebugLogger logger;
};
} // namespace Koala