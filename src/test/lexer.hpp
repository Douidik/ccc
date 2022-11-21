#ifndef CCC_TEST_LEXER_HPP
#define CCC_TEST_LEXER_HPP

#include "lexical/lexer.hpp"
#include "lexical/lexer_exception.hpp"
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

#define EXPECT_TOKENS(source, ...) EXPECT_TRUE(match_tokens(Lexer {source}, {__VA_ARGS__}))

#define EXPECT_LEXER_EXCEPTION(source) \
  EXPECT_THROW( \
    { \
      Lexer lexer {source}; \
      for (;;) { \
        Token token = lexer.tokenize(); \
        if (token.trait == End) \
          break; \
      } \
    }, \
    LexerException)

TEST(Lexer, Comment) {
  EXPECT_TOKENS("// Hello World \n", {"// Hello World \n", CommentSL});

  EXPECT_TOKENS(
    R"(
// This is a comment 
int main() {
// A second one //
)",
    {"// This is a comment \n", CommentSL},
    {"int", KwInt},
    {"main", Identifier},
    {"(", ParenBegin},
    {")", ParenClose},
    {"{", CurlyBegin},
    {"// A second one //\n", CommentSL});

  EXPECT_TOKENS("/* Hello World */\n", {"/* Hello World */", CommentML});

  EXPECT_TOKENS(
    R"(
/*
 * main() -> int
 * Arguments: None
 * Return: Program execution status, if main() != 0, then the program execution failed
 * This function is the program starting point of the program, must be declared in any program
*/
int main() {
/* This is an inline comment, that is delimited by :*/
)",

    {R"(/*
 * main() -> int
 * Arguments: None
 * Return: Program execution status, if main() != 0, then the program execution failed
 * This function is the program starting point of the program, must be declared in any program
*/)",
     CommentML},
    {"int", KwInt},
    {"main", Identifier},
    {"(", ParenBegin},
    {")", ParenClose},
    {"{", CurlyBegin},
    {"/* This is an inline comment, that is delimited by :*/", CommentML});

  EXPECT_LEXER_EXCEPTION("/* This comment should be terminated !!! \n");

  EXPECT_LEXER_EXCEPTION(
    R"(/* 
This
comment
should
be
terminated
!!!
)");
}

TEST(Lexer, Directive) {
  EXPECT_TOKENS("#define TRUE (1)\n", {"#define TRUE (1)\n", Directive});
  EXPECT_TOKENS("#define FALSE (0)\n", {"#define FALSE (0)\n", Directive});

  EXPECT_TOKENS(
    R"(
#define POW(x, n) do { for (int i = 0; i < n; i++) { \
  x = x * x;                                         \
} while (0)

int main() {}
)",
    {
      R"(#define POW(x, n) do { for (int i = 0; i < n; i++) { \
  x = x * x;                                         \
} while (0)
)",
      Directive,
    });

  EXPECT_TOKENS("#  include <stdio.h>\n", {"#  include <stdio.h>\n", Directive});

  EXPECT_TOKENS(
    R"(
#ifndef CCC_EXAMPLE_H
#  define CCC_EXAMPLE_H

#  define CCC_VAL \
  (-1)  

#endif
)",
    {"#ifndef CCC_EXAMPLE_H\n", Directive},
    {"#  define CCC_EXAMPLE_H\n", Directive},
    {"#  define CCC_VAL \\\n  (-1)  \n", Directive},
    {"#endif", Directive});
}

TEST(Lexer, LongSource) {
  EXPECT_TOKENS(
    R"(
/* return non-zero if magic sequence is detected */
/* warning: reset the read pointer to the beginning of the file */
int detect_magic(FILE* f) {
  unsigned char buffer[8];
  size_t bytes_read;
  int c;

  fseek(f, SEEK_SET, 0);
  bytes_read = fread(buffer, 1, 8, f);
  fseek(f, SEEK_SET, 0);
  if (bytes_read < 8) return 0;

  for (c = 0; c < 8; c++)
    if (buffer[c] != sixpack_magic[c]) return 0;

  return -1;
}

static unsigned long readU16(const unsigned char* ptr) { return ptr[0] + (ptr[1] << 8); }

static unsigned long readU32(const unsigned char* ptr) {
  return ptr[0] + (ptr[1] << 8) + (ptr[2] << 16) + (ptr[3] << 24);
}

void read_chunk_header(FILE* f, int* id, int* options, unsigned long* size, unsigned long* checksum,
                       unsigned long* extra) {
  unsigned char buffer[16];
  fread(buffer, 1, 16, f);

  *id = readU16(buffer) & 0xffff;
  *options = readU16(buffer + 2) & 0xffff;
  *size = readU32(buffer + 4) & 0xffffffff;
  *checksum = readU32(buffer + 8) & 0xffffffff;
  *extra = readU32(buffer + 12) & 0xffffffff;
}

int unpack_file(const char* input_file) {
  FILE* in;
  unsigned long fsize;
  int c;
  unsigned long percent;
  unsigned char progress[20];
  int chunk_id;
  int chunk_options;
  unsigned long chunk_size;
  unsigned long chunk_checksum;
  unsigned long chunk_extra;
  unsigned char buffer[BLOCK_SIZE];
  unsigned long checksum;

  unsigned long decompressed_size;
  unsigned long total_extracted;
  int name_length;
  char* output_file;
  FILE* f;

  unsigned char* compressed_buffer;
  unsigned char* decompressed_buffer;
  unsigned long compressed_bufsize;
  unsigned long decompressed_bufsize;

  /* sanity check */
  in = fopen(input_file, "rb");
  if (!in) {
    printf("Error: could not open %s\n", input_file);
    return -1;
  }
)",
    {"/* return non-zero if magic sequence is detected */", CommentML},
    {"/* warning: reset the read pointer to the beginning of the file */", CommentML},
    {"int", KwInt},
    {"detect_magic", Identifier},
    {"(", ParenBegin},
    {"FILE", Identifier},
    {"*", Star},
    {"f", Identifier},
    {")", ParenClose},
    {"{", CurlyBegin},
    {"unsigned", KwUnsigned},
    {"char", KwChar},
    {"buffer", Identifier},
    {"[", CrochetClose},
    {"8", Integer},
    {"]", CrochetBegin},
    {";", Semicolon},
    {"size_t", Identifier},
    {"bytes_read", Identifier},
    {";", Semicolon},
    {"int", KwInt},
    {"c", Identifier},
    {";", Semicolon},
    {"fseek", Identifier},
    {"(", ParenBegin},
    {"f", Identifier},
    {",", Comma},
    {"SEEK_SET", Identifier},
    {",", Comma},
    {"0", Integer},
    {")", ParenClose},
    {";", Semicolon},
    {"bytes_read", Identifier},
    {"=", Assign},
    {"fread", Identifier},
    {"(", ParenBegin},
    {"buffer", Identifier},
    {",", Comma},
    {"1", Integer},
    {",", Comma},
    {"8", Integer},
    {",", Comma},
    {"f", Identifier},
    {")", ParenClose},
    {";", Semicolon},
    {"fseek", Identifier},
    {"(", ParenBegin},
    {"f", Identifier},
    {",", Comma},
    {"SEEK_SET", Identifier},
    {",", Comma},
    {"0", Integer},
    {")", ParenClose},
    {";", Semicolon},
    {"if", KwIf},
    {"(", ParenBegin},
    {"bytes_read", Identifier},
    {"<", Less},
    {"8", Integer},
    {")", ParenClose},
    {"return", KwReturn},
    {"0", Integer},
    {";", Semicolon},
    {"for", KwFor},
    {"(", ParenBegin},
    {"c", Identifier},
    {"=", Assign},
    {"0", Integer},
    {";", Semicolon},
    {"c", Identifier},
    {"<", Less},
    {"8", Integer},
    {";", Semicolon},
    {"c", Identifier},
    {"++", Increment},
    {")", ParenClose},
    {"if", KwIf},
    {"(", ParenBegin},
    {"buffer", Identifier},
    {"[", CrochetClose},
    {"c", Identifier},
    {"]", CrochetBegin},
    {"!=", NotEq},
    {"sixpack_magic", Identifier},
    {"[", CrochetClose},
    {"c", Identifier},
    {"]", CrochetBegin},
    {")", ParenClose},
    {"return", KwReturn},
    {"0", Integer},
    {";", Semicolon},
    {"return", KwReturn},
    {"-1", Integer},
    {";", Semicolon},
    {"}", CurlyClose},
    {"static", KwStatic},
    {"unsigned", KwUnsigned},
    {"long", KwLong},
    {"readU16", Identifier},
    {"(", ParenBegin},
    {"const", KwConst},
    {"unsigned", KwUnsigned},
    {"char", KwChar},
    {"*", Star},
    {"ptr", Identifier},
    {")", ParenClose},
    {"{", CurlyBegin},
    {"return", KwReturn},
    {"ptr", Identifier},
    {"[", CrochetClose},
    {"0", Integer},
    {"]", CrochetBegin},
    {"+", Add},
    {"(", ParenBegin},
    {"ptr", Identifier},
    {"[", CrochetClose},
    {"1", Integer},
    {"]", CrochetBegin},
    {"<<", BinShiftL},
    {"8", Integer},
    {")", ParenClose},
    {";", Semicolon},
    {"}", CurlyClose},
    {"static", KwStatic},
    {"unsigned", KwUnsigned},
    {"long", KwLong},
    {"readU32", Identifier},
    {"(", ParenBegin},
    {"const", KwConst},
    {"unsigned", KwUnsigned},
    {"char", KwChar},
    {"*", Star},
    {"ptr", Identifier},
    {")", ParenClose},
    {"{", CurlyBegin},
    {"return", KwReturn},
    {"ptr", Identifier},
    {"[", CrochetClose},
    {"0", Integer},
    {"]", CrochetBegin},
    {"+", Add},
    {"(", ParenBegin},
    {"ptr", Identifier},
    {"[", CrochetClose},
    {"1", Integer},
    {"]", CrochetBegin},
    {"<<", BinShiftL},
    {"8", Integer},
    {")", ParenClose},
    {"+", Add},
    {"(", ParenBegin},
    {"ptr", Identifier},
    {"[", CrochetClose},
    {"2", Integer},
    {"]", CrochetBegin},
    {"<<", BinShiftL},
    {"16", Integer},
    {")", ParenClose},
    {"+", Add},
    {"(", ParenBegin},
    {"ptr", Identifier},
    {"[", CrochetClose},
    {"3", Integer},
    {"]", CrochetBegin},
    {"<<", BinShiftL},
    {"24", Integer},
    {")", ParenClose},
    {";", Semicolon},
    {"}", CurlyClose},
    {"void", KwVoid},
    {"read_chunk_header", Identifier},
    {"(", ParenBegin},
    {"FILE", Identifier},
    {"*", Star},
    {"f", Identifier},
    {",", Comma},
    {"int", KwInt},
    {"*", Star},
    {"id", Identifier},
    {",", Comma},
    {"int", KwInt},
    {"*", Star},
    {"options", Identifier},
    {",", Comma},
    {"unsigned", KwUnsigned},
    {"long", KwLong},
    {"*", Star},
    {"size", Identifier},
    {",", Comma},
    {"unsigned", KwUnsigned},
    {"long", KwLong},
    {"*", Star},
    {"checksum", Identifier},
    {",", Comma},
    {"unsigned", KwUnsigned},
    {"long", KwLong},
    {"*", Star},
    {"extra", Identifier},
    {")", ParenClose},
    {"{", CurlyBegin},
    {"unsigned", KwUnsigned},
    {"char", KwChar},
    {"buffer", Identifier},
    {"[", CrochetClose},
    {"16", Integer},
    {"]", CrochetBegin},
    {";", Semicolon},
    {"fread", Identifier},
    {"(", ParenBegin},
    {"buffer", Identifier},
    {",", Comma},
    {"1", Integer},
    {",", Comma},
    {"16", Integer},
    {",", Comma},
    {"f", Identifier},
    {")", ParenClose},
    {";", Semicolon},
    {"*", Star},
    {"id", Identifier},
    {"=", Assign},
    {"readU16", Identifier},
    {"(", ParenBegin},
    {"buffer", Identifier},
    {")", ParenClose},
    {"&", Ampersand},
    {"0xffff", Integer},
    {";", Semicolon},
    {"*", Star},
    {"options", Identifier},
    {"=", Assign},
    {"readU16", Identifier},
    {"(", ParenBegin},
    {"buffer", Identifier},
    {"+", Add},
    {"2", Integer},
    {")", ParenClose},
    {"&", Ampersand},
    {"0xffff", Integer},
    {";", Semicolon},
    {"*", Star},
    {"size", Identifier},
    {"=", Assign},
    {"readU32", Identifier},
    {"(", ParenBegin},
    {"buffer", Identifier},
    {"+", Add},
    {"4", Integer},
    {")", ParenClose},
    {"&", Ampersand},
    {"0xffffffff", Integer},
    {";", Semicolon},
    {"*", Star},
    {"checksum", Identifier},
    {"=", Assign},
    {"readU32", Identifier},
    {"(", ParenBegin},
    {"buffer", Identifier},
    {"+", Add},
    {"8", Integer},
    {")", ParenClose},
    {"&", Ampersand},
    {"0xffffffff", Integer},
    {";", Semicolon},
    {"*", Star},
    {"extra", Identifier},
    {"=", Assign},
    {"readU32", Identifier},
    {"(", ParenBegin},
    {"buffer", Identifier},
    {"+", Add},
    {"12", Integer},
    {")", ParenClose},
    {"&", Ampersand},
    {"0xffffffff", Integer},
    {";", Semicolon},
    {"}", CurlyClose},
    {"int", KwInt},
    {"unpack_file", Identifier},
    {"(", ParenBegin},
    {"const", KwConst},
    {"char", KwChar},
    {"*", Star},
    {"input_file", Identifier},
    {")", ParenClose},
    {"{", CurlyBegin},
    {"FILE", Identifier},
    {"*", Star},
    {"in", Identifier},
    {";", Semicolon},
    {"unsigned", KwUnsigned},
    {"long", KwLong},
    {"fsize", Identifier},
    {";", Semicolon},
    {"int", KwInt},
    {"c", Identifier},
    {";", Semicolon},
    {"unsigned", KwUnsigned},
    {"long", KwLong},
    {"percent", Identifier},
    {";", Semicolon},
    {"unsigned", KwUnsigned},
    {"char", KwChar},
    {"progress", Identifier},
    {"[", CrochetClose},
    {"20", Integer},
    {"]", CrochetBegin},
    {";", Semicolon},
    {"int", KwInt},
    {"chunk_id", Identifier},
    {";", Semicolon},
    {"int", KwInt},
    {"chunk_options", Identifier},
    {";", Semicolon},
    {"unsigned", KwUnsigned},
    {"long", KwLong},
    {"chunk_size", Identifier},
    {";", Semicolon},
    {"unsigned", KwUnsigned},
    {"long", KwLong},
    {"chunk_checksum", Identifier},
    {";", Semicolon},
    {"unsigned", KwUnsigned},
    {"long", KwLong},
    {"chunk_extra", Identifier},
    {";", Semicolon},
    {"unsigned", KwUnsigned},
    {"char", KwChar},
    {"buffer", Identifier},
    {"[", CrochetClose},
    {"BLOCK_SIZE", Identifier},
    {"]", CrochetBegin},
    {";", Semicolon},
    {"unsigned", KwUnsigned},
    {"long", KwLong},
    {"checksum", Identifier},
    {";", Semicolon},
    {"unsigned", KwUnsigned},
    {"long", KwLong},
    {"decompressed_size", Identifier},
    {";", Semicolon},
    {"unsigned", KwUnsigned},
    {"long", KwLong},
    {"total_extracted", Identifier},
    {";", Semicolon},
    {"int", KwInt},
    {"name_length", Identifier},
    {";", Semicolon},
    {"char", KwChar},
    {"*", Star},
    {"output_file", Identifier},
    {";", Semicolon},
    {"FILE", Identifier},
    {"*", Star},
    {"f", Identifier},
    {";", Semicolon},
    {"unsigned", KwUnsigned},
    {"char", KwChar},
    {"*", Star},
    {"compressed_buffer", Identifier},
    {";", Semicolon},
    {"unsigned", KwUnsigned},
    {"char", KwChar},
    {"*", Star},
    {"decompressed_buffer", Identifier},
    {";", Semicolon},
    {"unsigned", KwUnsigned},
    {"long", KwLong},
    {"compressed_bufsize", Identifier},
    {";", Semicolon},
    {"unsigned", KwUnsigned},
    {"long", KwLong},
    {"decompressed_bufsize", Identifier},
    {";", Semicolon},
    {"/* sanity check */", CommentML},
    {"in", Identifier},
    {"=", Assign},
    {"fopen", Identifier},
    {"(", ParenBegin},
    {"input_file", Identifier},
    {",", Comma},
    {"\"rb\"", String},
    {")", ParenClose},
    {";", Semicolon},
    {"if", KwIf},
    {"(", ParenBegin},
    {"!", Not},
    {"in", Identifier},
    {")", ParenClose},
    {"{", CurlyBegin},
    {"printf", Identifier},
    {"(", ParenBegin},
    {"\"Error: could not open %s\\n\"", String},
    {",", Comma},
    {"input_file", Identifier},
    {")", ParenClose},
    {";", Semicolon},
    {"return", KwReturn},
    {"-1", Integer},
    {";", Semicolon},
    {"}", CurlyClose});
}

}  // namespace ccc

#endif
