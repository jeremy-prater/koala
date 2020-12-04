#include "render-objects.hpp"
#include "assets/gltf-asset.hpp"
#include "objects/scene-renderable-groups.hpp"
#include <Magnum/GL/Renderer.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Math/Matrix4.h>

using namespace Koala;
using namespace Magnum;
using namespace Math::Literals;

KoalaTest::KoalaTest(const Arguments &arguments)
    : Engine::Engine(arguments), dTime(0),
      logger("render-objects", DebugLogger::DebugColor::COLOR_BLUE, false) {
  if (arguments.argc != 2) {
    logger.Error("No path given! Usage : %s <path to project>",
                 arguments.argv[0]);
    exit(-1);
  }

  const std::string projectRoot = arguments.argv[1];

  struct stat projectInfo;
  if (stat(projectRoot.c_str(), &projectInfo) != 0) {
    logger.Error("Failed to stat config [%s] ==> [%s]", projectRoot.c_str(),
                 strerror(errno));
    exit(-errno);
  }

  if (!S_ISDIR(projectInfo.st_mode)) {
    logger.Error("[%s] is not a directory!", projectRoot.c_str());
    exit(-1);
  }

  logger.Info("Opening %s", projectRoot.c_str());
  auto start = std::chrono::system_clock::now();

  project = std::make_shared<Assets::Project>(projectRoot);

  auto uuids = project->GetAssetUUIDs();
  for (auto uuid : uuids) {
    auto asset = project->GetAsset(uuid);
    asset->Load();
    asset->Parse();
  }

  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
                      std::chrono::system_clock::now() - start)
                      .count();
  logger.Info("Asset loading complete in [%d] ms", duration);
  lastFrameTime = std::chrono::system_clock::now();

  camera = std::make_unique<Objects::Camera>("default", scene);
  UpdateCameraPosition();

  cloud = std::make_unique<Cloud::Cloud>();

  scene->CreateRenderableFromGroup(
      project, project->GetGroupByPath("/models/grid/the-grid"),
      Magnum::Matrix4::translation({0.0f, 0.0f, 0.0f}));

  scene->CreateRenderableFromGroup(
      project, project->GetGroupByPath("/models/x/the-x"),
      Magnum::Matrix4::translation({0.0f, 0.0f, 0.0f}));

  scene->CreateRenderableFromGroup(
      project, project->GetGroupByPath("/models/o/the-o"),
      Magnum::Matrix4::rotationX(Rad{90.0_degf}) *
          Magnum::Matrix4::translation({10.0f, 0.0f, 0.0f}));

  scene->CreateRenderableFromGroup(
      project, project->GetGroupByPath("/models/o/the-o"),
      Magnum::Matrix4::rotationX(Rad{90.0_degf}) *
          Magnum::Matrix4::translation({0.0f, 10.0f, 0.0f}));

  Magnum::GL::Renderer::enable(GL::Renderer::Feature::DepthTest);
  Magnum::GL::Renderer::setClearColor(0x4040FF_rgbf);
}

void KoalaTest::UpdateCameraPosition() noexcept {
  static const float dX = 30.0;
  static const float dY = 10.0;
  static const float dZ = 30.0;
  camera->camera.setTransformation(
      Matrix4::lookAt({dX * cos(dTime), dY * cos(dTime), dZ * sin(dTime)},
                      {0.0f, 0.0f, 0.0f}, Vector3::yAxis()));
}

void KoalaTest::tickEvent() {
  auto currentFrameTime = std::chrono::system_clock::now();

  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
                      currentFrameTime - lastFrameTime)
                      .count();
  dTime += (duration * 0.000001f);
  UpdateCameraPosition();
  redraw();
  lastFrameTime = currentFrameTime;
  // logger.WriteLog(duration > 25000 ? DebugLogger::DebugLevel::DEBUG_WARNING
  //                                  : DebugLogger::DebugLevel::DEBUG_INFO,
  //                 "Tick took [%d] us ... %f FPS dTime %f", duration,
  //                 static_cast<double>(1000 * 1000) / duration, dTime);

  // if (dTime > 3)
  //   exit(0);
}

void KoalaTest::drawEvent() {

  GL::defaultFramebuffer.clear(GL::FramebufferClear::Color |
                               GL::FramebufferClear::Depth);

  auto &renderGroups = Koala::Objects::SceneRenderableGroup::GetRenderGroups();
  for (auto &renderGroup : renderGroups) {
    camera->cameraLens.draw(dynamic_cast<Magnum::SceneGraph::DrawableGroup3D &>(
        *renderGroup.first));
  }

  swapBuffers();
}

KoalaTest::~KoalaTest() {}

MAGNUM_APPLICATION_MAIN(KoalaTest)
