
/// @file AlgBase_sort.cc
/// @brief AlgBase のソート関係の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "AlgBase.h"
#include "AlgBlock.h"


BEGIN_NAMESPACE_YM_ALG

//////////////////////////////////////////////////////////////////////
// クラス AlgBase
//////////////////////////////////////////////////////////////////////

// @brief マージソートを行う下請け関数
void
AlgBase::_sort_sub(
  AlgBitVect& bv,
  SizeType start,
  SizeType end
)
{
  SizeType n = end - start;
  if ( n <= 1 ) {
    return;
  }
  if ( n == 2 ) {
    // (0, 1) と (1, 0) の2通りだけ
    auto cube0 = _cube_begin(bv, start + 0);
    auto cube1 = _cube_begin(bv, start + 1);
    if ( _cube_compare(cube0, cube1) < 0 ) {
      // (1, 0) だったので交換する．
      _resize_buff(1);
      _cube_swap(cube0, cube1);
    }
    return;
  }
  if ( n == 3 ) {
    // (0, 1, 2), (0, 2, 1), (1, 0, 2), (1, 2, 0), (2, 0, 1), (2, 1, 0)
    // の6通りなので虱潰し
    auto cube0 = _cube_begin(bv, start + 0);
    auto cube1 = _cube_begin(bv, start + 1);
    auto cube2 = _cube_begin(bv, start + 2);
    if ( _cube_compare(cube0, cube1) < 0 ) {
      // (1, 0, 2), (1, 2, 0), (2, 1, 0)
      if ( _cube_compare(cube0, cube2) < 0 ) {
	// (1, 2, 0), (2, 1, 0)
	if ( _cube_compare(cube1, cube2) < 0 ) {
	  // (2, 1, 0)
	  // 0 と 2 を交換
	  _resize_buff(1);
	  _cube_swap(cube0, cube2);
	}
	else {
	  // (1, 2, 0)
	  // 0 <- 1, 1 <- 2, 2 <- 0
	  _resize_buff(1);
	  _cube_rotate3(cube0, cube1, cube2);
	}
      }
      else {
	// (1, 0, 2)
	// 0 <-> 1
	_resize_buff(1);
	_cube_swap(cube0, cube1);
      }
    }
    else {
      // (0, 1, 2), (0, 2, 1), (2, 0, 1)
      if ( _cube_compare(cube0, cube2) < 0 ) {
	// (2, 0, 1)
	// 0 <- 2, 2 <- 1, 1 <- 0
	_resize_buff(1);
	_cube_rotate3(cube0, cube2, cube1);
      }
      else {
	// (0, 1, 2), (0, 2, 1)
	if ( _cube_compare(cube1, cube2) < 0 ) {
	  // (0, 2, 1)
	  // 1 <-> 2
	  _resize_buff(1);
	  _cube_swap(cube1, cube2);
	}
	else {
	  // (0, 1, 2)
	  // そのまま
	}
      }
    }
    return;
  }
  if ( n == 4 ) {
    auto cube0 = _cube_begin(bv, start + 0);
    auto cube1 = _cube_begin(bv, start + 1);
    auto cube2 = _cube_begin(bv, start + 2);
    auto cube3 = _cube_begin(bv, start + 3);
    _resize_buff(1);
    // 0 と 1 を整列
    if ( _cube_compare(cube0, cube1) < 0 ) {
      _cube_swap(cube0, cube1);
    }
    // 2 と 3 を整列
    if ( _cube_compare(cube2, cube3) < 0 ) {
      _cube_swap(cube2, cube3);
    }
    // 0 と 2 を比較
    if ( _cube_compare(cube0, cube2) < 0 ) {
      if ( cube_compare(cube0, cube3) < 0 ) {
	// (0, 1) と (2, 3) を交換
	_cube_swap(cube0, cube2);
	_cube_swap(cube1, cube3);
      }
      else if ( cube_compare(cube1, cube3) < 0 ) {
	// 0 <- 2, 2 <- 3, 3 <- 1, 1 <- 0
	_cube_rotate4(cube0, cube2, cube3, cube1);
      }
      else {
	// 0 <- 2, 2 <- 1, 1 <- 0
	_cube_rotate3(cube0, cube2, cube1);
      }
    }
    else if ( _cube_compare(cube1, cube2) < 0 ) {
      if ( _cube_compare(cube1, cube3) < 0 ) {
	// 1 <- 2, 2 <- 3, 3 <- 1
	_cube_rotate3(cube1, cube2, cube3);
      }
      else {
	// 1 <- 2, 2 <- 1
	_cube_swap(cube1, cube2);
      }
    }
    else {
      // そのまま
    }
    return;
  }

  // 半分に分割してそれぞれソートする．
  SizeType hn = (n + 1) / 2;
  SizeType start1 = start;
  SizeType end1 = start + hn;
  SizeType start2 = end1;
  SizeType end2 = end;
  _sort_sub(bv, start1, end1);
  _sort_sub(bv, start2, end2);

  // trivial case
  // 前半部分の末尾が後半部分の先頭より大きければ
  // すでに整列している．
  auto cube_end1 = _cube_begin(bv, end1 - 1);
  auto cube_start2 = _cube_begin(bv, start2);
  if ( _cube_compare(cube_end1, cube_start2) > 0 ) {
    return;
  }

  // マージする．
  // 前半部分を一旦 mTmpBuff にコピーする．
  _resize_buff(hn);
  {
    auto dst_cube = _cube_begin(mTmpBlock.body);
    auto src_cube = _cube_begin(bv, start1);
    _copy(dst_cube, src_cube, hn);
  }
  auto src1_iter = _cube_begin(mTmpBlock.body);
  auto src1_end = src1_iter + hn;
  auto src2_iter = _cube_begin(bv, start2);
  auto src2_end = _cube_begin(bv, end2);
  auto dst_iter = _cube_begin(bv);
  while ( src1_iter != src1_end && src2_iter != src2_end ) {
    int comp_res = _cube_compare(src1_iter, src2_iter);
    if ( comp_res > 0 ) {
      _cube_copy(dst_iter, src1_iter);
      ++ src1_iter;
      ++ dst_iter;
    }
    else if ( comp_res < 0 ) {
      _cube_copy(dst_iter, src2_iter);
      ++ src2_iter;
      ++ dst_iter;
    }
    else {
      // 重複したキューブはエラー
      ASSERT_NOT_REACHED;
    }
  }
  while ( src1_iter != src1_end ) {
    _cube_copy(dst_iter, src1_iter);
    ++ src1_iter;
    ++ dst_iter;
  }
  // 後半部分が残っている時はそのままでいいはず．
  ASSERT_COND( src2_iter == dst_iter );
}

END_NAMESPACE_YM_ALG
