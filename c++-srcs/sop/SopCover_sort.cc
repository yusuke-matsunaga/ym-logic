
/// @file SopCover_sort.cc
/// @brief SopCover のソート関係の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/SopCover.h"
#include "SopSorter.h"


BEGIN_NAMESPACE_YM_SOP

//////////////////////////////////////////////////////////////////////
// クラス SopCover
//////////////////////////////////////////////////////////////////////

// @brief ソートする．
void
SopCover::_sort()
{
  SopSorter sorter{variable_num()};
  sorter.sort(cube_num(), chunk());
}


//////////////////////////////////////////////////////////////////////
// クラス SopSorter
//////////////////////////////////////////////////////////////////////

// @brief マージソートを行う下請け関数
void
SopSorter::sort(
  SizeType cube_num,
  Chunk& chunk
)
{
  // 必要なサイズはもとのカバーの半分
  SizeType size = (cube_num + 1) / 2;
  mTmpChunk = _new_chunk(size);

  sort_sub(chunk, 0, cube_num);
}

void
SopSorter::sort_sub(
  Chunk& chunk,
  SizeType begin,
  SizeType end
)
{
  SizeType n = end - begin;
  if ( n <= 1 ) {
    return;
  }
  if ( n == 2 ) {
    // (0, 1) と (1, 0) の2通りだけ
    auto cube0 = _cube_begin(chunk, begin + 0);
    auto cube1 = _cube_begin(chunk, begin + 1);
    if ( _cube_compare(cube0, cube1) < 0 ) {
      // (1, 0) だったので交換する．
      _cube_swap(cube0, cube1);
    }
    return;
  }
  if ( n == 3 ) {
    // (0, 1, 2), (0, 2, 1), (1, 0, 2), (1, 2, 0), (2, 0, 1), (2, 1, 0)
    // の6通りなので虱潰し
    auto cube0 = _cube_begin(chunk, begin + 0);
    auto cube1 = _cube_begin(chunk, begin + 1);
    auto cube2 = _cube_begin(chunk, begin + 2);
    if ( _cube_compare(cube0, cube1) < 0 ) {
      // (1, 0, 2), (1, 2, 0), (2, 1, 0)
      if ( _cube_compare(cube0, cube2) < 0 ) {
	// (1, 2, 0), (2, 1, 0)
	if ( _cube_compare(cube1, cube2) < 0 ) {
	  // (2, 1, 0)
	  // 0 と 2 を交換
	  _cube_swap(cube0, cube2);
	}
	else {
	  // (1, 2, 0)
	  // 0 <- 1, 1 <- 2, 2 <- 0
	  _cube_rotate3(cube0, cube1, cube2);
	}
      }
      else {
	// (1, 0, 2)
	// 0 <-> 1
	_cube_swap(cube0, cube1);
      }
    }
    else {
      // (0, 1, 2), (0, 2, 1), (2, 0, 1)
      if ( _cube_compare(cube0, cube2) < 0 ) {
	// (2, 0, 1)
	// 0 <- 2, 2 <- 1, 1 <- 0
	_cube_rotate3(cube0, cube2, cube1);
      }
      else {
	// (0, 1, 2), (0, 2, 1)
	if ( _cube_compare(cube1, cube2) < 0 ) {
	  // (0, 2, 1)
	  // 1 <-> 2
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
    auto cube0 = _cube_begin(chunk, begin + 0);
    auto cube1 = _cube_begin(chunk, begin + 1);
    auto cube2 = _cube_begin(chunk, begin + 2);
    auto cube3 = _cube_begin(chunk, begin + 3);
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
      if ( _cube_compare(cube0, cube3) < 0 ) {
	// (0, 1) と (2, 3) を交換
	_cube_swap(cube0, cube2);
	_cube_swap(cube1, cube3);
      }
      else if ( _cube_compare(cube1, cube3) < 0 ) {
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
  SizeType begin1 = begin;
  SizeType end1 = begin + hn;
  SizeType begin2 = end1;
  SizeType end2 = end;
  sort_sub(chunk, begin1, end1);
  sort_sub(chunk, begin2, end2);

  // trivial case
  // 前半部分の末尾が後半部分の先頭より大きければ
  // すでに整列している．
  auto cube_end1 = _cube_begin(chunk, end1 - 1);
  auto cube_begin2 = _cube_begin(chunk, begin2);
  if ( _cube_compare(cube_end1, cube_begin2) > 0 ) {
    return;
  }

  // マージする．
  // 前半部分を一旦 mTmpBuff にコピーする．
  {
    auto dst_cube = _cube_begin(mTmpChunk);
    auto src_cube = _cube_begin(chunk, begin1);
    _copy(dst_cube, src_cube, hn);
  }
  auto cube1 = _cube_begin(mTmpChunk);
  auto cube1_end = cube1 + hn;
  auto cube2 = _cube_begin(chunk, begin2);
  auto cube2_end = _cube_begin(chunk, end2);
  auto dst_cube = _cube_begin(chunk);
  while ( cube1 != cube1_end && cube2 != cube2_end ) {
    int comp_res = _cube_compare(cube1, cube2);
    if ( comp_res > 0 ) {
      _cube_copy(dst_cube, cube1);
      _cube_next(cube1);
      _cube_next(dst_cube);
    }
    else if ( comp_res < 0 ) {
      _cube_copy(dst_cube, cube2);
      _cube_next(cube2);
      _cube_next(dst_cube);
    }
    else {
      // 重複したキューブはエラー
      ASSERT_NOT_REACHED;
    }
  }
  while ( cube1 != cube1_end ) {
    _cube_copy(dst_cube, cube1);
    _cube_next(cube1);
    _cube_next(dst_cube);
  }
  // 後半部分が残っている時はそのままでいいはず．
  ASSERT_COND( cube2 == dst_cube );
}

END_NAMESPACE_YM_SOP
