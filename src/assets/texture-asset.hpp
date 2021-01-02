#pragma once

#include <Magnum/GL/Texture.h>
#include <Corrade/Containers/Optional.h>
#include <Magnum/Trade/ImageData.h>

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

  void bindToSlot(uint32_t slot);

  Magnum::GL::Texture2D texture;

private:
  DebugLogger logger;
  Corrade::Containers::Optional<Magnum::Trade::ImageData2D> image;
};

} // namespace Assets
} // namespace Koala