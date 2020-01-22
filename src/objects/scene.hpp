#pragma once

#include "debuglogger/debuglogger.hpp"
#include "engine/typedefs.hpp"
#include "objects/scene-renderable-groups.hpp"
#include "rapidjson/document.h"
#include "scene.hpp"
#include <Magnum/SceneGraph/Scene.h>
#include <Magnum/SceneGraph/SceneGraph.h>

namespace Koala {
namespace Objects {

class Scene : public Scene3D, SceneRenderableGrouping {
public:
  Scene();
  virtual ~Scene();

private:
  DebugLogger logger;
};

} // namespace Objects
} // namespace Koala
