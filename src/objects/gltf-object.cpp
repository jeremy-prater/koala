#include "gltf-object.hpp"
#include <Corrade/Containers/ArrayView.h>
#include <chrono>

using namespace Koala;

GLTFObject::GLTFObject(
    rapidjson::GenericObject<false, rapidjson::Value::ValueType> props,
    const std::string rootDir)
    : BaseObject(props, rootDir),
      logger("GLTF-Object-" + path + "/" + name,
             DebugLogger::DebugColor::COLOR_GREEN, false) {
  logger.Info("Created GLTF Object [%s] ==> [%s]", uuid.c_str(),
              parser.c_str());
}

GLTFObject::~GLTFObject() {
  logger.Info("Destroyed GLTF Object [%s] ==> [%s]", uuid.c_str(),
              parser.c_str());
}

[[nodiscard]] bool GLTFObject::Parse() noexcept {
  auto start = std::chrono::system_clock::now();

  parsed = gltfImporter.openData(Corrade::Containers::ArrayView<const char>{
      reinterpret_cast<const char *>(GetData()), static_cast<size_t>(size)});

  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
                      std::chrono::system_clock::now() - start)
                      .count();
  logger.Info("GLTF parsed [%d] meshes [%d] objects in [%d] us",
              gltfImporter.mesh3DCount(), gltfImporter.object3DCount(),
              duration);

  return parsed;
}