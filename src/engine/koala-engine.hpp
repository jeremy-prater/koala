#include "debuglogger.hpp"
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/Platform/Sdl2Application.h>

using namespace Magnum;

class Koala : public Platform::Application {
public:
  explicit Koala(const Arguments &arguments);

private:
  DebugLogger logger;
  void drawEvent() override;
};
