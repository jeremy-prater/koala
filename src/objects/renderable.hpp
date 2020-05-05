#pragma once

#include "engine/classes.hpp"

#include "debuglogger/debuglogger.hpp"
#include "engine/typedefs.hpp"
#include "objects/base-object.hpp"
#include "objects/scene.hpp"
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
                          SceneRenderableGroup);
  ~Renderable();

private:
  void draw(const Magnum::Matrix4 &transformationMatrix,
            Magnum::SceneGraph::Camera3D &camera) override;

  std::unordered_map<std::string, std::string> properties;
};

} // namespace Objects
} // namespace Koala