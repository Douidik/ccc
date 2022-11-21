#ifndef CCC_LEXER_HPP
#define CCC_LEXER_HPP

#include "syntax_map.hpp"
#include "token.hpp"

namespace ccc {

class Lexer {
public:
  Lexer(std::string_view source, SyntaxMap map = syntax_ansi());
  auto tokenize() -> Token;

private:
  auto match() -> Token;

  SyntaxMap m_map;
  std::string_view m_next;
  std::string_view m_source;
};

}  // namespace ccc

#endif
