#ifndef CCC_REGEX_AUTOMATA_HPP
#define CCC_REGEX_AUTOMATA_HPP

#include "node.hpp"
#include <array>
#include <memory>
#include <span>

namespace ccc::regex {

class Automata {
public:
  Automata();
  Automata(const Automata &automata) = default;
  
  auto nodes() -> std::span<Node> {
    return {&m_buffer[0], m_size};
  }

  auto nodes() const -> std::span<const Node> {
    return {&m_buffer[0], m_size};
  }

  auto size() const -> u64 {
    return m_size;
  }

  auto empty() const -> u64 {
    return !m_size;
  }

  auto root() const -> const Node * {
    return !empty() ? &m_buffer[0] : nullptr;
  }

  auto submit(std::string_view expression) const -> size_t {
    return !empty() ? root()->submit(expression, 0) : npos();
  }

  auto merge(const Automata &automata, Node::MutSet owners, Node::Set edges) -> const Node *;
  auto push(State state, Node::MutSet owners, Node::Set edges) -> Node &;
  auto outers() -> Node::MutSet;

private:
  u64 m_size;
  std::shared_ptr<Node[]> m_buffer;
};

}  // namespace ccc::regex

#endif
