
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
  auto dst_list = _cube_list(dst_chunk);
  auto cube1_list = _cube_list(chunk(), 0, num1);
  auto cube1_iter = cube1_list.begin();
  auto cube1_end = cube1_list.end();
  auto cube2_list = _cube_list(chunk2, 0, num2);
  auto cube2_iter = cube2_list.begin();
  auto cube2_end = cube2_list.end();
  while ( cube1_iter != cube1_end &&
	  cube2_iter != cube2_end ) {
    auto cube1 = *cube1_iter;
    auto cube2 = *cube2_iter;
    int res = _cube_compare(cube1, cube2);
    if ( res > 0 ) {
      // cube1 のみ存在する．
      // そのまま結果に含める．
      auto dst_cube = dst_list.back();
      _cube_copy(dst_cube, cube1);
      ++ cube1_iter;
      dst_list.inc();
    }
    else if ( res < 0 ) {
      // cube2 のみ存在する．
      ++ cube2_iter;
    }
    else {
      // cube1 == cube2 だった．
      // つまり結果には含めない．
      ++ cube1_iter;
      ++ cube2_iter;
    }
  }
  while ( cube1_iter != cube1_end ) {
    auto cube1 = *cube1_iter;
    auto dst_cube = dst_list.back();
    _cube_copy(dst_cube, cube1);
    ++ cube1_iter;
    dst_list.inc();
  }
  dst_num = dst_list.num();
  return dst_chunk;
}

END_NAMESPACE_YM_SOP
