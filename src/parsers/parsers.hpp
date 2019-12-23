#include "koala-object.hpp"
#include <memory>

namespace Koala {

template <typename Output> class Parser {
public:
  Parser();
  virtual ~Parser();
  virtual Output Generate(std::shared_ptr<BaseObject> input) = 0;

private:
};

class Parsers {
public:
  static void RegisterParser(std::string parserName, Parser *parserObject);
  static 

private:
  static std::mutex parserLock;
  static std::unordered_map<std::string, Parser *> parsers;
};
} // namespace  Koala