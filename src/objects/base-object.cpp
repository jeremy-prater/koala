#include "objects/base-object.hpp"
#include "engine/engine.hpp"
#include <boost/uuid/uuid_io.hpp>

using namespace Koala::Objects;

BaseObject::BaseObject(const std::string &name, Object3D *parent)
    : Object3D{parent}, uuid(Koala::Engine::Engine::RandomUUIDGenerator()),
      logger("Object3D-" + boost::uuids::to_string(uuid) + "-" + name,
             DebugLogger::DebugColor::COLOR_YELLOW, false) {
  logger.Info("Creating object");
}
