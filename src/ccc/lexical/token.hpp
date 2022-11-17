#ifndef CCC_TOKEN_HPP
#define CCC_TOKEN_HPP

#include "scalar_types.hpp"
#include <string_view>

namespace ccc {

struct Token {
  std::string_view view;
  u32 trait;
};

}  // namespace ccc

#endif
