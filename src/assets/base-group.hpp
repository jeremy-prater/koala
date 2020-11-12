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
    Texture0 = 0x00010000,
    Texture1 = 0x00010001,
    Texture2 = 0x00010002,
    Texture3 = 0x00010003,
    Texture4 = 0x00010004,
    Texture5 = 0x00010005,
    Texture6 = 0x00010006,
    Texture7 = 0x00010007,
    Texture8 = 0x00010008,
    Texture9 = 0x00010009,
    Texture10 = 0x0001000a,
    Texture11 = 0x0001000b,
    Texture12 = 0x0001000c,
    Texture13 = 0x0001000d,
    Texture14 = 0x0001000e,
    Texture15 = 0x0001000f,
    Texture16 = 0x00010010,
    Texture17 = 0x00010011,
    Texture18 = 0x00010012,
    Texture19 = 0x00010013,
    Texture20 = 0x00010014,
    Texture21 = 0x00010015,
    Texture22 = 0x00010016,
    Texture23 = 0x00010017,
    Texture24 = 0x00010018,
    Texture25 = 0x00010019,
    Texture26 = 0x0001001a,
    Texture27 = 0x0001001b,
    Texture28 = 0x0001001c,
    Texture29 = 0x0001001d,
    Texture30 = 0x0001001e,
    Texture31 = 0x0001001f,

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
  GetNodeRenderGroup(const std::string &nodeName) const noexcept;

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