
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

  auto cube1 = _cube_begin(chunk());
  auto cube2 = _cube_begin(right.chunk());
  auto end1 = _cube_end(cube1, cube_num());
  auto end2 = _cube_end(cube2, right.cube_num());
  for ( ; cube1 != end1 && cube2 != end2;
	_cube_next(cube1), _cube_next(cube2) ) {
    int res = _cube_compare(cube1, cube2);
    if ( res != 0 ) {
      return res;
    }
  }
  if ( cube1 != end1 ) {
    // cube2 == end2 なので block1 が大きい
    return 1;
  }
  if ( cube2 != end2 ) {
    // cube1 == end1 なので block2 が大きい
    return -1;
  }
  // 等しい
  return 0;
}

END_NAMESPACE_YM_SOP
