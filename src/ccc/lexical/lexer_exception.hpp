#ifndef CCC_LEXER_EXCEPTION_HPP
#define CCC_LEXER_EXCEPTION_HPP

#include "exception.hpp"
#include "source_exception.hpp"
#include "token.hpp"
#include <algorithm>
#include <source_location>

namespace ccc {

class LexerException : SourceException, public Exception {
public:
  LexerException(std::string_view description, std::string_view source, const Token &token) :
    SourceException(description, source, token.view),
    Exception(fmt(), line(), snippet(), "", cursor_offset(), "", token.view.size(), description) {}

  auto name() const noexcept -> std::string_view override {
    return "LexerException";
  }

private:
  auto fmt() const noexcept -> std::string_view override {
    return R"(with {{
  {} | {}
      {:>{}}{:^>{}} {}
}})";
  };
};

}  // namespace ccc

#endif
