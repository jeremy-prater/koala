#pragma once

#include "assets/project-configuration.hpp"
#include "cloud/primary-endpoint.hpp"
#include "engine/engine.hpp"
#include "objects/camera.hpp"
#include <chrono>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

class KoalaTest : public Koala::Engine::Engine {
public:
  KoalaTest(const Arguments &arguments);
  virtual ~KoalaTest();

protected:
  std::unique_ptr<Koala::Cloud::PrimaryEndpoint> cloudEndpoints;
  std::unique_ptr<Koala::Objects::Camera> camera;
  std::shared_ptr<Koala::Assets::Project> project;

private:
  void drawEvent() override;

  Koala::DebugLogger logger;
};
