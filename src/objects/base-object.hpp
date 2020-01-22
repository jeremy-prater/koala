#pragma once

#include "debuglogger/debuglogger.hpp"
#include "engine/typedefs.hpp"

namespace Koala {
namespace Objects {

class BaseObject : Object3D {
public:
  BaseObject();
  ~BaseObject();

protected:
  DebugLogger logger;
};

} // namespace Objects
} // namespace Koala
