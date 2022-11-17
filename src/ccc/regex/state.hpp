#ifndef CCC_REGEX_STATE_HPP
#define CCC_REGEX_STATE_HPP

#include "escape_sequence.hpp"
#include "match.hpp"

namespace ccc::regex {

enum class Type {
  Epsilon,
  Any,
  Dash,
  Text,
  Range,
};

class State {
public:
  State(Type type = Type::Epsilon, std::string_view data = "");

  auto submit(std::string_view expression, size_t index) const -> size_t;

  auto type() const -> Type {
    return m_type;
  }

  auto data() const -> std::string_view {
    return m_data;
  }

  auto ok() const -> bool {
    return m_type != Type::Any;
  }

private:
  Type m_type;
  std::string_view m_data;
};

}  // namespace ccc::regex

namespace fmt {
using namespace ccc::regex;

template<>
struct formatter<State> {
  constexpr auto parse(format_parse_context &context) {
    return context.begin();
  }

  constexpr auto format(const State &state, auto &context) {
    switch (state.type()) {
    case Type::Epsilon: {
      return format_to(context.out(), "<$>");
    }

    case Type::Dash: {
      return format_to(context.out(), "</>");
    }

    case Type::Any: {
      return format_to(context.out(), "<^>");
    }

    case Type::Text: {
      return format_to(context.out(), "'{}'", FmtEscaped {state.data()});
    }

    case Type::Range: {
      auto range = state.data();

      if (range.size() < 1) {
        return format_to(context.out(), "[]");
      }
      if (range.size() < 2) {
        return format_to(context.out(), "[{}]", escaped(range[0]));
      }

      return format_to(context.out(), "[{}..{}]", escaped(range.front()), escaped(range.back()));
    }
    }

    return context.out();
  }
};

}  // namespace fmt

#endif
