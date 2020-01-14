#include "engine.hpp"
#include "koala-version.hpp"

using namespace Koala;
using namespace Magnum;

Engine::Engine(const Arguments &arguments)
    : Platform::Application{arguments},
      logger("Main", DebugLogger::DebugColor::COLOR_WHITE, false) {
  logger.Info("Koala Engine Startup V%d.%d [%s]", KOALA_ENGINE_VERSION_MAJOR,
              KOALA_ENGINE_VERSION_MINOR, KOALA_ENGINE_BUILD_SHA);
  /* TODO: Add your initialization code here */
}

Engine::~Engine() { logger.Info("Koala Engine Destroyed..."); }

void Engine::DumpScene(Object3D &parent, const int depth) const noexcept {
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

  line += dirty ? "DIRTY" : "CLEAN";

  logger.Info(line.c_str());
  for (Object3D &child : parent.children()) {
    DumpScene(child, depth + 1);
  }
}