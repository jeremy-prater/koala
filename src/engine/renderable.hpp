#pragma once

#include "debuglogger.hpp"
#include "typedefs.hpp"
#include <Magnum/SceneGraph/Drawable.h>
#include <memory>
#include <rapidjson/rapidjson.h>

namespace Koala {

class Renderable : public Object3D, public Magnum::SceneGraph::Drawable3D {
public:
  explicit Renderable(
      rapidjson::GenericObject<false, rapidjson::Value::ValueType> props,
      std::shared_ptr<Object3D> parent,
      std::shared_ptr<Magnum::SceneGraph::DrawableGroup3D> group);
  ~Renderable();

private:
  DebugLogger logger;
};

} // namespace Koala