#include "glsl-asset.hpp"
#include <boost/algorithm/string.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <chrono>

using namespace Koala::Assets;

const std::unordered_map<std::string, const Magnum::GL::Version>
    GLSLAsset::shaderVersions{{"GL460", Magnum::GL::Version::GL460},
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
    GLSLAsset::shaderTypes{
        {"Vertex", Magnum::GL::Shader::Type::Vertex},
        {"Fragment", Magnum::GL::Shader::Type::Fragment},
        {"Geometry", Magnum::GL::Shader::Type::Geometry},
        {"TessellationControl", Magnum::GL::Shader::Type::TessellationControl},
        {"TessellationEvaluation",
         Magnum::GL::Shader::Type::TessellationEvaluation},
        {"Compute", Magnum::GL::Shader::Type::Compute}};

Magnum::GL::Version GLSLAsset::GetShaderVersionFromString(
    const std::string &version) const noexcept {
  auto it = shaderVersions.find(version);
  if (it == shaderVersions.end()) {
    logger.Warning("Unable to get Shader version [%s] Defaulting to GL330!",
                   version.c_str());
    return Magnum::GL::Version::GL330;
  }
  return it->second;
}

Magnum::GL::Shader::Type GLSLAsset::GetShaderTypeFromString(
    const std::string &shaderType) const noexcept {
  auto it = shaderTypes.find(shaderType);
  if (it == shaderTypes.end()) {
    logger.Warning("Unable to get Shader type [%s] Defaulting to Vertex");
    return Magnum::GL::Shader::Type::Vertex;
  }
  return it->second;
}

GLSLAsset::GLSLAsset(
    rapidjson::GenericObject<false, rapidjson::Value::ValueType> props,
    const std::string &rootDir)
    : BaseAsset(props, rootDir),
      shader(GetShaderVersionFromString(GetMetaObject("Version")),
             GetShaderTypeFromString(GetMetaObject("Type"))),
      logger("GLSL-Asset-" + path + "/" + name,
             DebugLogger::DebugColor::COLOR_GREEN, false) {
  logger.Info("Created GLSL Shader [%s] ==> [%s][%s]",
              boost::uuids::to_string(uuid).c_str(),
              GetMetaObject("Type").c_str(), GetMetaObject("Version").c_str());
}

GLSLAsset::~GLSLAsset() {
  logger.Info("Destroyed GLSL Shader [%s] ==> [%s][%s]",
              boost::uuids::to_string(uuid).c_str(),
              GetMetaObject("Type").c_str(), GetMetaObject("Version").c_str());
}

[[nodiscard]] std::string GLSLAsset::GetRequired() noexcept {
  std::string required;
  auto requires = GetMetaObject("Requires");
  if (requires.empty()) {
    return required;
  }

  std::vector<std::string> requirements;
  boost::split(requirements, requires, [](char c) { return c == ':'; });

  for (auto &req : requirements) {
    logger.Info("Welll!!! Getting required shader [%s]", req.c_str());
  }

  return required;
}

[[nodiscard]] const std::string GLSLAsset::GetShaderText() noexcept {
  if (!IsLoaded()) {
    Load();
  }
  return std::string(reinterpret_cast<const char *>(GetData()), size);
}

[[nodiscard]] bool GLSLAsset::ParseInternal() noexcept {
  auto start = std::chrono::system_clock::now();

  const std::string shaderString = GetShaderText();

  // Add GLSL modules
  shader.addSource(GetRequired());
  // Add shader source
  shader.addSource(shaderString);

  parsed = shader.compile();
  if (!parsed) {
    logger.Error("Failed to compile shader!");
    DumpShaderCode();
    abort();
  }

  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
                      std::chrono::system_clock::now() - start)
                      .count();
  logger.Info("Compiled in [%d] us", duration);

  return parsed;
}

void GLSLAsset::DumpShaderCode() const noexcept {
  logger.Info("Shader code : ");
  for (const auto &chunk : shader.sources()) {
    logger.Info("%s", chunk.c_str());
  }
}