
/// @file AlgCover_concate.cc
/// @brief AlgCover の concate の実装ファイル
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

// @brief 論理和を計算する．
AlgCover
AlgCover::operator+(
  const AlgCover& right ///< [in] オペランド
) const
{
  if ( variable_num() != right.variable_num() ) {
    throw std::invalid_argument("variable_num() is different from each other");
  }
  SizeType dst_num;
  auto dst_chunk = concate(right.cube_num(), right.chunk(), dst_num);
  return AlgCover{variable_num(), dst_num, std::move(dst_chunk)};
}

// @brief 論理和を計算して代入する．
AlgCover&
AlgCover::operator+=(
  const AlgCover& right ///< [in] オペランド
)
{
  if ( variable_num() != right.variable_num() ) {
    throw std::invalid_argument("variable_num() is different from each other");
  }
  SizeType dst_num;
  auto dst_chunk = concate(right.cube_num(), right.chunk(), dst_num);
  _set(dst_num, std::move(dst_chunk));
  return *this;
}

// @brief 論理和を計算する(キューブ版)．
AlgCover
AlgCover::operator+(
  const AlgCube& right ///< [in] オペランド
) const
{
  if ( variable_num() != right.variable_num() ) {
    throw std::invalid_argument("variable_num() is different from each other");
  }
  SizeType dst_num;
  auto dst_chunk = concate(1, right.chunk(), dst_num);
  return AlgCover{variable_num(), dst_num, std::move(dst_chunk)};
}

// @brief 論理和を計算して代入する(キューブ版)．
AlgCover&
AlgCover::operator+=(
  const AlgCube& right ///< [in] オペランド
)
{
  if ( variable_num() != right.variable_num() ) {
    throw std::invalid_argument("variable_num() is different from each other");
  }
  SizeType dst_num;
  auto dst_chunk = concate(1, right.chunk(), dst_num);
  _set(dst_num, std::move(dst_chunk));
  return *this;
}

// @brief concate の共通処理
AlgBase::Chunk
AlgCover::concate(
  SizeType num2,
  const Chunk& chunk2,
  SizeType& dst_num
) const
{
  SizeType num1 = cube_num();
  auto dst_chunk = _new_chunk(num1 + num2);
  auto dst_list = _cube_list(dst_chunk);
  auto cube1_list = _cube_list(chunk(), 0, num1);
  auto cube2_list = _cube_list(chunk2, 0, num2);
  auto cube1_iter = cube1_list.begin();
  auto cube1_end = cube1_list.end();
  auto cube2_iter = cube2_list.begin();
  auto cube2_end = cube2_list.end();
  while ( cube1_iter != cube1_end &&
	  cube2_iter != cube2_end ) {
    auto cube1 = *cube1_iter;
    auto cube2 = *cube2_iter;
    int res = _cube_compare(cube1, cube2);
    auto dst_cube = dst_list.back();
    if ( res > 0 ) {
      _cube_copy(dst_cube, cube1);
      ++ cube1_iter;
    }
    else if ( res < 0 ) {
      _cube_copy(dst_cube, cube2);
      ++ cube2_iter;
    }
    else {
      _cube_copy(dst_cube, cube1);
      ++ cube1_iter;
      ++ cube2_iter;
    }
    dst_list.inc();
  }
  for ( ; cube1_iter != cube1_end; ++ cube1_iter, dst_list.inc() ) {
    auto cube1 = *cube1_iter;
    auto dst_cube = dst_list.back();
    _cube_copy(dst_cube, cube1);
  }
  for ( ; cube2_iter != cube2_end; ++ cube2_iter, dst_list.inc() ) {
    auto cube2 = *cube2_iter;
    auto dst_cube = dst_list.back();
    _cube_copy(dst_cube, cube2);
  }
  dst_num = dst_list.num();
  return dst_chunk;
}

END_NAMESPACE_YM_ALG
