#include "gltf-object.hpp"

using namespace Koala;

GLTFObject::GLTFObject(
    rapidjson::GenericObject<false, rapidjson::Value::ValueType> props,
    const std::string rootDir)
    : BaseObject(props, rootDir),
      logger("GLTF-Object" + path + "/" + name,
             DebugLogger::DebugColor::COLOR_GREEN, false) {
  logger.Info("Created GLTF Object [%s] ==> [%s]", uuid.c_str(),
              parser.c_str());
}

GLTFObject::~GLTFObject() {
  logger.Info("Destoryed GLTF Object [%s] ==> [%s]", uuid.c_str(),
              parser.c_str());
}