#pragma once

#include "base-asset.hpp"
#include "debuglogger/debuglogger.hpp"
#include <memory>
#include <mutex>
#include <rapidjson/pointer.h>
#include <rapidjson/rapidjson.h>
#include <string>
#include <unordered_map>

namespace Koala {
namespace Assets {

class BaseGroup {
public:
  const std::string uuid;
  const std::string parentPath;
  const std::string name;

  BaseGroup(rapidjson::GenericObject<false, rapidjson::Value::ValueType> props);

protected:
  std::shared_ptr<BaseAsset> parent;
  std::unordered_map<std::string, std::unordered_map<std::string, std::string>>
      nodes;
  DebugLogger logger;
};

} // namespace Assets
} // namespace Koala