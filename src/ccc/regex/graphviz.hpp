#ifndef CCC_REGEX_GRAPHVIZ_HPP
#define CCC_REGEX_GRAPHVIZ_HPP

#include "regex.hpp"
#include <fmt/format.h>
#include <string>

namespace ccc::regex {

enum GraphvizOption : u32 {
  GraphvizNone = 0,
  GraphvizCredits = 1 << 0,
  GraphvizAnon = 1 << 1,
  GraphvizCluster = 1 << 2,
};

class Graphviz {
public:
  Graphviz(const Regex &regex, GraphvizOption options = GraphvizNone, std::string_view name = "");

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
  void write_attributes();
  void write_edges();

  const Regex &m_regex;
  u32 m_options;
  std::string_view m_name;
  std::string m_document;
};

}  // namespace ccc::regex

#endif
