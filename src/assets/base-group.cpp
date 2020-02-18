#include "base-group.hpp"

using namespace Koala::Assets;

BaseGroup::BaseGroup(
    rapidjson::GenericObject<false, rapidjson::Value::ValueType> props)
    : logger("BaseGroup-invalid", DebugLogger::DebugColor::COLOR_CYAN, false) {        
    }
