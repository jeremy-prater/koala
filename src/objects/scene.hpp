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

class SceneRenderableGroup;

class Scene : public Scene3D {
public:
  Scene();
  virtual ~Scene();

  void CreateRenderableFromGroup(
      const std::shared_ptr<Koala::Assets::BaseGroup> group);

  SceneRenderableGroup SceneRenderableGroups;

private:
  DebugLogger logger;
};

} // namespace Objects
} // namespace Koala
