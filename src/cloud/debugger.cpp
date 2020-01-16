#include "cloud/debugger.hpp"

using namespace Koala::Cloud;
Debugger::Debugger()
    : logger("Cloud-Debugger", DebugLogger::DebugColor::COLOR_MAGENTA, false) {
  logger.Info("Created Cloud Debugger");
}

Debugger::~Debugger() { logger.Info("Destroyed Cloud Debugger"); }
