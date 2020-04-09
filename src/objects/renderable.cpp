#include "objects/renderable.hpp"
#include "engine/typedefs.hpp"
#include <Magnum/SceneGraph/Object.h>

using namespace Koala::Objects;
using namespace Magnum;

Renderable::Renderable(const std::string &name,
                       std::shared_ptr<Koala::Objects::SceneRenderableGroup>
                           SceneRenderableGroup,
                       std::shared_ptr<Scene> scene)
    : BaseObject("Renderable-" + name),
      SceneGraph::Drawable3D{*this, SceneRenderableGroup->getInstance()} {}

Renderable::~Renderable() {}
