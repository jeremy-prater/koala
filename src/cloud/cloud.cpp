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
  logger.Info("Created Cloud Infrastructure [%s]",
              cloudSessionUUIDString.c_str());

  primaryEndpoint = std::make_unique<PrimaryEndpoint>();
}
Cloud::~Cloud() {
  primaryEndpoint.reset();
  logger.Info("Destroyed Cloud Infrastructure");
}

zmq::context_t &Cloud::GetContext() { return zmqContext; }