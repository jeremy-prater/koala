#pragma once

#include "camera.hpp"
#include "engine.hpp"
#include "project-configuration.hpp"
#include <chrono>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

using namespace Koala;

class KoalaTest : public Engine {
public:
  KoalaTest(const Arguments &arguments);
  virtual ~KoalaTest();

protected:
  std::unique_ptr<Koala::Camera> camera;
  std::shared_ptr<Koala::Project> project;

private:
  void drawEvent() override;

  DebugLogger logger;
};
