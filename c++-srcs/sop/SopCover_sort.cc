
/// @file SopCover_sort.cc
/// @brief SopCover のソート関係の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/SopCover.h"
#include "SopSorter.h"

#define DEBUG 0
#define VERIFY 0

BEGIN_NAMESPACE_YM_SOP

//////////////////////////////////////////////////////////////////////
// クラス SopCover
//////////////////////////////////////////////////////////////////////

// @brief ソートする．
void
SopCover::_sort()
{
#if DEBUG
  {
    std::cout << "SopCover::_sort()" << std::endl;
    for ( SizeType i = 0; i < cube_num(); ++ i ) {
      auto cube = get_cube(i);
      std::cout << "CUBE0#" << i << ": ";
      cube.print(std::cout);
      std::cout << std::endl;
    }
    std::cout << "----------------------" << std::endl;
  }
#endif
  SopSorter sorter{variable_num()};
  mCubeNum = sorter.sort(cube_num(), chunk());

#if DEBUG
  {
    std::cout << "after SopCover::_sort()" << std::endl;
    for ( SizeType i = 0; i < cube_num(); ++ i ) {
      auto cube = get_cube(i);
      std::cout << "CUBE0#" << i << ": ";
      cube.print(std::cout);
      std::cout << std::endl;
    }
    std::cout << "----------------------" << std::endl;
  }
#endif
}


//////////////////////////////////////////////////////////////////////
// クラス SopSorter
//////////////////////////////////////////////////////////////////////

// @brief マージソートを行う下請け関数
SizeType
SopSorter::sort(
  SizeType cube_num,
  Chunk& chunk
)
{
  // もとのカバーと同じ大きさのバッファを確保する．
  mTmpChunk = _new_chunk(cube_num);
  return sort_sub(chunk, 0, cube_num);
}

SizeType
SopSorter::sort_sub(
  Chunk& chunk,
  SizeType begin,
  SizeType end
)
{
  SizeType n = end - begin;
  if ( n <= 1 ) {
    return end;
  }
  if ( n == 2 ) {
    return sort2(chunk, begin, end);
  }
  if ( n == 3 ) {
    return sort3(chunk, begin, end);
  }
  if ( n == 4 ) {
    return sort4(chunk, begin, end);
  }

  // 半分に分割してそれぞれソートする．
  SizeType hn = (n + 1) / 2;
  SizeType begin1 = begin;
  SizeType end1 = begin + hn;
  SizeType begin2 = end1;
  SizeType end2 = end;
  end1 = sort_sub(chunk, begin1, end1);
  end2 = sort_sub(chunk, begin2, end2);
  end = end1 + end2 - begin2;

  // trivial case
  // 前半部分の末尾が後半部分の先頭より大きければ
  // すでに整列している．
  {
    auto cube_end1 = _cube(chunk, end1 - 1);
    auto cube_begin2 = _cube(chunk, begin2);
    if ( _cube_compare(cube_end1, cube_begin2) > 0 ) {
      if ( end1 < begin2 ) {
	auto cube2_list = _cube_list(chunk, begin2, end2);
	auto cube2_iter = cube2_list.begin();
	auto cube2_end = cube2_list.end();
	SizeType new_end = end1;
	while ( cube2_iter != cube2_end ) {
	  auto cube2 = *cube2_iter;
	  auto dst_cube = _dst_cube(chunk, new_end);
	  _cube_copy(dst_cube, cube2);
	  ++ cube2_iter;
	  ++ new_end;
	}
	end = new_end;
      }
#if VERIFY
      _check(chunk, begin, end);
#endif
      return end;
    }
  }

  // マージする．
  // 前半部分を一旦 mTmpChunk にコピーする．
  {
    auto dst_cube = _dst_cube(mTmpChunk, begin1);
    auto src_list = _cube_list(chunk, begin1, end1);
    auto src_cube = *src_list.begin();
    auto src_end = *src_list.end();
    std::copy(src_cube, src_end, dst_cube);
  }
#if DEBUG
  {
    std::cout << "cube1_list[" << begin1 << ": " << end1 << ")" << std::endl;
    auto cube1_list = _cube_list(mTmpChunk, begin1, end1);
    auto cube1_iter = cube1_list.begin();
    auto cube1_end = cube1_list.end();
    SizeType i = begin1;
    for ( ; cube1_iter != cube1_end; ++ cube1_iter ) {
      std::cout << "CUBE1#" << i << ": "; ++ i;
      _print(std::cout, *cube1_iter);
      std::cout << std::endl;
    }
    std::cout << "=========================" << std::endl;
  }
  {
    std::cout << "cube2_list[" << begin2 << ": " << end2 << ")" << std::endl;
    auto cube2_list = _cube_list(chunk, begin2, end2);
    auto cube2_iter = cube2_list.begin();
    auto cube2_end = cube2_list.end();
    SizeType i = begin2;
    for ( ; cube2_iter != cube2_end; ++ cube2_iter ) {
      std::cout << "CUBE2#" << i << ": "; ++ i;
      _print(std::cout, *cube2_iter);
      std::cout << std::endl;
    }
    std::cout << "=========================" << std::endl;
  }
#endif
  auto cube1_list = _cube_list(mTmpChunk, begin1, end1);
  auto cube1_iter = cube1_list.begin();
  auto cube1_end = cube1_list.end();
  auto cube2_list = _cube_list(chunk, begin2, end2);
  auto cube2_iter = cube2_list.begin();
  auto cube2_end = cube2_list.end();
  SizeType new_end = begin;
  while ( cube1_iter != cube1_end &&
	  cube2_iter != cube2_end ) {
    auto cube1 = *cube1_iter;
    auto cube2 = *cube2_iter;
    if ( 0 ) { /*DD*/
      std::cout << "cube1: ";
      _print(std::cout, cube1);
      std::cout << std::endl;
      std::cout << "cube2: ";
      _print(std::cout, cube2);
      std::cout << std::endl;
    }
    int comp_res = _cube_compare(cube1, cube2);
    auto dst_cube = _dst_cube(chunk, new_end);
    if ( comp_res > 0 ) {
      _cube_copy(dst_cube, cube1);
      ++ cube1_iter;
    }
    else if ( comp_res < 0 ) {
      _cube_copy(dst_cube, cube2);
      ++ cube2_iter;
    }
    else {
      // 重複したキューブ
      _cube_copy(dst_cube, cube1);
      ++ cube1_iter;
      ++ cube2_iter;
    }
    ++ new_end;
    if ( 0 ) { /*DD*/
      std::cout << " => [#" << new_end - 1 << "]" ;
      _print(std::cout, _cube(chunk, new_end - 1));
      std::cout << std::endl;
      {
	std::cout << "sort_list[" << begin << ": "
	     << new_end << ")" << std::endl;
	auto cube_list = _cube_list(chunk, begin, new_end);
	auto cube_iter = cube_list.begin();
	auto cube_end = cube_list.end();
	SizeType i = begin;
	for ( ; cube_iter != cube_end; ++ cube_iter ) {
	  std::cout << "CUBE3#" << i << ": "; ++ i;
	  _print(std::cout, *cube_iter);
	  std::cout << std::endl;
	}
	std::cout << "=============================" << std::endl;
      }
    }
  }
  while ( cube1_iter != cube1_end ) {
    auto cube1 = *cube1_iter;
    auto dst_cube = _dst_cube(chunk, new_end);
    _cube_copy(dst_cube, cube1);
    ++ cube1_iter;
    ++ new_end;
    if ( 0 ) { /*DD*/
      std::cout << "cube1 => dst_cube: ";
      _print(std::cout, dst_cube);
      std::cout << std::endl;
      {
	std::cout << "sort_list[" << begin << ": "
	     << new_end << ")" << std::endl;
	auto cube_list = _cube_list(chunk, begin, new_end);
	auto cube_iter = cube_list.begin();
	auto cube_end = cube_list.end();
	SizeType i = begin;
	for ( ; cube_iter != cube_end; ++ cube_iter ) {
	  std::cout << "CUBE3#" << i << ": "; ++ i;
	  _print(std::cout, *cube_iter);
	  std::cout << std::endl;
	}
	std::cout << "=============================" << std::endl;
      }
    }
  }
  while ( cube2_iter != cube2_end ) {
    auto cube2 = *cube2_iter;
    auto dst_cube = _dst_cube(chunk, new_end);
    _cube_copy(dst_cube, cube2);
    ++ cube2_iter;
    ++ new_end;
    if ( 0 ) { /*DD*/
      std::cout << "cube2 => dst_cube: ";
      _print(std::cout, dst_cube);
      std::cout << std::endl;
      {
	std::cout << "sort_list[" << begin << ": "
	     << new_end << ")" << std::endl;
	auto cube_list = _cube_list(chunk, begin, new_end);
	auto cube_iter = cube_list.begin();
	auto cube_end = cube_list.end();
	SizeType i = begin;
	for ( ; cube_iter != cube_end; ++ cube_iter ) {
	  std::cout << "CUBE3#" << i << ": "; ++ i;
	  _print(std::cout, *cube_iter);
	  std::cout << std::endl;
	}
	std::cout << "=============================" << std::endl;
      }
    }
  }
  end = new_end;

#if DEBUG
  {
    std::cout << "sort_list[" << begin << ": " << end << ")" << std::endl;
    auto cube_list = _cube_list(chunk, begin, end);
    auto cube_iter = cube_list.begin();
    auto cube_end = cube_list.end();
    SizeType i = begin;
    for ( ; cube_iter != cube_end; ++ cube_iter ) {
      std::cout << "CUBE3#" << i << ": "; ++ i;
      _print(std::cout, *cube_iter);
      std::cout << std::endl;
    }
    std::cout << "=============================" << std::endl;
  }
#endif

  // 後半部分が残っている時はそのままでいいはず．
#if VERIFY
  _check(chunk, begin, end);
#endif
  return end;
}

// @brief 要素数が3の場合のソート
SizeType
SopSorter::sort3(
  Chunk& chunk,
  SizeType begin,
  SizeType end
)
{
  // (0),
  // (0, 1), (1, 0),
  // (0, 2), (2, 0),
  // (0, 1, 2), (0, 2, 1), (1, 0, 2), (1, 2, 0), (2, 0, 1), (2, 1, 0)
  // の11通りなのでしらみ潰し
  auto cube0 = _dst_cube(chunk, begin + 0);
  auto cube1 = _dst_cube(chunk, begin + 1);
  auto cube2 = _dst_cube(chunk, begin + 2);
  auto res01 = _cube_compare(cube0, cube1);
  if ( res01 < 0 ) {
    auto res12 = _cube_compare(cube1, cube2);
    if ( res12 < 0 ) {
      // (2, 1, 0)
      // 0 と 2 を交換
      _cube_swap(cube0, cube2);
    }
    else if ( res12 == 0 ) {
      // (1=2, 0)
      // 0 と 1 を交換
      _cube_swap(cube0, cube1);
      -- end;
    }
    else { // res12 > 0
      auto res02 = _cube_compare(cube0, cube2);
      if ( res02 < 0 ) {
	// (1, 2, 0)
	// 0 <- 1, 1 <- 2, 2 <- 0
	_cube_rotate3(cube0, cube1, cube2);
      }
      else { // res02 >= 0
	// (1, 0, 2)
	_cube_swap(cube0, cube1);
	if ( res02 == 0 ) {
	  -- end;
	}
      }
    }
  }
  else if ( res01 == 0 ) {
    auto res02 = _cube_compare(cube0, cube2);
    if ( res02 < 0 ) {
      // (2, 0, 1) or (2, 1, 0)
      // 2 と 0 を交換
      _cube_swap(cube0, cube2);
    }
    else if ( res02 == 0 ) {
      // (0, 1, 2)
      -- end;
    }
    else { // res02 > 0
      // (0, 2, 1)
      // 1 と 2 を交換
      _cube_swap(cube1, cube2);
    }
    -- end;
  }
  else { // res01 > 0
    auto res02 = _cube_compare(cube0, cube2);
    if ( res02 < 0 ) {
      // (2, 0, 1)
      // 0 <- 2, 2 <- 1, 1 <- 0
      _cube_rotate3(cube0, cube2, cube1);
    }
    else if ( res02 == 0 ) {
      -- end;
    }
    else { // res02 > 0
      auto res12 = _cube_compare(cube1, cube2);
      if ( res12 < 0 ) {
	// (0, 2, 1)
	// 1 と 2 を交換
	_cube_swap(cube1, cube2);
      }
      else { // res12 >= 0
	// (0, 1, 2)
	if ( res12 == 0 ) {
	  -- end;
	}
      }
    }
  }
#if VERIFY
  _check(chunk, begin, end);
#endif
  return end;
}

// @brief 要素数が3の場合のソート
SizeType
SopSorter::sort4(
  Chunk& chunk,
  SizeType begin,
  SizeType end
)
{
#if DEBUG
  {
    std::cout << "sort4 begin" << std::endl;
    auto cube1_list = _cube_list(chunk, begin, end);
    auto cube1_iter = cube1_list.begin();
    auto cube1_end = cube1_list.end();
    for ( ; cube1_iter != cube1_end; ++ cube1_iter ) {
      _print(std::cout, *cube1_iter);
      std::cout << std::endl;
    }
  }
#endif
  auto cube0 = _dst_cube(chunk, begin + 0);
  auto cube1 = _dst_cube(chunk, begin + 1);
  auto cube2 = _dst_cube(chunk, begin + 2);
  auto cube3 = _dst_cube(chunk, begin + 3);
  bool has_cube1 = true;
  bool has_cube3 = true;
  // 0 と 1 を整列
  auto res01 = _cube_compare(cube0, cube1);
  if ( res01 < 0 ) {
    _cube_swap(cube0, cube1);
  }
  else if ( res01 == 0 ) {
    has_cube1 = false;
    -- end;
  }
  // (0, 1), (0=1)

  // 2 と 3 を整列
  auto res23 = _cube_compare(cube2, cube3);
  if ( res23 < 0 ) {
    _cube_swap(cube2, cube3);
  }
  else if ( res23 == 0 ) {
    has_cube3 = false;
    -- end;
  }
  // (2, 3), (2=3)

  // 0 と 2 を比較
  auto res02 = _cube_compare(cube0, cube2);
  if ( res02 < 0 ) {
    // (2, 013) は確定
    if ( has_cube3 ) {
      auto res03 = _cube_compare(cube0, cube3);
      if ( res03 < 0 ) {
	// (2, 3, 0, [1])
	// (0, 1) と (2, 3) を交換
	_cube_swap(cube0, cube2);
	_cube_swap(cube1, cube3);
	// 1 はあってもなくても同じ
      }
      else if ( res03 == 0 ) {
	// (2, 0, [1], [3])
	// 0 <- 2, 2 <- 1, 1 <- 0
	_cube_rotate3(cube0, cube2, cube1);
	-- end;
      }
      else {
	// (2, 0, 31) は確定
	if ( has_cube1 ) {
	  auto res13 = _cube_compare(cube1, cube3);
	  if ( res13 < 0 ) {
	    // (2, 0, 3, 1)
	    // 0 <- 2, 2 <- 3, 3 <= 1, 1 <- 0
	    _cube_rotate4(cube0, cube2, cube3, cube1);
	  }
	  else {
	    // (2, 0, 1, [3])
	    // 0 <- 2, 2 <= 1, 1 <= 0
	    _cube_rotate3(cube0, cube2, cube1);
	    if ( res13 == 0 ) {
	      -- end;
	    }
	  }
	}
	else {
	  // (2, 0, 3, [1])
	  // 0 <- 2, 2 <- 3, 3 <= 1, 1 <- 0
	  _cube_rotate4(cube0, cube2, cube3, cube1);
	}
      }
    }
    else {
      // (2, 0, [1], [3])
      // 0 <- 2, 2 <- 1, 1 <- 0
      _cube_rotate3(cube0, cube2, cube1);
    }
  }
  else if ( res02 == 0 ) {
    if ( has_cube1 ) {
      if ( has_cube3 ) {
	// (0=2, 1, 3), (0=2, 1=3), (0=2, 3, 1)
	auto res13 = _cube_compare(cube1, cube3);
	if ( res13 < 0 ) {
	  // (0, 3, 1, [2])
	  // 1 <- 3, 3 <- 2, 2 <- 2
	  _cube_rotate3(cube1, cube3, cube2);
	}
	else if ( res13 == 0 ) {
	  // (0, 1, [2], [3])
	  -- end;
	}
	else { // res13 > 0
	  // (0, 1, 3, [2])
	  // 2 と 3 を入れ替える．
	  _cube_swap(cube2, cube3);
	}
      }
      else {
	// (0=2=3, 1, [2], [3])
      }
    }
    else {
      if ( has_cube3 ) {
	// (0=1=2, 3, [1], [2])
	// 1 <- 3, 3 <- 2, 2 <- 1
	_cube_rotate3(cube1, cube3, cube2);
      }
      else {
	// (0=1=2=3, 1, 2, 3)
      }
    }
    -- end;
  }
  else { // res02 > 0
    if ( has_cube1 ) {
      auto res12 = _cube_compare(cube1, cube2);
      if ( res12 < 0 ) {
	// (0, 2=3, 1), (0, 2, 3, 1), (0, 2, 1=3), (0, 2, 1, 3)
	if ( has_cube3 ) {
	  auto res13 = _cube_compare(cube1, cube3);
	  if ( res13 < 0 ) {
	    // (0, 2, 3, 1)
	    // 1 <- 2, 2 < 3, 3 <- 1
	    _cube_rotate3(cube1, cube2, cube3);
	  }
	  else {
	    // (0, 2, 1, [3])
	    // 1 <- 2, 2 <- 1
	    _cube_swap(cube1, cube2);
	    if ( res13 == 0 ) {
	      -- end;
	    }
	  }
	}
	else {
	  // (0, 2, 1, [3])
	  // 1 <- 2, 2 <- 1
	  _cube_swap(cube1, cube2);
	}
      }
      else if ( res12 == 0 ) {
	if ( has_cube3 ) {
	  // (0, 1, 3)
	  // 2 <- 3, 3 <- 2
	  _cube_swap(cube2, cube3);
	}
	else {
	  // (0, 1=2=3)
	}
	-- end;
      }
      else {
	// (0, 1, 2=3), (0, 1, 2, 3),
      }
    }
    else {
      // (0, 2, 3)
      // 1 <= 2, 2 <- 3, 3 <- 1
      _cube_rotate3(cube1, cube2, cube3);
    }
  }
#if VERIFY
  _check(chunk, begin, end);
#endif
#if DEBUG
  {
    std::cout << "sort4 end" << std::endl;
    auto cube1_list = _cube_list(chunk, begin, end);
    auto cube1_iter = cube1_list.begin();
    auto cube1_end = cube1_list.end();
    for ( ; cube1_iter != cube1_end; ++ cube1_iter ) {
      _print(std::cout, *cube1_iter);
      std::cout << std::endl;
    }
  }
#endif
  return end;
}

END_NAMESPACE_YM_SOP
