#pragma once

#include "debuglogger/debuglogger.hpp"
#include "engine/typedefs.hpp"
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
  void DumpScene() const noexcept;

private:
  void DumpScene(const Object3D &parent, const int depth) const noexcept;

  DebugLogger logger;
};

} // namespace Objects
} // namespace Koala
