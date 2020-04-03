#pragma once

#include "base-asset.hpp"
#include "debuglogger/debuglogger.hpp"
#include "project-configuration.hpp"
#include <memory>
#include <mutex>
#include <rapidjson/pointer.h>
#include <rapidjson/rapidjson.h>
#include <string>
#include <unordered_map>

namespace Koala {
namespace Assets {

class Project;

class BaseGroup {
public:
  const std::string uuid;
  const std::string parentPath;
  const std::string name;

  std::shared_ptr<BaseAsset> parent;

  static std::shared_ptr<BaseGroup> CreateGroup(
      Project *project,
      rapidjson::GenericObject<false, rapidjson::Value::ValueType> props);

  [[nodiscard]] const std::string GetUUID() const noexcept;
  [[nodiscard]] const std::string GetPath() const noexcept;
  [[nodiscard]] const std::string GetParentPath() const noexcept;
  [[nodiscard]] const std::vector<std::string> &GetNodeList() const noexcept;
  [[nodiscard]] const std::unordered_map<std::string,
                                         std::shared_ptr<BaseAsset>> &
  GetNodeLinks(const std::string &nodeName) const noexcept;

  BaseGroup(Project *project,
            rapidjson::GenericObject<false, rapidjson::Value::ValueType> props);

protected:
  std::mutex nodeMutex;
  std::vector<std::string> nodeNames;
  std::unordered_map<
      std::string, std::unordered_map<std::string, std::shared_ptr<BaseAsset>>>
      nodes;
  DebugLogger logger;
};

} // namespace Assets
} // namespace Koala