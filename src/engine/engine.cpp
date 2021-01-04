#include "engine/engine.hpp"
#include "koala-version.hpp"
#include "objects/scene-renderable-groups.hpp"
#include "objects/shader-program.hpp"


using namespace Koala::Engine;
using namespace Magnum;

boost::uuids::string_generator Engine::StringUUIDGenerator;
boost::uuids::basic_random_generator<boost::mt19937>
    Engine::RandomUUIDGenerator;
Magnum::PluginManager::Manager<Magnum::Trade::AbstractImporter>
    Engine::MagnumPluginManager;

Engine::Engine(const Arguments &arguments)
    : Platform::Application{arguments},
      logger("Main", DebugLogger::DebugColor::COLOR_WHITE, false) {
  logger.Info("Koala Engine Startup V%d.%d [%s]", KOALA_ENGINE_VERSION_MAJOR,
              KOALA_ENGINE_VERSION_MINOR, KOALA_ENGINE_BUILD_SHA);

  Koala::Objects::ShaderProgram::PopulateMaxUniforms();
  Koala::Objects::ShaderProgram::PopulateMaxTextures();
  
  logger.Info("MaxUniformVertex : [%d]",
              Koala::Objects::ShaderProgram::MaxUniformVertex);
  logger.Info("MaxUniformFragment : [%d]",
              Koala::Objects::ShaderProgram::MaxUniformFragment);
  logger.Info("MaxUniformCompute : [%d]",
              Koala::Objects::ShaderProgram::MaxUniformCompute);
  logger.Info("MaxUniformGeometry : [%d]",
              Koala::Objects::ShaderProgram::MaxUniformGeometry);
  logger.Info("MaxUniformTessControl : [%d]",
              Koala::Objects::ShaderProgram::MaxUniformTessControl);
  logger.Info("MaxUniformTessEval : [%d]",
              Koala::Objects::ShaderProgram::MaxUniformTessEval);

  logger.Info("MaxTextureVertex : [%d]",
              Koala::Objects::ShaderProgram::MaxTextureVertex);
  logger.Info("MaxTextureFragment : [%d]",
              Koala::Objects::ShaderProgram::MaxTextureFragment);
  logger.Info("MaxTextureCompute : [%d]",
              Koala::Objects::ShaderProgram::MaxTextureCompute);
  logger.Info("MaxTextureGeometry : [%d]",
              Koala::Objects::ShaderProgram::MaxTextureGeometry);
  logger.Info("MaxTextureTessControl : [%d]",
              Koala::Objects::ShaderProgram::MaxTextureTessControl);
  logger.Info("MaxTextureTessEval : [%d]",
              Koala::Objects::ShaderProgram::MaxTextureTessEval);

  Koala::Objects::SceneRenderableGroup::Init(); 

  scene = std::make_unique<Koala::Objects::Scene>();
}

Engine::~Engine() {
  scene.reset();
  Koala::Objects::SceneRenderableGroup::Shutdown();
  logger.Info("Koala Engine Destroyed...");
}
