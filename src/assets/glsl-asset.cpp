#include "glsl-asset.hpp"
#include "glsl-settings.hpp"
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
    const std::string &rootDir,
    rapidjson::GenericObject<false, rapidjson::Value::ValueType> props,
    Project *project)
    : BaseAsset(rootDir, props, project),
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

[[nodiscard]] const std::string
GLSLAsset::LoadIncludedShaderText(const std::string shaderPath) noexcept {
  std::string required;
  auto shader = std::dynamic_pointer_cast<Koala::Assets::GLSLAsset>(
      project->GetAssetByPath(shaderPath));

  // TODO : We just crash if we try to find an asset by path that doesn't exist
  // if (!shader.operator bool()) {
  //   required += "// #include : " + shaderPath + " -- Not found!\n";
  //   return required;
  // }

  required = "// #include : " + shaderPath + " -- start\n";
  required += shader->ParsedGetShaderText();
  required += "// #include : " + shaderPath + " -- end\n";

  return required;
}

[[nodiscard]] const std::string GLSLAsset::ParsedGetShaderText() noexcept {
  std::istringstream rawShader(GetShaderText());
  std::ostringstream parsedShader;
  std::string shaderLine;
  while (std::getline(rawShader, shaderLine)) {
    if (shaderLine.rfind("#include", 0) == 0) {
      std::string includeShader =
          shaderLine.substr(strlen("#include "), std::string::npos);

      parsedShader << LoadIncludedShaderText(includeShader);
    } else {
      // Do variable substition
      size_t start = 0;
      size_t pos = 0;
      while ((pos = shaderLine.find("${")) != std::string::npos) {
        auto end = shaderLine.find("}", pos);
        if (end == std::string::npos) {
          break;
        }

        auto token = shaderLine.substr(pos + 2, end - pos - 2);

        auto result = Koala::Settings::GLSLSettings::GetGlobalSetting(token);

        shaderLine = shaderLine.substr(0, pos) + result +
                     shaderLine.substr(end + 1, std::string::npos);
      }

      parsedShader << shaderLine;
    }
    parsedShader << "\n";
  }
  return parsedShader.str();
}

[[nodiscard]] const std::string GLSLAsset::GetShaderText() noexcept {
  if (!IsLoaded()) {
    Load();
  }
  return std::string(reinterpret_cast<const char *>(GetData()), size);
}

[[nodiscard]] bool GLSLAsset::ParseInternal() noexcept {
  auto start = std::chrono::system_clock::now();

  const std::string shaderString = ParsedGetShaderText();

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
  logger.Error("Shader code : ");

  int line = -1;
  for (const auto &chunk : shader.sources()) {
    std::istringstream shaderText(chunk);
    std::string shaderLine;
    while (std::getline(shaderText, shaderLine)) {
      logger.Error(": %05d : %s", line++, shaderLine.c_str());
    }
  }
}