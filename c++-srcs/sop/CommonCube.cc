
/// @file CommonCube.cc
/// @brief CommonCube の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
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
  auto bv = common_cube(to_block());
  return common_cube.make_cube(std::move(bv));
}


//////////////////////////////////////////////////////////////////////
// クラス CommonCube
//////////////////////////////////////////////////////////////////////

// @brief カバー中のすべてのキューブの共通部分を求める．
SopBitVect
CommonCube::operator()(
  const SopBlockRef& src_block
)
{
  auto& src_bv = src_block.body;
  auto dst_bv = new_cube();

  auto src_cube = _cube_begin(src_bv);
  auto dst_cube = _cube_begin(dst_bv);

  // 最初のキューブをコピーする．
  cube_copy(dst_cube, src_cube);

  // 2番目以降のキューブとの共通部分を求める．
  auto src_end = _cube_end(src_cube, src_block.cube_num);
  for ( ; src_cube != src_end; src_cube += _cube_size() ) {
    SopPatWord tmp = 0ULL;
    auto src_iter = src_cube;
    auto src_end1 = _cube_end(src_iter);
    auto dst_iter = dst_cube;
    for ( ; src_iter != src_end1;
	  ++ src_iter, ++ dst_iter) {
      *dst_iter &= *src_iter;
      tmp |= *dst_iter;
    }
    if ( tmp == 0ULL ) {
      // 空になった．
      break;
    }
  }
  return dst_bv;
}

END_NAMESPACE_YM_SOP
