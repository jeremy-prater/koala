#include "png-asset.hpp"
#include <boost/uuid/uuid_io.hpp>
#include <chrono>

using namespace Koala::Assets;

PNGAsset::PNGAsset(
    rapidjson::GenericObject<false, rapidjson::Value::ValueType> props,
    const std::string &rootDir)
    : BaseAsset(props, rootDir),
      logger("PNG-Object-" + path + "/" + name,
             DebugLogger::DebugColor::COLOR_GREEN, false) {
  logger.Info("Created PNG Texture [%s]",
              boost::uuids::to_string(uuid).c_str());
}

PNGAsset::~PNGAsset() {
  logger.Info("Destroyed PNG Texture [%s]",
              boost::uuids::to_string(uuid).c_str());
}

[[nodiscard]] bool PNGAsset::ParseInternal() noexcept {
  auto start = std::chrono::system_clock::now();

  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
                      std::chrono::system_clock::now() - start)
                      .count();
  logger.Info("Loaded in [%d] us", duration);

  return parsed;
}