#pragma once

#include "base-asset.hpp"
#include "base-group.hpp"
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

  [[nodiscard]] std::vector<std::string> GetAssetUUIDs() const noexcept;
  [[nodiscard]] std::shared_ptr<BaseAsset>
  GetAsset(const std::string &uuid) const noexcept;
  [[nodiscard]] std::shared_ptr<BaseAsset>
  GetAssetByPath(const std::string &path) const noexcept;

  [[nodiscard]] std::vector<std::string> GetGroupUUIDs() const noexcept;
  [[nodiscard]] std::shared_ptr<BaseGroup>
  GetGroup(const std::string &uuid) const noexcept;
  [[nodiscard]] std::shared_ptr<BaseGroup>
  GetGroupByPath(const std::string &fullPath) const noexcept;

private:
  const std::string rootDir;
  const std::string configFile;

  rapidjson::Document configDocument;

  mutable std::mutex assetsMutex;
  std::unordered_map<std::string, std::shared_ptr<BaseAsset>> assets;
  std::unordered_map<std::string, std::shared_ptr<BaseAsset>> assetsByPath;

  mutable std::mutex groupsMutex;
  std::unordered_map<std::string, std::shared_ptr<BaseGroup>> groups;
  std::unordered_map<std::string, std::shared_ptr<BaseGroup>> groupsByPath;

  DebugLogger logger;
};

} // namespace Assets
} // namespace Koala
