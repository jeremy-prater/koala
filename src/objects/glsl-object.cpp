#include "glsl-object.hpp"
#include <chrono>

using namespace Koala;

const std::unordered_map<std::string, const Magnum::GL::Version>
    GLSLObject::shaderVersions{{"GL460", Magnum::GL::Version::GL460},
                               {"GL450", Magnum::GL::Version::GL450},
                               {"GL440", Magnum::GL::Version::GL440},
                               {"GL430", Magnum::GL::Version::GL430},
                               {"GL420", Magnum::GL::Version::GL420},
                               {"GL410", Magnum::GL::Version::GL410},
                               {"GL400", Magnum::GL::Version::GL400},
                               {"GL330", Magnum::GL::Version::GL330},
                               {"GL320", Magnum::GL::Version::GL320},
                               {"GL310", Magnum::GL::Version::GL310},
                               {"GL300", Magnum::GL::Version::GL300}};

const std::unordered_map<std::string, const Magnum::GL::Shader::Type>
    GLSLObject::shaderTypes{
        {"Vertex", Magnum::GL::Shader::Type::Vertex},
        {"Fragment", Magnum::GL::Shader::Type::Fragment},
        {"Geometry", Magnum::GL::Shader::Type::Geometry},
        {"TessellationControl", Magnum::GL::Shader::Type::TessellationControl},
        {"TessellationEvaluation",
         Magnum::GL::Shader::Type::TessellationEvaluation},
        {"Compute", Magnum::GL::Shader::Type::Compute}};

const Magnum::GL::Version
GLSLObject::GetShaderVersionFromString(const std::string version) {
  auto it = shaderVersions.find(version);
  if (it == shaderVersions.end()) {
    logger.Warning("Unable to get Shader version [%s] Defaulting to GL330!",
                   version.c_str());
    return Magnum::GL::Version::GL330;
  }
  return it->second;
}

const Magnum::GL::Shader::Type
GLSLObject::GetShaderTypeFromString(const std::string type) {
  auto it = shaderTypes.find(type);
  if (it == shaderTypes.end()) {
    logger.Warning("Unable to get Shader type [%s] Defaulting to Vertex");
    return Magnum::GL::Shader::Type::Vertex;
  }
  return it->second;
}

GLSLObject::GLSLObject(
    rapidjson::GenericObject<false, rapidjson::Value::ValueType> props,
    const std::string rootDir)
    : BaseObject(props, rootDir),
      shader(GetShaderVersionFromString(GetMetaObject("Version")),
             GetShaderTypeFromString(GetMetaObject("Type"))),
      logger("GLSL-Object-" + path + "/" + name,
             DebugLogger::DebugColor::COLOR_GREEN, false) {
  logger.Info("Created GLSL Shader [%s] ==> [%s][%s]", uuid.c_str(),
              GetMetaObject("Type").c_str(), GetMetaObject("Version").c_str());
}

GLSLObject::~GLSLObject() {
  logger.Info("Destroyed GLSL Shader [%s] ==> [%s][%s]", uuid.c_str(),
              GetMetaObject("Type").c_str(), GetMetaObject("Version").c_str());
}

[[nodiscard]] bool GLSLObject::Parse() noexcept {
  auto start = std::chrono::system_clock::now();

  const std::string shaderString(reinterpret_cast<const char *>(GetData()));
  shader.addSource(shaderString);
  parsed = shader.compile();
  if (!parsed) {
    logger.Error("Failed to comple shader!\n\n--- Begin Shader---\n%s\n--- End "
                 "Shader ---",
                 shaderString.c_str());
  }

  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
                      std::chrono::system_clock::now() - start)
                      .count();
  logger.Info("Compiled in [%d] us", duration);

  return parsed;
}