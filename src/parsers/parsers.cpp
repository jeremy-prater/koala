#include "parsers.hpp"

using namespace Koala;

std::unordered_map<std::string, std::function<ParserResult(BaseObject *)>>
    Parsers::parsers({{"obj", &Parsers::ObjParser}});

std::function<ParserResult(BaseObject *)>
Parsers::GetParser(std::string parserName) {
  auto it = parsers.find(parserName);
  if (it != parsers.end()) {
    return it->second;
  } else {
    return std::function<ParserResult(BaseObject *)>(nullptr);
  }
}
