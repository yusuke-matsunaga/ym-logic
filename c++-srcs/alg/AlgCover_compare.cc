
/// @file AlgCover_compare.cc
/// @brief AlgCover の比較演算の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/AlgCover.h"


BEGIN_NAMESPACE_YM_ALG

//////////////////////////////////////////////////////////////////////
// クラス AlgCover
//////////////////////////////////////////////////////////////////////

int
AlgCover::_compare(
  const AlgCover& right
) const
{
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
    // cube1_list のほうが長い
    return 1;
  }
  if ( cube2_iter != cube2_end ) {
    // cube2_list の方が長い
    return -1;
  }
  // 等しい
  return 0;
}

END_NAMESPACE_YM_ALG
