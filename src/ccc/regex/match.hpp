#ifndef CCC_REGEX_MATCH_HPP
#define CCC_REGEX_MATCH_HPP

#include "scalar_types.hpp"
#include <string_view>

namespace ccc::regex {

class Match {
public:
  Match(std::string_view expression, size_t index) : m_expression(expression), m_index(index) {}

  operator bool() const {
    return m_index != npos();
  }

  auto index() const -> size_t {
    return m_index;
  }

  auto begin() const -> const char * {
    return m_expression.begin();
  }

  auto end() const -> const char * {
    return m_index != npos() ? &m_expression[m_index] : &m_expression[0];
  }

  auto view() const -> std::string_view {
    return {begin(), end()};
  }

  auto next() const -> std::string_view {
    return {end(), m_expression.end()};
  }

  auto expression() const -> std::string_view {
    return m_expression;
  }

private:
  std::string_view m_expression;
  size_t m_index;
};

}  // namespace ccc::regex

#endif
