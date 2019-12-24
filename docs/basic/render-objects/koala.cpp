#include "koala-engine.hpp"
#include "project-configuration.hpp"
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

using namespace Koala;

class KoalaTest : public Engine {
public:
  KoalaTest(const Arguments &arguments);
  virtual ~KoalaTest();

private:
  DebugLogger logger;
};

KoalaTest::KoalaTest(const Arguments &arguments)
    : Engine(arguments),
      logger("render-objects", DebugLogger::DebugColor::COLOR_BLUE, false) {
  if (arguments.argc != 2) {
    logger.Error("No path given! Usage : %s <path to project>",
                 arguments.argv[0]);
    exit(-1);
  }

  const std::string projectRoot = arguments.argv[1];

  struct stat projectInfo;
  if (stat(projectRoot.c_str(), &projectInfo) != 0) {
    logger.Error("Failed to stat config [%s] ==> [%s]", projectRoot.c_str(),
                 strerror(errno));
    exit(-errno);
  }

  if (!S_ISDIR(projectInfo.st_mode)) {
    logger.Error("[%s] is not a directory!", projectRoot.c_str());
    exit(-1);
  }

  logger.Info("Opening %s", projectRoot.c_str());

  Koala::Project project(projectRoot);

  auto uuids = project.GetObjectUUIDs();
  for (auto uuid : uuids) {
    auto object = project.GetObject(uuid);
    object->Load();
  }

  for (auto uuid : uuids) {
    auto object = project.GetObject(uuid);
    object->Parse();
  }

}
KoalaTest::~KoalaTest() {}

MAGNUM_APPLICATION_MAIN(KoalaTest)
