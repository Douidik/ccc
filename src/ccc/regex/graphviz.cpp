#include "graphviz.hpp"
#include "regex.hpp"

namespace ccc::regex {

constexpr std::string_view CREDITS =
  R"(# Regex Graph generated with: https://github.com/douidik/ccc
# <?> Error
# <$> Epsilon state
# <^> Any state
# </> Dash state
# <!> Return state
# '...' Text
# [a..b] Range

)";

Graphviz::Graphviz(const Regex &regex, GraphvizOption options, std::string_view name) :
  m_regex(regex),
  m_options(options),
  m_name(name != "" ? name : regex.source()) {
  write_graph();
}

void Graphviz::write_graph() {
  if (m_options & GraphvizCredits) {
    write(CREDITS);
  }

  if (m_options & GraphvizCluster) {
    write("subgraph regex {{\n");
  } else {
    write("digraph regex {{\n");
  }

  if (!automata().empty()) {
    write_attributes();
    write_start();
    write_edges();
  }

  write("}}");
}

void Graphviz::write_start() {
  write("  rankdir = LR;\n");
  write("  \"{}\" [shape = none];\n", FmtEscaped {m_name, 1});
  write("  \"{}\" -> 0 [label = \"{}\"];\n", FmtEscaped {m_name, 1}, automata().root()->state());
}

void Graphviz::write_attributes() {
  constexpr std::string_view SHAPE_FMT = "  {} [shape = {}];\n";
  constexpr std::string_view ANON_FMT = "  {} [label = \"\"];\n";

  for (const Node &node : automata().nodes()) {
    write(SHAPE_FMT, node.index(), node.ok() ? "circle" : "square");

    if (m_options & GraphvizAnon) {
      write(ANON_FMT, node.index());
    }
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
