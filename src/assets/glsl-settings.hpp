#pragma once

#include <mutex>
#include <string>
#include <unordered_map>

namespace Koala {
namespace Settings {
class GLSLSettings {
public:
  static void InitDefaultGlobalSettings();
  static void SetGlobalSetting(const std::string &key,
                               const std::string &value) noexcept;
  static const std::string GetGlobalSetting(const std::string &key) noexcept;

private:
  static std::mutex globalSettingsLock;
  static std::unordered_map<std::string, std::string> globalSettings;
};
} // namespace Settings
} // namespace Koala