#include "gltf-asset.hpp"
#include <Corrade/Containers/ArrayView.h>
#include <Corrade/Containers/Optional.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/Trade/MeshData.h>
#include <Magnum/Trade/MeshObjectData3D.h>
#include <Magnum/Trade/SceneData.h>
#include <boost/uuid/uuid_io.hpp>
#include <chrono>

using namespace Koala::Assets;

const std::string
GLTFAsset::ConvertMeshAttributeToString(Magnum::Trade::MeshAttribute attr) {
  switch (attr) {
  case Magnum::Trade::MeshAttribute::Position:
    return "Position";
  case Magnum::Trade::MeshAttribute::Tangent:
    return "Tangent";
  case Magnum::Trade::MeshAttribute::Bitangent:
    return "Bitangent";
  case Magnum::Trade::MeshAttribute::Normal:
    return "Normal";
  case Magnum::Trade::MeshAttribute::TextureCoordinates:
    return "TextureCoordinates";
  case Magnum::Trade::MeshAttribute::Color:
    return "Color";
  case Magnum::Trade::MeshAttribute::ObjectId:
    return "ObjectId";
  default:
    return "unknown : " + std::to_string(static_cast<uint32_t>(attr));
  }
}

GLTFAsset::GLTFAsset(
    const std::string &rootDir,
    rapidjson::GenericObject<false, rapidjson::Value::ValueType> props,
    Project *project)
    : BaseAsset(rootDir, props, project),
      logger("GLTF-Asset-" + path + "/" + name,
             DebugLogger::DebugColor::COLOR_GREEN, false) {
  logger.Info("Created GLTF Asset [%s] ==> [%s]",
              boost::uuids::to_string(uuid).c_str(), parser.c_str());
}

GLTFAsset::~GLTFAsset() {
  logger.Info("Destroyed GLTF Asset [%s] ==> [%s]",
              boost::uuids::to_string(uuid).c_str(), parser.c_str());
}

void GLTFAsset::BuildChildTree(const std::string &localPath,
                               Magnum::UnsignedInt parentNode) noexcept {

  auto objectNode = gltfImporter.object3D(parentNode);
  const std::string objectName =
      localPath + (localPath.empty() ? "" : "/") +
      gltfImporter.object3DName(objectNode->instance());
  logger.Info("Parsing scene : [%d] = [%s]", parentNode, objectName.c_str());

  meshNames[parentNode] = objectName;

  auto childNodes = objectNode->children();
  for (auto currentNode : childNodes) {
    BuildChildTree(objectName, currentNode);
  }
}

[[nodiscard]] bool GLTFAsset::ParseInternal() noexcept {
  auto start = std::chrono::system_clock::now();

  parsed = gltfImporter.openData(Corrade::Containers::ArrayView<const char>{
      reinterpret_cast<const char *>(GetData()), static_cast<size_t>(size)});

  auto sceneCount = gltfImporter.sceneCount();

  for (uint32_t sceneID = 0; sceneID < sceneCount; sceneID++) {
    const std::string sceneName = gltfImporter.sceneName(sceneID);
    Magnum::Trade::SceneData sceneData = *gltfImporter.scene(sceneID);

    auto meshCount = gltfImporter.meshCount();
    meshNames.resize(meshCount);
    compiledMeshes.resize(meshCount);

    auto sceneChildren = sceneData.children3D();
    for (auto parentNode : sceneChildren) {
      BuildChildTree("", parentNode);
    }

    logger.Info("Parsing GLTF scene : [%s] w/ [%d] nodes", sceneName.c_str(),
                meshCount);

    for (uint32_t meshID = 0; meshID < meshCount; meshID++) {
      const std::string meshName = gltfImporter.meshName(meshID);

      auto submesh = gltfImporter.mesh(meshID);
      auto subobject = gltfImporter.object3D(meshID);

      logger.Info("Compiling mesh : %s w/ TRS [%d] Type [%d]", meshName.c_str(),
                  subobject->flags(), subobject->instanceType());

      uint32_t meshAttrCount = submesh->attributeCount();
      for (uint32_t currentMeshAttr = 0; currentMeshAttr < meshAttrCount;
           currentMeshAttr++) {
        Magnum::Trade::MeshAttribute attr =
            submesh->attributeName(currentMeshAttr);
        // submesh->attributeFormat(currentMeshAttr);
        auto attrName = ConvertMeshAttributeToString(attr);
        logger.Info("Mesh Attribute index [%d] of type [%s]", currentMeshAttr,
                    attrName.c_str());
      }

      compiledMeshes[meshID] = Magnum::MeshTools::compile(*submesh);
    }
  }

  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
                      std::chrono::system_clock::now() - start)
                      .count();

  logger.Info("GLTF parsed [%d] objects in [%d] ms", compiledMeshes.size(),
              duration);

  return parsed;
}