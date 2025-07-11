
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

inline
SizeType
calc_lit(
  const SopCover& q,
  const SopCover& d,
  const SopCover& r
)
{
  return q.literal_num() + d.literal_num() + r.literal_num();
}

// @brief 除算を行う．
std::pair<SopCover, SopCover>
BoolDivision::divide(
  const SopCover& f,
  const SopCover& d
)
{
  auto ni = f.variable_num();
  auto f_func = f.tvfunc();
  auto d_func = d.tvfunc();
  // ~d をドントケアにして q を求める．
  auto q_list = Tv2Sop::isop(f_func, ~d_func);
  auto q_func = SopCube::tvfunc(ni, q_list);
  // d & q をドントケアにして r を求める．
  auto r_list = Tv2Sop::isop(f_func, d_func & q_func);
  auto q = SopCover(ni, q_list);
  auto r = SopCover(ni, r_list);
  // 安全策で WeakDivision の結果も求めておく．
  {
    auto p = WeakDivision::divide(f, d);
    auto& q1 = p.first;
    auto& r1 = p.second;
    // リテラル数の少ない方を答とする．
    if ( q1.cube_num() > 0 && calc_lit(q, d, r) > calc_lit(q1, d, r1) ) {
      return std::make_pair(std::move(q1), std::move(r1));
    }
  }
  return std::make_pair(std::move(q), std::move(r));
}

END_NAMESPACE_YM_SOP
