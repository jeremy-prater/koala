#include "debuglogger.hpp"
#include "parsers.hpp"
#include "sstream"
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

using namespace Koala;

static DebugLogger logger("ObjParser", DebugLogger::DebugColor::COLOR_YELLOW,
                          false);

[[nodiscard]] ParserResult Parsers::ObjParser(BaseObject *object) noexcept {
  ParserResult result;

  logger.Info("Parsing [%s]", object->GetName().c_str());

  std::stringstream objStream(
      reinterpret_cast<const char *>(object->GetData()));

  while (objStream.good()) {
    std::string line;
    std::getline(objStream, line);
    std::stringstream lineStream(line);
    std::string token;
    lineStream >> token;

    if (token == "v") {
      double x, y, z;
      lineStream >> x >> y >> z;
      // logger.Info("Parsing --> %s (%f, %f, %f)", token.c_str(), x, y, z);
    } else if (token == "vt") {
      double u, v;
      lineStream >> u >> v;
      // logger.Info("Parsing --> %s (%f, %f)", token.c_str(), u, v);
    } else if (token == "vn") {
      double x, y, z;
      lineStream >> x >> y >> z;
      // logger.Info("Parsing --> %s (%f, %f, %f)", token.c_str(), x, y, z);
    } else if (token == "f") {
      std::string face;
      while (lineStream.good()) {
        lineStream >> face;
        std::vector<std::string> indices;
        boost::split(indices, face, boost::algorithm::is_any_of("/"),
                     boost::token_compress_on);
        // logger.Info("Parsing --> %d", indices.size());
        int v = std::stoi(indices[0]);
        int vt = std::stoi(indices[1]);
        int vn = std::stoi(indices[2]);
      }
      // logger.Info("Parsing --> %s (%f, %f, %f)", token.c_str(), x, y, z);
    }
  }

  return result;
}
