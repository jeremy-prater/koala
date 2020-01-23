#pragma once

#include "debuglogger/debuglogger.hpp"
#include "engine/typedefs.hpp"

namespace Koala {
namespace Objects {

class BaseObject : public Object3D {
public:
  BaseObject(const std::string name);

protected:
  DebugLogger logger;
};

} // namespace Objects
} // namespace Koala
