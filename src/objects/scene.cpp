#include "objects/scene.hpp"

using namespace Koala::Objects;

Scene::Scene() : logger("Scene", DebugLogger::DebugColor::COLOR_YELLOW, false) {
  logger.Info("Created Scene");
}

Scene::~Scene() { logger.Info("Destroyed Scene"); }

void Scene::CreateRenderableFromGroup(
    const std::shared_ptr<Koala::Assets::BaseGroup> group) {
  logger.Info("Creating Renderabler from group [%s]", group->GetPath().c_str());
}