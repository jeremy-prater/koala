#pragma once

#include "base-object.hpp"
#include <MagnumPlugins/TinyGltfImporter/TinyGltfImporter.h>

namespace Koala {

class GLTFObject : public BaseObject {
public:
  GLTFObject(rapidjson::GenericObject<false, rapidjson::Value::ValueType> props,
             const std::string rootDir);
  virtual ~GLTFObject();
  [[nodiscard]] virtual bool Parse() noexcept override;

  Magnum::Trade::TinyGltfImporter gltfImporter;

private:
  bool parsed;

  DebugLogger logger;
};
} // namespace Koala