#ifndef CCC_CATEGORY_HPP
#define CCC_CATEGORY_HPP

#include "scalar_types.hpp"

namespace ccc {

enum class Category : u8 {
  None = 0,
  Blank,
  End,

  PpDefine,
  PpUndef,
  PpIf,
  PpElse,
  PpEndif,
  PpIfdef,
  PpIfndef,

  KwAuto,
  KwBreak,
  KwCase,
  KwConst,
  KwContinue,
  KwDefault,
  KwDo,
  KwDouble,
  KwElse,
  KwEnum,
  KwExtern,
  KwFloat,
  KwFor,
  KwGoto,
  KwIf,
  KwInt,
  KwLong,
  KwRegister,
  KwReturn,
  KwShort,
  KwSigned,
  KwSizeof,
  KwStatic,
  KwStruct,
  KwSwitch,
  KwTypedef,
  KwUnion,
  KwUnsigned,
  KwVoid,
  KwVolatile,
  KwWhile,

  OpAddress,
  OpPointer,
  OpArrow,
  OpTernary,
  OpColumn,
  OpSemicolon,
  OpComma,
  OpAssign,

  OpAdd,
  OpSub,
  OpMul,
  OpDiv,
  OpMod,
  OpInc,
  OpDec,

  OpEq,
  OpNotEq,
  OpLess,
  OpEqLess,
  OpGreater,
  OpEqGreater,

  OpAnd,
  OpOr,
  OpNot,

  BinAnd,
  BinOr,
  BinXor,
  BinNot,
  BinShiftL,
  BinShiftR,

  ScopeBegin,
  ScopeClose,
  BraceBegin,
  BraceClose,
  BracketBegin,
  BracketClose,

  CommentSL,
  CommentML,
  Identifier,
  Decimal,
  Integer,
  String,
  Char,
};

}

#endif
