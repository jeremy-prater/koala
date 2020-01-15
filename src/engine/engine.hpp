#pragma once

#include "debuglogger.hpp"
#include "rapidjson/document.h"
#include "scene.hpp"
#include "typedefs.hpp"
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/SceneGraph/Camera.h>
#include <Magnum/SceneGraph/Drawable.h>
#include <Magnum/SceneGraph/DualQuaternionTransformation.h>
#include <Magnum/SceneGraph/MatrixTransformation3D.h>
#include <Magnum/SceneGraph/Object.h>
#include <Magnum/SceneGraph/Scene.h>
#include <Magnum/SceneGraph/SceneGraph.h>

namespace Koala {

class Engine : public Magnum::Platform::Application {
public:
  explicit Engine(const Arguments &arguments);
  virtual ~Engine();

  Scene scene;

private:
  DebugLogger logger;
};

} // namespace Koala
