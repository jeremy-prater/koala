#include "cloud.hpp"

using namespace Koala::Cloud;

zmq::context_t Cloud::zmqContext;

Cloud::Cloud()
    : logger("Cloud", DebugLogger::DebugColor::COLOR_MAGENTA, false) {
  logger.Info("Created Cloud Infrastructure");
}
Cloud::~Cloud() { logger.Info("Destroyed Cloud Infrastructure"); }

zmq::context_t &Cloud::GetContext() { return zmqContext; }