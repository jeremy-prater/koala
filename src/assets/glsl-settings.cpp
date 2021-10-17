#include "glsl-settings.hpp"

using namespace Koala;
using namespace Settings;

std::mutex GLSLSettings::globalSettingsLock;
std::unordered_map<std::string, std::string> GLSLSettings::globalSettings;

void GLSLSettings::InitDefaultGlobalSettings() {
  std::scoped_lock<std::mutex> lock(globalSettingsLock);
  globalSettings["graphics_lighting_max_directional"] = "2";
  globalSettings["graphics_lighting_max_point"] = "5";
  globalSettings["graphics_lighting_max_spot"] = "5";
}

void GLSLSettings::SetGlobalSetting(const std::string &key,
                                    const std::string &value) noexcept {
  std::scoped_lock<std::mutex> lock(globalSettingsLock);
  globalSettings[key] = std::string(value);
}

const std::string
GLSLSettings::GetGlobalSetting(const std::string &key) noexcept {
  std::scoped_lock<std::mutex> lock(globalSettingsLock);
  return globalSettings.at(key);
}
