#pragma once

#include "assets/project-configuration.hpp"
#include "debuglogger/debuglogger.hpp"
#include "engine/classes.hpp"

#include <boost/signals2.hpp>
#include <boost/uuid/uuid.hpp>
#include <memory>
#include <mutex>
#include <rapidjson/pointer.h>
#include <rapidjson/rapidjson.h>
#include <unordered_map>
#include <vector>

namespace Koala {
namespace Assets {

struct BaseAssetException : public std::exception {
  std::string s;
  BaseAssetException(const std::string &ss) : s(ss) {}
  ~BaseAssetException() throw() {}
  const char *what() const throw() { return s.c_str(); }
};

class BaseAsset {
public:
  enum class AssetType : uint32_t {
    Unknown = 0x00000000,
    GLTF = 0x00000001,
    GLSL = 0x00000002,
    PNG = 0x00000003,
  };

  static std::shared_ptr<BaseAsset> CreateAsset(
      const std::string &rootDir,
      rapidjson::GenericObject<false, rapidjson::Value::ValueType> props);

  [[nodiscard]] const boost::uuids::uuid GetUUID() const noexcept;
  [[nodiscard]] const std::string GetPath() const noexcept;
  [[nodiscard]] const std::string GetFullPath() const noexcept;
  [[nodiscard]] const std::string GetName() const noexcept;
  [[nodiscard]] const std::string GetParser() const noexcept;
  [[nodiscard]] AssetType GetType() const noexcept;

  void AddTag(const std::string &tag) noexcept;
  [[nodiscard]] bool DeleteTag(const std::string &tag) noexcept;
  [[nodiscard]] bool HasTag(const std::string &tag) const noexcept;

  void Load();
  void Unload();
  [[nodiscard]] bool IsLoaded() const noexcept;

  [[nodiscard]] bool Parse() noexcept;
  [[nodiscard]] bool IsParsed() const noexcept;

  void LoadParse() noexcept;

  [[nodiscard]] const uint8_t *GetData() const noexcept;

  [[nodiscard]] const std::string &
  GetMetaObject(const std::string &key) const noexcept;
  void SetMetaObject(const std::string &key, const std::string &value) noexcept;

  BaseAsset(rapidjson::GenericObject<false, rapidjson::Value::ValueType> props,
            const std::string &rootDir);
  virtual ~BaseAsset();

  // Life cycle events
  boost::signals2::signal<void()> onLoadComplete();

  boost::signals2::signal<void()> onParsed();

  boost::signals2::signal<void()> onRunning();

  // Object is alive and steady state

  // User implementation here

  // Object shutdown

  boost::signals2::signal<void()> onShutdown();

  boost::signals2::signal<void()> onReleased();

protected:
  const boost::uuids::uuid uuid;
  const std::string path;
  const std::string name;
  const std::string fullPath;
  const std::string parser;
  const AssetType type;
  const size_t size;
  const std::string md5Sum;
  const std::string rootDir;
  bool parsed;

  [[nodiscard]] static AssetType
  ConvertAssetType(const std::string &parser) noexcept;

  [[nodiscard]] virtual bool ParseInternal() noexcept = 0;

private:
  mutable std::mutex loadLock;
  uint8_t *data;

  mutable std::mutex tagsLock;
  std::vector<std::string> tags;

  mutable std::mutex metaObjectLock;
  std::unordered_map<std::string, std::string> metaObjects;

  DebugLogger logger;
};

} // namespace Assets
} // namespace Koala