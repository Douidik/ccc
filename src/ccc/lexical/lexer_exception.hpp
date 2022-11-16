#ifndef CCC_LEXER_EXCEPTION_HPP
#define CCC_LEXER_EXCEPTION_HPP

#include "exception.hpp"
#include "source_view.hpp"
#include "token.hpp"
#include <algorithm>
#include <source_location>

namespace ccc {

class LexerException : public Exception {
public:
  LexerException(std::string_view description, std::string_view source, const Token &token) :
    Exception {
      fmt(),
      line_number(source, token),
      line_view(source, token),
      "",
      cursor_offset(source, token),
      "",
      !token.view.empty() ? token.view.size() - 1 : 0,
      description,
    } {}

private:
  auto line_number(std::string_view source, const Token &token) -> size_t {
    return std::count(source.begin(), token.view.begin(), '\n');
  }

  auto line_view(std::string_view source, const Token &token) -> std::string_view {
    return SourceView {source}.line(line_number(source, token));
  }

  auto cursor_offset(std::string_view source, const Token &token) -> size_t {
    return std::distance(token.view.begin(), line_view(source, token).begin());
  }

  auto name() const noexcept -> std::string_view override {
    return "LexerException";
  }

  auto fmt() const noexcept -> std::string_view override {
    return R"(
with {{
  {} | {}
      {:>{}}{:^>{}} {}
}})";
  };
};

}  // namespace ccc

#endif
