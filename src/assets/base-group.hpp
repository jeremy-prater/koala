#pragma once

#include "engine/classes.hpp"

#include "base-asset.hpp"
#include "debuglogger/debuglogger.hpp"
#include "project-configuration.hpp"
#include <boost/container_hash/hash.hpp>
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
  const boost::uuids::uuid parentUUID;
  const std::string name;

  std::shared_ptr<BaseAsset> parent;

  [[nodiscard]] static std::shared_ptr<BaseGroup> CreateGroup(
      Project *project,
      rapidjson::GenericObject<false, rapidjson::Value::ValueType> props);

  [[nodiscard]] const boost::uuids::uuid GetUUID() const noexcept;
  [[nodiscard]] const std::string GetPath() const noexcept;

  [[nodiscard]] const std::string GetParentPath() const noexcept;
  [[nodiscard]] const boost::uuids::uuid GetParentUUID() const noexcept;

  [[nodiscard]] std::shared_ptr<Koala::Objects::SceneRenderableGroup>
  GetNodeRenderGroup(const std::string nodeName) const noexcept;

  [[nodiscard]] const std::vector<const boost::uuids::uuid> GetNodeUUIDs();
  [[nodiscard]] const std::unordered_map<
      BaseGroup::NodeType, std::shared_ptr<Koala::Assets::BaseAsset>>
  GetNodeAssets();
  [[nodiscard]] NodeType
  ConvertStringToNodeType(const std::string &nodeTypeName) noexcept;

  BaseGroup(Project *project,
            rapidjson::GenericObject<false, rapidjson::Value::ValueType> props);

protected:
  std::mutex nodeMutex;
  std::unordered_map<std::string,
                     std::shared_ptr<Koala::Objects::SceneRenderableGroup>>
      renderGroupNodes;
  std::unordered_map<BaseGroup::NodeType,
                     std::shared_ptr<Koala::Assets::BaseAsset>>
      nodeElementsMap;
  DebugLogger logger;
};

} // namespace Assets
} // namespace Koala