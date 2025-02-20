#ifndef YM_LOGIC_H
#define YM_LOGIC_H

/// @file ym/logic.h
/// @brief ym-logic パッケージの基本ヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

/// @defgroup LogicGroup 論理式と論理関数を扱うクラス
///
/// 論理式(ファクタードフォーム)と論理関数を扱うためのクラスライブラリ
/// C++ の機能を活かして論理演算子のオーバーロードを使っている．
///
/// 主なクラスは以下の通り
/// - Literal: 変数リテラル(変数番号＋極性)
/// - Expr: 通常の論理式
/// - TvFunc: 真理値表を用いた論理関数表現
/// - Bdd: Binary Decision Diagram
/// - Zdd: Zero-surpressed Decision Diagram
/// - SopCover, SopCube: 積和形論理式とキューブ
/// - AlgCover, AlgCube: 代数的表現にもとづく積和形論理式とキューブ
///
/// SopCover/SopCube と AlgCover/AlgCube は似ているが，内部のビットパタンの
/// エンコーディングが異なる．
/// 具体的には SopCover/SopCube ではドントケアは '11' だが，
/// AlgCover/AlgCube ではドントケアは '00' となる．

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
enum class PrimType : std::uint8_t {
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

/// @relates PrimType
/// @brief ストリーム出力演算子
inline
ostream&
operator<<(
  ostream& s,      ///< [in] 出力ストリーム
  PrimType type ///< [in] 組み込み型
)
{
  switch ( type ) {
  case PrimType::None: s << "None"; break;
  case PrimType::C0:   s << "C0"; break;
  case PrimType::C1:   s << "C1"; break;
  case PrimType::Buff: s << "Buff"; break;
  case PrimType::Not:  s << "Not"; break;
  case PrimType::And:  s << "And"; break;
  case PrimType::Nand: s << "Nand"; break;
  case PrimType::Or:   s << "Or"; break;
  case PrimType::Nor:  s << "Nor"; break;
  case PrimType::Xor:  s << "Xor"; break;
  case PrimType::Xnor: s << "Xnor"; break;
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

/// @brief DD 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_DD \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsDd)

/// @brief DD 用の名前空間の終了
#define END_NAMESPACE_YM_DD \
END_NAMESPACE(nsDd) \
END_NAMESPACE_YM

BEGIN_NAMESPACE_YM_DD

//////////////////////////////////////////////////////////////////////
// クラスの先行宣言
//////////////////////////////////////////////////////////////////////

class Bdd;
class BddVar;
class BddVarSet;
class BddLit;
class BddMgr;

class Zdd;
class ZddItem;
class ZddMgr;

END_NAMESPACE_YM_DD

/// @brief SOP 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_SOP \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsSop)

/// @brief SOP 用の名前空間の終了
#define END_NAMESPACE_YM_SOP \
END_NAMESPACE(nsSop) \
END_NAMESPACE_YM

BEGIN_NAMESPACE_YM_SOP

// クラス名の前方参照用宣言
class SopCube;
class SopCover;

END_NAMESPACE_YM_SOP

/// @brief RectCover 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_RC \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsRectCover)

/// @brief SOP 用の名前空間の終了
#define END_NAMESPACE_YM_RC \
END_NAMESPACE(nsRectCover) \
END_NAMESPACE_YM

BEGIN_NAMESPACE_YM

class Literal;
class TvFunc;
class TvFuncM;

using nsLogic::Expr;
using nsLogic::ExprWriter;
using nsLogic::NpnMap;
using nsLogic::NpnMapM;
using nsLogic::NpnVmap;
using nsLogic::PrimType;
using nsLogic::FuncAnalyzer;

using nsDd::Bdd;
using nsDd::BddVar;
using nsDd::BddVarSet;
using nsDd::BddLit;
using nsDd::BddMgr;

using nsDd::Zdd;
using nsDd::ZddItem;
using nsDd::ZddMgr;

/// @brief 不正な変数番号
const SizeType BAD_VARID = static_cast<SizeType>(-1);

/// @brief SopCover/SopCube 中のパタンを表す列挙型
enum class SopPat : std::uint8_t {
  __ = 0, ///< 未使用
  _1 = 1, ///< 正極性
  _0 = 2, ///< 負極性
  _X = 3  ///< なし
};

/// @relates SopPat
/// @brief SopPat のストリーム出力
/// @return s を返す．
inline
ostream&
operator<<(
  ostream& s, ///< [in] ストリーム
  SopPat pat  ///< [in] パタン
)
{
  switch ( pat ) {
  case SopPat::__: s << '_'; break;
  case SopPat::_1: s << '1'; break;
  case SopPat::_0: s << '0'; break;
  case SopPat::_X: s << 'X'; break;
  }
  return s;
}

using nsSop::SopCube;
using nsSop::SopCover;

END_NAMESPACE_YM

#endif // YM_LOGIC_H
