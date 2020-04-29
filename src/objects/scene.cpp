#include "objects/scene.hpp"

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

  // Load all children
  auto nodeAssets = group->GetNodeAssets();
  for (auto nodeAsset : nodeAssets) {
    if (!nodeAsset.second->IsLoaded()) {
      nodeAsset.second->Load();
    }
  }
}