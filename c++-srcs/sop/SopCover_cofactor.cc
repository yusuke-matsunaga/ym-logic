
/// @file SopCover_cofactor.cc
/// @brief SopCover の cofactor 関係の実装ファイル
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

// @brief コファクターを計算する．
SopCover
SopCover::cofactor(
  const SopCube& right
) const
{
  if ( variable_num() != right.variable_num() ) {
    throw std::invalid_argument("variable_num() is different from each other");
  }
  // 結果のキューブ数はもとのキューブ数を超えない．
  SizeType num1 = cube_num();
  auto cube2 = _cube(right.chunk());
  auto dst_chunk = _new_chunk(num1);
  auto dst_list = _cube_list(dst_chunk);
  auto cube1_list = _cube_list(chunk(), 0, num1);
  for ( auto cube1: cube1_list ) {
    auto dst_cube = dst_list.back();
    if ( _cube_cofactor(dst_cube, cube1, cube2) ) {
      dst_list.inc();
    }
  }
  auto dst_num = dst_list.num();
  return SopCover{variable_num(), dst_num, std::move(dst_chunk)};
}

// @brief コファクターを計算する．
SopCover
SopCover::cofactor(
  Literal lit
) const
{
  // 結果のキューブ数はもとのキューブ数を超えない．
  SizeType num1 = cube_num();
  auto dst_chunk = _new_chunk(num1);
  auto dst_list = _cube_list(dst_chunk);
  auto cube1_list = _cube_list(chunk(), 0, num1);
  for ( auto cube1: cube1_list ) {
    auto dst_cube = dst_list.back();
    if ( _cube_cofactor(dst_cube, cube1, lit) ) {
      dst_list.inc();
    }
  }
  auto dst_num = dst_list.num();
  return SopCover{variable_num(), dst_num, std::move(dst_chunk)};
}

END_NAMESPACE_YM_SOP
