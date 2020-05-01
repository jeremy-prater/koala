#include "objects/base-object.hpp"

using namespace Koala::Objects;

BaseObject::BaseObject(const std::string &name)
    : logger("Object3D-" + name, DebugLogger::DebugColor::COLOR_YELLOW, false) {
  logger.Info("Creating object.");
}
