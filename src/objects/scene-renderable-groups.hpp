#pragma once

#include <Magnum/SceneGraph/Drawable.h>
#include <memory>
#include <mutex>
#include <unordered_map>
#include "objects/base-object.hpp"

namespace Koala {
namespace Objects {

/////////////////////////////////////////////////////////////////////
//
// Remember...  this is with-in the context of Koala::Objects::Scene
//
// A scene has 1 set of render groups...
//

class SceneRenderableGrouping {
public:
  [[nodiscard]] const std::vector<uint32_t> GetRenderGroups() const noexcept;

  [[nodiscard]] std::shared_ptr<Magnum::SceneGraph::DrawableGroup3D>
  GetRenderGroupByID(const uint32_t groupID) const noexcept;
  void
  AddRenderToGroupByID(const uint32_t groupID,
                       std::shared_ptr<Koala::Objects::BaseObject>) noexcept;

private:
  mutable std::mutex groupMappingsMutex;
  std::unordered_map<uint32_t,
                     std::shared_ptr<Magnum::SceneGraph::DrawableGroup3D>>
      groupMappings;
};

} // namespace Objects
} // namespace Koala