#include "objects/renderable.hpp"
#include "assets/glsl-asset.hpp"
#include "engine/typedefs.hpp"
#include <Magnum/SceneGraph/Object.h>
#include <memory>

using namespace Koala::Objects;
using namespace Magnum;

Renderable::Renderable(
    const std::string &name, BaseObject *parent,
    std::shared_ptr<Koala::Objects::SceneRenderableGroup> SceneRenderableGroup,
    const Magnum::Vector3 &translation, const Magnum::Quaternion &rotation,
    const Magnum::Vector3 &scaling, Magnum::GL::Mesh &mesh)
    : BaseObject(name, parent),
      SceneGraph::Drawable3D{*this, SceneRenderableGroup->getInstance()},
      sceneRenderableGroup(SceneRenderableGroup), _mesh(mesh) {}

Renderable::~Renderable() {}

void Renderable::draw(const Magnum::Matrix4 &transformationMatrix,
                      Magnum::SceneGraph::Camera3D &camera) {

  // auto &props = sceneRenderableGroup->assetMap;
  sceneRenderableGroup->shaderProgram.draw(_mesh);
}
