#include "koala-engine.hpp"
#include "koala-version.hpp"

KoalaEngine::KoalaEngine(const Arguments &arguments)
    : Platform::Application{arguments},
      logger("Main", DebugLogger::DebugColor::COLOR_WHITE, false) {
  logger.WriteLog(DebugLogger::DebugLevel::DEBUG_INFO,
                  "Koala Engine Startup V%d.%d [%s]",
                  KOALA_ENGINE_VERSION_MAJOR, KOALA_ENGINE_VERSION_MINOR,
                  KOALA_ENGINE_BUILD_SHA);
  /* TODO: Add your initialization code here */
}

void KoalaEngine::drawEvent() {
  GL::defaultFramebuffer.clear(GL::FramebufferClear::Color);

  /* TODO: Add your drawing code here */

  swapBuffers();
}