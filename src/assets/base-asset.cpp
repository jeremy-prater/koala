#include "base-asset.hpp"
#include "engine/engine.hpp"
#include "glsl-asset.hpp"
#include "gltf-asset.hpp"
#include "texture-asset.hpp"
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#ifdef WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

using namespace Koala::Assets;

[[nodiscard]] BaseAsset::AssetType
BaseAsset::ConvertAssetType(const std::string &parser) noexcept {
  auto lowParser = boost::algorithm::to_lower_copy<const std::string>(parser);

  if (lowParser == "gltf")
    return AssetType::GLTF;
  else if (lowParser == "glsl")
    return AssetType::GLSL;
  else if (lowParser == "png")
    return AssetType::PNG;
  else
    return AssetType::Unknown;
}

std::shared_ptr<BaseAsset> BaseAsset::CreateAsset(
    const std::string &rootDir,
    rapidjson::GenericObject<false, rapidjson::Value::ValueType> props,
    Project *project) {
  std::shared_ptr<BaseAsset> newAsset;

  const std::string parser = props["parser"].GetString();

  if (newAsset.operator bool()) {
    std::scoped_lock<std::mutex> lock(newAsset->tagsLock);
    const std::string tagStringRaw = props["tags"].GetString();
    boost::split(newAsset->tags, tagStringRaw, [](char c) { return c == ' '; });
  }

  if (parser == "gltf") {
    newAsset = std::dynamic_pointer_cast<BaseAsset>(
        std::make_shared<GLTFAsset>(rootDir, props, project));
  } else if (parser == "glsl" || parser == "vert" || parser == "frag") {
    newAsset = std::dynamic_pointer_cast<BaseAsset>(
        std::make_shared<GLSLAsset>(rootDir, props, project));
  } else if (parser == "png") {
    newAsset = std::dynamic_pointer_cast<BaseAsset>(
        std::make_shared<TextureAsset>(rootDir, props, project));
  } else {
    const std::string exceptionString = "Unknown parser type [" + parser + "]";
    throw BaseAssetException(exceptionString);
  }

  return newAsset;
}

BaseAsset::BaseAsset(
    const std::string &rootDir,
    rapidjson::GenericObject<false, rapidjson::Value::ValueType> props,
    Project *project)
    : uuid(Koala::Engine::Engine::StringUUIDGenerator(
          props["uuid"].GetString())),
      path(props["path"].GetString()), name(props["name"].GetString()),
      fullPath(path + "/" + name), parser(props["parser"].GetString()),
      type(ConvertAssetType(parser)), size(props["size"].GetUint()),
      md5Sum(props["hash"].GetString()), rootDir(rootDir), parsed(false),
      data(nullptr), logger("Asset : " + fullPath,
                            DebugLogger::DebugColor::COLOR_GREEN, false) {
      logger.Info("Created Asset [%s] ==> [%s]",

                  boost::uuids::to_string(uuid).c_str(), parser.c_str());
      for (auto &value : props["metadata"].GetObject()) {
        std::string metaName = value.name.GetString();
        std::string metaValue = value.value.GetString();
        metaObjects[metaName] = metaValue;
        logger.Info("Setting asset metadata [%s] ==> [%s]", metaName.c_str(),
                    metaValue.c_str());
      }
    }

    BaseAsset::~BaseAsset() {
      logger.Info("Destroyed Asset [%s] ==> [%s]",
                  boost::uuids::to_string(uuid).c_str(), parser.c_str());
    }

    [[nodiscard]] const boost::uuids::uuid BaseAsset::GetUUID() const noexcept {
      return uuid;
    }
    [[nodiscard]] const std::string BaseAsset::GetPath() const noexcept {
      return path;
    }
    [[nodiscard]] const std::string BaseAsset::GetFullPath() const noexcept {
      return fullPath;
    }
    [[nodiscard]] const std::string BaseAsset::GetName() const noexcept {
      return name;
    }
    [[nodiscard]] const std::string BaseAsset::GetParser() const noexcept {
      return parser;
    }

    [[nodiscard]] BaseAsset::AssetType BaseAsset::GetType() const noexcept {
      return type;
    }

    void BaseAsset::AddTag(const std::string &tag) noexcept {
      std::scoped_lock<std::mutex> lock(tagsLock);
      if (std::find(tags.begin(), tags.end(), tag) == tags.end()) {
        tags.push_back(tag);
      }
    }

    [[nodiscard]] bool BaseAsset::DeleteTag(const std::string &tag) noexcept {
      std::scoped_lock<std::mutex> lock(tagsLock);
      auto it = std::find(tags.begin(), tags.end(), tag);
      if (it != tags.end()) {
        tags.erase(it);
        return true;
      }
      return false;
    }

    [[nodiscard]] bool
    BaseAsset::HasTag(const std::string &tag) const noexcept {
      std::scoped_lock<std::mutex> lock(tagsLock);
      return std::find(tags.begin(), tags.end(), tag) != tags.end();
    }

    void BaseAsset::Load() {
      auto start = std::chrono::system_clock::now();

      std::scoped_lock<std::mutex> lock(loadLock);

      if (data == nullptr) {
        data = static_cast<uint8_t *>(malloc(size));
      } else {
        // logger.Warning("Failed to load : already loaded");
        return;
      }

      if (data == nullptr) {
        logger.Error(
            "Failed to load : Unable allocate memory [%s][%d] ==> [%s]",
            boost::uuids::to_string(uuid).c_str(), size, strerror(errno));
        return;
      }

      const std::string filePath =
          rootDir + "/" + boost::uuids::to_string(uuid);

      auto fd = open(filePath.c_str(), O_RDONLY
#ifdef _WIN64
                                           | O_BINARY
#endif
      );

      if (fd == -1) {
        logger.Error("Failed to load : Unable to open [%s] ==> [%s]",
                     boost::uuids::to_string(uuid).c_str(), strerror(errno));
        free(data);
        data = nullptr;
        return;
      }

      size_t readSize = 0;
      size_t totalRead = 0;
      while (totalRead != size) {
        auto sizeToRead = size - totalRead;
        readSize = read(fd, &data[totalRead], sizeToRead);
        if (readSize == -1) {
          logger.Warning("Failed to load : Failed to read [%s] ==> [%s]",
                         filePath.c_str(), strerror(errno));
          break;
        }
        logger.Info("Reading file! +%d [%d / %d] (+%d)", sizeToRead, totalRead,
                    size, readSize);
        totalRead += readSize;
      }

      if (totalRead != size) {
        logger.Warning("Failed to load : Read size mis-match [%d] != [%d]",
                       readSize, size);
      }

      close(fd);

      logger.Info("Loaded! [%d] us",
                  std::chrono::duration_cast<std::chrono::microseconds>(
                      std::chrono::system_clock::now() - start)
                      .count());
    }

    [[nodiscard]] bool BaseAsset::IsParsed() const noexcept { return parsed; }

    void BaseAsset::Unload() {
      std::scoped_lock<std::mutex> lock(loadLock);
      if (data != nullptr) {
        free(data);
        data = nullptr;
        logger.Info("Unloaded!");
      } else {
        logger.Warning("Failed to unload : not loaded");
      }
    }

    [[nodiscard]] bool BaseAsset::Parse() noexcept {
      if (parsed) {
        // logger.Warning("Already parsed!");
        return parsed;
      }
      return ParseInternal();
    }

    void BaseAsset::LoadParse() noexcept {
      if (!IsLoaded()) {
        Load();
      }

      if (!IsParsed()) {
        if (!Parse()) {
          logger.Error("Failed to parse asset!");
        }
      }
    }

    [[nodiscard]] bool BaseAsset::IsLoaded() const noexcept {
      std::scoped_lock<std::mutex> lock(loadLock);
      return data != nullptr;
    }

    [[nodiscard]] const uint8_t *BaseAsset::GetData() const noexcept {
      return static_cast<const uint8_t *>(data);
    }

    [[nodiscard]] const std::string &
    BaseAsset::GetMetaObject(const std::string &key) const noexcept {
      static const std::string emptyMetaObject = "";

      std::scoped_lock<std::mutex> lock(metaObjectLock);
      auto it = metaObjects.find(key);
      if (it == metaObjects.end()) {
        logger.Warning("No asset metadata key! [%s]", key.c_str());
        return emptyMetaObject;
      }
      return it->second;
    }
    void BaseAsset::SetMetaObject(const std::string &key,
                                  const std::string &value) noexcept {
      std::scoped_lock<std::mutex> lock(metaObjectLock);
      metaObjects[key] = value;
    }
