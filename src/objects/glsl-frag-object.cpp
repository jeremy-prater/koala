#include "glsl-frag-object.hpp"
#include <chrono>

using namespace Koala;

GLSLFragObject::GLSLFragObject(
    rapidjson::GenericObject<false, rapidjson::Value::ValueType> props,
    const std::string rootDir)
    : BaseObject(props, rootDir),
      logger("GLSL-Frag-Object-" + path + "/" + name,
             DebugLogger::DebugColor::COLOR_GREEN, false) {
  logger.Info("Created GLSL Fragment Shader [%s] ==> [%s]", uuid.c_str(),
              parser.c_str());
}

GLSLFragObject::~GLSLFragObject() {
  logger.Info("Destroyed GLSL Fragment Shader [%s] ==> [%s]", uuid.c_str(),
              parser.c_str());
}

[[nodiscard]] bool GLSLFragObject::Parse() noexcept {
  auto start = std::chrono::system_clock::now();

  parsed = true;

  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
                      std::chrono::system_clock::now() - start)
                      .count();
  logger.Info("Compiled in [%d] us", duration);

  return parsed;
}