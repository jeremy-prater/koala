#include "base-group.hpp"
#include "debuglogger/debuglogger.hpp"
#include "glsl-asset.hpp"
#include "objects/scene-renderable-groups.hpp"
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

[[nodiscard]] BaseGroup::NodeType
BaseGroup::ConvertStringToNodeType(const std::string &nodeTypeName) noexcept {
  const std::string nodeName =
      boost::algorithm::to_lower_copy<const std::string>(nodeTypeName);

  if (nodeName == "vertex") {
    return NodeType::VertexShader;
  } else if (nodeName == "fragment") {
    return NodeType::FragmentShader;
  } else if (nodeName.find("texture.") != std::string::npos) {
    std::vector<std::string> nodeTextureParts;
    boost::split(nodeTextureParts, nodeName, [](char c) { return c == '.'; });
    if (nodeTextureParts.size() != 2) {
      logger.Warning(
          "ConvertStringToNodeType ==> Incorrect number of texture parts [%s]",
          nodeTypeName.c_str());
      return NodeType::Unknown;
    }
    uint32_t textureID = std::stoi(nodeTextureParts[1]);
    if (textureID > GLSLAsset::GetMaxFragmentTextures()) {
      logger.Error("Texture index [%d] is greater than allowed (%d)", textureID,
                   GLSLAsset::GetMaxFragmentTextures());
      return NodeType::Unknown;
    }
    return static_cast<NodeType>(static_cast<uint32_t>(NodeType::Texture0) +
                                 textureID);
  } else {
    logger.Warning("ConvertStringToNodeType ==> Unknown type! [%s]",
                   nodeTypeName.c_str());
    return NodeType::Unknown;
  }
}

BaseGroup::BaseGroup(
    Project *project,
    rapidjson::GenericObject<false, rapidjson::Value::ValueType> props)
    : uuid(Koala::Engine::Engine::StringUUIDGenerator(
          props["uuid"].GetString())),
      parentPath(props["parentPath"].GetString()),
      parentUUID(project->GetAssetByPath(parentPath)->GetUUID()),
      name(props["name"].GetString()),
      parent(project->GetAssetByPath(parentPath)),
      logger("Group : " + boost::uuids::to_string(uuid),
             DebugLogger::DebugColor::COLOR_CYAN, true) {
  logger.Info("Created Group [%s][%s]", GetPath().c_str(),
              boost::uuids::to_string(uuid).c_str());

  auto jsonNodes = props["nodes"].GetObject();

  for (auto &jsonNode : jsonNodes) {
    // logger.Info("Loading Node --> %s", nodeName.c_str());

    const std::string nodeName = jsonNode.name.GetString();
    auto nodeDataElements = jsonNode.value.GetArray();
    for (auto &nodeDataElement : nodeDataElements) {
      auto nodeObject = nodeDataElement.GetObject();

      const std::string type = nodeObject["type"].GetString();
      const std::string assetPath = nodeObject["asset"].GetString();

      auto asset = project->GetAssetByPath(assetPath);
      if (asset.operator bool() == true) {
        nodeElementsMap[ConvertStringToNodeType(type)] = asset;
      } else {
        logger.Warning("BaseGroup ==> Unable to find asset [%s] for group [%s]",
                       assetPath.c_str(), nodeName.c_str());
      }
    }

    std::shared_ptr<Koala::Objects::SceneRenderableGroup> renderGroup =
        Koala::Objects::SceneRenderableGroup::GetRenderGroupByAssetSet(
            nodeElementsMap);

    logger.Info("Linking Node --> %s RenderGroup [%s]", nodeName.c_str(),
                boost::uuids::to_string(renderGroup->uuid).c_str());

    renderGroupNodes[nodeName] = renderGroup;
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

[[nodiscard]] const boost::uuids::uuid
BaseGroup::GetParentUUID() const noexcept {
  return parentUUID;
}

[[nodiscard]] std::shared_ptr<Koala::Objects::SceneRenderableGroup>
BaseGroup::GetNodeRenderGroup(const std::string &nodeName) const noexcept {
  return renderGroupNodes.at(nodeName);
}

[[nodiscard]] const std::unordered_map<
    BaseGroup::NodeType, std::shared_ptr<Koala::Assets::BaseAsset>>
BaseGroup::GetNodeAssets() {
  return nodeElementsMap;
}
