#pragma once

#include "engine/classes.hpp"

#include "debuglogger/debuglogger.hpp"
#include "engine/typedefs.hpp"
#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/uuid.hpp>

namespace Koala {
namespace Objects {

class BaseObject : public Object3D {
public:
  explicit BaseObject(const std::string &name, Object3D *parent);

protected:
  boost::uuids::uuid uuid;
  DebugLogger logger;
};

} // namespace Objects
} // namespace Koala
