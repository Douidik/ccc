#ifndef CCC_REGEX_PARSER_EXCEPTION_HPP
#define CCC_REGEX_PARSER_EXCEPTION_HPP

#include "exception.hpp"

namespace ccc::regex {

class ParserException : public Exception {
public:
  ParserException(std::string_view description, std::string_view source, const char *token) :
    Exception {fmt(), source, '^', token - &source[0] + 1, description} {}

private:
  auto name() const noexcept -> std::string_view override {
    return "regex::ParserException";
  }

  auto fmt() const noexcept -> std::string_view override {
    return R"(
with {{
  {}
  {:>{}} {}
}})";
  };
};

}  // namespace ccc::regex

#endif
