#include "primary-endpoint.hpp"
#include <zmq.hpp>

using namespace Koala::Cloud;

PrimaryEndpoint::PrimaryEndpoint(const std::string host)
    : logger("Cloud-PrimaryEndpoint", DebugLogger::DebugColor::COLOR_MAGENTA,
             false) {
  zmq::context_t ctx;
  zmq::socket_t sock(ctx, zmq::socket_type::req);
  std::string url = "tcp://" + host + ":20100";
  logger.Info("Created Cloud Primary Endpoint [%s]", url.c_str());
  sock.connect(url);
  const std::string_view m = "Hello, world";
  auto result = sock.send(zmq::buffer(m), zmq::send_flags::none);
}
PrimaryEndpoint::~PrimaryEndpoint() {}

[[nodiscard]] const std::vector<const std::string>
PrimaryEndpoint::GetServiceEndpointNames() const noexcept {
  std::vector<const std::string> endpoints;
  for (auto &endpoint : serviceEndpoints) {
    endpoints.push_back(endpoint.first);
  }
  return endpoints;
}

[[nodiscard]] unsigned short
PrimaryEndpoint::GetServicePort(const std::string name) {
  auto it = serviceEndpoints.find(name);
  if (it != serviceEndpoints.end()) {
    return it->second;
  }
  return 0;
}
