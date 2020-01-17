#pragma once

#include "debuglogger/debuglogger.hpp"
#include <zmq.hpp>

namespace Koala {
namespace Cloud {

class Cloud {
public:
  Cloud();
  ~Cloud();

  static zmq::context_t &GetContext();

private:
  static zmq::context_t zmqContext;

  DebugLogger logger;
};

} // namespace Cloud
} // namespace Koala
