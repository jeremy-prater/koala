#pragma once

#include "debuglogger/debuglogger.hpp"
#include <unordered_map>

namespace Koala {

namespace Cloud {

class PrimaryEndpoint {
public:
  PrimaryEndpoint(const std::string host = "127.0.0.1");
  ~PrimaryEndpoint();

  [[nodiscard]] const std::vector<const std::string>
  GetServiceEndpointNames() const noexcept;
  [[nodiscard]] unsigned short GetServicePort(const std::string name);

private:
  std::unordered_map<std::string, unsigned short> serviceEndpoints;

  DebugLogger logger;
};

} // namespace Cloud
} // namespace Koala
