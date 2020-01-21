#pragma once

#include "debuglogger/debuglogger.hpp"
#include <unordered_map>
#include <vector>
#include <zmq.hpp>

namespace Koala {

namespace Cloud {

class PrimaryEndpoint {
public:
  PrimaryEndpoint(const std::string host = "127.0.0.1");
  ~PrimaryEndpoint();

  [[nodiscard]] std::vector<std::string> GetServiceEndpointNames() const
      noexcept;
  [[nodiscard]] unsigned short GetServicePort(const std::string name) const
      noexcept;

private:
  std::unordered_map<std::string, unsigned short> serviceEndpoints;

  zmq::socket_t socket;
  DebugLogger logger;
};

} // namespace Cloud
} // namespace Koala
