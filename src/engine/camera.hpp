#pragma once

#include "debuglogger.hpp"
#include "typedefs.hpp"
#include <Magnum/SceneGraph/Camera.h>
#include <Magnum/SceneGraph/DualQuaternionTransformation.h>
#include <Magnum/SceneGraph/MatrixTransformation3D.h>
#include <Magnum/SceneGraph/Scene.h>
#include <Magnum/SceneGraph/SceneGraph.h>
// #include <Magnum/SceneGraph/Object.h>
// #include <Magnum/SceneGraph/Camera.h>
// #include <Magnum/SceneGraph/Drawable.h>
// #include <Magnum/SceneGraph/DualQuaternionTransformation.h>
// #include <Magnum/SceneGraph/Object.h>
// #include <Magnum/SceneGraph/Scene.h>
// #include <Magnum/SceneGraph/SceneGraph.h>

namespace Koala {

class Camera {
public:
  Camera(const std::string name, Scene3D *scene);
  ~Camera();

private:
  const std::string cameraName;

  Magnum::SceneGraph::Object<Magnum::SceneGraph::MatrixTransformation3D> camera;
  Magnum::SceneGraph::Camera3D cameraLens;

  DebugLogger logger;
};

} // namespace Koala