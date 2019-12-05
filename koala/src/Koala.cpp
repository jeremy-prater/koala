#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/Platform/Sdl2Application.h>

using namespace Magnum;

class Koala: public Platform::Application {
    public:
        explicit Koala(const Arguments& arguments);

    private:
        void drawEvent() override;
};

Koala::Koala(const Arguments& arguments): Platform::Application{arguments} {
    /* TODO: Add your initialization code here */
}

void Koala::drawEvent() {
    GL::defaultFramebuffer.clear(GL::FramebufferClear::Color);

    /* TODO: Add your drawing code here */

    swapBuffers();
}

MAGNUM_APPLICATION_MAIN(Koala)
