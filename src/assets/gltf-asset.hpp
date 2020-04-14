#pragma once

#include "engine/classes.hpp"

#include <unordered_map>
#include "base-asset.hpp"
#include <MagnumPlugins/TinyGltfImporter/TinyGltfImporter.h>
#include <Magnum/GL/Mesh.h>

namespace Koala {
namespace Assets {

class GLTFAsset : public BaseAsset {
public:
  GLTFAsset(rapidjson::GenericObject<false, rapidjson::Value::ValueType> props,
            const std::string &rootDir);
  virtual ~GLTFAsset();
  [[nodiscard]] virtual bool Parse() noexcept override;

  Magnum::Trade::TinyGltfImporter gltfImporter;

  const tinygltf::Model *state;

  std::unordered_map<std::string, Magnum::GL::Mesh> compiledMeshes;

private:
  bool parsed;

  DebugLogger logger;
};

} // namespace Assets
} // namespace Koala