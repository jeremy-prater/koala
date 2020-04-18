#include "gltf-asset.hpp"
#include <Corrade/Containers/ArrayView.h>
#include <Corrade/Containers/Optional.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/Trade/MeshData3D.h>
#include <Magnum/Trade/MeshObjectData3D.h>
#include <Magnum/Trade/SceneData.h>
#include <boost/uuid/uuid_io.hpp>
#include <chrono>

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

void GLTFAsset::BuildChildTree(const std::string path,
                               Magnum::UnsignedInt parentNode) noexcept {

  auto objectNode = gltfImporter.object3D(parentNode);
  const std::string objectName =
      gltfImporter.object3DName(objectNode->instance());
  logger.Info("Parsing scene : [%s/%s]", path.c_str(), objectName.c_str());

  auto childNodes = objectNode->children();
  for (auto parentNode : childNodes) {
    BuildChildTree(objectName, parentNode);
  }
}

[[nodiscard]] bool GLTFAsset::Parse() noexcept {
  auto start = std::chrono::system_clock::now();

  parsed = gltfImporter.openData(Corrade::Containers::ArrayView<const char>{
      reinterpret_cast<const char *>(GetData()), static_cast<size_t>(size)});

  state = gltfImporter.importerState();

  auto sceneCount = gltfImporter.sceneCount();

  uint32_t nodeCount = 0;

  for (uint32_t sceneID = 0; sceneID < sceneCount; sceneID++) {
    const std::string sceneName = gltfImporter.sceneName(sceneID);
    Magnum::Trade::SceneData sceneData = *gltfImporter.scene(sceneID);
    auto sceneChildren = sceneData.children3D();
    logger.Info("Parsing scene : [%s] w/ [%d] children", sceneName.c_str(),
                sceneChildren.size());

    for (auto parentNode : sceneChildren) {
      BuildChildTree("", parentNode);
    }
  }

  // auto meshCount = gltfImporter.mesh3DCount();
  // for (uint32_t meshID = 0; meshID < meshCount; meshID++) {
  //   const std::string meshName = gltfImporter.mesh3DName(meshID);
  //   logger.Info("Compiling mesh : %s", meshName.c_str());
  //   meshNames.push_back(meshName);
  //   // Corrade::Containers::Optional<Magnum::Trade::MeshData3D> meshData =
  //   //         gltfImporter.mesh3D(meshID);

  //   compiledMeshes.push_back(
  //       Magnum::MeshTools::compile(*gltfImporter.mesh3D(meshID)));
  // }

  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
                      std::chrono::system_clock::now() - start)
                      .count();

  logger.Info("GLTF parsed [%d] nodes in [%d] ms", nodeCount, duration);

  return parsed;
}