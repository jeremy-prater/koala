#pragma once

#include "base-asset.hpp"
#include <MagnumPlugins/TinyGltfImporter/TinyGltfImporter.h>

namespace Koala {
namespace Assets {

class GLTFAsset : public BaseAsset {
public:
  GLTFAsset(rapidjson::GenericObject<false, rapidjson::Value::ValueType> props,
            const std::string &rootDir);
  virtual ~GLTFAsset();
  [[nodiscard]] virtual bool Parse() noexcept override;

  Magnum::Trade::TinyGltfImporter gltfImporter;

private:
  bool parsed;

  DebugLogger logger;
};

} // namespace Assets
} // namespace Koala