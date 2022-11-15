#ifndef CCC_TEST_REGEX_HPP
#define CCC_TEST_REGEX_HPP

#include "regex/parser_exception.hpp"
#include "regex/regex.hpp"
#include <catch2/catch_test_macros.hpp>

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

TEST_CASE("Regex") {
  SECTION("Regex: Unknown tokens") {
    CHECK_THROWS_AS("N"_rx, ParserException);
    CHECK_THROWS_AS(")"_rx, ParserException);
    CHECK_THROWS_AS("ù"_rx, ParserException);
  }

  SECTION("Regex: Literals") {
    SECTION("Basic") {
      CHECK("'abc'"_rx.match("abc"));
      CHECK("'abc'"_rx.match("abcccccccccc"));
      CHECK("'hello' ' ' 'world'"_rx.match("hello world"));
      CHECK("'hello\nworld'"_rx.match("hello\nworld"));
      CHECK(Regex(quoted(LOREM_IPSUM)).match(LOREM_IPSUM));
    }

    SECTION("Invalid") {
      CHECK_THROWS_AS("'hello"_rx, ParserException);
      CHECK_THROWS_AS("hello'"_rx, ParserException);
      CHECK_THROWS_AS("hello"_rx, ParserException);
    }

    SECTION("False") {
      CHECK_FALSE("'cba'"_rx.match("abc"));
      CHECK_FALSE("'cbaa'"_rx.match("abcc"));
      CHECK_FALSE(Regex(quoted(LOREM_IPSUM)).match(LOREM_IPSUM.substr(1)));
      CHECK_FALSE(Regex(quoted(LOREM_IPSUM)).match(LOREM_IPSUM.substr(0, LOREM_IPSUM.size() - 2)));
    }
  }

  SECTION("Regex: Character classes") {
    SECTION("Basic") {
      CHECK("_"_rx.match("\n"));
      CHECK("a"_rx.match("a"));
      CHECK("o"_rx.match("+"));
      CHECK("n"_rx.match("7"));
      CHECK("Q"_rx.match("\""));
      CHECK("q"_rx.match("'"));
    }

    SECTION("False") {
      CHECK_FALSE("_"_rx.match("b"));
      CHECK_FALSE("a"_rx.match("4"));
      CHECK_FALSE("o"_rx.match("\t"));
      CHECK_FALSE("n"_rx.match("|"));
      CHECK_FALSE("Q"_rx.match("^"));
      CHECK_FALSE("q"_rx.match("&"));
    }
  }

  SECTION("Regex: Sequences") {
    SECTION("Basic") {
      CHECK("{'abc'}"_rx.match("abc"));
      CHECK("{'ab'} {'c'}"_rx.match("abc"));
      CHECK("{{{{{{'ab'} {'c'}}}}}}"_rx.match("abc"));
    }

    SECTION("Invalid") {
      CHECK_THROWS_AS("{'abc'"_rx, ParserException);
      CHECK_THROWS_AS("{"_rx, ParserException);
      CHECK_THROWS_AS("}"_rx, ParserException);
      CHECK_THROWS_AS("{{{'abc'"_rx, ParserException);
      CHECK_THROWS_AS("'abc'}}}"_rx, ParserException);
    }
  }

  SECTION("Regex: Quantifier plus") {
    SECTION("Basic") {
      CHECK("{'abc'}+"_rx.match("abcabcabc"));
      CHECK("{'ab'n}+"_rx.match("ab1ab2ab3"));
      CHECK("n+n+"_rx.match("12"));
    }

    SECTION("Invalid") {
      CHECK_THROWS_AS("+"_rx, ParserException);
      CHECK_THROWS_AS("++"_rx, ParserException);
      CHECK_THROWS_AS("+a"_rx, ParserException);
      CHECK_THROWS_AS("{}+"_rx, ParserException);
    }
  }

  SECTION("Regex: Quantifier kleene") {
    SECTION("Basic") {
      CHECK("{'abc'}*"_rx.match("abc"));
      CHECK("{'abc'}*"_rx.match(""));
      CHECK("{'ab'n}*"_rx.match("ab1ab2ab3"));
      CHECK("{{{'hello'}}}*"_rx.match(""));
      CHECK("{{{'hello'}}}*"_rx.match("hellohellohello"));
    }

    SECTION("Invalid") {
      CHECK_THROWS_AS("*"_rx, ParserException);
      CHECK_THROWS_AS("***"_rx, ParserException);
      CHECK_THROWS_AS("*a"_rx, ParserException);
      CHECK_THROWS_AS("{}*"_rx, ParserException);
    }
  }

  SECTION("Regex: Quantifier quest") {
    SECTION("Basic") {
      CHECK("{'abc'}?"_rx.match("abc"));
      CHECK("{'abc'}?"_rx.match(""));
      CHECK("{'ab'n}?"_rx.match("ab1"));
      CHECK("{{{'hello'}}}?"_rx.match(""));
      CHECK("{{{'hello'}}}?"_rx.match("hello"));
    }

    SECTION("Invalid") {
      CHECK_THROWS_AS("?"_rx, ParserException);
      CHECK_THROWS_AS("???"_rx, ParserException);
      CHECK_THROWS_AS("?a"_rx, ParserException);
      CHECK_THROWS_AS("{}?"_rx, ParserException);
    }
  }

  SECTION("Regex: Alternative") {
    SECTION("Basic") {
      CHECK("{'a'|'b'}"_rx.match("a"));
      CHECK("{'a'|'b'}"_rx.match("a"));
      CHECK("{'a' | 'b'}"_rx.match("a"));
      CHECK("{'a' | 'b'}"_rx.match("b"));
      CHECK("a{a|'_'|n}*"_rx.match("snake_case_variable123"));
    }

    SECTION("Invalid") {
      CHECK_THROWS_AS("|"_rx, ParserException);
      CHECK_THROWS_AS("||"_rx, ParserException);
      CHECK_THROWS_AS("|||"_rx, ParserException);

      CHECK_THROWS_AS("'a'|{}"_rx, ParserException);
      CHECK_THROWS_AS("{}|'b'"_rx, ParserException);
      CHECK_THROWS_AS("'a'|"_rx, ParserException);
      CHECK_THROWS_AS("|'b'"_rx, ParserException);
    }
  }

  SECTION("Regex: Wave") {
    CHECK("{'a'~'f'}"_rx.match("abcdef"));
    CHECK_THROWS_AS("{'a'~{}}"_rx.match("abcdef"), ParserException);
  }
}

}  // namespace ccc::regex

#endif
