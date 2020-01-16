#include "engine/engine.hpp"

using namespace Koala::Engine;
using namespace Magnum;

class KoalaTest : public Engine {
public:
  KoalaTest(const Arguments &arguments);
  virtual ~KoalaTest();

private:
  void drawEvent() override;
};

KoalaTest::KoalaTest(const Arguments &arguments) : Engine(arguments) {}
KoalaTest::~KoalaTest() {}

void KoalaTest::drawEvent() {
  GL::defaultFramebuffer.clear(GL::FramebufferClear::Color);

  /* TODO: Add your drawing code here */

  swapBuffers();
}

MAGNUM_APPLICATION_MAIN(KoalaTest)
