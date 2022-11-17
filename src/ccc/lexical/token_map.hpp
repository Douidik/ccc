#ifndef CCC_TOKEN_MAP_HPP
#define CCC_TOKEN_MAP_HPP

#include "regex/regex.hpp"
#include "trait.hpp"
#include <span>

namespace ccc {
using namespace trait;
using namespace regex::literals;

static auto token_map() {
  static const std::pair<u32, Regex> map[] {
    {Blank, "{_|'@'}+"_rx},
    {CommentSL, "'//' ~ '\n'"_rx},
    {CommentML, "'/*' ~ '*/'"_rx},

    {HsDefine, "'#' _* 'define'"_rx},
    {HsUndef, "'#' _* 'undef'"_rx},
    {HsIf, "'#' _* 'if'"_rx},
    {HsElse, "'#' _* 'else'"_rx},
    {HsEndif, "'#' _* 'endif'"_rx},
    {HsIfdef, "'#' _* 'ifdef'"_rx},
    {HsIfndef, "'#' _* 'ifndef'"_rx},

    {Sizeof, "'sizeof'"_rx},
    {Star, "'*'"_rx},
    {Ampersand, "'&'"_rx},

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

    {Identifier, "{a|'_'} {a|'_'|n}*"_rx},
    {Float, "{'+'|'-'}* {n* '.' n+} | {n+ '.' n*} 'f'?"_rx},
    {Integer, "{'+'|'-'}* n+"_rx},
    {String, "Q ~ {'\n' | Q}"_rx},
    {Char, "q ~ q"_rx},

    {Semicolon, "';'"_rx},
    {Comma, "','"_rx},
    {Increment, "'++'"_rx},
    {Decrement, "'--'"_rx},
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
    {Mul, "'*'"_rx},
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

    {None, "~_"_rx},
  };

  return std::span(map, sizeof(map));
}

using TokenMap = std::invoke_result_t<decltype(token_map)>;

}  // namespace ccc

#endif
