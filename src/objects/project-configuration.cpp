#include "project-configuration.hpp"

using namespace Koala;

Project::Project(const std::string path)
    : rootDir(path),
      logger("Project-" + path, DebugLogger::DebugColor::COLOR_MAGENTA, false) {
  logger.Info("Loading Project [%s]", rootDir.c_str());
}
