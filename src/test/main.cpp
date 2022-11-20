#include "regex.hpp"
//
#include "lexer.hpp"
//
#include <gtest/gtest.h>

using namespace ccc;

auto main(i32 argc, char **argv) -> i32 {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
