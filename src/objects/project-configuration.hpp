#include "debuglogger.hpp"
#include "koala-object.hpp"
#include "rapidjson/document.h"

namespace Koala {

class Project {
public:
  Project(const std::string path, const std::string defaultConfigFile  = "projectConfig.json");

private:
  const std::string rootDir;
  const std::string configFile;
  
  rapidjson::Document configDocument;
  
  DebugLogger logger;
};
} // namespace Koala
