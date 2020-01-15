#pragma once

#include "base-asset.hpp"
#include <Magnum/GL/Shader.h>
#include <Magnum/GL/Version.h>

namespace Koala {

class GLSLAsset : public BaseAsset {
public:
  GLSLAsset(rapidjson::GenericObject<false, rapidjson::Value::ValueType> props,
             const std::string rootDir);
  virtual ~GLSLAsset();
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