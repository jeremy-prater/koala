#include "koala-engine.hpp"

class KoalaTest : public KoalaEngine {
public:
  KoalaTest(const Arguments &arguments);
  virtual ~KoalaTest();
};

KoalaTest::KoalaTest(const Arguments &arguments) : KoalaEngine(arguments) {}
KoalaTest::~KoalaTest() {}

MAGNUM_APPLICATION_MAIN(KoalaTest)
