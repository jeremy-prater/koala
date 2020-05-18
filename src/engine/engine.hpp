#pragma once

#include "debuglogger/debuglogger.hpp"
#include "engine/typedefs.hpp"
#include "objects/scene.hpp"
#include "rapidjson/document.h"
#include <Corrade/PluginManager/Manager.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/SceneGraph/Camera.h>
#include <Magnum/SceneGraph/Drawable.h>
#include <Magnum/SceneGraph/DualQuaternionTransformation.h>
#include <Magnum/SceneGraph/MatrixTransformation3D.h>
#include <Magnum/SceneGraph/Object.h>
#include <Magnum/SceneGraph/Scene.h>
#include <Magnum/SceneGraph/SceneGraph.h>
#include <Magnum/Trade/AbstractImporter.h>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/string_generator.hpp>
#include <memory>

namespace Koala {
namespace Engine {

class Engine : public Magnum::Platform::Application {
public:
  static boost::uuids::string_generator StringUUIDGenerator;
  static boost::uuids::basic_random_generator<boost::mt19937>
      RandomUUIDGenerator;
  static Magnum::PluginManager::Manager<Magnum::Trade::AbstractImporter>
      MagnumPluginManager;

  explicit Engine(const Arguments &arguments);
  virtual ~Engine();

  std::shared_ptr<Objects::Scene> scene;

private:
  DebugLogger logger;
};

} // namespace Engine
} // namespace Koala
