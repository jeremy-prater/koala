#include "cloud/debugger.hpp"
#include "cloud/cloud.hpp"

using namespace Koala::Cloud;
Debugger::Debugger(const std::string host)
    : socket(Cloud::getInstance()->GetContext(), zmq::socket_type::req),
      logger("Cloud-Debugger", DebugLogger::DebugColor::COLOR_MAGENTA, false) {
  std::string url = "tcp://" + host + ":20100";
  logger.Info("Created Cloud Debugger Endpoint [%s]", url.c_str());
  // socket.connect(url);
}

Debugger::~Debugger() { logger.Info("Destroyed Cloud Debugger"); }
