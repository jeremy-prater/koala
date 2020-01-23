#pragma once

#include "base-asset.hpp"
#include "debuglogger/debuglogger.hpp"
#include "rapidjson/document.h"
#include <memory>
#include <mutex>
#include <unordered_map>

namespace Koala {
namespace Assets {

class Project {
public:
  Project(const std::string &path,
          const std::string &defaultConfigFile = "projectConfig.json");

  [[nodiscard]] std::vector<std::string> GetObjectUUIDs() const noexcept;

  [[nodiscard]] std::shared_ptr<BaseAsset>
  GetObject(const std::string &uuid) const noexcept;
  [[nodiscard]] std::shared_ptr<BaseAsset>
  GetObjectByPath(const std::string &path) const noexcept;

private:
  const std::string rootDir;
  const std::string configFile;

  rapidjson::Document configDocument;

  mutable std::mutex objectsMutex;
  std::unordered_map<std::string, std::shared_ptr<BaseAsset>> objects;
  std::unordered_map<std::string, std::shared_ptr<BaseAsset>> objectsByPath;

  DebugLogger logger;
};

} // namespace Assets
} // namespace Koala
