#pragma once

#include "koala-object.hpp"
#include <functional>
#include <memory>
#include <unordered_map>

namespace Koala {

struct ParserResult {
  uint8_t *output;
  size_t size;
};

class Parsers {
public:
  static std::function<ParserResult(BaseObject *)>
  GetParser(std::string parserName);

private:
  static std::unordered_map<std::string,
                            std::function<ParserResult(BaseObject *)>>
      parsers;

  [[nodiscard]] static ParserResult ObjParser(BaseObject *object) noexcept;
};
} // namespace  Koala