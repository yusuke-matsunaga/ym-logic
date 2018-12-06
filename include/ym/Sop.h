#ifndef SOP_H
#define SOP_H

/// @file Sop.h
/// @brief Sop のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.


#include "ym/logic.h"


BEGIN_NAMESPACE_YM_LOGIC

/// @brief SopPat をパックしたビットベクタ型
using SopBitVect = ymuint64;

// クラス名の前方参照用宣言
class SopBlock;
class SopMgr;

END_NAMESPACE_YM_LOGIC

BEGIN_NAMESPACE_YM

/// @brief パタンを表す列挙型
enum class SopPat {
  _X = 0, //@< なし
  _1 = 1, //@< 正極性
  _0 = 2  //@< 負極性
};

/// @relates SopPat
/// @brief SopPat のストリーム出力
/// @param[in] s ストリーム
/// @param[in] pat パタン
inline
ostream&
operator<<(ostream& s,
	   SopPat pat)
{
  switch ( pat ) {
  case SopPat::_0: s << '0'; break;
  case SopPat::_1: s << '1'; break;
  case SopPat::_X: s << 'X'; break;
  default: ASSERT_NOT_REACHED;
  }
  return s;
}

END_NAMESPACE_YM

#endif // SOP_H
