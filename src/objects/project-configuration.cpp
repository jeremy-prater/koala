#include "project-configuration.hpp"
#include "rapidjson/filereadstream.h"
#include <cstdio>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

using namespace Koala;

Project::Project(const std::string path, const std::string defaultConfigFile)
    : rootDir(path), configFile(defaultConfigFile),
      logger("Project-" + path, DebugLogger::DebugColor::COLOR_MAGENTA, false) {
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

  auto objects = configDocument["objects"].GetArray();
  logger.Info(
      "Loaded Project Config [%s] ==> [%d bytes]... Creating [%d] objects",
      configFile.c_str(), configStats.st_size, objects.Size());

  {
    std::scoped_lock<std::mutex> lock(objectsMutex);
    for (auto &object : objects) {

      auto newObject = BaseObject::CreateObject(path, object.GetObject());
      this->objects[newObject->GetUUID()] = newObject;
    }
  }
}

[[nodiscard]] std::vector<std::string> Project::GetObjectUUIDs() const
    noexcept {
  std::scoped_lock<std::mutex> lock(objectsMutex);
  std::vector<std::string> keys;
  for (const auto &[key, value] : objects) {
    keys.push_back(key);
  }
  return keys;
}

[[nodiscard]] std::shared_ptr<BaseObject>
Project::GetObject(std::string uuid) const noexcept {
  std::scoped_lock<std::mutex> lock(objectsMutex);
  return objects.at(uuid);
}
