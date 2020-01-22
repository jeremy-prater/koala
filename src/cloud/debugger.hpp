#pragma once

#include "debuglogger/debuglogger.hpp"
#include "zmq.hpp"

namespace Koala {

namespace Cloud {

class Debugger {
public:
  Debugger(const std::string host = "127.0.0.1");
  ~Debugger();

  void AddLog(const std::string logMessage);

private:
  zmq::socket_t socket;
  DebugLogger logger;
};

} // namespace Cloud
} // namespace Koala