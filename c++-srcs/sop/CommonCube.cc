
/// @file CommonCube.cc
/// @brief CommonCube の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "CommonCube.h"


BEGIN_NAMESPACE_YM_SOP

//////////////////////////////////////////////////////////////////////
// クラス SopCover
//////////////////////////////////////////////////////////////////////

// @brief 共通なキューブを返す．
SopCube
SopCover::common_cube() const
{
  CommonCube common_cube{variable_num()};
  auto src = block();
  auto bv = common_cube(src);
  return common_cube.make_cube(bv);
}


//////////////////////////////////////////////////////////////////////
// クラス CommonCube
//////////////////////////////////////////////////////////////////////

// @brief カバー中のすべてのキューブの共通部分を求める．
SopBitVect*
CommonCube::operator()(
  const SopBlock& src1
)
{
  auto bv1 = src1.body;
  auto bv1_end = _calc_offset(bv1, src1.cube_num);

  auto dst_bv = new_bv();

  // 最初のキューブをコピーする．
  cube_copy(dst_bv, bv1);

  // 2番目以降のキューブとの共通部分を求める．
  bv1 += _cube_size();
  while ( bv1 != bv1_end ) {
    SopBitVect tmp = 0ULL;
    auto dst_bv1 = dst_bv;
    auto bv1_end1 = _calc_offset(bv1, 1);
    for ( ; bv1 != bv1_end1; ++ dst_bv1, ++ bv1) {
      *dst_bv1 &= *bv1;
      tmp |= *dst_bv1;
    }
    if ( tmp == 0ULL ) {
      // 空になった．
      break;
    }
  }
  return dst_bv;
}

END_NAMESPACE_YM_SOP
