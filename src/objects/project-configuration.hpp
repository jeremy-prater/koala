#include "debuglogger.hpp"
#include "rapidjson/document.h"
#include "koala-object.hpp"

using namespace Magnum;

namespace Koala {

class Project {
public:
  Project(const std::string rootDir);

private:
  DebugLogger logger;
  
};
} // namespace Koala
