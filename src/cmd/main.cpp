#include "lexical/lexer.hpp"
#include "lexical/lexer_exception.hpp"
#include "regex/graphviz.hpp"
#include "regex/regex.hpp"
#include <fmt/format.h>
#include <iostream>
#include <magic_enum.hpp>

using namespace ccc;
using namespace regex;
using namespace regex::literals;

constexpr std::string_view source = R"(
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
)";

namespace ccc {

auto print_tokens(Lexer &lexer, size_t count = 0) -> size_t {
  Token token {};

  try {
    token = lexer.tokenize();
  } catch (const LexerException &exception) {
    std::cerr << exception.what() << std::endl;
  }

  if (token.trait == trait::End) {
    return count;
  }

  fmt::print("{:<{}} => {}\n", token.view, 15, trait_name(token.trait));
  return print_tokens(lexer, count + 1);
}

}  // namespace ccc

#include <fstream>

int main(int argc, char **argv) {
  // Lexer lexer {source};
  // fmt::print("Lexer tokenized #{} tokens from source\n", print_tokens(lexer));

  auto map = token_map();

  for (const auto &[trait, regex] : map) {
    std::ofstream stream {fmt::format("graph/{}.dot", trait_name(trait))};
    stream << Graphviz(regex, GraphvizAnon, trait_name(trait)).document() << std::endl;
  }
}
