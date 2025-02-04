
/// @file SopCover_concate.cc
/// @brief SopCover の concate 関係の実装ファイル
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

// @brief 論理和を計算する．
SopCover
SopCover::operator+(
  const SopCover& right ///< [in] オペランド
) const
{
  if ( variable_num() != right.variable_num() ) {
    throw std::invalid_argument("variable_num() is different from each other");
  }
  SizeType dst_num;
  auto dst_chunk = concate(right.cube_num(), right.chunk(), dst_num);
  return SopCover{variable_num(), dst_num, std::move(dst_chunk)};
}

// @brief 論理和を計算して代入する．
SopCover&
SopCover::operator+=(
  const SopCover& right ///< [in] オペランド
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
SopCover
SopCover::operator+(
  const SopCube& right ///< [in] オペランド
) const
{
  if ( variable_num() != right.variable_num() ) {
    throw std::invalid_argument("variable_num() is different from each other");
  }
  SizeType dst_num;
  auto dst_chunk = concate(1, right.chunk(), dst_num);
  return SopCover{variable_num(), dst_num, std::move(dst_chunk)};
}

// @brief 論理和を計算して代入する(キューブ版)．
SopCover&
SopCover::operator+=(
  const SopCube& right ///< [in] オペランド
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
SopBase::Chunk
SopCover::concate(
  SizeType num2,
  const Chunk& chunk2,
  SizeType& dst_num
) const
{
  SizeType num1 = cube_num();
  auto dst_chunk = _new_chunk(num1 + num2);
  auto dst_cube = _cube_begin(dst_chunk);
  dst_num = 0;
  auto cube1 = _cube_begin(chunk());
  auto end1 = _cube_end(cube1, num1);
  auto cube2 = _cube_begin(chunk2);
  auto end2 = _cube_end(cube2, num2);
  while ( cube1 != end1 && cube2 != end2 ) {
    int res = _cube_compare(cube1, cube2);
    if ( res > 0 ) {
      _cube_copy(dst_cube, cube1);
      _cube_next(cube1);
    }
    else if ( res < 0 ) {
      _cube_copy(dst_cube, cube2);
      _cube_next(cube2);
    }
    else {
      _cube_copy(dst_cube, cube1);
      _cube_next(cube1);
      _cube_next(cube2);
    }
    _cube_next(dst_cube);
    ++ dst_num;
  }
  while ( cube1 != end1 ) {
    _cube_copy(dst_cube, cube1);
    _cube_next(cube1);
    _cube_next(dst_cube);
    ++ dst_num;
  }
  while ( cube2 != end2 ) {
    _cube_copy(dst_cube, cube2);
    _cube_next(cube2);
    _cube_next(dst_cube);
    ++ dst_num;
  }
  return dst_chunk;
}

END_NAMESPACE_YM_SOP
