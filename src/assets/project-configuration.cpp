#include "project-configuration.hpp"
#include "rapidjson/filereadstream.h"
#include <cstdio>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

using namespace Koala::Assets;

Project::Project(const std::string &path, const std::string &defaultConfigFile)
    : rootDir(path), configFile(defaultConfigFile),
      logger("Project-" + path, DebugLogger::DebugColor::COLOR_GREEN, false) {
  const std::string configFilePath = rootDir + "/" + configFile;

  struct stat configStats = {};
  if (stat(configFilePath.c_str(), &configStats) != 0) {
    logger.Error("Failed to stat project config file [%s]",
                 configFilePath.c_str());
  }

  char *configRawBuffer = static_cast<char *>(malloc(configStats.st_size));

  FILE *configFileStream = fopen(configFilePath.c_str(), "rb");

  rapidjson::FileReadStream jsonInputStream(configFileStream, configRawBuffer,
                                            configStats.st_size);
  configDocument.ParseStream(jsonInputStream);
  fclose(configFileStream);
  free(configRawBuffer);

  logger.SetDebugModuleName(std::string("Project-") +
                            configDocument["name"].GetString());

  auto objects = configDocument["assets"].GetArray();
  logger.Info(
      "Loaded Project Config [%s] ==> [%d bytes]... Creating [%d] objects",
      configFile.c_str(), configStats.st_size, objects.Size());

  {
    std::scoped_lock<std::mutex> lock(assetsMutex);
    for (auto &object : objects) {

      auto newObject = BaseAsset::CreateObject(path, object.GetObject());
      this->assets[newObject->GetUUID()] = newObject;
      this->assetsByPath[newObject->GetPath() + "/" + newObject->GetName()] =
          newObject;
    }
  }
}

[[nodiscard]] std::vector<std::string> Project::GetAssetUUIDs() const
    noexcept {
  std::scoped_lock<std::mutex> lock(assetsMutex);
  std::vector<std::string> keys;
  for (const auto &[key, value] : assets) {
    keys.push_back(key);
  }
  return keys;
}

[[nodiscard]] std::shared_ptr<BaseAsset>
Project::GetAsset(const std::string &uuid) const noexcept {
  std::scoped_lock<std::mutex> lock(assetsMutex);
  return assets.at(uuid);
}

[[nodiscard]] std::shared_ptr<BaseAsset>
Project::GetAssetByPath(const std::string &path) const noexcept {
  std::scoped_lock<std::mutex> lock(assetsMutex);
  return assetsByPath.at(path);
}

[[nodiscard]] std::shared_ptr<BaseGroup>
Project::GetGroup(const std::string &uuid) const noexcept {
  std::scoped_lock<std::mutex> lock(groupsMutex);
  return groups.at(uuid);
}

[[nodiscard]] std::shared_ptr<BaseGroup>
Project::GetGroupByPath(const std::string &path) const noexcept {
  std::scoped_lock<std::mutex> lock(groupsMutex);
  return groupsByPath.at(path);
}