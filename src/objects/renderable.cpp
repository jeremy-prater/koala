#include "renderable.hpp"

using namespace Koala;
using namespace Magnum;

Renderable::Renderable(Object3D *parent, SceneGraph::DrawableGroup3D *group)
    : Object3D{parent}, SceneGraph::Drawable3D{*this, group},
      logger("Renderable-", DebugLogger::DebugColor::COLOR_YELLOW, false) {}

Renderable::~Renderable() {}