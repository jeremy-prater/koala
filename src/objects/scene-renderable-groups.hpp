#pragma once

#include "objects/renderable.hpp"
#include <Magnum/SceneGraph/Drawable.h>
#include <memory>
#include <mutex>
#include <unordered_map>

namespace Koala {
namespace Objects {

/////////////////////////////////////////////////////////////////////
//
// Remember...  this is with-in the context of Koala::Objects::Scene
//
// A scene has 1 set of render groups...
//

class Renderable;

class SceneRenderableGrouping {
public:
  [[nodiscard]] const std::vector<uint32_t> GetRenderGroups() const noexcept;

  [[nodiscard]] std::shared_ptr<Magnum::SceneGraph::DrawableGroup3D>
  GetRenderGroupByID(const uint32_t groupID) const noexcept;

  void AddRenderableToGroup(std::shared_ptr<Renderable>) noexcept;

private:
  mutable std::mutex groupMappingsMutex;
  std::unordered_map<uint32_t, std::shared_ptr<Magnum::SceneGraph::DrawableGroup3D>> groupMappings;
};

} // namespace Objects
} // namespace Koala