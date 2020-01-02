#pragma once

#include "koala-object.hpp"
#include <MagnumPlugins/TinyGltfImporter/TinyGltfImporter.h>

namespace Koala {

class GLTFObject : public BaseObject {
public:
  GLTFObject(rapidjson::GenericObject<false, rapidjson::Value::ValueType> props,
             const std::string rootDir);
  virtual ~GLTFObject();

private:
  DebugLogger logger;
};
} // namespace Koala