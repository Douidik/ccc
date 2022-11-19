#ifndef CCC_ESCAPE_SEQUENCE_HPP
#define CCC_ESCAPE_SEQUENCE_HPP

#include <fmt/format.h>
#include <string_view>

namespace ccc {

constexpr auto escaped(const char &c) -> std::string_view {
  switch (c) {
  case '\t': return R"(\\t)";
  case '\v': return R"(\\v)";
  case '\0': return R"(\\0)";
  case '\b': return R"(\\b)";
  case '\f': return R"(\\f)";
  case '\n': return R"(\\n)";
  case '\r': return R"(\\r)";
  case '\"': return R"(\")";

  default: return {&c, 1};
  }
}

struct FmtEscaped {
  std::string_view source;
};

}  // namespace ccc

namespace fmt {
using namespace ccc;

template<>
struct formatter<FmtEscaped> {
  constexpr auto parse(format_parse_context &context) {
    return context.begin();
  }

  constexpr auto format(const FmtEscaped &fmt_escaped, auto &context) {
    for (const char &c : fmt_escaped.source) {
      format_to(context.out(), "{}", escaped(c));
    }

    return context.out();
  }
};

};  // namespace fmt

#endif
