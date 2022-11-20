#ifndef CCC_SYNTAX_MAP_HPP
#define CCC_SYNTAX_MAP_HPP

#include "regex/regex.hpp"
#include "trait.hpp"
#include <span>

namespace ccc {
using namespace trait;
using namespace regex::literals;

using SyntaxMap = std::span<const std::pair<u32, Regex>>;

static auto syntax_ansi() -> SyntaxMap {
  
  static const std::pair<u32, Regex> map[] {
    {Blank, "{_|'@'}+"_rx},
    {CommentSL, "'//' ~ '\n'"_rx},
    {CommentML, "'/*' ~ '*/'"_rx},
    {Directive, "'#' {~ '\\'}* ~ '\n'"_rx},

    {Sizeof, "'sizeof' / {o|_}"_rx},
    {Star, "'*'"_rx},

    {KwAuto, "'auto' / {o|_}"_rx},
    {KwDouble, "'double' / {o|_}"_rx},
    {KwChar, "'char' / {o|_}"_rx},
    {KwFloat, "'float' / {o|_}"_rx},
    {KwInt, "'int' / {o|_}"_rx},
    {KwLong, "'long' / {o|_}"_rx},
    {KwShort, "'short' / {o|_}"_rx},
    {KwVoid, "'void' / {o|_}"_rx},
    {KwEnum, "'enum' / {o|_}"_rx},
    {KwTypedef, "'typedef' / {o|_}"_rx},
    {KwUnion, "'union' / {o|_}"_rx},
    {KwStruct, "'struct' / {o|_}"_rx},
    {KwVolatile, "'volatile' / {o|_}"_rx},
    {KwConst, "'const' / {o|_}"_rx},
    {KwExtern, "'extern' / {o|_}"_rx},
    {KwRegister, "'register' / {o|_}"_rx},
    {KwStatic, "'static' / {o|_}"_rx},
    {KwSigned, "'signed' / {o|_}"_rx},
    {KwUnsigned, "'unsigned' / {o|_}"_rx},
    {KwBreak, "'break' / {o|_}"_rx},
    {KwCase, "'case' / {o|_}"_rx},
    {KwContinue, "'continue' / {o|_}"_rx},
    {KwDefault, "'default' / {o|_}"_rx},
    {KwDo, "'do' / {o|_}"_rx},
    {KwElse, "'else' / {o|_}"_rx},
    {KwFor, "'for' / {o|_}"_rx},
    {KwGoto, "'goto' / {o|_}"_rx},
    {KwIf, "'if' / {o|_}"_rx},
    {KwReturn, "'return' / {o|_}"_rx},
    {KwSwitch, "'switch' / {o|_}"_rx},
    {KwWhile, "'while' / {o|_}"_rx},

    {CurlyBegin, "'{'"_rx},
    {CurlyClose, "'}'"_rx},
    {ParenBegin, "'('"_rx},
    {ParenClose, "')'"_rx},
    {CrochetBegin, "']'"_rx},
    {CrochetClose, "'['"_rx},

    {Increment, "'++'"_rx},
    {Decrement, "'--'"_rx},

    {
      Integer,
      "{'+' | '-'}*"
      "{'0b' {'0'|'1'}+} |"
      "{'0x' { a | n }+} |"
      "{     {   n   }+}"
      "a?"_rx,
    },

    {
      Float,
      " {'+' | '-'}*"
      " {'0b' {{'0'|'1'}+ '.' {'0'|'1'}*} | {{'0'|'1'}+ '.' {'0'|'1'}*}} |"
      " {'0x' {{ a | n }+ '.' { a | n }*} | {{ a | n }+ '.' { a | n }*}} |"
      " {     {{   n   }+ '.' {   n   }*} | {{   n   }+ '.' {   n   }*}}  "
      
      "{{'e'|'E'} {'-'|'+'}? n+}?"
      "a?"_rx,
    },

    {Identifier, "{a|'_'} {a|'_'|n}*"_rx},
    {Integer, " {'+'|'-'}* n+"_rx},
    {String, "Q ~ {'\n' | Q}"_rx},
    {Char, "'L'? {q ~ q}"_rx},

    {Semicolon, "';'"_rx},
    {Comma, "','"_rx},
    {And, "'&&'"_rx},
    {Or, "'||'"_rx},
    {Dot, "'.'"_rx},
    {Arrow, "'->'"_rx},
    {BinNot, "'~'"_rx},
    {BinOr, "'|'"_rx},
    {BinXor, "'^'"_rx},
    {BinShiftL, "'<<'"_rx},
    {BinShiftR, "'>>'"_rx},
    {Add, "'+'"_rx},
    {Sub, "'-'"_rx},
    {Div, "'/'"_rx},
    {Mod, "'%'"_rx},
    {Equal, "'=='"_rx},
    {NotEq, "'!='"_rx},
    {LessEq, "'<='"_rx},
    {GreaterEq, "'>='"_rx},
    {Less, "'<'"_rx},
    {Greater, "'>'"_rx},
    {Not, "'!'"_rx},
    {Assign, "'='"_rx},
    {Ampersand, "'&'"_rx},

    {None, "{~/_}"_rx},
  };

  return {map};
}

static auto syntax_directives() -> SyntaxMap {
  static const std::pair<u32, Regex> map[] {
    {0, "~/_"_rx},
  };

  return map;
}

}  // namespace ccc

#endif
