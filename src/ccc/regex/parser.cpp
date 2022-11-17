#include "parser.hpp"
#include "parser_exception.hpp"
#include <algorithm>

namespace ccc::regex {
using enum Type;

Parser::Parser(std::string_view source) : m_source(source) {}

auto Parser::parse() -> Automata {
  for (m_token = m_source.begin(); m_token != m_source.end(); m_token++) {
    parse_new_token();
  }

  return merge_stack();
}

auto Parser::merge_stack() -> Automata {
  Automata automata {};

  for (const auto &sequence : m_stack) {
    automata.merge(sequence, automata.outers(), {});
  }

  return automata;
}

auto Parser::sequence_source() -> std::string_view {
  i32 depth = 0;

  const char *begin = m_token + 1;
  const char *end = std::find_if(m_token, m_source.end(), [&depth](char token) -> bool {
    switch (token) {
    case '{': depth++; break;
    case '}': depth--; break;
    }
    return depth < 1;
  });

  if (end == m_source.end()) {
    throw ParserException {"Unmatched sequence brace, missing <}> character", m_source, m_token};
  }

  return {begin, end};
};

auto Parser::parse_new_token() -> size_t {
  size_t base_size = m_stack.size();

  if (m_token >= m_source.end()) {
    return 0;
  }

  switch (*m_token) {
  case ' ': m_token++, parse_new_token(); break;

  case '_': parse_range("\n\t\v\b\f "); break;
  case 'a': parse_range("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"); break;
  case 'o': parse_range("!#$%&()*+,-./:;<=>?@[\\]^`{|}~"); break;
  case 'n': parse_range("0123456789"); break;
  case 'Q': parse_range("\""); break;
  case 'q': parse_range("'"); break;

  case '^': parse_any(); break;
  case '/': parse_dash(); break;
  case '\'': parse_text(); break;
  case '{': parse_sequence(); break;
  case '|': parse_or(); break;
  case '?': parse_quest(); break;
  case '*': parse_star(); break;
  case '+': parse_plus(); break;
  case '~': parse_wave(); break;

  case '}': {
    throw ParserException {"Unmatched sequence brace, missing <{> character", m_source, m_token};
  }

  default: {
    throw ParserException {"Unrecognized token, none of [_aonQq^'{}|?*+~]", m_source, m_token};
  }
  }

  // Pop top-level empty automatas
  while (!m_stack.empty() && stack_top().empty()) {
    stack_pop();
  }

  return base_size - m_stack.size();
}

auto Parser::parse_binary_op(char op) -> std::pair<Automata, Automata> {
  return {parse_pre_op(op), parse_post_op(op)};
}

auto Parser::parse_pre_op(char op) -> Automata {
  auto token_op = m_token;

  if (!m_stack.size()) {
    auto description = fmt::format("Missing pre-operand for <{}> operator", op);
    throw ParserException {description, m_source, token_op};
  }

  return stack_pop();
}

auto Parser::parse_post_op(char op) -> Automata {
  auto token_op = m_token++;

  if (!parse_new_token()) {
    auto description = fmt::format("Missing post-operand for <{}> operator", op);
    throw ParserException {description, m_source, token_op};
  }

  return stack_pop();
}

void Parser::parse_range(std::string_view data) {
  stack_push().push({Range, data}, {}, {});
}

void Parser::parse_any() {
  stack_push().push({Any}, {}, {});
}

void Parser::parse_dash() {
  stack_push().push({Dash}, {}, {});
}

void Parser::parse_text() {
  const char *bounds[2] {m_token + 1, std::find(m_token + 1, m_source.end(), '\'')};

  if (bounds[1] != m_source.end()) {
    m_token = bounds[1];
  } else {
    throw ParserException {"Unmatched text quote, missing <'> ending quote", m_source, m_token};
  }

  if (bounds[0] < bounds[1]) {
    stack_push().push({Text, {bounds[0], bounds[1]}}, {}, {});
  }
}

void Parser::parse_sequence() {
  auto source = sequence_source();
  stack_push(Parser(source).parse());
  m_token = source.end();
}

/*
  Control flow schemes:
  
  a: first binary operand
  b: second binary operand
  o: unary operand
  $: epsilon
  ^: any
  >: owns
*/

void Parser::parse_or() {
  //   > a
  // $
  //   > b
  auto [a, b] = parse_binary_op('|');

  auto &automata = stack_push();
  auto &epsilon = automata.push({Epsilon}, {}, {});
  {
    automata.merge(a, {&epsilon}, {});
    automata.merge(b, {&epsilon}, {});
  }
}

void Parser::parse_quest() {
  //   > o
  // $
  //   > $'

  auto operand = parse_pre_op('?');
  auto &automata = stack_push();
  auto &gate = automata.push({Epsilon}, {}, {});

  automata.merge(operand, {&gate}, {});
  automata.push({Epsilon}, {&gate}, {});
}

void Parser::parse_star() {
  //   > o > $
  // $
  //   > $'

  auto operand = parse_pre_op('*');
  auto &automata = stack_push();
  auto &gate = automata.push({Epsilon}, {}, {});

  automata.merge(operand, {&gate}, {&gate});
  automata.push({Epsilon}, {&gate}, {});
}

void Parser::parse_plus() {
  // $ > e > $

  auto operand = parse_pre_op('+');
  operand.push({Epsilon}, {operand.outers()}, {operand.root()});
  stack_push(operand);
}

void Parser::parse_wave() {
  //   > o
  // $
  //   > ^ > $

  auto operand = parse_post_op('~');
  auto &automata = stack_push();
  auto &gate = automata.push({Epsilon}, {}, {});

  automata.merge(operand, {&gate}, {});
  automata.push({Any}, {&gate}, {&gate});
}

}  // namespace ccc::regex
