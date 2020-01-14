#include "renderable.hpp"

using namespace Koala;
using namespace Magnum;

Renderable::Renderable(
    rapidjson::GenericObject<false, rapidjson::Value::ValueType> props,
    std::shared_ptr<Object3D> parent,
    std::shared_ptr<SceneGraph::DrawableGroup3D> group)
    : Object3D{parent}, SceneGraph::Drawable3D{*this, group},
      logger("Renderable-", DebugLogger::DebugColor::COLOR_YELLOW, false) {
  {}
  Renderable::~Renderable() {}
