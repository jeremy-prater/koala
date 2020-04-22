#pragma once

#include "engine/classes.hpp"

#include "base-asset.hpp"
#include <Magnum/GL/Mesh.h>
#include <MagnumPlugins/TinyGltfImporter/TinyGltfImporter.h>
#include <vector>

namespace Koala {
namespace Assets {

class GLTFAsset : public BaseAsset {
public:
  GLTFAsset(rapidjson::GenericObject<false, rapidjson::Value::ValueType> props,
            const std::string &rootDir);
  virtual ~GLTFAsset();
  [[nodiscard]] virtual bool Parse() noexcept override;

  Magnum::Trade::TinyGltfImporter gltfImporter;

  std::vector<std::string> meshNames;
  std::vector<Magnum::GL::Mesh> compiledMeshes;

private:
  void BuildChildTree(const std::string &path,
                      Magnum::UnsignedInt parentNode) noexcept;

  DebugLogger logger;
};

} // namespace Assets
} // namespace Koala