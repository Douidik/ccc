#ifndef CCC_TOKEN_HPP
#define CCC_TOKEN_HPP

#include "category.hpp"
#include <string_view>

namespace ccc {

struct Token {
  std::string_view view;
  Category category;
};

}  // namespace ccc

#endif
