﻿#ifndef YM_LOGIC_H
#define YM_LOGIC_H

/// @file ym/logic.h
/// @brief ym-logic パッケージの基本ヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2017 Yusuke Matsunaga
/// All rights reserved.

/// @defgroup LogicGroup 論理式と論理関数を扱うクラス
///
/// 論理式(ファクタードフォーム)と論理関数を扱うためのクラスライブラリ
/// C++ の機能を活かして論理演算子のオーバーロードを使っている．

#include "ym_config.h"


/// @brief ym-logic 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_LOGIC \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsLogic)

/// @brief expr 用の名前空間の終了
#define END_NAMESPACE_YM_LOGIC \
END_NAMESPACE(nsLogic) \
END_NAMESPACE_YM


BEGIN_NAMESPACE_YM_LOGIC

//////////////////////////////////////////////////////////////////////
/// @brief 組み込みタイプ
//////////////////////////////////////////////////////////////////////
enum class BuiltinType : ymuint8 {
  None, ///< 不正値
  C0,   ///< 定数0
  C1,   ///< 定数1
  Buff, ///< バッファ
  Not,  ///< NOT
  And,  ///< AND
  Nand, ///< NAND
  Or,   ///< OR
  Nor,  ///< NOR
  Xor,  ///< XOR
  Xnor  ///< XNOR
};

/// @relates BuiltinType
/// @brief ストリーム出力演算子
inline
ostream&
operator<<(
  ostream& s,      ///< [in] 出力ストリーム
  BuiltinType type ///< [in] 組み込み型
)
{
  switch ( type ) {
  case BuiltinType::None: s << "None"; break;
  case BuiltinType::C0:   s << "C0"; break;
  case BuiltinType::C1:   s << "C1"; break;
  case BuiltinType::Buff: s << "Buff"; break;
  case BuiltinType::Not:  s << "Not"; break;
  case BuiltinType::And:  s << "And"; break;
  case BuiltinType::Nand: s << "Nand"; break;
  case BuiltinType::Or:   s << "Or"; break;
  case BuiltinType::Nor:  s << "Nor"; break;
  case BuiltinType::Xor:  s << "Nor"; break;
  case BuiltinType::Xnor: s << "Xnor"; break;
  }
  return s;
}

//////////////////////////////////////////////////////////////////////
// クラスの先行宣言
//////////////////////////////////////////////////////////////////////

class Expr;
class ExprWriter;
class NpnMap;
class NpnMapM;
class NpnVmap;
class FuncAnalyzer;

END_NAMESPACE_YM_LOGIC


BEGIN_NAMESPACE_YM

class Literal;
class TvFunc;
class TvFuncM;

using nsLogic::Expr;
using nsLogic::ExprWriter;
using nsLogic::NpnMap;
using nsLogic::NpnMapM;
using nsLogic::NpnVmap;
using nsLogic::BuiltinType;
using nsLogic::FuncAnalyzer;

/// @brief 不正な変数番号
const SizeType BAD_VARID = static_cast<SizeType>(-1);

END_NAMESPACE_YM

#endif // YM_LOGIC_H
