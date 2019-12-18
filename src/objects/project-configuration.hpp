#include "debuglogger.hpp"
#include "koala-object.hpp"
#include "rapidjson/document.h"

using namespace Magnum;

namespace Koala {

class Project {
public:
  Project(const std::string path);

private:
  const std::string rootDir;
  DebugLogger logger;
};
} // namespace Koala
