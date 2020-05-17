#pragma once

#include "engine/classes.hpp"

#include "assets/base-group.hpp"
#include "debuglogger/debuglogger.hpp"
#include "engine/typedefs.hpp"
#include "objects/scene-renderable-groups.hpp"
#include "rapidjson/document.h"
#include "scene.hpp"
#include <Magnum/SceneGraph/Scene.h>
#include <Magnum/SceneGraph/SceneGraph.h>

namespace Koala {
namespace Objects {

class Scene : public Scene3D {
public:
  Scene();
  virtual ~Scene();

  void CreateRenderableFromGroup(
      const std::shared_ptr<Koala::Assets::Project> project,
      const std::shared_ptr<Koala::Assets::BaseGroup> group,
      const Magnum::Matrix4 &identityTransform);

private:
  DebugLogger logger;
};

} // namespace Objects
} // namespace Koala
