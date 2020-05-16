#include "shader-program.hpp"
#include "engine/engine.hpp"
#include <boost/uuid/uuid_io.hpp>

using namespace Koala::Objects;

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
