
/// @file SopCover_diff.cc
/// @brief SopCover の diff 関係の実装ファイル
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

// @brief 差分を計算する．
SopCover
SopCover::operator-(
  const SopCover& right ///< [in] オペランド
) const
{
  if ( variable_num() != right.variable_num() ) {
    throw std::invalid_argument("variable_num() is different from each other");
  }
  SizeType dst_num;
  auto dst_chunk = diff(right.cube_num(), right.chunk(), dst_num);
  return SopCover{variable_num(), dst_num, std::move(dst_chunk)};
}

// @brief 差分を計算して代入する．
SopCover&
SopCover::operator-=(
  const SopCover& right ///< [in] オペランド
)
{
  if ( variable_num() != right.variable_num() ) {
    throw std::invalid_argument("variable_num() is different from each other");
  }
  SizeType dst_num;
  auto dst_chunk = diff(right.cube_num(), right.chunk(), dst_num);
  _set(dst_num, std::move(dst_chunk));
  return *this;
}

// @brief 差分を計算する(キューブ版)．
SopCover
SopCover::operator-(
  const SopCube& right ///< [in] オペランド
) const
{
  if ( variable_num() != right.variable_num() ) {
    throw std::invalid_argument("variable_num() is different from each other");
  }
  SizeType dst_num;
  auto dst_chunk = diff(1, right.chunk(), dst_num);
  return SopCover{variable_num(), dst_num, std::move(dst_chunk)};
}

// @brief 差分を計算して代入する(キューブ版)．
SopCover&
SopCover::operator-=(
  const SopCube& right ///< [in] オペランド
)
{
  if ( variable_num() != right.variable_num() ) {
    throw std::invalid_argument("variable_num() is different from each other");
  }
  SizeType dst_num;
  auto dst_chunk = diff(1, right.chunk(), dst_num);
  _set(dst_num, std::move(dst_chunk));
  return *this;
}

// @brief diff の共通処理
SopBase::Chunk
SopCover::diff(
  SizeType num2,
  const Chunk& chunk2,
  SizeType& dst_num
) const
{
  SizeType num1 = cube_num();

  // 結果のキューブ数は block1 のキューブ数を超えない．
  auto dst_chunk = _new_chunk(num1);
  auto dst_cube = _cube_begin(dst_chunk);
  auto cube1 = _cube_begin(chunk());
  auto end1 = _cube_end(cube1, num1);
  auto cube2 = _cube_begin(chunk2);
  auto end2 = _cube_end(cube2, num2);
  dst_num = 0;
  while ( cube1 != end1 && cube2 != end2 ) {
    int res = _cube_compare(cube1, cube2);
    if ( res > 0 ) {
      // cube1 のみ存在する．
      // そのまま結果に含める．
      _cube_copy(dst_cube, cube1);
      _cube_next(cube1);
      _cube_next(dst_cube);
      ++ dst_num;
    }
    else if ( res < 0 ) {
      // cube2 のみ存在する．
      _cube_next(cube2);
    }
    else {
      // cube1 == cube2 だった．
      // つまり結果には含めない．
      _cube_next(cube1);
      _cube_next(cube2);
    }
  }
  while ( cube1 != end1 ) {
    _cube_copy(dst_cube, cube1);
    _cube_next(cube1);
    _cube_next(dst_cube);
    ++ dst_num;
  }

  return dst_chunk;
}

END_NAMESPACE_YM_SOP
