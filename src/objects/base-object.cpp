#include "objects/base-object.hpp"

using namespace Koala::Objects;

BaseObject::BaseObject(const std::string &name)
    : logger(name, DebugLogger::DebugColor::COLOR_YELLOW,
             false) {}
