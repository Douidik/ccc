#ifndef CCC_REGEX_PARSER_HPP
#define CCC_REGEX_PARSER_HPP

#include "automata.hpp"
#include <stack>
#include <vector>

namespace ccc::regex {

class Parser {
public:
  Parser(std::string_view source);
  auto parse() -> Automata;

private:
  auto merge_stack() -> Automata;
  auto sequence_source() -> std::string_view;
  auto parse_new_token() -> size_t;
  
  auto parse_binary_op(char op) -> std::pair<Automata, Automata>;
  auto parse_pre_op(char op) -> Automata;
  auto parse_post_op(char op) -> Automata;
  
  void parse_range(std::string_view data);
  void parse_any();
  void parse_text();
  void parse_sequence();
  void parse_or();
  void parse_quest();
  void parse_star();
  void parse_plus();
  void parse_wave();
  void parse_dash();
  
  auto stack_push(Automata automata = {}) -> Automata & {
    return m_stack.emplace_back(automata);
  }

  auto stack_pop() -> Automata {
    auto automata = stack_top();
    return m_stack.pop_back(), automata;
  }

  auto stack_top() -> Automata & {
    return m_stack.back();
  }

  const char *m_token;
  std::string_view m_source;
  std::vector<Automata> m_stack;
};

}  // namespace ccc::regex

#endif
