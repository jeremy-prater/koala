#pragma once

#include "base-asset.hpp"
#include "base-group.hpp"
#include "debuglogger/debuglogger.hpp"
#include <boost/container_hash/hash.hpp>
#include <boost/uuid/uuid.hpp>
#include <memory>
#include <mutex>
#include <rapidjson/pointer.h>
#include <rapidjson/rapidjson.h>
#include <unordered_map>

namespace Koala {
namespace Assets {

class BaseGroup;

class Project {
public:
  Project(const std::string &path,
          const std::string &defaultConfigFile = "projectConfig.json");

  [[nodiscard]] const std::vector<boost::uuids::uuid> GetAssetUUIDs() const
      noexcept;
  [[nodiscard]] std::shared_ptr<BaseAsset>
  GetAsset(const boost::uuids::uuid &uuid) const noexcept;
  [[nodiscard]] std::shared_ptr<BaseAsset>
  GetAssetByPath(const std::string &path) const noexcept;

  [[nodiscard]] const std::vector<boost::uuids::uuid> GetGroupUUIDs() const
      noexcept;
  [[nodiscard]] std::shared_ptr<BaseGroup>
  GetGroup(const boost::uuids::uuid &uuid) const noexcept;
  [[nodiscard]] std::shared_ptr<BaseGroup>
  GetGroupByPath(const std::string &fullPath) const noexcept;

private:
  const std::string rootDir;
  const std::string configFile;

  rapidjson::Document configDocument;

  mutable std::mutex assetsMutex;
  std::vector<boost::uuids::uuid> assetsUUIDs;
  std::unordered_map<boost::uuids::uuid, std::shared_ptr<BaseAsset>,
                     boost::hash<boost::uuids::uuid>>
      assets;
  std::unordered_map<std::string, std::shared_ptr<BaseAsset>> assetsByPath;

  mutable std::mutex groupsMutex;
  std::vector<boost::uuids::uuid> groupsUUIDs;
  std::unordered_map<boost::uuids::uuid, std::shared_ptr<BaseGroup>,
                     boost::hash<boost::uuids::uuid>>
      groups;
  std::unordered_map<std::string, std::shared_ptr<BaseGroup>> groupsByPath;

  DebugLogger logger;
};

} // namespace Assets
} // namespace Koala
