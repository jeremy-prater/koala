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
    const Magnum::Vector3 &scaling, Magnum::GL::Mesh &mesh,
    const Magnum::Matrix4 &parentTransformation)
    : BaseObject(name, parent),
      SceneGraph::Drawable3D{*this, SceneRenderableGroup->getInstance()},
      sceneRenderableGroup(SceneRenderableGroup), _mesh(mesh),
      _translation(translation), _rotation(rotation), _scaling(scaling),
      _parentTransform(parentTransformation) {}

Renderable::~Renderable() {}

void Renderable::draw(const Magnum::Matrix4 &transformationMatrix,
                      Magnum::SceneGraph::Camera3D &camera) {
  auto rotationMatrix = _rotation.toMatrix();
  rotationMatrix[0] *= _scaling[0];
  rotationMatrix[1] *= _scaling[1];
  rotationMatrix[2] *= _scaling[2];
  const Magnum::Matrix4 localMatrix =
      Magnum::Matrix4::from(rotationMatrix, _translation);
  sceneRenderableGroup->shaderProgram
      .setShaderUniform<const Magnum::Matrix4 &>("localMatrix",
                                                 _parentTransform * localMatrix)
      .setShaderUniform<const Magnum::Matrix4 &>("transformationMatrix",
                                                 transformationMatrix)
      .setShaderUniform<const Magnum::Matrix4 &>("projectionMatrix",
                                                 camera.projectionMatrix()),
      sceneRenderableGroup->shaderProgram.draw(_mesh);
}
