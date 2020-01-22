#include "cloud.hpp"
#include <boost/uuid/uuid_io.hpp>

using namespace Koala::Cloud;

Cloud *Cloud::instance = nullptr;

Cloud *Cloud::getInstance() { return instance; }

Cloud::Cloud()
    : cloudSessionUUID(boost::uuids::random_generator()()),
      cloudSessionUUIDString(boost::uuids::to_string(cloudSessionUUID)),
      logger("Cloud", DebugLogger::DebugColor::COLOR_MAGENTA, false) {
  instance = this;

  primaryEndpoint = std::make_unique<PrimaryEndpoint>();
  debugger = std::make_unique<Debugger>();

  logger.Info("Created Cloud Infrastructure [%s]",
              cloudSessionUUIDString.c_str());
}

Cloud::~Cloud() {
  debugger.reset();
  primaryEndpoint.reset();
  logger.Info("Destroyed Cloud Infrastructure");
}

zmq::context_t &Cloud::GetContext() { return zmqContext; }