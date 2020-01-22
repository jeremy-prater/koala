#include "objects/base-object.hpp"

using namespace Koala::Objects;

BaseObject::BaseObject()
    : logger("Unknown Object", DebugLogger::DebugColor::COLOR_YELLOW, false) {}

BaseObject::~BaseObject() {}