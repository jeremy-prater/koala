#pragma once

#include "assets/project-configuration.hpp"
#include "cloud/cloud.hpp"
#include "engine/engine.hpp"
#include "objects/camera.hpp"
#include <chrono>
#include <sys/stat.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>

class KoalaTest : public Koala::Engine::Engine {
public:
  explicit KoalaTest(const Arguments &arguments);
  virtual ~KoalaTest();

protected:
  std::unique_ptr<Koala::Cloud::Cloud> cloud;
  std::unique_ptr<Koala::Objects::Camera> camera;
  std::shared_ptr<Koala::Assets::Project> project;

private:
  bool running;
  std::thread gameThread;
  void drawEvent() override;

  Koala::DebugLogger logger;
};
