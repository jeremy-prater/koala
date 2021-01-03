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
  // typedef Magnum::GL::Attribute<0, Magnum::Vector3> Position;
  // typedef Magnum::GL::Attribute<1, Magnum::Vector3> Normal;
  // typedef Magnum::GL::Attribute<2, Magnum::Vector2> TextureCoordinates;

  explicit ShaderProgram(
      const std::unordered_map<Koala::Assets::BaseGroup::NodeType,
                               std::shared_ptr<Koala::Assets::BaseAsset>>
          &assetTemplate);

  template <class T>
  ShaderProgram &setShaderUniform(const std::string &uniformName,
                                                T value) {
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
