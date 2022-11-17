#include "lexer.hpp"
#include "regex.hpp"
#include <gtest/gtest.h>

using namespace ccc;

auto main(i32 argc, char **argv) -> i32 {
  testing::InitGoogleTest();
  return RUN_ALL_TESTS();
}
