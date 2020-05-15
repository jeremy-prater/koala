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
  explicit ShaderProgram(
      const std::unordered_map<Koala::Assets::BaseGroup::NodeType,
                               std::shared_ptr<Koala::Assets::BaseAsset>>
          &assetTemplate);

private:
  const boost::uuids::uuid uuid;
  std::unordered_map<Koala::Assets::BaseGroup::NodeType,
                     std::shared_ptr<Koala::Assets::GLSLAsset>>
      shaderLinks;

  DebugLogger logger;
};

} // namespace Objects

} // namespace Koala
