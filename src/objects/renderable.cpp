#include "objects/renderable.hpp"
#include "engine/typedefs.hpp"
#include <Magnum/SceneGraph/Object.h>

using namespace Koala::Objects;
using namespace Magnum;

Renderable::Renderable(
    const std::string &name, BaseObject *parent,
    std::shared_ptr<Koala::Objects::SceneRenderableGroup> SceneRenderableGroup)
    : BaseObject(name, parent), SceneGraph::Drawable3D{
                                    *this,
                                    SceneRenderableGroup->getInstance()} {}

Renderable::~Renderable() {}

void Renderable::draw(const Magnum::Matrix4 &transformationMatrix,
                      Magnum::SceneGraph::Camera3D &camera) {
  logger.Info("draw");
}
