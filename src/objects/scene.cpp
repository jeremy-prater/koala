#include "objects/scene.hpp"
#include "assets/gltf-asset.hpp"
#include "base-object.hpp"
#include <Magnum/Trade/MeshData.h>
#include <Magnum/Trade/MeshObjectData3D.h>
#include <Magnum/Trade/SceneData.h>

using namespace Koala::Objects;

Scene::Scene() : logger("Scene", DebugLogger::DebugColor::COLOR_YELLOW, false) {
  logger.Info("Created Scene");
}

Scene::~Scene() { logger.Info("Destroyed Scene"); }

void Scene::CreateRenderableFromGroup(
    const std::shared_ptr<Assets::Project> project,
    const std::shared_ptr<Koala::Assets::BaseGroup> group) {
  logger.Info("Creating Renderable from group [%s]", group->GetPath().c_str());

  // Get and load primary asset...
  auto primaryAsset = project->GetAsset(group->GetParentUUID());

  if (!primaryAsset.operator bool() ||
      primaryAsset->GetType() != Koala::Assets::BaseAsset::AssetType::GLTF) {
    logger.Error("Can not create renderable from group! [%s]",
                 group->name.c_str());
    return;
  }

  if (!primaryAsset->IsLoaded()) {
    primaryAsset->Load();
  }

  if (!primaryAsset->IsParsed()) {
    primaryAsset->Parse();
  }

  // Load all children
  auto nodeAssets = group->GetNodeAssets();
  for (auto nodeAsset : nodeAssets) {
    auto &childAsset = nodeAsset.second;
    if (!childAsset->IsLoaded()) {
      childAsset->Load();
    }

    if (!childAsset->IsParsed()) {
      childAsset->Parse();
    }
  }

  // Get a pointer to the GLTF asset
  auto gltfAsset =
      std::dynamic_pointer_cast<Koala::Assets::GLTFAsset>(primaryAsset);

  // Add a root node to the scene graph to hold all the top level nodes for this
  std::shared_ptr<BaseObject> newObject =
      std::make_shared<BaseObject>(group->name);

  auto sceneCount = gltfAsset->gltfImporter.sceneCount;
  for (uint32_t sceneID = 0; sceneID < sceneCount; sceneID++) {
    // const std::string sceneName = gltfAsset->gltfImporter.sceneName(sceneID);
    Magnum::Trade::SceneData sceneData =
        *gltfAsset->gltfImporter.scene(sceneID);

    auto topLevelNodes = sceneData.children3D();
    for (auto node : topLevelNodes) {
      // Renderable
    }
    // newObject->p
  }
}