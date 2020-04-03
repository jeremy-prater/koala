#include "base-group.hpp"
#include "debuglogger/debuglogger.hpp"

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
    : uuid(props["uuid"].GetString()),
      parentPath(props["parentPath"].GetString()),
      name(props["name"].GetString()),
      parent(project->GetAssetByPath(parentPath)),
      logger("Group : " + uuid, DebugLogger::DebugColor::COLOR_CYAN, true) {
  logger.Info("Created Group [%s][%s]", GetPath().c_str(), uuid.c_str());

  auto nodes = props["nodes"].GetObject();

  for (auto &node : nodes) {
    std::string nodeName = node.name.GetString();
    nodeNames.push_back(nodeName);

    // logger.Info("Loading Node --> %s", nodeName.c_str());

    auto nodeDataElements = node.value.GetArray();
    for (auto &nodeDataElement : nodeDataElements) {
      auto nodeObject = nodeDataElement.GetObject();
      const std::string type = nodeObject["type"].GetString();
      const std::string asset = nodeObject["asset"].GetString();
      logger.Info("Linking Node --> %s [%s] ==> [%s]", nodeName.c_str(),
                  type.c_str(), asset.c_str());
      // nodeDataElement.name
      this->nodes[nodeName][type] = project->GetAssetByPath(asset);
    }
  }
}

[[nodiscard]] const std::string BaseGroup::GetUUID() const noexcept {
  return uuid;
}

[[nodiscard]] const std::string BaseGroup::GetPath() const noexcept {
  return parentPath + "/" + name;
}

[[nodiscard]] const std::string BaseGroup::GetParentPath() const noexcept {
  return parentPath;
}

[[nodiscard]] const std::vector<std::string> BaseGroup::GetNodeList() const
    noexcept {
  return nodeNames;
}

[[nodiscard]] const std::unordered_map<std::string, std::shared_ptr<BaseAsset>>
BaseGroup::GetNodeLinks(const std::string &nodeName) const noexcept {
  return nodes.at(nodeName);
}
