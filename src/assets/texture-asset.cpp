#include "texture-asset.hpp"
#include "engine/engine.hpp"
#include <Corrade/Containers/ArrayView.h>
#include <Corrade/Utility/Resource.h>
#include <Magnum/GL/TextureFormat.h>
#include <Magnum/ImageView.h>
#include <boost/uuid/uuid_io.hpp>
#include <chrono>

using namespace Koala::Assets;

static Magnum::Containers::Pointer<Magnum::Trade::AbstractImporter>
    textureImporter = nullptr;
static std::mutex textureImporterMutex;

TextureAsset::TextureAsset(
    const std::string &rootDir,
    rapidjson::GenericObject<false, rapidjson::Value::ValueType> props,
    Project *project)
    : BaseAsset(rootDir, props, project),
      logger("Texture-Asset-" + path + "/" + name,
             DebugLogger::DebugColor::COLOR_GREEN, false) {
  logger.Info("Created Texture [%s]", boost::uuids::to_string(uuid).c_str());
}

TextureAsset::~TextureAsset() {
  logger.Info("Destroyed Texture [%s]", boost::uuids::to_string(uuid).c_str());
}

[[nodiscard]] bool TextureAsset::ParseInternal() noexcept {
  // RAII lock the importer
  std::scoped_lock<std::mutex> lock(textureImporterMutex);

  auto start = std::chrono::system_clock::now();

  if (!textureImporter) {
    logger.Info("Creating AnyImageImporter...");
    textureImporter =
        Koala::Engine::Engine::MagnumPluginManager.loadAndInstantiate(
            "AnyImageImporter");
    if (!textureImporter.operator bool()) {
      logger.Error("Failed to load AnyImageImporter! abort()");
      abort();
    }
  }

  const char *textureData = reinterpret_cast<const char *>(GetData());

  textureImporter->openData(Corrade::Containers::ArrayView<const char>{
      textureData, static_cast<size_t>(size)});

  image = textureImporter->image2D(0);

  auto format = image->format();
  auto resolution = image->size();

  logger.Info("Loaded image %d %dx%d", format, resolution[0], resolution[1]);
  // Magnum::ImageView2D imageView{format, resolution, image->data()};

  texture.setMagnificationFilter(Magnum::GL::SamplerFilter::Linear)
      .setMinificationFilter(Magnum::GL::SamplerFilter::Linear,
                             Magnum::GL::SamplerMipmap::Linear)
      .setWrapping(Magnum::GL::SamplerWrapping::ClampToEdge)
      .setMaxAnisotropy(Magnum::GL::Sampler::maxMaxAnisotropy())
      .setStorage(log2(2048) + 1, Magnum::GL::textureFormat(image->format()),
                  image->size())
      .setSubImage(0, {}, *image)
      .generateMipmap();

  // Koala::Engine manager.loadAndInstantiate("AnyImageImporter");

  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
                      std::chrono::system_clock::now() - start)
                      .count();
  logger.Info("Loaded in [%d] us", duration);

  parsed = true;

  return parsed;
}

void TextureAsset::bindToSlot(uint32_t slot) {
  // logger.Info("Binding to texture slot [%d]", slot);
  texture.bind(slot);
}