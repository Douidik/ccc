#ifndef CCC_ESCAPE_SEQUENCE_HPP
#define CCC_ESCAPE_SEQUENCE_HPP

#include <fmt/format.h>
#include <string_view>

namespace ccc {

constexpr auto escaped_size(std::string_view s) -> size_t {
  size_t i = 0;

  for (const char &c : s) {
    switch (c) {
    case '\t':
    case '\v':
    case '\0':
    case '\b':
    case '\f':
    case '\n':
    case '\r':
    case '\"': i++;
    }
    i++;
  }

  return i;
}

struct FmtEscaped {
  std::string_view source;
  size_t depth = 0;
};

}  // namespace ccc

namespace fmt {
using namespace ccc;

template<>
struct formatter<FmtEscaped> {
  constexpr auto parse(format_parse_context &context) {
    return context.begin();
  }

  constexpr auto format_escape_sequence(std::string_view sequence, size_t depth, auto &context) {
    for (size_t i = 0; i < depth; i++) {
      format_to(context.out(), "\\");
    }

    return format_to(context.out(), "{}", sequence);
  }

  constexpr auto format_char(const char &c, size_t depth, auto &context) {
    switch (c) {
    case '\t': return format_escape_sequence(R"(\t)", depth, context);
    case '\v': return format_escape_sequence(R"(\v)", depth, context);
    case '\0': return format_escape_sequence(R"(\0)", depth, context);
    case '\b': return format_escape_sequence(R"(\b)", depth, context);
    case '\f': return format_escape_sequence(R"(\f)", depth, context);
    case '\n': return format_escape_sequence(R"(\n)", depth, context);
    case '\r': return format_escape_sequence(R"(\r)", depth, context);
    case '\\': return format_escape_sequence(R"(\\)", depth, context);
    case '\"': return format_escape_sequence(R"(\")", depth, context);

    default: return format_to(context.out(), "{}", c);
    }
  }

  constexpr auto format(const FmtEscaped &fmt_escaped, auto &context) {
    for (const char &c : fmt_escaped.source) {
      format_char(c, fmt_escaped.depth, context);
    }

    return context.out();
  }
};

};  // namespace fmt

#endif
