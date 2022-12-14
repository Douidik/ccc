#ifndef CCC_TEST_REGEX_HPP
#define CCC_TEST_REGEX_HPP

#include "regex/parser_exception.hpp"
#include "regex/regex.hpp"
#include <gtest/gtest.h>

namespace ccc::regex {
using namespace literals;

constexpr std::string_view LOREM_IPSUM = R"(
Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut
labore et dolore magna aliqua. Id porta nibh venenatis cras sed felis eget velit. Viverra tellus
in hac habitasse. Sed risus pretium quam vulputate dignissim suspendisse in est. In eu mi
bibendum neque egestas congue quisque egestas. Mi proin sed libero enim sed faucibus turpis in.
Aliquam vestibulum morbi blandit cursus. Tellus in hac habitasse platea dictumst vestibulum.
Massa ultricies mi quis hendrerit. Molestie a iaculis at erat pellentesque adipiscing commodo.
Vulputate eu scelerisque felis imperdiet proin fermentum. Vitae congue eu consequat ac felis. Nec
ultrices dui sapien eget mi proin sed. Nunc mattis enim ut tellus elementum sagittis vitae et.
Mauris ultrices eros in cursus turpis massa tincidunt dui ut. Nisi porta lorem mollis aliquam ut
porttitor leo a diam. Diam phasellus vestibulum lorem sed risus ultricies. Arcu vitae elementum
curabitur vitae nunc sed velit dignissim. Ut eu sem integer vitae justo eget magna fermentum
iaculis.In eu mi bibendum neque.
)";

inline auto quoted(std::string_view expression) -> std::string {
  return fmt::format("'{}'", expression);
}

TEST(Regex, UnknownToken) {
  EXPECT_THROW("N"_rx, ParserException);
  EXPECT_THROW(")"_rx, ParserException);
  EXPECT_THROW("ù"_rx, ParserException);
}

TEST(Regex, Text) {
  EXPECT_TRUE("'abc'"_rx.match("abc"));
  EXPECT_TRUE("'abc'"_rx.match("abcccccccccc"));
  EXPECT_TRUE("'hello' ' ' 'world'"_rx.match("hello world"));
  EXPECT_TRUE("'hello\nworld'"_rx.match("hello\nworld"));
  EXPECT_TRUE(Regex(quoted(LOREM_IPSUM)).match(LOREM_IPSUM));

  EXPECT_THROW("'hello"_rx, ParserException);
  EXPECT_THROW("hello'"_rx, ParserException);
  EXPECT_THROW("hello"_rx, ParserException);

  EXPECT_FALSE("'cba'"_rx.match("abc"));
  EXPECT_FALSE("'cbaa'"_rx.match("abcc"));
  EXPECT_FALSE(Regex(quoted(LOREM_IPSUM)).match(LOREM_IPSUM.substr(1)));
  EXPECT_FALSE(Regex(quoted(LOREM_IPSUM)).match(LOREM_IPSUM.substr(0, LOREM_IPSUM.size() - 2)));
}

TEST(Regex, Range) {
  EXPECT_TRUE("_"_rx.match("\n"));
  EXPECT_TRUE("a"_rx.match("a"));
  EXPECT_TRUE("o"_rx.match("+"));
  EXPECT_TRUE("n"_rx.match("7"));
  EXPECT_TRUE("Q"_rx.match("\""));
  EXPECT_TRUE("q"_rx.match("'"));

  EXPECT_FALSE("_"_rx.match("b"));
  EXPECT_FALSE("a"_rx.match("4"));
  EXPECT_FALSE("o"_rx.match("\t"));
  EXPECT_FALSE("n"_rx.match("|"));
  EXPECT_FALSE("Q"_rx.match("^"));
  EXPECT_FALSE("q"_rx.match("&"));
}

TEST(Regex, Sequence) {
  EXPECT_TRUE("{'abc'}"_rx.match("abc"));
  EXPECT_TRUE("{'ab'} {'c'}"_rx.match("abc"));
  EXPECT_TRUE("{{{{{{'ab'} {'c'}}}}}}"_rx.match("abc"));

  EXPECT_THROW("{'abc'"_rx, ParserException);
  EXPECT_THROW("{"_rx, ParserException);
  EXPECT_THROW("}"_rx, ParserException);
  EXPECT_THROW("{{{'abc'"_rx, ParserException);
  EXPECT_THROW("'abc'}}}"_rx, ParserException);
}

TEST(Regex, Plus) {
  EXPECT_TRUE("{'abc'}+"_rx.match("abcabcabc"));
  EXPECT_TRUE("{'ab'n}+"_rx.match("ab1ab2ab3"));
  EXPECT_TRUE("n+n+"_rx.match("12"));

  EXPECT_THROW("+"_rx, ParserException);
  EXPECT_THROW("++"_rx, ParserException);
  EXPECT_THROW("+a"_rx, ParserException);
  EXPECT_THROW("{}+"_rx, ParserException);
}

TEST(Regex, Star) {
  EXPECT_TRUE("{'abc'}*"_rx.match("abc"));
  EXPECT_TRUE("{'abc'}*"_rx.match(""));
  EXPECT_TRUE("{'ab'n}*"_rx.match("ab1ab2ab3"));
  EXPECT_TRUE("{{{'hello'}}}*"_rx.match(""));
  EXPECT_TRUE("{{{'hello'}}}*"_rx.match("hellohellohello"));

  EXPECT_THROW("*"_rx, ParserException);
  EXPECT_THROW("***"_rx, ParserException);
  EXPECT_THROW("*a"_rx, ParserException);
  EXPECT_THROW("{}*"_rx, ParserException);
}

TEST(Regex, Quest) {
  EXPECT_TRUE("{'abc'}?"_rx.match("abc"));
  EXPECT_TRUE("{'abc'}?"_rx.match(""));
  EXPECT_TRUE("{'ab'n}?"_rx.match("ab1"));
  EXPECT_TRUE("{{{'hello'}}}?"_rx.match(""));
  EXPECT_TRUE("{{{'hello'}}}?"_rx.match("hello"));

  EXPECT_THROW("?"_rx, ParserException);
  EXPECT_THROW("???"_rx, ParserException);
  EXPECT_THROW("?a"_rx, ParserException);
  EXPECT_THROW("{}?"_rx, ParserException);
}

TEST(Regex, Or) {
  EXPECT_TRUE("{'a'|'b'}"_rx.match("a"));
  EXPECT_TRUE("{'a'|'b'}"_rx.match("a"));
  EXPECT_TRUE("{'a' | 'b'}"_rx.match("a"));
  EXPECT_TRUE("{'a' | 'b'}"_rx.match("b"));
  EXPECT_TRUE("a{a|'_'|n}*"_rx.match("snake_case_variable123"));

  EXPECT_THROW("|"_rx, ParserException);
  EXPECT_THROW("||"_rx, ParserException);
  EXPECT_THROW("|||"_rx, ParserException);
  EXPECT_THROW("'a'|{}"_rx, ParserException);
  EXPECT_THROW("{}|'b'"_rx, ParserException);
  EXPECT_THROW("'a'|"_rx, ParserException);
  EXPECT_THROW("|'b'"_rx, ParserException);
}

TEST(Regex, Wave) {
  EXPECT_TRUE("{'a'~'f'}"_rx.match("abcdef"));
  EXPECT_EQ("~'9'"_rx.match("4382923").view(), "43829");
  EXPECT_THROW("{'a'~{}}"_rx.match("abcdef"), ParserException);
}

}  // namespace ccc::regex

#endif
