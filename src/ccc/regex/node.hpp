#ifndef CCC_REGEX_NODE_HPP
#define CCC_REGEX_NODE_HPP

#include "state.hpp"
#include <set>

namespace ccc::regex {

class Node {
public:
  struct Cmp {
    auto operator()(const Node *a, const Node *b) const -> bool;
  };

  using Set = std::set<const Node *, Cmp>;
  using MutSet = std::set<Node *, Cmp>;

  Node() = default;
  Node(Set edges, u64 index, State state);

  auto submit(std::string_view expression, size_t index) const -> size_t;

  auto index() const -> u64 {
    return m_index;
  }

  auto state() const -> const State & {
    return m_state;
  }

  auto edges() const -> const Set & {
    return m_edges;
  }

  auto push(const Node &edge) -> const Node * {
    return *m_edges.insert(&edge).first;
  }

  auto breadth() const -> u64 {
    return m_edges.size();
  }

  auto deepest_member() const -> const Node * {
    return breadth() > 0 ? *m_edges.rbegin() : nullptr;
  }

  auto branch() const -> bool {
    return breadth() > 0 && deepest_member()->index() > m_index;
  }

  auto ok() const -> bool {
    return m_state.type() != Type::Any && !branch();
  }

private:
  u64 m_index;
  Set m_edges;
  State m_state;
};

}  // namespace ccc::regex

#endif
