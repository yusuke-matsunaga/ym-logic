#ifndef SOP_NSDEF_H
#define SOP_NSDEF_H

/// @file sop_nsdef.h
/// @brief ym-sop の名前空間の定義
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"


/// @brief ym-sop 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_SOP \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsSop)

/// @brief ym-sop 用の名前空間の終了
#define END_NAMESPACE_YM_SOP \
END_NAMESPACE(nsSop) \
END_NAMESPACE_YM


BEGIN_NAMESPACE_YM_SOP

/// @brief SopPat をパックしたビットベクタ型
using SopBitVect = ymuint64;

// クラス名の前方参照用宣言
class SopBlock;
class SopMgr;
class SopCube;
class SopCover;

END_NAMESPACE_YM_SOP

BEGIN_NAMESPACE_YM

/// @brief パタンを表す列挙型
enum class SopPat {
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
  default: ASSERT_NOT_REACHED;
  }
  return s;
}

using nsSop::SopCube;
using nsSop::SopCover;

END_NAMESPACE_YM

#endif // SOP_NSDEF_H
