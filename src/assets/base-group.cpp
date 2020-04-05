#include "base-group.hpp"
#include "debuglogger/debuglogger.hpp"
#include <boost/algorithm/string.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <engine/engine.hpp>

using namespace Koala::Assets;

std::shared_ptr<BaseGroup> BaseGroup::CreateGroup(
    Project *project,
    rapidjson::GenericObject<false, rapidjson::Value::ValueType> props) {
  std::shared_ptr<BaseGroup> newGroup;

  //   if (parent->GetParser() == "gltf") {
  //     newGroup = std::make_shared<BaseGroup>(project, props);
  //   }

  newGroup = std::make_shared<BaseGroup>(project, props);

  return newGroup;
}

BaseGroup::BaseGroup(
    Project *project,
    rapidjson::GenericObject<false, rapidjson::Value::ValueType> props)
    : uuid(Koala::Engine::Engine::StringUUIDGenerator(
          props["uuid"].GetString())),
      parentPath(props["parentPath"].GetString()),
      name(props["name"].GetString()),
      parent(project->GetAssetByPath(parentPath)),
      nodeTypeHash(static_cast<uint32_t>(NodeType::Unknown)),
      logger("Group : " + boost::uuids::to_string(uuid),
             DebugLogger::DebugColor::COLOR_CYAN, true) {
  logger.Info("Created Group [%s][%s]", GetPath().c_str(),
              boost::uuids::to_string(uuid).c_str());

  auto nodes = props["nodes"].GetObject();

  for (auto &node : nodes) {
    const std::string nodeName =
        boost::algorithm::to_lower_copy(node.name.GetString());

    NodeType nodeType = NodeType::Unknown;

    if (nodeName == "vertex") {
      nodeType = NodeType::VertexShader;
    } else if (nodeName == "fragment") {
      nodeType = NodeType::FragmentShader;
    }

    nodeTypeHash |= static_cast<uint32_t>(nodeType);

    // logger.Info("Loading Node --> %s", nodeName.c_str());

    auto nodeDataElements = node.value.GetArray();
    for (auto &nodeDataElement : nodeDataElements) {
      auto nodeObject = nodeDataElement.GetObject();
      const std::string type = nodeObject["type"].GetString();
      const std::string asset = nodeObject["asset"].GetString();
      logger.Info("Linking Node --> %s [%s] ==> [%s]", nodeName.c_str(),
                  type.c_str(), asset.c_str());

      // Optimize me...
      //
      // 1. First nodeTypeHash is a bitmap
      // 2. The local string type is possible an element of nodeTypeHash

      // this->nodes[nodeName][type] = project->GetAssetByPath(asset);
    }
  }
}

[[nodiscard]] const boost::uuids::uuid BaseGroup::GetUUID() const noexcept {
  return uuid;
}

[[nodiscard]] const std::string BaseGroup::GetPath() const noexcept {
  return parentPath + "/" + name;
}

[[nodiscard]] const std::string BaseGroup::GetParentPath() const noexcept {
  return parentPath;
}

[[nodiscard]] uint32_t BaseGroup::GetNodeTypeHash() const noexcept {
  return nodeTypeHash;
}

[[nodiscard]] const std::unordered_map<BaseGroup::NodeType,
                                       std::shared_ptr<BaseAsset>> &
BaseGroup::GetNodeLinks(const boost::uuids::uuid nodeUUID) const noexcept {
  return nodes.at(nodeUUID);
}