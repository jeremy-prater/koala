#include "shader-program.hpp"
#include "engine/engine.hpp"
#include <boost/uuid/uuid_io.hpp>

using namespace Koala::Objects;

uint32_t ShaderProgram::MaxUniformVertex;
uint32_t ShaderProgram::MaxUniformFragment;
uint32_t ShaderProgram::MaxUniformCompute;
uint32_t ShaderProgram::MaxUniformGeometry;
uint32_t ShaderProgram::MaxUniformTessControl;
uint32_t ShaderProgram::MaxUniformTessEval;
uint32_t ShaderProgram::MaxTextureVertex;
uint32_t ShaderProgram::MaxTextureFragment;
uint32_t ShaderProgram::MaxTextureCompute;
uint32_t ShaderProgram::MaxTextureGeometry;
uint32_t ShaderProgram::MaxTextureTessControl;
uint32_t ShaderProgram::MaxTextureTessEval;

void ShaderProgram::PopulateMaxUniforms() {
  using namespace Magnum::GL;

  MaxUniformVertex =
      Shader::maxUniformComponents(Magnum::GL::Shader::Type::Vertex);
  MaxUniformFragment =
      Shader::maxUniformComponents(Magnum::GL::Shader::Type::Fragment);
  MaxUniformCompute =
      Shader::maxUniformComponents(Magnum::GL::Shader::Type::Compute);
  MaxUniformGeometry =
      Shader::maxUniformComponents(Magnum::GL::Shader::Type::Geometry);
  MaxUniformTessControl = Shader::maxUniformComponents(
      Magnum::GL::Shader::Type::TessellationControl);
  MaxUniformTessEval = Shader::maxUniformComponents(
      Magnum::GL::Shader::Type::TessellationEvaluation);
}

void ShaderProgram::PopulateMaxTextures() {
  using namespace Magnum::GL;

  MaxTextureVertex =
      Shader::maxTextureImageUnits(Magnum::GL::Shader::Type::Vertex);
  MaxTextureFragment =
      Shader::maxTextureImageUnits(Magnum::GL::Shader::Type::Fragment);
  MaxTextureCompute =
      Shader::maxTextureImageUnits(Magnum::GL::Shader::Type::Compute);
  MaxTextureGeometry =
      Shader::maxTextureImageUnits(Magnum::GL::Shader::Type::Geometry);
  MaxTextureTessControl = Shader::maxTextureImageUnits(
      Magnum::GL::Shader::Type::TessellationControl);
  MaxTextureTessEval = Shader::maxTextureImageUnits(
      Magnum::GL::Shader::Type::TessellationEvaluation);
}

ShaderProgram::ShaderProgram(
    const std::unordered_map<Koala::Assets::BaseGroup::NodeType,
                             std::shared_ptr<Koala::Assets::BaseAsset>>
        &assetTemplate)
    : uuid(Engine::Engine::RandomUUIDGenerator()),
      logger("ShaderProgram-" + boost::uuids::to_string(uuid),
             DebugLogger::DebugColor::COLOR_CYAN, false) {
  logger.Info("Created ShaderProgram");

  auto fragIt =
      assetTemplate.find(Koala::Assets::BaseGroup::NodeType::FragmentShader);
  auto vertIt =
      assetTemplate.find(Koala::Assets::BaseGroup::NodeType::VertexShader);

  if (vertIt != assetTemplate.end()) {
    auto vertAsset =
        std::dynamic_pointer_cast<Koala::Assets::GLSLAsset>(vertIt->second);

    vertAsset->LoadParse();
    shaderLinks[vertIt->first] = vertAsset;

    attachShader(vertAsset->shader);
    logger.Info("Added vertex shader [%s]", vertAsset->GetFullPath().c_str());
  }

  if (fragIt != assetTemplate.end()) {
    auto fragAsset =
        std::dynamic_pointer_cast<Koala::Assets::GLSLAsset>(fragIt->second);
    fragAsset->LoadParse();
    shaderLinks[fragIt->first] = fragAsset;
    attachShader(fragAsset->shader);
    logger.Info("Added fragment shader [%s]", fragAsset->GetFullPath().c_str());
  }

  if (!link()) {
    logger.Warning("Failed to link shaders!!");
  } else {
    logger.Info("Shaders linked!");
  }
}
