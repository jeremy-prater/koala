#pragma once

#include "debuglogger.hpp"
#include "rapidjson/document.h"
#include "scene.hpp"
#include "typedefs.hpp"
#include <Magnum/SceneGraph/Scene.h>
#include <Magnum/SceneGraph/SceneGraph.h>

namespace Koala {

class Scene : public Scene3D {
public:
  Scene();
  virtual ~Scene();
  void DumpScene() const noexcept;

private:
  void DumpScene(const Object3D &parent, const int depth) const noexcept;

  DebugLogger logger;
};

} // namespace Koala
