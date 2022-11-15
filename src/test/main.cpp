#include "regex.hpp"
#include <catch2/catch_session.hpp>

using namespace ccc;

auto main(i32 argc, char **argv) -> i32 {
  return Catch::Session().run(argc, argv);
}
