#ifndef CCC_SOURCE_VIEW_HPP
#define CCC_SOURCE_VIEW_HPP

#include <string_view>

namespace ccc {

class SourceView {
public:
  SourceView(std::string_view source);

  auto line(size_t n) const -> std::string_view;
  auto region(size_t n, size_t m) const -> std::string_view;

private:
  std::string_view m_source;
};

}  // namespace ccc

#endif
