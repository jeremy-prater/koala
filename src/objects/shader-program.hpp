#pragma once

#include <Magnum/GL/AbstractShaderProgram.h>
#include <Magnum/GL/Shader.h>
#include <assets/base-group.hpp>
#include <assets/glsl-asset.hpp>
#include <boost/uuid/uuid.hpp>

namespace Koala {

namespace Objects {

class ShaderProgram : public Magnum::GL::AbstractShaderProgram {
public:
  static uint32_t MaxUniformVertex;
  static uint32_t MaxUniformFragment;
  static uint32_t MaxUniformCompute;
  static uint32_t MaxUniformGeometry;
  static uint32_t MaxUniformTessControl;
  static uint32_t MaxUniformTessEval;
  static uint32_t MaxTextureVertex;
  static uint32_t MaxTextureFragment;
  static uint32_t MaxTextureCompute;
  static uint32_t MaxTextureGeometry;
  static uint32_t MaxTextureTessControl;
  static uint32_t MaxTextureTessEval;
  static void PopulateMaxUniforms();
  static void PopulateMaxTextures();

  explicit ShaderProgram(
      const std::unordered_map<Koala::Assets::BaseGroup::NodeType,
                               std::shared_ptr<Koala::Assets::BaseAsset>>
          &assetTemplate);

  template <class T>
  ShaderProgram &setShaderUniform(const std::string &uniformName, T value) {
    auto location = uniformLocation(uniformName);
    if (location == -1) {
      logger.Warning("Unable to find uniform [%s]", uniformName.c_str());
    } else {
      setUniform(location, value);
    }

    return *this;
  }

private:
  const boost::uuids::uuid uuid;
  std::unordered_map<Koala::Assets::BaseGroup::NodeType,
                     std::shared_ptr<Koala::Assets::GLSLAsset>>
      shaderLinks;

  DebugLogger logger;
};

} // namespace Objects

} // namespace Koala
