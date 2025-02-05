
/// @file SopCover_compare.cc
/// @brief SopCover の比較演算の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/SopCover.h"


BEGIN_NAMESPACE_YM_SOP

//////////////////////////////////////////////////////////////////////
// クラス SopCover
//////////////////////////////////////////////////////////////////////

int
SopCover::compare(
  const SopCover& right
) const
{
  if ( variable_num() != right.variable_num() ) {
    throw std::invalid_argument("variable_num() is different from each other");
  }

  auto cube1_list = _cube_list(chunk(), 0, cube_num());
  auto cube1_iter = cube1_list.begin();
  auto cube1_end = cube1_list.end();
  auto cube2_list = _cube_list(right.chunk(), 0, right.cube_num());
  auto cube2_iter = cube2_list.begin();
  auto cube2_end = cube2_list.end();
  for ( ; cube1_iter != cube1_end &&
	  cube2_iter != cube2_end;
	++ cube1_iter, ++ cube2_iter ) {
    auto cube1 = *cube1_iter;
    auto cube2 = *cube2_iter;
    int res = _cube_compare(cube1, cube2);
    if ( res != 0 ) {
      return res;
    }
  }
  if ( cube1_iter != cube1_end ) {
    // cube2_iter == cube2_end なので cube1_list が大きい
    return 1;
  }
  if ( cube2_iter != cube2_end ) {
    // cube1_iter == cube1_end なので cube2_list が大きい
    return -1;
  }
  // 等しい
  return 0;
}

END_NAMESPACE_YM_SOP
