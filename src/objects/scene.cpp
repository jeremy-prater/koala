#include "objects/scene.hpp"
#include "assets/gltf-asset.hpp"
#include "base-object.hpp"
#include "renderable.hpp"
#include <Corrade/Containers/Optional.h>
#include <Magnum/Trade/MeshData.h>
#include <Magnum/Trade/MeshObjectData3D.h>
#include <Magnum/Trade/SceneData.h>
#include <chrono>

using namespace Koala::Objects;

Scene::Scene() : logger("Scene", DebugLogger::DebugColor::COLOR_YELLOW, false) {
  logger.Info("Created Scene");
}

Scene::~Scene() { logger.Info("Destroyed Scene"); }

static void AddNode(const std::shared_ptr<Koala::Assets::BaseGroup> group,
                    BaseObject *parent,
                    std::shared_ptr<Koala::Assets::GLTFAsset> gltfAsset,
                    uint32_t nodeID, const std::string &pathPrefix,
                    const Magnum::Matrix4 &parentTransform) {
  auto nodeName = gltfAsset->gltfImporter.meshName(nodeID);
  auto node = gltfAsset->gltfImporter.object3D(nodeID);
  auto &mesh = gltfAsset->compiledMeshes[nodeID];

  Magnum::Quaternion rotation{Magnum::Math::IdentityInit};
  Magnum::Vector3 scaling{1.0f, 1.0f, 1.0f};
  Magnum::Vector3 translation{Magnum::Math::ZeroInit};

  std::string nodePath = group->name + "/" + nodeName;

  if (node->flags() &
      Magnum::Trade::ObjectFlag3D::HasTranslationRotationScaling) {
    // logger->Info("%s has TRS!", nodePath.c_str());
    rotation = node->rotation();
    scaling = node->scaling();
    translation = node->translation();
  }

  const auto &nodeTransformation = node->transformation();

  auto newRenderable = new Koala::Objects::Renderable(
      nodePath, parent, group->GetNodeRenderGroup(pathPrefix + nodeName),
      translation, rotation, scaling, mesh, parentTransform);
  auto children = node->children();
  for (auto &child : children) {
    AddNode(group, newRenderable, gltfAsset, child, nodeName + "/" + pathPrefix,
            parentTransform * nodeTransformation);
  }
}

void Scene::CreateRenderableFromGroup(
    const std::shared_ptr<Assets::Project> project,
    const std::shared_ptr<Koala::Assets::BaseGroup> group,
    const Magnum::Matrix4 &identityTransform) {
  auto start = std::chrono::system_clock::now();

  logger.Info("Creating Renderable from group [%s]", group->GetPath().c_str());

  // Get and load primary asset...
  auto primaryAsset = project->GetAsset(group->GetParentUUID());

  if (!primaryAsset.operator bool() ||
      primaryAsset->GetType() != Koala::Assets::BaseAsset::AssetType::GLTF) {
    logger.Error("Can not create renderable from group! [%s]",
                 group->name.c_str());
    return;
  }

  primaryAsset->LoadParse();

  // Load all children
  auto nodeAssets = group->GetNodeAssets();
  for (auto nodeAsset : nodeAssets) {
    auto &childAsset = nodeAsset.second;
    childAsset->LoadParse();
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
      AddNode(group, newObject, gltfAsset, nodeID, "", identityTransform);
    }
  }

  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
                      std::chrono::system_clock::now() - start)
                      .count();
  logger.Info("GLTF instance inserted into scene graph in [%d] us", duration);
}