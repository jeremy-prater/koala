#pragma once

#include "debuglogger/debuglogger.hpp"
#include "zmq.hpp"

namespace Koala {

namespace Cloud {

class Debugger {
public:
  Debugger();
  ~Debugger();

private:
  DebugLogger logger;
};

} // namespace Cloud
} // namespace Koala