#ifndef CCC_TOKEN_MAP_HPP
#define CCC_TOKEN_MAP_HPP

#include "category.hpp"
#include "regex/regex.hpp"
#include <span>

namespace ccc {
  
using enum Category;
using namespace regex::literals;

static auto token_map() {
  static const std::pair<Category, Regex> map[] {
    {Blank, "{_|'@'}+"_rx},
    
    {CommentML, "'/*' ~ '*/'"_rx},
    {CommentSL, "'//' ~ '\n'"_rx},

    {PpIfndef, "'#' _* 'ifndef'"_rx},
    {PpIfdef, "'#' _* 'ifdef'"_rx},
    {PpEndif, "'#' _* 'endif'"_rx},
    {PpElse, "'#' _* 'else'"_rx},
    {PpIf, "'#' _* 'if'"_rx},
    {PpUndef, "'#' _* 'undef'"_rx},
    {PpDefine, "'#' _* 'define'"_rx},

    {KwWhile, "'while' {o|_}"_rx},
    {KwVolatile, "'volatile' {o|_}"_rx},
    {KwVoid, "'void' {o|_}"_rx},
    {KwUnsigned, "'unsigned' {o|_}"_rx},
    {KwUnion, "'union' {o|_}"_rx},
    {KwTypedef, "'typedef' {o|_}"_rx},
    {KwSwitch, "'switch' {o|_}"_rx},
    {KwStruct, "'struct' {o|_}"_rx},
    {KwStatic, "'static' {o|_}"_rx},
    {KwSizeof, "'sizeof' {o|_}"_rx},
    {KwSigned, "'signed' {o|_}"_rx},
    {KwShort, "'short' {o|_}"_rx},
    {KwReturn, "'return' {o|_}"_rx},
    {KwRegister, "'register' {o|_}"_rx},
    {KwLong, "'long' {o|_}"_rx},
    {KwInt, "'int' {o|_}"_rx},
    {KwIf, "'if' {o|_}"_rx},
    {KwGoto, "'goto' {o|_}"_rx},
    {KwFor, "'for' {o|_}"_rx},
    {KwFloat, "'float' {o|_}"_rx},
    {KwExtern, "'extern' {o|_}"_rx},
    {KwEnum, "'enum' {o|_}"_rx},
    {KwElse, "'else' {o|_}"_rx},
    {KwDouble, "'double' {o|_}"_rx},
    {KwDo, "'do' {o|_}"_rx},
    {KwDefault, "'default' {o|_}"_rx},
    {KwContinue, "'continue' {o|_}"_rx},
    {KwConst, "'const' {o|_}"_rx},
    {KwCase, "'case' {o|_}"_rx},
    {KwBreak, "'break' {o|_}"_rx},
    {KwAuto, "'auto' {o|_}"_rx},

    {Char, "q ~ q"_rx},
    {String, "Q ~ {'\n' | Q}"_rx},
    {Decimal, "{'+'|'-'}* {n* '.' n+} | {n+ '.' n*} 'f'?"_rx},
    {Integer, "{'+'|'-'}* n+"_rx},
    {Identifier, "{a|'_'} {a|'_'|n}*"_rx},
    
    {BraceClose, "']'"_rx},
    {BraceBegin, "'['"_rx},
    {BraceClose, "')'"_rx},
    {BraceBegin, "'('"_rx},
    {ScopeClose, "'}'"_rx},
    {ScopeBegin, "'{'"_rx},

    {BinShiftR, "'>>'"_rx},
    {BinShiftL, "'<<'"_rx},
    {BinNot, "'~'"_rx},
    {BinXor, "'^'"_rx},
    {BinOr, "'|'"_rx},
    {BinAnd, "'&'"_rx},

    {OpNot, "'!'"_rx},
    {OpOr, "'||'"_rx},
    {OpAnd, "'&&'"_rx},

    {OpEqGreater, "'>='"_rx},
    {OpGreater, "'>'"_rx},
    {OpEqLess, "'<='"_rx},
    {OpLess, "'<'"_rx},
    {OpNotEq, "'!='"_rx},
    {OpEq, "'=='"_rx},

    {OpDec, "'--'"_rx},
    {OpInc, "'++'"_rx},
    {OpMod, "'%'"_rx},
    {OpDiv, "'/'"_rx},
    {OpMul, "'*'"_rx},
    {OpSub, "'-'"_rx},
    {OpAdd, "'+'"_rx},

    {OpAssign, "'='"_rx},
    {OpComma, "','"_rx},
    {OpSemicolon, "';'"_rx},
    {OpColumn, "':'"_rx},
    {OpTernary, "'?'"_rx},
    {OpArrow, "'->'"_rx},
    {OpPointer, "'*'"_rx},
    {OpAddress, "'&'"_rx},

    {None, "~_"_rx},
  };

  return std::span(map, sizeof(map));
}

using TokenMap = std::invoke_result_t<decltype(token_map)>;
  
}  // namespace ccc

#endif
