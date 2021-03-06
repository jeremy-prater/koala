#pragma once

#include "engine/classes.hpp"

#include "assets/base-asset.hpp"
#include "assets/base-group.hpp"
#include "objects/shader-program.hpp"
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

class SceneRenderableGroup : public Magnum::SceneGraph::DrawableGroup3D {

  /////////////////////////////////////////////////////////////////////
  //
  // Static methods for SceneRenderableGroup management
  //

public:
  [[nodiscard]] static std::shared_ptr<SceneRenderableGroup>
  GetRenderGroupByAssetSet(
      const std::unordered_map<Koala::Assets::BaseGroup::NodeType,
                               std::shared_ptr<Koala::Assets::BaseAsset>>
          &assetSet) noexcept;

  [[nodiscard]] static const std::unordered_map<
      std::shared_ptr<SceneRenderableGroup>,
      std::unordered_map<Koala::Assets::BaseGroup::NodeType,
                         std::shared_ptr<Koala::Assets::BaseAsset>>>
  GetRenderGroups() noexcept;
  static void Init();
  static void Shutdown();

private:
  [[nodiscard]] static bool CompareAssetSets(
      const std::unordered_map<Koala::Assets::BaseGroup::NodeType,
                               std::shared_ptr<Koala::Assets::BaseAsset>> &a,
      const std::unordered_map<Koala::Assets::BaseGroup::NodeType,
                               std::shared_ptr<Koala::Assets::BaseAsset>>
          &b) noexcept;

  static std::mutex groupMappingsMutex;
  static Koala::DebugLogger groupLogger;
  static std::unordered_map<
      std::shared_ptr<SceneRenderableGroup>,
      std::unordered_map<Koala::Assets::BaseGroup::NodeType,
                         std::shared_ptr<Koala::Assets::BaseAsset>>>
      groupMappings;

  static std::mutex textureSlotsMutex;
  static std::vector<boost::uuids::uuid> textureSlots;

  /////////////////////////////////////////////////////////////////////
  //
  // SceneRenderableGroup implementation
  //

public:
  SceneRenderableGroup(
      const std::unordered_map<Koala::Assets::BaseGroup::NodeType,
                               std::shared_ptr<Koala::Assets::BaseAsset>>
          &assetTemplate);

  [[nodiscard]] Magnum::SceneGraph::DrawableGroup3D *getInstance() noexcept;

  const boost::uuids::uuid uuid;
  ShaderProgram shaderProgram;

  const std::unordered_map<Koala::Assets::BaseGroup::NodeType,
                           std::shared_ptr<Koala::Assets::BaseAsset>>
      assetMap;

  void BindTexturesToShaderProgram() noexcept;

private:
  DebugLogger logger;
};

} // namespace Objects
} // namespace Koala