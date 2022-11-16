#include "lexer.hpp"
#include "lexer_exception.hpp"

namespace ccc {
using enum Category;

Lexer::Lexer(std::string_view source) : m_map(token_map()), m_next(source), m_source(source) {}

auto Lexer::tokenize() -> Token {
  auto token = match();

  switch (token.category) {
  case None: {
    throw LexerException {"Unrecognized token", m_source, token};
  }

  case Blank: {
    return tokenize();
  }

  default: {
    return token;
  }
  }
}

auto Lexer::match() -> Token {
  if (m_next.empty()) {
    return {"", End};
  }

  for (const auto &[category, regex] : m_map) {
    auto match = regex.match(m_next);

    if (match.index() != npos()) {
      m_next = match.next();
      return {match.view(), category};
    }
  }

  return {m_next, None};
}

}  // namespace ccc
