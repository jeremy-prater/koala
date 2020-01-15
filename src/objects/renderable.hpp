#pragma once

#include "debuglogger.hpp"
#include "typedefs.hpp"
#include <Magnum/SceneGraph/Drawable.h>
#include <memory>
#include <rapidjson/pointer.h>
#include <unordered_map>

namespace Koala {

class Renderable : public Object3D, public Magnum::SceneGraph::Drawable3D {
public:
  explicit Renderable(Object3D *parent,
                      Magnum::SceneGraph::DrawableGroup3D *group);
  ~Renderable();

private:
  std::unordered_map<std::string, std::string> properties;
  DebugLogger logger;
};

} // namespace Koala