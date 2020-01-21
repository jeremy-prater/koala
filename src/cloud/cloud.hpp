#pragma once

#include "debuglogger/debuglogger.hpp"
#include "cloud/primary-endpoint.hpp"
#include <boost/uuid/random_generator.hpp>
#include <zmq.hpp>
#include <memory>

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
  std::unique_ptr<PrimaryEndpoint> primaryEndpoint;
  
private:
  static Cloud *instance;
  zmq::context_t zmqContext;

  DebugLogger logger;
};

} // namespace Cloud
} // namespace Koala
