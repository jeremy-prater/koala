#include "objects/scene-renderable-groups.hpp"
#include "engine/engine.hpp"
#include <boost/uuid/uuid_io.hpp>

using namespace Koala::Objects;

/////////////////////////////////////////////////////////////////////
//
// Remember...  this is with-in the context of Koala::Objects::Scene
//
// A scene has 1 set of render groups...
//

/////////////////////////////////////////////////////////////////////
//
// Static Members
//

std::mutex SceneRenderableGroup::groupMappingsMutex;
Koala::DebugLogger
    SceneRenderableGroup::groupLogger("SceneRenderableGroup",
                                      Koala::DebugLogger::COLOR_YELLOW, false);
std::unordered_map<
    std::shared_ptr<SceneRenderableGroup>,
    std::unordered_map<Koala::Assets::BaseGroup::NodeType,
                       std::shared_ptr<Koala::Assets::BaseAsset>>>
    SceneRenderableGroup::groupMappings;

[[nodiscard]] bool SceneRenderableGroup::CompareAssetSets(
    const std::unordered_map<Koala::Assets::BaseGroup::NodeType,
                             std::shared_ptr<Koala::Assets::BaseAsset>> &a,
    const std::unordered_map<Koala::Assets::BaseGroup::NodeType,
                             std::shared_ptr<Koala::Assets::BaseAsset>>
        &b) noexcept {

  return a == b;
}

[[nodiscard]] std::shared_ptr<SceneRenderableGroup>
SceneRenderableGroup::GetRenderGroupByAssetSet(
    const std::unordered_map<Koala::Assets::BaseGroup::NodeType,
                             std::shared_ptr<Koala::Assets::BaseAsset>>
        &assetSet) noexcept {
  std::scoped_lock<std::mutex> lock(groupMappingsMutex);

  bool addNewGroup = true;

  std::shared_ptr<SceneRenderableGroup> sceneRenderGroupTest;
  std::unordered_map<Koala::Assets::BaseGroup::NodeType,
                     std::shared_ptr<Koala::Assets::BaseAsset>>
      assetSetTest;

  for (auto &groupMapping : groupMappings) {
    sceneRenderGroupTest = groupMapping.first;
    assetSetTest = groupMapping.second;

    if (CompareAssetSets(assetSet, assetSetTest)) {
      addNewGroup = false;
      break;
    }
  }

  // groupLogger.Info("GetRenderGroupByAssetSet addNewGroup [%s]",
  //                  DebugLogger::ConvertBoolToString(addNewGroup).c_str());

  if (addNewGroup) {
    sceneRenderGroupTest = std::make_shared<SceneRenderableGroup>(assetSet);
    groupMappings[sceneRenderGroupTest] = assetSet;
  }

  return sceneRenderGroupTest;
}

/////////////////////////////////////////////////////////////////////
//
// Instance Members
//
SceneRenderableGroup::SceneRenderableGroup(
    const std::unordered_map<Koala::Assets::BaseGroup::NodeType,
                             std::shared_ptr<Koala::Assets::BaseAsset>>
        assetTemplate)
    : uuid(Engine::Engine::RandomUUIDGenerator()), assetMap(assetTemplate),
      logger("SceneRenderableGroup-" + boost::uuids::to_string(uuid),
             DebugLogger::COLOR_YELLOW, false) {
  logger.Info("Created SceneRenderableGroup");
}

[[nodiscard]] Magnum::SceneGraph::DrawableGroup3D *
SceneRenderableGroup::getInstance() noexcept {
  // return reinterpret_cast<Magnum::SceneGraph::DrawableGroup3D *>(this);
  return this;
}