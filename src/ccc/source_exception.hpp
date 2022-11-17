#ifndef CCC_SOURCE_EXCEPTION_HPP
#define CCC_SOURCE_EXCEPTION_HPP

#include "exception.hpp"
#include <algorithm>

namespace ccc {

class SourceException {
public:
  SourceException(std::string_view description, std::string_view source, std::string_view token) :
    m_description(description),
    m_source(source),
    m_token(token) {}

protected:
  auto line() const -> size_t {
    return std::count(m_source.begin(), m_token.begin(), '\n');
  }

  auto snippet() const -> std::string_view {
    auto rbegin = std::find(m_token.rend(), m_source.rend(), '\n');
    const char *end = std::find(m_token.end(), m_source.end(), '\n');

    return {std::max(rbegin.base(), m_source.begin()), end};
  }

  auto cursor_offset() const -> size_t {
    return std::distance(snippet().begin(), m_token.begin());
  }

private:
  std::string_view m_description;
  std::string_view m_source;
  std::string_view m_token;
};

}  // namespace ccc

#endif
