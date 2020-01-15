
#include "camera.hpp"
#include "koala-version.hpp"

using namespace Koala;
using namespace Magnum;
using namespace Math::Literals;

Camera::Camera(const std::string name, Scene3D *scene)
    : cameraName(name), cameraLens{camera},
      logger("Camera-" + cameraName, DebugLogger::DebugColor::COLOR_YELLOW,
             false) {
  logger.Info("Camera Created");
  camera.setParent(scene).translate(Vector3::zAxis(5.0f));
  cameraLens
      .setProjectionMatrix(
          Matrix4::perspectiveProjection(70.0_degf, 1.0f, 0.001f, 100.0f))
      .setAspectRatioPolicy(SceneGraph::AspectRatioPolicy::Extend);
}

Camera::~Camera() { logger.Info("Camera Destroyed"); }
