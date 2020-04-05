#include "engine/engine.hpp"
#include "koala-version.hpp"

using namespace Koala::Engine;
using namespace Magnum;

boost::uuids::string_generator Engine::StringUUIDGenerator;

Engine::Engine(const Arguments &arguments)
    : Platform::Application{arguments},
      logger("Main", DebugLogger::DebugColor::COLOR_WHITE, false) {
  logger.Info("Koala Engine Startup V%d.%d [%s]", KOALA_ENGINE_VERSION_MAJOR,
              KOALA_ENGINE_VERSION_MINOR, KOALA_ENGINE_BUILD_SHA);
  scene = std::make_unique<Koala::Objects::Scene>();
}

Engine::~Engine() {
  scene.reset();
  logger.Info("Koala Engine Destroyed...");
}
