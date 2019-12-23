#include "debuglogger.hpp"
#include "koala-object.hpp"
#include "rapidjson/document.h"
#include <mutex>
#include <unordered_map>
#include <memory>

namespace Koala {

class Project {
public:
  Project(const std::string path,
          const std::string defaultConfigFile = "projectConfig.json");

  [[nodiscard]] std::vector<std::string> GetObjectUUIDs() const noexcept;

  [[nodiscard]] std::shared_ptr<BaseObject> GetObject(std::string uuid) const
      noexcept;

private:
  const std::string rootDir;
  const std::string configFile;

  rapidjson::Document configDocument;

  mutable std::mutex objectsMutex;
  std::unordered_map<std::string, std::shared_ptr<BaseObject>> objects;

  DebugLogger logger;
};
} // namespace Koala
