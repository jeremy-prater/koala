#pragma once

#include <Magnum/GL/Texture.h>

#include "base-asset.hpp"
#include "engine/classes.hpp"

namespace Koala {
namespace Assets {

class TextureAsset : public BaseAsset {
public:
  TextureAsset(
      rapidjson::GenericObject<false, rapidjson::Value::ValueType> props,
      const std::string &rootDir);
  virtual ~TextureAsset();
  [[nodiscard]] virtual bool ParseInternal() noexcept override;

  Magnum::GL::Texture2D texture;

private:
  DebugLogger logger;
};

} // namespace Assets
} // namespace Koala