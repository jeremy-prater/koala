#include "render-objects.hpp"
#include "assets/gltf-asset.hpp"
#include "objects/scene-renderable-groups.hpp"

using namespace Koala;
using namespace Magnum;

KoalaTest::KoalaTest(const Arguments &arguments)
    : Engine::Engine(arguments),
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

  camera = std::make_unique<Objects::Camera>("default", scene);
  camera->camera.setTransformation(Matrix4::lookAt(
      {5.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, Vector3::yAxis()));

  cloud = std::make_unique<Cloud::Cloud>();

  auto x_group = project->GetGroupByPath("/default/x/the-x");

  scene->CreateRenderableFromGroup(project, x_group);

  gameThread = std::thread([this]() {
    running = true;
    while (running) {
      auto start = std::chrono::system_clock::now();

      redraw();
      usleep(250 * 1000);

      auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
                          std::chrono::system_clock::now() - start)
                          .count();

      logger.Info("Render frame took [%d] us ... %f FPS", duration,
                  static_cast<double>(1000 * 1000) / duration);
      // running = false;
    }
  });
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

KoalaTest::~KoalaTest() {
  running = false;

  if (gameThread.joinable())
    gameThread.join();
}

MAGNUM_APPLICATION_MAIN(KoalaTest)
