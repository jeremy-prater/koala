#include "engine/engine.hpp"
#include "koala-version.hpp"

using namespace Koala::Engine;
using namespace Magnum;

Engine::Engine(const Arguments &arguments)
    : Platform::Application{arguments},
      logger("Main", DebugLogger::DebugColor::COLOR_WHITE, false) {
  logger.Info("Koala Engine Startup V%d.%d [%s]", KOALA_ENGINE_VERSION_MAJOR,
              KOALA_ENGINE_VERSION_MINOR, KOALA_ENGINE_BUILD_SHA);
  /* TODO: Add your initialization code here */
}

Engine::~Engine() { logger.Info("Koala Engine Destroyed..."); }
