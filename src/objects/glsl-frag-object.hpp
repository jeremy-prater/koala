#pragma once

#include "koala-object.hpp"

namespace Koala {

class GLSLFragObject : public BaseObject {
public:
  GLSLFragObject(rapidjson::GenericObject<false, rapidjson::Value::ValueType> props,
             const std::string rootDir);
  virtual ~GLSLFragObject();
  [[nodiscard]] virtual bool Parse() noexcept override;

private:
  bool parsed;
  

  DebugLogger logger;
};
} // namespace Koala