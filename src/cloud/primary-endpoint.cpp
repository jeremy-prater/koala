#include "primary-endpoint.hpp"
#include <zmq.hpp>

using namespace Koala::Cloud;

PrimaryEndpoint::PrimaryEndpoint(const std::string host)
    : logger("Cloud-PrimaryEndpoint", DebugLogger::DebugColor::COLOR_MAGENTA,
             false) {
  zmq::context_t context;
  zmq::socket_t socket(context, zmq::socket_type::req);
  std::string url = "tcp://" + host + ":20100";
  logger.Info("Created Cloud Primary Endpoint [%s]", url.c_str());
  socket.connect(url);
  const std::string_view message = "{command: \"GetServices\"}";
  auto result = socket.send(zmq::buffer(message), zmq::send_flags::dontwait);
}
PrimaryEndpoint::~PrimaryEndpoint() {
  logger.Info("Destroyed Cloud Primary Endpoint");
}

[[nodiscard]] std::vector<std::string>
PrimaryEndpoint::GetServiceEndpointNames() const noexcept {
  std::vector<std::string> endpoints;
  for (auto &endpoint : serviceEndpoints) {
    endpoints.push_back(endpoint.first);
  }
  return endpoints;
}

[[nodiscard]] unsigned short
PrimaryEndpoint::GetServicePort(const std::string name) const noexcept {
  auto it = serviceEndpoints.find(name);
  if (it != serviceEndpoints.end()) {
    return it->second;
  }
  return 0;
}
