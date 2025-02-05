
/// @file WeakDivision.cc
/// @brief WeakDivision の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "WeakDivision.h"


BEGIN_NAMESPACE_YM_ALG

//////////////////////////////////////////////////////////////////////
// クラス WeakDivision
//////////////////////////////////////////////////////////////////////

// @brief 除算を行う．
pair<AlgCover, AlgCover>
WeakDivision::divide(
  const AlgCover& f,
  const AlgCover& d
)
{
  auto q = f / d;
  auto r = f - (q * d);
  return make_pair(std::move(q), std::move(r));
}

END_NAMESPACE_YM_ALG
