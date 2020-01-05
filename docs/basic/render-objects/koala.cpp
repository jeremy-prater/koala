#include "koala-engine.hpp"
#include "project-configuration.hpp"
#include <chrono>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

using namespace Koala;

class KoalaTest : public Engine {
public:
  KoalaTest(const Arguments &arguments);
  virtual ~KoalaTest();

private:
  std::shared_ptr<Koala::Project> project;
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
  auto start = std::chrono::system_clock::now();

  project = std::make_shared<Koala::Project>(projectRoot);

  auto uuids = project->GetObjectUUIDs();
  for (auto uuid : uuids) {
    auto object = project->GetObject(uuid);
    object->Load();
    object->Parse();
  }

  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
                      std::chrono::system_clock::now() - start)
                      .count();
  logger.Info("Asset loading complete in [%d] ms", duration);
}
KoalaTest::~KoalaTest() {}

MAGNUM_APPLICATION_MAIN(KoalaTest)
