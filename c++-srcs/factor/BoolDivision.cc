
/// @file BoolDivision.cc
/// @brief BoolDivision の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "BoolDivision.h"
#include "WeakDivision.h"
#include "ym/TvFunc.h"
#include "ym/Tv2Sop.h"


BEGIN_NAMESPACE_YM_SOP

//////////////////////////////////////////////////////////////////////
// クラス BoolDivision
//////////////////////////////////////////////////////////////////////

// @brief 除算を行う．
pair<SopCover, SopCover>
BoolDivision::divide(
  const SopCover& f,
  const SopCover& d
)
{
  SizeType ni = f.variable_num();
  auto f_func = f.tvfunc();
  auto d_func = d.tvfunc();
  auto dc = ~d_func;
  // ~d をドントケアにして q を求める．
  auto q_list = Tv2Sop::isop(f_func, dc);
  auto q_func = SopCube::tvfunc(ni, q_list);
  // 元のカバーから q & d でカバーされた部分をドントケアにして
  // r のカバーを求める．
  auto r_list = Tv2Sop::isop(f_func, d_func & q_func);
  auto q = SopCover{ni, q_list};
  auto r = SopCover{ni, r_list};
  // 安全策で WeakDivision の結果も求めておく．
  auto p = WeakDivision::divide(f, d);
  auto& q1 = p.first;
  auto& r1 = p.second;
  if ( q1.cube_num() < q.cube_num() ) {
    return make_pair(std::move(q1), std::move(r1));
  }
  else {
    return make_pair(std::move(q), std::move(r));
  }
}

END_NAMESPACE_YM_SOP
