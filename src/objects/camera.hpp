#pragma once

#include "engine/classes.hpp"

#include "debuglogger/debuglogger.hpp"
#include "engine/typedefs.hpp"
#include "objects/base-object.hpp"
#include "objects/scene.hpp"
#include <Magnum/SceneGraph/Camera.h>
#include <Magnum/SceneGraph/DualQuaternionTransformation.h>
#include <Magnum/SceneGraph/MatrixTransformation3D.h>
#include <Magnum/SceneGraph/Scene.h>
#include <Magnum/SceneGraph/SceneGraph.h>
#include <memory>
// #include <Magnum/SceneGraph/Object.h>
// #include <Magnum/SceneGraph/Camera.h>
// #include <Magnum/SceneGraph/Drawable.h>
// #include <Magnum/SceneGraph/DualQuaternionTransformation.h>
// #include <Magnum/SceneGraph/Object.h>
// #include <Magnum/SceneGraph/Scene.h>
// #include <Magnum/SceneGraph/SceneGraph.h>

namespace Koala {
namespace Objects {

class Camera : BaseObject {
public:
  Camera(const std::string &name, std::shared_ptr<Scene> scene);
  ~Camera();

private:
  const std::string cameraName;

  Object3D camera;
  Magnum::SceneGraph::Camera3D cameraLens;
};

} // namespace Objects
} // namespace Koala