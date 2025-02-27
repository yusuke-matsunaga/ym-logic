
/// @file SopCover_sort.cc
/// @brief SopCover のソート関係の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/SopCover.h"
#include "SopSorter.h"

#define VERIFY 0

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

  {
    for ( auto& cube: literal_list() ) {
      for ( auto lit: cube ) {
	if ( lit.varid() >= variable_num() ) {
	  throw std::logic_error{"lit is out of range"};
	}
      }
    }
  }

  // 重複したキューブを削除する．
  SizeType n = cube_num();
  if ( n > 0 ) {
    SizeType prev_pos = 0;
    SizeType dst_pos = 1;
    for ( SizeType src_pos = 1; src_pos < n; ++ src_pos ) {
      auto prev_cube = _cube(chunk(), prev_pos);
      auto cube = _cube(chunk(), src_pos);
      if ( _cube_compare(prev_cube, cube) != 0 ) {
	if ( dst_pos < src_pos ) {
	  auto dst_cube = _dst_cube(chunk(), dst_pos);
	  _cube_copy(dst_cube, cube);
	}
	++ dst_pos;
	prev_pos = src_pos;
      }
    }
    mCubeNum = dst_pos;
  }
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
    auto cube0 = _dst_cube(chunk, begin + 0);
    auto cube1 = _dst_cube(chunk, begin + 1);
    if ( _cube_compare(cube0, cube1) < 0 ) {
      // (1, 0) だったので交換する．
      _cube_swap(cube0, cube1);
    }
#if VERIFY
    _check(chunk, begin, end);
#endif
    return;
  }
  if ( n == 3 ) {
    // (0, 1, 2), (0, 2, 1), (1, 0, 2), (1, 2, 0), (2, 0, 1), (2, 1, 0)
    // の6通りなのでしらみ潰し
    auto cube0 = _dst_cube(chunk, begin + 0);
    auto cube1 = _dst_cube(chunk, begin + 1);
    auto cube2 = _dst_cube(chunk, begin + 2);
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
#if VERIFY
    _check(chunk, begin, end);
#endif
    return;
  }
  if ( n == 4 ) {
    auto cube0 = _dst_cube(chunk, begin + 0);
    auto cube1 = _dst_cube(chunk, begin + 1);
    auto cube2 = _dst_cube(chunk, begin + 2);
    auto cube3 = _dst_cube(chunk, begin + 3);
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
#if VERIFY
    _check(chunk, begin, end);
#endif
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
  {
    auto cube_end1 = _cube(chunk, end1 - 1);
    auto cube_begin2 = _cube(chunk, begin2);
    if ( _cube_compare(cube_end1, cube_begin2) > 0 ) {
#if VERIFY
      _check(chunk, begin, end);
#endif
      return;
    }
  }

  // マージする．
  // 前半部分を一旦 mTmpBuff にコピーする．
  {
    auto dst_cube = _dst_cube(mTmpChunk);
    auto src_cube = _cube(chunk, begin1);
    auto src_end = src_cube + _cube_size() * hn;
    std::copy(src_cube, src_end, dst_cube);
  }
  auto cube1_list = _cube_list(mTmpChunk, 0, hn);
  auto cube1_iter = cube1_list.begin();
  auto cube1_end = cube1_list.end();
  auto cube2_list = _cube_list(chunk, begin2, end2);
  auto cube2_iter = cube2_list.begin();
  auto cube2_end = cube2_list.end();
  auto dst_list = _cube_list(chunk, begin);
  while ( cube1_iter != cube1_end &&
	  cube2_iter != cube2_end ) {
    auto cube1 = *cube1_iter;
    auto cube2 = *cube2_iter;
    int comp_res = _cube_compare(cube1, cube2);
    // 重複したキューブもそのままコピーする．
    if ( comp_res >= 0 ) {
      auto dst_cube = dst_list.back();
      _cube_copy(dst_cube, cube1);
      ++ cube1_iter;
      dst_list.inc();
    }
    if ( comp_res <= 0 ) {
      auto dst_cube = dst_list.back();
      _cube_copy(dst_cube, cube2);
      ++ cube2_iter;
      dst_list.inc();
    }
  }
  while ( cube1_iter != cube1_end ) {
    auto cube1 = *cube1_iter;
    auto dst_cube = dst_list.back();
    _cube_copy(dst_cube, cube1);
    ++ cube1_iter;
    dst_list.inc();
  }
  // 後半部分が残っている時はそのままでいいはず．
#if VERIFY
  _check(chunk, begin, end);
#endif
}

END_NAMESPACE_YM_SOP
