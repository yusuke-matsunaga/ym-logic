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
class ZddMgr;

END_NAMESPACE_YM_DD

/// @brief SOP 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_SOP \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsSop)

/// @brief ym-sop 用の名前空間の終了
#define END_NAMESPACE_YM_SOP \
END_NAMESPACE(nsSop) \
END_NAMESPACE_YM


BEGIN_NAMESPACE_YM_SOP

/// @brief SopPat をパックしたビットベクタ型
using SopBitVect = std::uint64_t;

// クラス名の前方参照用宣言
class SopBlock;
class SopMgr;
class SopCube;
class SopCover;

END_NAMESPACE_YM_SOP


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
using nsDd::ZddMgr;

/// @brief 不正な変数番号
const SizeType BAD_VARID = static_cast<SizeType>(-1);

/// @brief パタンを表す列挙型
enum class SopPat : std::uint8_t {
  _X = 0, ///< なし
  _1 = 1, ///< 正極性
  _0 = 2  ///< 負極性
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
  case SopPat::_0: s << '0'; break;
  case SopPat::_1: s << '1'; break;
  case SopPat::_X: s << 'X'; break;
  }
  return s;
}

using nsSop::SopCube;
using nsSop::SopCover;

END_NAMESPACE_YM

#endif // YM_LOGIC_H
