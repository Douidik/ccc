#ifndef CCC_TRAIT_HPP
#define CCC_TRAIT_HPP

#include "scalar_types.hpp"
#include <fmt/format.h>
#include <string_view>

// Namespace is Capitalized to simulate a C++11 scoped enumerator
// Trait should always be typed as u32 in the source code
namespace ccc::trait {

// Trait Layout
// C: Class bit
// G: Group bit
// T: N bit
// Each trait is 32 bits long {Class: 6 bits, Group: 14 bits, N: 12 bits}
// C|C|C|C|C|C|G|G|G|G|G|G|G|G|G|G|G|G|G|N|N|N|N|N|N|N|N|N|N|N|N|N

constexpr u32 TRAIT_SIZE = 32;
constexpr u32 CLASS_SIZE = 6;
constexpr u32 GROUP_SIZE = 14;
constexpr u32 N_SIZE = 12;

static_assert(CLASS_SIZE + GROUP_SIZE + N_SIZE == TRAIT_SIZE);

template<u32 N>
constexpr auto define_class() -> u32 {
  static_assert(N < CLASS_SIZE);
  return 1 << (TRAIT_SIZE - CLASS_SIZE + N);
}

template<u32 N>
constexpr auto define_group() -> u32 {
  static_assert(N < GROUP_SIZE);
  return 1 << (TRAIT_SIZE - CLASS_SIZE - GROUP_SIZE + N);
}

template<u32 C, u32 G, u32 N>
constexpr auto define() -> u32 {
  static_assert(N < N_SIZE);
  return 1 << (TRAIT_SIZE - CLASS_SIZE - GROUP_SIZE - N_SIZE + N) | C | G;
}

enum TraitEnum : u32 {
  CsNone = 0,
  CsMeta = define_class<0>(),
  CsDirective = define_class<1>(),
  CsKeyword = define_class<2>(),
  CsOperator = define_class<3>(),
  CsBracket = define_class<4>(),
  CsData = define_class<5>(),

  GpNone = 0,
  GpNpc = define_group<0>(),
  GpComment = define_group<1>(),
  GpDefine = define_group<2>(),
  GpFlow = define_group<3>(),
  GpType = define_group<4>(),
  GpModifier = define_group<5>(),
  GpArithmetic = define_group<6>(),
  GpLogic = define_group<7>(),
  GpCompare = define_group<8>(),
  GpAccess = define_group<9>(),
  GpBin = define_group<10>(),
  GpBinary = define_group<11>(),
  GpImmediate = define_group<12>(),

  None = define<CsMeta, GpNpc, 0>(),
  Blank = define<CsMeta, GpNpc, 1>(),
  End = define<CsMeta, GpNpc, 2>(),
  CommentSL = define<CsMeta, GpComment, 0>(),
  CommentML = define<CsMeta, GpComment, 1>(),

  HsDefine = define<CsDirective, GpDefine, 0>(),
  HsUndef = define<CsDirective, GpDefine, 1>(),
  HsIf = define<CsDirective, GpFlow, 0>(),
  HsElse = define<CsDirective, GpFlow, 1>(),
  HsEndif = define<CsDirective, GpFlow, 2>(),
  HsIfdef = define<CsDirective, GpFlow, 3>(),
  HsIfndef = define<CsDirective, GpFlow, 4>(),

  Sizeof = define<CsKeyword | CsOperator, GpAccess, 0>(),
  Star = define<CsOperator | CsKeyword, GpAccess | GpModifier, 0>(),
  Ampersand = define<CsOperator, GpAccess | GpBin | GpBinary, 0>(),

  KwAuto = define<CsKeyword, GpType, 0>(),
  KwDouble = define<CsKeyword, GpType, 1>(),
  KwFloat = define<CsKeyword, GpType, 2>(),
  KwInt = define<CsKeyword, GpType, 3>(),
  KwLong = define<CsKeyword, GpType, 4>(),
  KwShort = define<CsKeyword, GpType, 5>(),
  KwVoid = define<CsKeyword, GpType, 6>(),
  KwEnum = define<CsKeyword, GpDefine, 0>(),
  KwTypedef = define<CsKeyword, GpDefine, 1>(),
  KwUnion = define<CsKeyword, GpDefine, 2>(),
  KwStruct = define<CsKeyword, GpDefine, 3>(),
  KwVolatile = define<CsKeyword, GpModifier, 0>(),
  KwConst = define<CsKeyword, GpModifier, 1>(),
  KwExtern = define<CsKeyword, GpModifier, 2>(),
  KwRegister = define<CsKeyword, GpModifier, 3>(),
  KwStatic = define<CsKeyword, GpModifier, 4>(),
  KwSigned = define<CsKeyword, GpModifier, 5>(),
  KwUnsigned = define<CsKeyword, GpModifier, 6>(),
  KwPointer = Star,
  KwBreak = define<CsKeyword, GpFlow, 0>(),
  KwCase = define<CsKeyword, GpFlow, 1>(),
  KwContinue = define<CsKeyword, GpFlow, 2>(),
  KwDefault = define<CsKeyword, GpFlow, 3>(),
  KwDo = define<CsKeyword, GpFlow, 4>(),
  KwElse = define<CsKeyword, GpFlow, 5>(),
  KwFor = define<CsKeyword, GpFlow, 6>(),
  KwGoto = define<CsKeyword, GpFlow, 7>(),
  KwIf = define<CsKeyword, GpFlow, 8>(),
  KwReturn = define<CsKeyword, GpFlow, 9>(),
  KwSwitch = define<CsKeyword, GpFlow, 10>(),
  KwWhile = define<CsKeyword, GpFlow, 11>(),

  CurlyBegin = define<CsBracket, GpNone, 0>(),
  CurlyClose = define<CsBracket, GpNone, 1>(),
  ParenBegin = define<CsBracket, GpNone, 2>(),
  ParenClose = define<CsBracket, GpNone, 3>(),
  CrochetBegin = define<CsBracket, GpNone, 4>(),
  CrochetClose = define<CsBracket, GpNone, 5>(),

  Identifier = define<CsData, GpNone, 0>(),
  Float = define<CsData, GpImmediate, 0>(),
  Integer = define<CsData, GpImmediate, 1>(),
  String = define<CsData, GpImmediate, 2>(),
  Char = define<CsData, GpImmediate, 3>(),

  Increment = define<CsOperator, GpNone, 1>(),
  Decrement = define<CsOperator, GpNone, 2>(),
  Assign = define<CsOperator, GpBinary, 0>(),
  Not = define<CsOperator, GpLogic, 0>(),
  And = define<CsOperator, GpLogic | GpBinary, 0>(),
  Or = define<CsOperator, GpLogic | GpBinary, 1>(),
  Add = define<CsOperator, GpArithmetic | GpBinary, 0>(),
  Sub = define<CsOperator, GpArithmetic | GpBinary, 1>(),
  Mul = define<CsOperator, GpArithmetic | GpBinary, 2>(),
  Div = define<CsOperator, GpArithmetic | GpBinary, 3>(),
  Mod = define<CsOperator, GpArithmetic | GpBinary, 4>(),
  BinNot = define<CsOperator, GpBin, 0>(),
  BinAnd = Ampersand,
  BinOr = define<CsOperator, GpBin | GpBinary, 1>(),
  BinXor = define<CsOperator, GpBin | GpBinary, 2>(),
  BinShiftL = define<CsOperator, GpBin | GpBinary, 3>(),
  BinShiftR = define<CsOperator, GpBin | GpBinary, 4>(),
  Equal = define<CsOperator, GpCompare | GpBinary, 0>(),
  NotEq = define<CsOperator, GpCompare | GpBinary, 1>(),
  Less = define<CsOperator, GpCompare | GpBinary, 2>(),
  Greater = define<CsOperator, GpCompare | GpBinary, 3>(),
  LessEq = define<CsOperator, GpCompare | GpBinary, 4>(),
  GreaterEq = define<CsOperator, GpCompare | GpBinary, 5>(),
  Deref = Star,
  Address = Ampersand,
  Dot = define<CsOperator, GpAccess, 1>(),
  Arrow = define<CsOperator, GpAccess, 2>(),
  Semicolon = define<CsOperator, None, 0>(),
  Comma = define<CsOperator, None, 1>(),
};

}  // namespace ccc::trait

namespace ccc {
using namespace trait;

constexpr auto trait_name(u32 trait) -> std::string_view {
  switch (trait) {
  case None: return "None";
  case Blank: return "Blank";
  case End: return "End";
  case CommentSL: return "CommentSL";
  case CommentML: return "CommentML";
  case HsDefine: return "HsDefine";
  case HsUndef: return "HsUndef";
  case HsIf: return "HsIf";
  case HsElse: return "HsElse";
  case HsEndif: return "HsEndif";
  case HsIfdef: return "HsIfdef";
  case HsIfndef: return "HsIfndef";
  case Sizeof: return "Sizeof";
  case Star: return "Star";
  case Ampersand: return "Ampersand";
  case KwAuto: return "KwAuto";
  case KwDouble: return "KwDouble";
  case KwFloat: return "KwFloat";
  case KwInt: return "KwInt";
  case KwLong: return "KwLong";
  case KwShort: return "KwShort";
  case KwVoid: return "KwVoid";
  case KwEnum: return "KwEnum";
  case KwTypedef: return "KwTypedef";
  case KwUnion: return "KwUnion";
  case KwStruct: return "KwStruct";
  case KwVolatile: return "KwVolatile";
  case KwConst: return "KwConst";
  case KwExtern: return "KwExtern";
  case KwRegister: return "KwRegister";
  case KwStatic: return "KwStatic";
  case KwSigned: return "KwSigned";
  case KwUnsigned: return "KwUnsigned";
  case KwBreak: return "KwBreak";
  case KwCase: return "KwCase";
  case KwContinue: return "KwContinue";
  case KwDefault: return "KwDefault";
  case KwDo: return "KwDo";
  case KwElse: return "KwElse";
  case KwFor: return "KwFor";
  case KwGoto: return "KwGoto";
  case KwIf: return "KwIf";
  case KwReturn: return "KwReturn";
  case KwSwitch: return "KwSwitch";
  case KwWhile: return "KwWhile";
  case CurlyBegin: return "CurlyBegin";
  case CurlyClose: return "CurlyClose";
  case ParenBegin: return "ParenBegin";
  case ParenClose: return "ParenClose";
  case CrochetBegin: return "CrochetBegin";
  case CrochetClose: return "CrochetClose";
  case Identifier: return "Identifier";
  case Float: return "Float";
  case Integer: return "Integer";
  case String: return "String";
  case Char: return "Char";
  case Increment: return "Increment";
  case Decrement: return "Decrement";
  case Assign: return "Assign";
  case Not: return "Not";
  case And: return "And";
  case Or: return "Or";
  case Add: return "Add";
  case Sub: return "Sub";
  case Mul: return "Mul";
  case Div: return "Div";
  case Mod: return "Mod";
  case BinNot: return "BinNot";
  case BinOr: return "BinOr";
  case BinXor: return "BinXor";
  case BinShiftL: return "BinShiftL";
  case BinShiftR: return "BinShiftR";
  case Equal: return "Equal";
  case NotEq: return "NotEq";
  case Less: return "Less";
  case Greater: return "Greater";
  case LessEq: return "LessEq";
  case GreaterEq: return "GreaterEq";
  case Dot: return "Dot";
  case Arrow: return "Arrow";
  case Semicolon: return "Semicolon";
  case Comma: return "Comma";
  }

  return "";
}

}  // namespace ccc

#endif
