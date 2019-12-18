#include "koala-engine.hpp"

using namespace Koala;

class KoalaTest : public Engine {
public:
  KoalaTest(const Arguments &arguments);
  virtual ~KoalaTest();
};

KoalaTest::KoalaTest(const Arguments &arguments) : Engine(arguments) {}
KoalaTest::~KoalaTest() {}

MAGNUM_APPLICATION_MAIN(KoalaTest)
