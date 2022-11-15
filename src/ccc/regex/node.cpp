#include "node.hpp"

namespace ccc::regex {

auto Node::Cmp::operator()(const Node *a, const Node *b) const -> bool {
  return a->index() < b->index();
}

Node::Node(Set edges, u64 index, State state) : m_edges(edges), m_index(index), m_state(state) {}

auto Node::submit(std::string_view expression, size_t index) const -> size_t {
  size_t match = m_state.submit(expression, index);

  if (match != npos()) {
    if (ok() && match >= expression.size()) {
      return match;
    }

    for (const Node *edge : m_edges) {
      auto match_fwd = edge->submit(expression, match);

      if (match_fwd != npos()) {
        return match_fwd;
      }
    }

    if (ok()) {
      return match;
    }
  }

  return npos();
}

}  // namespace ccc::regex
