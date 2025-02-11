
/// @file WeakDivision.cc
/// @brief WeakDivision の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "WeakDivision.h"


BEGIN_NAMESPACE_YM_SOP

//////////////////////////////////////////////////////////////////////
// クラス WeakDivision
//////////////////////////////////////////////////////////////////////

// @brief 除算を行う．
pair<SopCover, SopCover>
WeakDivision::divide(
  const SopCover& f,
  const SopCover& d
)
{
  auto q = f.algdiv(d);
  auto r = f - (q & d);
  return make_pair(std::move(q), std::move(r));
}

END_NAMESPACE_YM_SOP
