#include "objects/scene-renderable-groups.hpp"
#include "objects/base-object.hpp"

using namespace Koala::Objects;

/////////////////////////////////////////////////////////////////////
//
// Remember...  this is with-in the context of Koala::Objects::Scene
//
// A scene has 1 set of render groups...
//
[[nodiscard]] const std::vector<uint32_t>
SceneRenderableGrouping::GetRenderGroups() const noexcept {}

[[nodiscard]] std::shared_ptr<Magnum::SceneGraph::DrawableGroup3D>
SceneRenderableGrouping::GetRenderGroupByID(const uint32_t groupID) const
    noexcept {
  //   mutable std::mutex groupMappingsMutex;
  //   std::unordered_map<
  //       uint32_t, std::shared_ptr<Magnum::SceneGraph::DrawableGroup3D>>
  //       groupMappings;
}

void SceneRenderableGrouping::AddRenderToGroupByID(
    const uint32_t groupID,
    std::shared_ptr<Koala::Objects::BaseObject>) noexcept {}
