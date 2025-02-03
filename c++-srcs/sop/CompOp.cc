
/// @file CompOp.cc
/// @brief CompOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "CompOp.h"


BEGIN_NAMESPACE_YM_SOP

//////////////////////////////////////////////////////////////////////
// クラス SopCover
//////////////////////////////////////////////////////////////////////

// @relates SopCover
int
compare(
  const SopCover& left,
  const SopCover& right
)
{
  if ( left.variable_num() != right.variable_num() ) {
    throw std::invalid_argument("variable_num() is different from each other");
  }

  CompOp compare{left.variable_num()};
  return compare(left.to_block(), right.to_block());
}


//////////////////////////////////////////////////////////////////////
// クラス CompOp
//////////////////////////////////////////////////////////////////////

// @brief カバーの比較を行う．
int
CompOp::operator()(
  const SopBlockRef& block1,
  const SopBlockRef& block2
)
{
  auto& src1_bv = block1.body;
  auto& src2_bv = block2.body;
  auto src1_iter = _cube_begin(src1_bv);
  auto src2_iter = _cube_begin(src2_bv);
  auto src1_end = _cube_end(src1_iter, block1.cube_num);
  auto src2_end = _cube_end(src2_iter, block2.cube_num);
  for ( ; src1_iter != src1_end && src2_iter != src2_end;
	src1_iter += _cube_size(), src2_iter += _cube_size() ) {
    int res = cube_compare(src1_iter, src2_iter);
    if ( res != 0 ) {
      return res;
    }
  }
  if ( src1_iter < src1_end ) {
    // src2_iter == src2_end なので block1 が大きい
    return 1;
  }
  if ( src2_iter < src2_end ) {
    // src1_iter == src1_end なので block2 が大きい
    return -1;
  }
  // 等しい
  return 0;
}

END_NAMESPACE_YM_SOP
