#pragma once

#include "debuglogger/debuglogger.hpp"
#include "engine/typedefs.hpp"
#include "objects/scene.hpp"
#include "rapidjson/document.h"
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/SceneGraph/Camera.h>
#include <Magnum/SceneGraph/Drawable.h>
#include <Magnum/SceneGraph/DualQuaternionTransformation.h>
#include <Magnum/SceneGraph/MatrixTransformation3D.h>
#include <Magnum/SceneGraph/Object.h>
#include <Magnum/SceneGraph/Scene.h>
#include <Magnum/SceneGraph/SceneGraph.h>
#include <memory>

namespace Koala {
namespace Engine {

class Engine : public Magnum::Platform::Application {
public:
  explicit Engine(const Arguments &arguments);
  virtual ~Engine();

  std::shared_ptr<Objects::Scene> scene;

private:
  DebugLogger logger;
};

} // namespace Engine
} // namespace Koala
