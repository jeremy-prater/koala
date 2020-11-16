#include "texture-asset.hpp"
#include "engine/engine.hpp"
#include <Corrade/Containers/ArrayView.h>
#include <Corrade/Containers/Optional.h>
#include <Corrade/Utility/Resource.h>
#include <Magnum/Trade/ImageData.h>
#include <boost/uuid/uuid_io.hpp>
#include <chrono>

using namespace Koala::Assets;

static Magnum::Containers::Pointer<Magnum::Trade::AbstractImporter>
    textureImporter = nullptr;
static std::mutex textureImporterMutex;

TextureAsset::TextureAsset(
    rapidjson::GenericObject<false, rapidjson::Value::ValueType> props,
    const std::string &rootDir)
    : BaseAsset(props, rootDir),
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

    const char *textureData = reinterpret_cast<const char *>(GetData());
    
    textureImporter->openData(Corrade::Containers::ArrayView<const char>{
        textureData, static_cast<size_t>(size)});

    Corrade::Containers::Optional<Magnum::Trade::ImageData2D> image =
        textureImporter->image2D(0);

    // texture.setWrapping(GL::SamplerWrapping::ClampToEdge)
    //     .setMagnificationFilter(GL::SamplerFilter::Linear)
    //     .setMinificationFilter(GL::SamplerFilter::Linear)
    //     .setStorage(1, GL::textureFormat(image->format()), image->size())
    //     .setSubImage(0, {}, *image);
  }

  // Koala::Engine manager.loadAndInstantiate("AnyImageImporter");

  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
                      std::chrono::system_clock::now() - start)
                      .count();
  logger.Info("Loaded in [%d] us", duration);

  return parsed;
}