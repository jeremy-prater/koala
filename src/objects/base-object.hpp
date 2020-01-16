#pragma once

#include "debuglogger/debuglogger.hpp"
#include "engine/typedefs.hpp"
#include <rapidjson/pointer.h>

namespace Koala {
namespace Objects {

class BaseObject : Object3D {
public:
  BaseObject(
      rapidjson::GenericObject<false, rapidjson::Value::ValueType> props);
  ~BaseObject();

private:
  DebugLogger logger;
};

} // namespace Objects
} // namespace Koala
