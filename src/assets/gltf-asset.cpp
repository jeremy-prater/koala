#include "gltf-asset.hpp"
#include <Corrade/Containers/ArrayView.h>
#include <boost/uuid/uuid_io.hpp>
#include <chrono>
#include <corrade/Containers/Optional.h>
#include <magnum/Trade/MeshData3D.h>

using namespace Koala::Assets;

GLTFAsset::GLTFAsset(
    rapidjson::GenericObject<false, rapidjson::Value::ValueType> props,
    const std::string &rootDir)
    : BaseAsset(props, rootDir),
      logger("GLTF-Asset-" + path + "/" + name,
             DebugLogger::DebugColor::COLOR_GREEN, false) {
  logger.Info("Created GLTF Asset [%s] ==> [%s]",
              boost::uuids::to_string(uuid).c_str(), parser.c_str());
}

GLTFAsset::~GLTFAsset() {
  logger.Info("Destroyed GLTF Asset [%s] ==> [%s]",
              boost::uuids::to_string(uuid).c_str(), parser.c_str());
}

[[nodiscard]] bool GLTFAsset::Parse() noexcept {
  auto start = std::chrono::system_clock::now();

  parsed = gltfImporter.openData(Corrade::Containers::ArrayView<const char>{
      reinterpret_cast<const char *>(GetData()), static_cast<size_t>(size)});

  state = gltfImporter.importerState();

  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
                      std::chrono::system_clock::now() - start)
                      .count();

  auto meshCount = gltfImporter.mesh3DCount();

  logger.Info("GLTF parsed [%d] objects in [%d] us", meshCount, duration);
  for (int meshID = 0; meshID < meshCount; meshID++) {
    const std::string meshName = gltfImporter.mesh3DName(meshID);
    logger.Info("GLTF mesh -> %s", meshName.c_str());
    Corrade::Containers::Optional<Magnum::Trade::MeshData3D> meshData =
        gltfImporter.mesh3D(meshID);
  }

  return parsed;
}