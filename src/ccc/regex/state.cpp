#include "state.hpp"

namespace ccc::regex {

State::State(Type type, std::string_view data) : m_type(type), m_data(data) {}

auto State::submit(std::string_view expression, size_t index) const -> size_t {
  using enum Type;

  switch (m_type) {
  case Epsilon: {
    return index;
  }
  case Any: {
    return index < expression.size() ? index + 1 : npos();
  }
  case Range: {
    return m_data.find(expression[index]) != npos() ? index + 1 : npos();
  }
  case Text: {
    return expression.substr(index, m_data.size()) == m_data ? index + m_data.size() : npos();
  }
  }

  return npos();
}

}  // namespace ccc::regex
