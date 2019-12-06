#include "debuglogger.hpp"
#include "rapidjson/document.h"
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/Platform/Sdl2Application.h>

using namespace Magnum;

class KoalaEngine : public Platform::Application {
public:
  explicit KoalaEngine(const Arguments &arguments);

private:
  DebugLogger logger;
  void drawEvent() override;
};
