#include "objects/scene.hpp"
#include "assets/gltf-asset.hpp"
#include "base-object.hpp"
#include "renderable.hpp"
#include <Corrade/Containers/Optional.h>
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
  // Currently all new objects are created as a child of the top scene node
  auto newObject = new BaseObject(group->name, this);

  auto sceneCount = gltfAsset->gltfImporter.sceneCount();
  for (uint32_t sceneID = 0; sceneID < sceneCount; sceneID++) {
    // const std::string sceneName = gltfAsset->gltfImporter.sceneName(sceneID);
    Magnum::Trade::SceneData sceneData =
        *gltfAsset->gltfImporter.scene(sceneID);

    auto topLevelNodes = sceneData.children3D();
    for (auto nodeID : topLevelNodes) {
      // Renderable
      auto nodeName = gltfAsset->gltfImporter.meshName(nodeID);
      auto node = gltfAsset->gltfImporter.object3D(nodeID);

      Magnum::Quaternion rotation{Magnum::Math::IdentityInit};
      Magnum::Vector3 scaling{1.0f, 1.0f, 1.0f};
      Magnum::Vector3 translation{Magnum::Math::ZeroInit};

      std::string nodePath = group->name + "/" + nodeName;

      if (node->flags() &
          Magnum::Trade::ObjectFlag3D::HasTranslationRotationScaling) {
        logger.Info("%s has TRS!", nodePath.c_str());
        rotation = node->rotation();
        scaling = node->scaling();
        translation = node->translation();
      }

      auto newRenderable = new Koala::Objects::Renderable(
          nodePath, newObject, group->GetNodeRenderGroup(nodeName));
    }
    // newObject->p
  }
}