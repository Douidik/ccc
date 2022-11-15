#include "regex/graphviz.hpp"
#include "regex/regex.hpp"
#include <fmt/format.h>
#include "regex/parser_exception.hpp"
#include <iostream>

using namespace ccc::regex;

int main(int argc, char **argv) {
  if (argc < 3) {
    return 1;
  }

  //try {
    Regex regex {argv[1]};
    Match match = regex.match(argv[2]);
    fmt::print("# '{}', matched: '{}' of '{}'\n", regex.source(), match.view(), match.expression());
    fmt::print("{}\n", Graphviz {regex}.document());
  //} catch (const ParserException &exception) {
  //  std::cerr << exception.what() << std::endl;
  //}
}
