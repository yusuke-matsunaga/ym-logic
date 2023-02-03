
/// @file CompOp.cc
/// @brief CompOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
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
  auto src1 = left.block();
  auto src2 = right.block();
  return compare(src1, src2);
}


//////////////////////////////////////////////////////////////////////
// クラス CompOp
//////////////////////////////////////////////////////////////////////

// @brief カバーの比較を行う．
int
CompOp::operator()(
  const SopBlock& src1,
  const SopBlock& src2
)
{
  auto bv1 = src1.body;
  auto bv1_end = _calc_offset(bv1, src1.cube_num);

  auto bv2 = src2.body;
  auto bv2_end = _calc_offset(bv2, src2.cube_num);

  for ( ; bv1 != bv1_end && bv2 != bv2_end;
	bv1 += _cube_size(), bv2 += _cube_size() ) {
    int res = cube_compare(bv1, bv2);
    if ( res != 0 ) {
      return res;
    }
  }
  if ( bv1 != bv1_end ) {
    // bv2 == bv2_end なので bv1 が大きい
    return 1;
  }
  if ( bv2 != bv2_end ) {
    // bv1 == bv1_end なので bv2 が大きい
    return -1;
  }
  // 等しい
  return 0;
}

END_NAMESPACE_YM_SOP
