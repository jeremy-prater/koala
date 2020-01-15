#pragma once

#include "debuglogger.hpp"
#include "typedefs.hpp"
#include <rapidjson/pointer.h>

namespace Koala {

class BaseObject : Object3D {
public:
  BaseObject(
      rapidjson::GenericObject<false, rapidjson::Value::ValueType> props);
  ~BaseObject();

private:
  DebugLogger logger;
};

} // namespace Koala
