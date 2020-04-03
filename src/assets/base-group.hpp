#pragma once

#include "base-asset.hpp"
#include "debuglogger/debuglogger.hpp"
#include "project-configuration.hpp"
#include <boost/uuid/uuid.hpp>
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
  enum class NodeType : uint32_t {
    Unknown = 0x00000000,
    VertexShader = 0x00000001,
    FragmentShader = 0x00000002,
  };

  const boost::uuids::uuid uuid;
  const std::string parentPath;
  const std::string name;

  std::shared_ptr<BaseAsset> parent;

  static std::shared_ptr<BaseGroup> CreateGroup(
      Project *project,
      rapidjson::GenericObject<false, rapidjson::Value::ValueType> props);

  [[nodiscard]] const boost::uuids::uuid GetUUID() const noexcept;
  [[nodiscard]] const std::string GetPath() const noexcept;
  [[nodiscard]] const std::string GetParentPath() const noexcept;
  [[nodiscard]] uint32_t GetNodeTypeHash() const noexcept;
  [[nodiscard]] const std::unordered_map<NodeType, std::shared_ptr<BaseAsset>> &
  GetNodeLinks(const std::string &nodeName) const noexcept;

  BaseGroup(Project *project,
            rapidjson::GenericObject<false, rapidjson::Value::ValueType> props);

protected:
  std::mutex nodeMutex;
  uint32_t nodeTypeHash;
  std::unordered_map<
      boost::uuids::uuid,
      std::unordered_map<std::string, std::shared_ptr<BaseAsset>>>
      nodes;
  DebugLogger logger;
};

} // namespace Assets
} // namespace Koala