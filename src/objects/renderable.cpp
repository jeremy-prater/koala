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
      sceneRenderableGroup(SceneRenderableGroup), _mesh(mesh),
      _translation(translation), _rotation(rotation), _scaling(scaling) {}

Renderable::~Renderable() {}

void Renderable::draw(const Magnum::Matrix4 &transformationMatrix,
                      Magnum::SceneGraph::Camera3D &camera) {

  const Magnum::Matrix4 localMatrix =
      Magnum::Matrix4::from(_rotation.toMatrix(), _translation);

  sceneRenderableGroup->shaderProgram
      .setShaderUniform<const Magnum::Matrix4 &>("localMatrix", localMatrix)
      .setShaderUniform<const Magnum::Matrix4 &>("transformationMatrix",
                                                 transformationMatrix)
      .setShaderUniform<const Magnum::Matrix4 &>("projectionMatrix",
                                                 camera.projectionMatrix())
      .draw(_mesh);
}
