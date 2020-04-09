#pragma once

#include "engine/classes.hpp"

#include "debuglogger/debuglogger.hpp"
#include "engine/typedefs.hpp"

namespace Koala {
namespace Objects {

class BaseObject : public Object3D {
public:
  explicit BaseObject(const std::string &name);

protected:
  DebugLogger logger;
};

} // namespace Objects
} // namespace Koala
