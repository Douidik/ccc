#ifndef CCC_LEXER_HPP
#define CCC_LEXER_HPP

#include "token.hpp"
#include "syntax_map.hpp"

namespace ccc {

class Lexer {
public:
  Lexer(std::string_view source, SyntaxMap map);
  auto tokenize() -> Token;

private:
  auto match() -> Token;

  SyntaxMap m_map;
  std::string_view m_next;
  std::string_view m_source;
};

}  // namespace ccc

#endif
