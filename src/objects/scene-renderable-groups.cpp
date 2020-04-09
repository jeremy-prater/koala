#include "objects/scene-renderable-groups.hpp"

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
std::unordered_map<
    std::shared_ptr<Magnum::SceneGraph::DrawableGroup3D>,
    std::unordered_map<Koala::Assets::BaseGroup::NodeType,
                       std::shared_ptr<Koala::Assets::BaseAsset>>>
    SceneRenderableGroup::groupMappings;

[[nodiscard]] std::shared_ptr<Magnum::SceneGraph::DrawableGroup3D>
SceneRenderableGroup::GetRenderGroupByAssetSet(
    const std::unordered_map<Koala::Assets::BaseGroup::NodeType,
                             std::shared_ptr<Koala::Assets::BaseAsset>>
        &assetSet) noexcept {
  std::scoped_lock<std::mutex> lock(groupMappingsMutex);

  // This is a very slow process...
  for (auto &groupMapping : groupMappings) {
    // groupMapping
  }

  return std::shared_ptr<Magnum::SceneGraph::DrawableGroup3D>();
}

/////////////////////////////////////////////////////////////////////
//
// Instance Members
//

[[nodiscard]] Magnum::SceneGraph::DrawableGroup3D *
SceneRenderableGroup::getInstance() const noexcept {
  return nullptr;
}