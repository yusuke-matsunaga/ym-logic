
/// @file SopCover_commoncube.cc
/// @brief SopCover の common_cube 関係の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/SopCover.h"
#include "ym/SopCube.h"


BEGIN_NAMESPACE_YM_SOP

//////////////////////////////////////////////////////////////////////
// クラス SopCover
//////////////////////////////////////////////////////////////////////

// @brief 共通なキューブを返す．
SopCube
SopCover::common_cube() const
{
  auto dst_chunk = _new_chunk(1);
  auto dst_cube = dst_chunk.begin();

  auto src_list = _cube_list(chunk(), 0, cube_num());
  auto src_iter = src_list.begin();

  // 最初のキューブをコピーする．
  auto src_cube = *src_iter;
  _cube_copy(dst_cube, src_cube);

  // 2番目以降のキューブとの共通部分を求める．
  auto src_end = src_list.end();
  ++ src_iter;
  for ( ; src_iter != src_end; ++ src_iter ) {
    auto src_cube = *src_iter;

    SopPatWord tmp = SOP_ALL1;
    auto src_p = src_cube;
    auto src_pend = _cube_end(src_cube);
    auto dst_p = dst_cube;
    for ( ; src_p != src_pend;
	  ++ src_p, ++ dst_p) {
      *dst_p |= *src_p;
      tmp &= *dst_p;
    }
    if ( tmp == SOP_ALL1 ) {
      // 空になった．
      return SopCube::invalid(variable_num());
    }
  }
  return SopCube{variable_num(), std::move(dst_chunk)};
}

END_NAMESPACE_YM_SOP
