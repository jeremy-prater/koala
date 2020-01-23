#include "primary-endpoint.hpp"
#include "cloud/cloud.hpp"
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

using namespace Koala::Cloud;

PrimaryEndpoint::PrimaryEndpoint(const std::string &host)
    : socket(Cloud::getInstance()->GetContext(), zmq::socket_type::req),
      logger("Cloud-PrimaryEndpoint", DebugLogger::DebugColor::COLOR_MAGENTA,
             false) {
  std::string url = "tcp://" + host + ":20100";
  logger.Info("Created Cloud Primary Endpoint [%s]", url.c_str());
  socket.connect(url);

  rapidjson::Document outgoingRequest;
  outgoingRequest.SetObject();
  outgoingRequest.AddMember("command", "GetServices",
                            outgoingRequest.GetAllocator());
  rapidjson::StringBuffer message;
  rapidjson::Writer<rapidjson::StringBuffer> writer(message);
  outgoingRequest.Accept(writer);

  socket.send(zmq::buffer(std::string(message.GetString())));
  zmq::message_t incomingMessage;
  auto result = socket.recv(incomingMessage);
  if (result > 0) {

    rapidjson::Document response;
    response.Parse(incomingMessage.to_string().c_str());

    for (auto &service : response.GetArray()) {
      auto serviceRecord = service.GetObject();
      serviceEndpoints[serviceRecord["name"].GetString()] =
          serviceRecord["port"].GetUint();
    }

    for (auto &service : serviceEndpoints) {
      logger.Info("Cloud Service [%s] --> [%d]", service.first.c_str(),
                  service.second);
    }
  } else {
    logger.Warning("Failed to receive GetServices!");
  }
}

PrimaryEndpoint::~PrimaryEndpoint() {
  logger.Info("Destroyed Cloud Primary Endpoint");
}

[[nodiscard]] const std::vector<std::string>
PrimaryEndpoint::GetServiceEndpointNames() const noexcept {
  std::vector<std::string> endpoints;
  for (auto &endpoint : serviceEndpoints) {
    endpoints.push_back(endpoint.first);
  }
  return endpoints;
}

[[nodiscard]] unsigned short
PrimaryEndpoint::GetServicePort(const std::string &name) const noexcept {
  auto it = serviceEndpoints.find(name);
  if (it != serviceEndpoints.end()) {
    return it->second;
  }
  return 0;
}
