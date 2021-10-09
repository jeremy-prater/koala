#pragma once

#include "engine/classes.hpp"

#include "base-asset.hpp"
#include <Magnum/GL/Shader.h>
#include <Magnum/GL/Version.h>
#include <memory>

namespace Koala {
namespace Assets {

class GLSLAsset : public BaseAsset {
public:
  GLSLAsset(const std::string &rootDir,
            rapidjson::GenericObject<false, rapidjson::Value::ValueType> props,
            Project *project);
  virtual ~GLSLAsset();
  [[nodiscard]] virtual bool ParseInternal() noexcept override;
  void DumpShaderCode() const noexcept;

  Magnum::GL::Shader shader;

private:
  static const std::unordered_map<std::string, const Magnum::GL::Version>
      shaderVersions;
  static const std::unordered_map<std::string, const Magnum::GL::Shader::Type>
      shaderTypes;
  Magnum::GL::Version
  GetShaderVersionFromString(const std::string &version) const noexcept;
  Magnum::GL::Shader::Type
  GetShaderTypeFromString(const std::string &type) const noexcept;

  [[nodiscard]] std::string GetRequired() noexcept;

  [[nodiscard]] const std::string GetShaderText() noexcept;

  DebugLogger logger;
};

} // namespace Assets
} // namespace Koala