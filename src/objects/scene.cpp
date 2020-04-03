#include "objects/scene.hpp"

using namespace Koala::Objects;

Scene::Scene() : logger("Scene", DebugLogger::DebugColor::COLOR_YELLOW, false) {
  logger.Info("Created Scene");
}

Scene::~Scene() { logger.Info("Destroyed Scene"); }

void Scene::CreateRenderableFromGroup(
    const std::shared_ptr<Koala::Assets::BaseGroup> group) {
  logger.Info("Creating Renderabler from group [%s]", group->GetPath().c_str());
  auto &nodeList = group->GetNodeList();
  for (auto &node : nodeList) {
    logger.Info("Node : %s", node.c_str());
    auto &nodeLinks = group->GetNodeLinks(node);
    for (auto &link : nodeLinks) {
      logger.Info("Node Link %s => %s", link.first.c_str(),
                  link.second->GetFullPath().c_str());
    }
  }
}