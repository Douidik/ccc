#ifndef CCC_REGEX_PARSER_EXCEPTION_HPP
#define CCC_REGEX_PARSER_EXCEPTION_HPP

#include "escape_sequence.hpp"
#include "exception.hpp"

namespace ccc::regex {

class ParserException : public Exception {
public:
  ParserException(std::string_view description, std::string_view source, const char *token) :
    Exception {fmt(), FmtEscaped {source}, '^', token - &source[0] + 1, description} {}

  auto name() const noexcept -> std::string_view override {
    return "regex::ParserException";
  }

private:
  auto fmt() const noexcept -> std::string_view override {
    return R"(with {{
  {}
  {:>{}} {}
}})";
  };
};

}  // namespace ccc::regex

#endif
