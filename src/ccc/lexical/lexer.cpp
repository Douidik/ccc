#include "lexer.hpp"
#include "lexer_exception.hpp"

// TODO ! Support different syntaxes

namespace ccc {
using namespace trait;

Lexer::Lexer(std::string_view source, SyntaxMap map) :
  m_map(map),
  m_next(source),
  m_source(source) {}

auto Lexer::tokenize() -> Token {
  auto token = match();

  switch (token.trait) {
  case None: throw LexerException {"Unrecognized token", m_source, token};
  case Blank: return tokenize();
  default: return token;
  }
}

auto Lexer::match() -> Token {
  if (m_next.empty()) {
    return {"", End};
  }

  for (const auto &[trait, regex] : m_map) {
    if (auto match = regex.match(m_next)) {
      m_next = match.next();
      return {match.view(), trait};
    }
  }

  throw LexerException {"Unmatched token", m_source, {{m_source.begin(), m_source.begin()}, None}};
}

}  // namespace ccc
