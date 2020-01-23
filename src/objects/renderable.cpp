#include "objects/renderable.hpp"
#include "engine/typedefs.hpp"
#include <Magnum/SceneGraph/Object.h>

using namespace Koala::Objects;
using namespace Magnum;

Renderable::Renderable(const std::string name, const uint32_t groupId,
                       std::shared_ptr<Scene> scene)
    : BaseObject("Renderable-" + name),
      SceneGraph::Drawable3D{*this, scene->GetRenderGroupByID(groupId).get()} {}

Renderable::~Renderable() {}
