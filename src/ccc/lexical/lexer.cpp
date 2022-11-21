#include "lexer.hpp"
#include "lexer_exception.hpp"

// TODO ! Support different syntaxes

namespace ccc {
using namespace trait;

Lexer::Lexer(std::string_view source, SyntaxMap map) :
  m_map(map),
  m_next(source),
  m_source(source) {
  if (!source.ends_with('\n')) {
    throw LexerException {
      "Source doesn't ends with '\\n'",
      m_source,
      {{&m_source.back(), &m_source.back() + 1}, None},
    };
  }
}

auto Lexer::tokenize() -> Token {
  auto token = match();

  [[unlikely]] if (token.trait & CsCatch) {
    throw LexerException {trait_catch_description(token.trait), m_source, token};
  }

  return token.trait != Blank ? token : tokenize();
}

auto Lexer::match() -> Token {
  if (m_next.empty()) {
    return {{m_source.end(), m_source.end()}, End};
  }

  for (const auto &[trait, regex] : m_map) {
    if (auto match = regex.match(m_next)) {
      m_next = match.next();
      return {match.view(), trait};
    }
  }

  throw Exception {"None should match everything"};
}

}  // namespace ccc
