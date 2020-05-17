#pragma once

#include "engine/classes.hpp"

#include "assets/base-group.hpp"
#include "debuglogger/debuglogger.hpp"
#include "engine/typedefs.hpp"
#include "objects/base-object.hpp"
#include "objects/scene.hpp"
#include <Magnum/GL/Mesh.h>
#include <Magnum/SceneGraph/Drawable.h>
#include <memory>
#include <rapidjson/pointer.h>
#include <unordered_map>

namespace Koala {
namespace Objects {

class Scene;

class Renderable : public BaseObject, public Magnum::SceneGraph::Drawable3D {
public:
  explicit Renderable(const std::string &name, BaseObject *parent,
                      std::shared_ptr<Koala::Objects::SceneRenderableGroup>
                          SceneRenderableGroup,
                      const Magnum::Vector3 &translation,
                      const Magnum::Quaternion &rotation,
                      const Magnum::Vector3 &scaling, Magnum::GL::Mesh &mesh,
                      const Magnum::Matrix4 &parentTransformation);
  ~Renderable();

private:
  void draw(const Magnum::Matrix4 &transformationMatrix,
            Magnum::SceneGraph::Camera3D &camera) override;

  std::shared_ptr<Koala::Objects::SceneRenderableGroup> sceneRenderableGroup;
  Magnum::GL::Mesh &_mesh;
  Magnum::Vector3 _translation;
  Magnum::Quaternion _rotation;
  Magnum::Vector3 _scaling;
  const Magnum::Matrix4 _parentTransform;

  std::unordered_map<Koala::Assets::BaseGroup::NodeType,
                     std::shared_ptr<Koala::Assets::BaseAsset>>
      assets;
  std::unordered_map<std::string, std::string> properties;
};

} // namespace Objects
} // namespace Koala