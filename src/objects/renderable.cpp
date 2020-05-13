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
    const Magnum::Vector3 &scaling, const Magnum::GL::Mesh &mesh)
    : BaseObject(name, parent),
      SceneGraph::Drawable3D{*this, SceneRenderableGroup->getInstance()},
      sceneRenderableGroup(SceneRenderableGroup), _mesh(mesh) {}

Renderable::~Renderable() {}

void Renderable::draw(const Magnum::Matrix4 &transformationMatrix,
                      Magnum::SceneGraph::Camera3D &camera) {

  auto &props = sceneRenderableGroup->assetMap;
  auto vertexShader =
      props.find(Koala::Assets::BaseGroup::NodeType::VertexShader);

  auto fragmentShader =
      props.find(Koala::Assets::BaseGroup::NodeType::FragmentShader);

  if (vertexShader != props.end()) {
    logger.Info("We have a VertexShader!");
    auto shader = std::dynamic_pointer_cast<Koala::Assets::GLSLAsset>(
        vertexShader->second);
    shader
        ->shader
        .uniformLocation("transformationMatrix");
        //.setTransformationMatrix(transformationMatrix)
        //.setProjectionMatrix(camera.projectionMatrix())
        //.draw(_mesh);
  }

  if (fragmentShader != props.end()) {
    logger.Info("We have a FragmentShader!");
  }
}
