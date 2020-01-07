#pragma once

#include "koala-engine.hpp"
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

private:
  void drawEvent() override;

  std::shared_ptr<Koala::Project> project;
  DebugLogger logger;
};
