#include "objects/renderable.hpp"
#include "engine/typedefs.hpp"
#include <Magnum/SceneGraph/Object.h>

using namespace Koala::Objects;
using namespace Magnum;

Renderable::Renderable(const std::string name, const uint32_t groupId,
                       std::shared_ptr<Scene> scene)
    : SceneGraph::Drawable3D{this, scene->GetRenderGroupByID(groupId).get()},
      logger("Renderable-" + name, DebugLogger::DebugColor::COLOR_YELLOW,
             false) {}

Renderable::~Renderable() {}
