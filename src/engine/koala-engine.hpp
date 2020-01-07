#pragma once

#include "debuglogger.hpp"
#include "rapidjson/document.h"
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/Platform/Sdl2Application.h>

using namespace Magnum;

namespace Koala {

class Engine : public Platform::Application {
public:
  explicit Engine(const Arguments &arguments);

private:
  DebugLogger logger;
};

} // namespace Koala
