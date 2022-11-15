#ifndef CCC_REGEX_GRAPHVIZ_HPP
#define CCC_REGEX_GRAPHVIZ_HPP

#include <fmt/format.h>
#include <string>
#include "regex.hpp"

namespace ccc::regex {

class Graphviz {
public:
  Graphviz(const Regex &regex);

  auto document() const -> std::string_view {
    return m_document;
  }

  auto write(std::string_view fmt, auto... args) {
    return fmt::format_to(std::back_inserter(m_document), fmt::runtime(fmt), args...);
  }

private:
  auto automata() const -> const Automata & {
    return m_regex.automata();
  }
  
  void write_graph();
  void write_start();
  void write_shapes();
  void write_edges();

  const Regex &m_regex;
  std::string m_document;
};

}  // namespace ccc::regex

#endif
