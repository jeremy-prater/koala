#include "scene.hpp"

using namespace Koala;

Scene::Scene() : logger("Scene", DebugLogger::DebugColor::COLOR_YELLOW, false) {
  logger.Info("Created Scene");
}

Scene::~Scene() { logger.Info("Destroyed Scene"); }

void Scene::DumpScene() const noexcept {
  this->DumpScene(static_cast<const Object3D &>(*this), 0);
}

void Scene::DumpScene(const Object3D &parent, const int depth) const noexcept {
  if (depth == 0) {
    logger.Info("<-- DumpScene -->");
  }
  std::string line = "";
  for (int spaces = 0; spaces < depth; spaces++) {
    line += " ";
  }
  if (parent.children().isEmpty()) {
    line += "| ";
  } else {
    line += "+ ";
  }

  bool dirty = parent.isDirty();

  line += dirty ? "[DIRTY] " : "[CLEAN] ";

  if (parent.isScene()) {
    line += "Scene Root";
  } else {
    line += "Some object...";
  }

  logger.Info(line.c_str());

  for (const Object3D &child : parent.children()) {
    DumpScene(child, depth + 1);
  }
}