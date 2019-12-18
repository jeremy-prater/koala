#include "koala-engine.hpp"
#include "project-configuration.hpp"

using namespace Koala;

class KoalaTest : public Engine {
public:
  KoalaTest(const Arguments &arguments);
  virtual ~KoalaTest();

private:
  DebugLogger logger;
};

KoalaTest::KoalaTest(const Arguments &arguments) : Engine(arguments), logger("render-objects", DebugLogger::DebugColor::COLOR_BLUE, false) {
  logger.Info("Using path : %d", arguments.argc)
}
KoalaTest::~KoalaTest() {}

MAGNUM_APPLICATION_MAIN(KoalaTest)
