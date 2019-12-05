#include "koala-engine.hpp"

Koala::Koala(const Arguments &arguments)
    : Platform::Application{arguments},
      logger("Main", DebugLogger::DebugColor::COLOR_WHITE, false) {
  logger.WriteLog(DebugLogger::DebugLevel::DEBUG_INFO, "I'm alive!");
  /* TODO: Add your initialization code here */
}

void Koala::drawEvent() {
  GL::defaultFramebuffer.clear(GL::FramebufferClear::Color);

  /* TODO: Add your drawing code here */

  swapBuffers();
}