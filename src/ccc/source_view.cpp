#include "source_view.hpp"
#include "exception.hpp"
#include <algorithm>

namespace ccc {

SourceView::SourceView(std::string_view source) : m_source(source) {}

auto SourceView::line(size_t n) const -> std::string_view {
  return region(n, n + 1);
}

auto SourceView::region(size_t n, size_t m) const -> std::string_view {
  const char *begin = m_source.begin(), *end = m_source.end();

  for (size_t i = 0; i < n; i++) {
    begin = std::find(begin, end, '\n');
  }

  for (size_t i = 0; i < m; i++) {
    end = std::find(begin, end, '\n');
  }

  if (begin != m_source.begin() && begin != m_source.end()) begin++;
  if (end != m_source.begin() && end != m_source.end()) end--;

  return {begin, end};
}

}  // namespace ccc
