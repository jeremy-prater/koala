#pragma once

#include "debuglogger.hpp"
#include <boost/signals2.hpp>
#include <memory>
#include <mutex>
#include <rapidjson/rapidjson.h>
#include <unordered_map>
#include <vector>

namespace Koala {

class BaseObject {
public:
  static std::shared_ptr<BaseObject> CreateObject(
      const std::string rootDir,
      rapidjson::GenericObject<false, rapidjson::Value::ValueType> props);

  [[nodiscard]] const std::string GetUUID() const noexcept;
  [[nodiscard]] const std::string GetPath() const noexcept;
  [[nodiscard]] const std::string GetName() const noexcept;
  [[nodiscard]] const std::string GetParser() const noexcept;

  void AddTag(const std::string tag) noexcept;
  [[nodiscard]] bool DeleteTag(const std::string tag) noexcept;
  [[nodiscard]] bool HasTag(const std::string tag) const noexcept;

  void Load();
  void Unload();
  [[nodiscard]] bool IsLoaded() const noexcept;

  [[nodiscard]] virtual bool Parse() noexcept = 0;
  [[nodiscard]] bool IsParsed() const noexcept;

  [[nodiscard]] const uint8_t *GetData() const noexcept;

  [[nodiscard]] const std::string GetMetaObject(const std::string key) const noexcept;
  void SetMetaObject(const std::string key, const std::string value) noexcept;

  BaseObject(rapidjson::GenericObject<false, rapidjson::Value::ValueType> props,
             const std::string rootDir);
  virtual ~BaseObject();

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
  const std::string uuid;
  const std::string path;
  const std::string name;
  const std::string parser;
  const ssize_t size;
  const std::string md5Sum;
  const std::string rootDir;
  bool parsed;

private:
  mutable std::mutex loadLock;
  uint8_t *data;

  mutable std::mutex tagsLock;
  std::vector<std::string> tags;

  mutable std::mutex metaObjectLock;
  std::unordered_map<std::string, std::string> metaObjects;

  DebugLogger logger;
};
} // namespace Koala