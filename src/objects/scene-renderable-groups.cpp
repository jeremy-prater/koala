#include "objects/scene-renderable-groups.hpp"
#include "assets/texture-asset.hpp"
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

std::mutex SceneRenderableGroup::textureSlotsMutex;
std::vector<boost::uuids::uuid> SceneRenderableGroup::textureSlots;

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

  for (auto &groupMapping : groupMappings) {

    sceneRenderGroupTest = groupMapping.first;
    auto &assetSetTest = groupMapping.second;

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

[[nodiscard]] const std::unordered_map<
    std::shared_ptr<SceneRenderableGroup>,
    std::unordered_map<Koala::Assets::BaseGroup::NodeType,
                       std::shared_ptr<Koala::Assets::BaseAsset>>>
SceneRenderableGroup::GetRenderGroups() noexcept {
  return groupMappings;
}

void SceneRenderableGroup::Init() {
  std::scoped_lock<std::mutex> textureLock(textureSlotsMutex);
  textureSlots = std::vector<boost::uuids::uuid>(Koala::Objects::ShaderProgram::MaxTextureFragment);
}

void SceneRenderableGroup::Shutdown() { groupMappings.clear(); }

/////////////////////////////////////////////////////////////////////
//
// Instance Members
//
SceneRenderableGroup::SceneRenderableGroup(
    const std::unordered_map<Koala::Assets::BaseGroup::NodeType,
                             std::shared_ptr<Koala::Assets::BaseAsset>>
        &assetTemplate)
    : uuid(Engine::Engine::RandomUUIDGenerator()), shaderProgram(assetTemplate),
      assetMap(assetTemplate),
      logger("SceneRenderableGroup-" + boost::uuids::to_string(uuid),
             DebugLogger::COLOR_YELLOW, false) {
  logger.Info("Created SceneRenderableGroup");
}

[[nodiscard]] Magnum::SceneGraph::DrawableGroup3D *
SceneRenderableGroup::getInstance() noexcept {
  // return reinterpret_cast<Magnum::SceneGraph::DrawableGroup3D *>(this);
  return this;
}

void SceneRenderableGroup::BindTexturesToShaderProgram() noexcept {
  using namespace Koala::Assets;
  // auto it = assetMap.find(BaseGroup::NodeType::VertexShader);
  // if (it == assetMap.end())
  // {
  //   logger.Warning("Unable to find vertex shader!");
  // }
  // auto& vertexShader =
  // std::dynamic_pointer_cast<Koala::Assets::GLSLAsset>(it->second)->shader;

  // it = assetMap.find(BaseGroup::NodeType::FragmentShader);
  // if (it == assetMap.end())
  // {
  //   logger.Warning("Unable to find fragment shader!");
  // }
  // auto& fragShader =
  // std::dynamic_pointer_cast<Koala::Assets::GLSLAsset>(it->second)->shader;

  for (uint32_t slot = 0;
       slot < Koala::Objects::ShaderProgram::MaxTextureFragment; slot++) {
    BaseGroup::NodeType nodeType = static_cast<BaseGroup::NodeType>(
        static_cast<uint32_t>(BaseGroup::NodeType::Texture0) + slot);
    auto it = assetMap.find(nodeType);
    if ((it != assetMap.end()) &&
        (textureSlots[slot] != it->second->GetUUID())) {
      std::scoped_lock<std::mutex> textureLock(textureSlotsMutex);
      std::dynamic_pointer_cast<Koala::Assets::TextureAsset>(it->second)
          ->bindToSlot(slot);
      textureSlots[slot] = it->second->GetUUID();
    }
  }
}