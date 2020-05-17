#pragma once

#include "engine/classes.hpp"

#include "base-asset.hpp"

namespace Koala {
namespace Assets {

class PNGAsset : public BaseAsset {
public:
  PNGAsset(rapidjson::GenericObject<false, rapidjson::Value::ValueType> props,
           const std::string &rootDir);
  virtual ~PNGAsset();
  [[nodiscard]] virtual bool ParseInternal() noexcept override;

private:
  DebugLogger logger;
};

} // namespace Assets
} // namespace Koala