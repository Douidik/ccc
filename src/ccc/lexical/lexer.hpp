#ifndef CCC_LEXER_HPP
#define CCC_LEXER_HPP

#include "token.hpp"
#include "token_map.hpp"

namespace ccc {

class Lexer {
public:
  Lexer(std::string_view source);
  auto tokenize() -> Token;

private:
  auto match() -> Token;
  
  TokenMap m_map;
  std::string_view m_next;
  std::string_view m_source;
};

}  // namespace ccc

#endif
