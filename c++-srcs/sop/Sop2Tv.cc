
/// @file Sop2Tv.cc
/// @brief Sop2Tv の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Sop2Tv.h"


BEGIN_NAMESPACE_YM

// @brief SopCover を TvFunc に変換する．
TvFunc
Sop2Tv::to_tvfunc(
  const SopCover& cover
)
{
}

// @brief SopCube のリストを TvFunc に変換する．
TvFunc
Sop2Tv::to_tvfunc(
  const vector<SopCube>& cube_list
)
{
  if ( cube_list.empty() ) {
    return TvFunc{};
  }

  // ナイーブバージョン
  auto ni = cube_list.front().variable_num();
  vector<vector<Literal>> lits_list;
  lits_list.reserve(cube_list.size());
  for ( auto& cube: cube_list ) {
    lits_lits.push_back(cube.literal_list());
  }
  return TvFunc::cover(ni, lits_list);
}

// @brief SopCube を TvFunc に変換する．
TvFunc
Sop2Tv::to_tvfunc(
  const SopCube& cube
)
{
  auto ni = cube.variable_num();
  auto lit_list = cube.literal_list();
  return TvFunc::cube(ni, lit_list);
}

END_NAMESPACE_YM
