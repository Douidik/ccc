#include "automata.hpp"
#include "exception.hpp"
#include <map>

namespace ccc::regex {
constexpr size_t AUTOMATA_CAPACITY = 64;

Automata::Automata() :
  m_size(0),
  m_buffer(std::make_shared_for_overwrite<Node[AUTOMATA_CAPACITY]>()) {}

auto Automata::merge(const Automata &automata, Node::MutSet owners, Node::Set edges)
  -> const Node * {
  std::map<const Node *, Node *, Node::Cmp> map {};

  // Merge nodes and link edges with outer-nodes
  for (const Node &node : automata.nodes()) {
    if (node.branch())
      map.emplace(&node, &push(node.state(), {}, {}));
    else
      map.emplace(&node, &push(node.state(), {}, edges));
  }

  // Remake inner connections
  for (const auto [node, merged] : map) {
    for (const Node *edge : node->edges()) {
      merged->push(*map[edge]);
    }
  }

  // Link merged root with given owners
  if (!map.empty()) {
    const Node *root = map[automata.root()];

    for (Node *owner : owners) {
      owner->push(*root);
    }

    return root;
  }

  return {};
}

auto Automata::push(State state, Node::MutSet owners, Node::Set edges) -> Node & {
  u64 index = m_size++;

  if (index >= AUTOMATA_CAPACITY) {
    throw Exception {"Automata buffer capacity exceeded (AUTOMATA_CAPACITY: {})", AUTOMATA_CAPACITY};
  }

  auto &node = m_buffer[index] = Node {
    edges,
    index,
    state,
  };

  for (Node *owner : owners) {
    owner->push(node);
  }

  return node;
}

auto Automata::outers() -> Node::MutSet {
  Node::MutSet output {};

  for (Node &node : nodes()) {
    if (!node.branch()) output.insert(&node);
  }

  return output;
}

}  // namespace ccc::regex
