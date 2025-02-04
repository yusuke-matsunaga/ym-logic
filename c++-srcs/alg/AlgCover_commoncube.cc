
/// @file AlgCover_commoncube.cc
/// @brief AlgCover の common_cube 関係の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/AlgCover.h"
#include "ym/AlgCube.h"


BEGIN_NAMESPACE_YM_ALG

//////////////////////////////////////////////////////////////////////
// クラス AlgCover
//////////////////////////////////////////////////////////////////////

// @brief 共通なキューブを返す．
AlgCube
AlgCover::common_cube() const
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

    AlgPatWord tmp = 0ULL;
    auto src_p = src_cube;
    auto src_pend = _cube_end(src_cube);
    auto dst_p = dst_cube;
    for ( ; src_p != src_pend;
	  ++ src_p, ++ dst_p) {
      *dst_p &= *src_p;
      tmp |= *dst_p;
    }
    if ( tmp == 0ULL ) {
      // 空になった．
      break;
    }
  }
  return AlgCube{variable_num(), std::move(dst_chunk)};
}

END_NAMESPACE_YM_ALG
