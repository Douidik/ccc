#ifndef CCC_REGEX_HPP
#define CCC_REGEX_HPP

#include "parser.hpp"

namespace ccc::regex {

class Regex {
public:
  Regex(std::string_view source) : m_source(source), m_automata(Parser {source}.parse()) {}

  auto automata() const -> const Automata & {
    return m_automata;
  }

  auto source() const -> std::string_view {
    return m_source;
  }

  auto match(std::string_view expression) const -> Match {
    return Match {expression, m_automata.submit(expression)};
  }

private:
  Automata m_automata;
  std::string_view m_source;
};

}  // namespace ccc::regex

namespace ccc::regex::literals {
inline Regex operator""_rx(const char *source, size_t) {
  return Regex {source};
}
}  // namespace ccc::regex::literals

namespace ccc {
using ccc::regex::Regex;
}

#endif
