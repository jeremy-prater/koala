#pragma once

#include "cloud/debugger.hpp"
#include "cloud/primary-endpoint.hpp"
#include "debuglogger/debuglogger.hpp"
#include <boost/uuid/random_generator.hpp>
#include <memory>
#include <zmq.hpp>

namespace Koala {
namespace Cloud {

class Cloud {
public:
  static Cloud *getInstance();
  Cloud();
  ~Cloud();

  zmq::context_t &GetContext();

  const boost::uuids::uuid cloudSessionUUID;
  const std::string cloudSessionUUIDString;
  std::shared_ptr<PrimaryEndpoint> primaryEndpoint;
  std::shared_ptr<Debugger> debugger;

private:
  static Cloud *instance;
  zmq::context_t zmqContext;

  DebugLogger logger;
};

} // namespace Cloud
} // namespace Koala
