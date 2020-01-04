#pragma once

#include "koala-object.hpp"
#include <Magnum/GL/Shader.h>
#include <Magnum/GL/Version.h>

namespace Koala {

class GLSLObject : public BaseObject {
public:
  GLSLObject(rapidjson::GenericObject<false, rapidjson::Value::ValueType> props,
             const std::string rootDir);
  virtual ~GLSLObject();
  [[nodiscard]] virtual bool Parse() noexcept override;

private:
  static const std::unordered_map<std::string, const Magnum::GL::Version>
      shaderVersions;
  static const std::unordered_map<std::string, const Magnum::GL::Shader::Type>
      shaderTypes;
  const Magnum::GL::Version GetShaderVersionFromString(const std::string);
  const Magnum::GL::Shader::Type GetShaderTypeFromString(const std::string);

  bool parsed;

  Magnum::GL::Shader shader;

  DebugLogger logger;
};
} // namespace Koala