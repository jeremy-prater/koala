#include "debuglogger.hpp"
#include "rapidjson/document.h"
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/Platform/Sdl2Application.h>

using namespace Magnum;

namespace Koala {

class Project {
public:
  explicit KoalaEngine(const Arguments &arguments);

private:
  DebugLogger logger;
  void drawEvent() override;
};
} // namespace Koala
