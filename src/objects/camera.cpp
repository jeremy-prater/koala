
#include "objects/camera.hpp"
#include "koala-version.hpp"

using namespace Koala::Objects;
using namespace Magnum;
using namespace Math::Literals;

Camera::Camera(const std::string &name, std::shared_ptr<Scene> scene)
    : BaseObject("Camera-" + name, this), cameraLens{camera}, cameraName(name) {
  logger.Info("Camera Created");
  camera.setParent(scene.get());
  cameraLens
      .setProjectionMatrix(
          Matrix4::perspectiveProjection(70.0_degf, 1.0f, 0.001f, 100.0f))
      .setAspectRatioPolicy(SceneGraph::AspectRatioPolicy::Extend);
}

Camera::~Camera() { logger.Info("Camera Destroyed"); }
