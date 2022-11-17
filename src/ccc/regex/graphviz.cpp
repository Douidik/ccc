#include "graphviz.hpp"
#include "regex.hpp"

namespace ccc::regex {

constexpr std::string_view CREDITS =
  R"(# Regex Graph generated with: https://github.com/douidik/ccc
# <?> Error
# <$> Epsilon state
# </> Dash state
# <^> Any state
# '...' Text
# [a..b] Range

)";

Graphviz::Graphviz(const Regex &regex) : m_regex(regex) {
  write_graph();
}

void Graphviz::write_graph() {
  write(CREDITS);
  write("digraph regex {{\n");

  if (!automata().empty()) {
    write_shapes();
    write_start();
    write_edges();
  }

  write("}}");
}

void Graphviz::write_start() {
  write("  rankdir = LR;\n");
  write("  start [shape = none];\n");
  write("  start -> 0 [label = \"{}\"];\n", automata().root()->state());
}

void Graphviz::write_shapes() {
  constexpr std::string_view SHAPE_FMT = "  {} [shape = {}];\n";
  
  for (const Node &node : automata().nodes()) {
    write(SHAPE_FMT, node.index(), node.ok() ? "circle" : "square");
  }
}

void Graphviz::write_edges() {
  constexpr std::string_view EDGE_FMT = "  {} -> {} [label = \"{}\"];\n";

  for (const Node &node : automata().nodes()) {
    for (const Node *edge : node.edges()) {
      write(EDGE_FMT, node.index(), edge->index(), edge->state());
    }
  }
}

}  // namespace ccc::regex
