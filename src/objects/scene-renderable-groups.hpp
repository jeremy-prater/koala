#pragma once

#include "engine/classes.hpp"

#include "assets/base-asset.hpp"
#include "assets/base-group.hpp"
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

class SceneRenderableGroup {

/////////////////////////////////////////////////////////////////////
//
// Static methods for SceneRenderableGroup management
//

public:
  [[nodiscard]] static std::shared_ptr<Magnum::SceneGraph::DrawableGroup3D>
  GetRenderGroupByAssetSet(
      const std::unordered_map<Koala::Assets::BaseGroup::NodeType,
                               std::shared_ptr<Koala::Assets::BaseAsset>>
          &assetSet) noexcept;

private:
  static std::mutex groupMappingsMutex;
  static std::unordered_map<
      std::shared_ptr<Magnum::SceneGraph::DrawableGroup3D>,
      std::unordered_map<Koala::Assets::BaseGroup::NodeType,
                         std::shared_ptr<Koala::Assets::BaseAsset>>>
      groupMappings;

/////////////////////////////////////////////////////////////////////
//
// SceneRenderableGroup implementation
//

public:
  [[nodiscard]] Magnum::SceneGraph::DrawableGroup3D * getInstance() const noexcept;


};

} // namespace Objects
} // namespace Koala