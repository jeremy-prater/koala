#pragma once

#include "base-object.hpp"
#include "debuglogger.hpp"
#include "rapidjson/document.h"
#include <memory>
#include <mutex>
#include <unordered_map>

namespace Koala {

class Project {
public:
  Project(const std::string path,
          const std::string defaultConfigFile = "projectConfig.json");

  [[nodiscard]] std::vector<std::string> GetObjectUUIDs() const noexcept;

  [[nodiscard]] std::shared_ptr<BaseObject>
  GetObject(const std::string uuid) const noexcept;
  [[nodiscard]] std::shared_ptr<BaseObject>
  GetObjectByPath(const std::string path) const noexcept;

private:
  const std::string rootDir;
  const std::string configFile;

  rapidjson::Document configDocument;

  mutable std::mutex objectsMutex;
  std::unordered_map<std::string, std::shared_ptr<BaseObject>> objects;
  std::unordered_map<std::string, std::shared_ptr<BaseObject>> objectsByPath;

  DebugLogger logger;
};
} // namespace Koala
