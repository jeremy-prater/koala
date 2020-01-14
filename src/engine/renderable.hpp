#pragma once

#include "debuglogger.hpp"
#include "typedefs.hpp"
#include <Magnum/SceneGraph/Drawable.h>
#include <memory>
#include <rapidjson/pointer.h>

namespace Koala {

class Renderable : public Object3D, public Magnum::SceneGraph::Drawable3D {
public:
  explicit Renderable(
      rapidjson::GenericObject<false, rapidjson::Value::ValueType> props,
      Object3D *parent, Magnum::SceneGraph::DrawableGroup3D *group);
  ~Renderable();

private:
  DebugLogger logger;
};

} // namespace Koala