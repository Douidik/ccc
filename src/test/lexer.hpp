#ifndef CCC_TEST_LEXER_HPP
#define CCC_TEST_LEXER_HPP

#include "lexical/lexer.hpp"
#include <gtest/gtest.h>
#include <magic_enum.hpp>

namespace ccc {
using namespace trait;

static auto match_tokens(Lexer lexer, const std::vector<Token> &&tokens)
  -> testing::AssertionResult {
  for (const Token &expected : tokens) {
    auto token = lexer.tokenize();

    if (token.trait != expected.trait) {
      auto token_trait = trait_name(token.trait);
      auto expected_trait = trait_name(expected.trait);
      return testing::AssertionFailure() << token_trait << " != " << expected_trait;
    }

    if (!(token.trait & CsKeyword) && token.view != expected.view) {
      return testing::AssertionFailure() << "'" << token.view << "' != '" << expected.view << "'";
    }
  }

  return testing::AssertionSuccess();
}

TEST(Lexer, Basic) {
  EXPECT_TRUE(match_tokens(
    Lexer {"int main() { return 0; }"},
    {
      {"int", KwInt},
      {"main", Identifier},
      {"(", ParenBegin},
      {")", ParenClose},
      {"{", CurlyBegin},
      {"return", KwReturn},
      {"0", Integer},
      {";", Semicolon},
      {"}", CurlyClose},
    }));
}

}  // namespace ccc

#endif
