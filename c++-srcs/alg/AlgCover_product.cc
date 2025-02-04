
/// @file AlgCover_product.cc
/// @brief AlgCover の product 関係の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/AlgCover.h"
#include "ym/AlgCube.h"
#include "AlgSorter.h"


BEGIN_NAMESPACE_YM_ALG

//////////////////////////////////////////////////////////////////////
// クラス AlgCover
//////////////////////////////////////////////////////////////////////

// @brief 論理積を計算する．
AlgCover
AlgCover::operator*(
  const AlgCover& right
) const
{
  if ( variable_num() != right.variable_num() ) {
    throw std::invalid_argument("variable_num() is different from each other");
  }
  SizeType dst_num;
  auto dst_chunk = product(right.cube_num(), right.chunk(), dst_num);
  return AlgCover{variable_num(), dst_num, std::move(dst_chunk)};
}

// @brief 論理積を計算して代入する．
AlgCover&
AlgCover::operator*=(
  const AlgCover& right
)
{
  if ( variable_num() != right.variable_num() ) {
    throw std::invalid_argument("variable_num() is different from each other");
  }
  SizeType dst_num;
  auto dst_chunk = product(right.cube_num(), right.chunk(), dst_num);
  _set(dst_num, std::move(dst_chunk));
  return *this;
}

// @brief 論理積を計算する(キューブ版)．
AlgCover
AlgCover::operator*(
  const AlgCube& right
) const
{
  if ( variable_num() != right.variable_num() ) {
    throw std::invalid_argument("variable_num() is different from each other");
  }
  SizeType dst_num;
  auto dst_chunk = product(1, right.chunk(), dst_num);
  return AlgCover{variable_num(), dst_num, std::move(dst_chunk)};
}

// @brief 論理積を計算して代入する(キューブ版)．
AlgCover&
AlgCover::operator*=(
  const AlgCube& right
)
{
  if ( variable_num() != right.variable_num() ) {
    throw std::invalid_argument("variable_num() is different from each other");
  }
  SizeType dst_num;
  auto dst_chunk = product(1, right.chunk(), dst_num);
  _set(dst_num, std::move(dst_chunk));
  return *this;
}

// @brief 論理積を計算する(リテラル版)．
AlgCover
AlgCover::operator*(
  Literal right
) const
{
  SizeType dst_num;
  auto dst_chunk = product(right, dst_num);
  return AlgCover{variable_num(), dst_num, std::move(dst_chunk)};
}

// @brief 論理積を計算して代入する(リテラル版)．
AlgCover&
AlgCover::operator*=(
  Literal right
)
{
  SizeType dst_num;
  auto dst_chunk = product(right, dst_num);
  _set(dst_num, std::move(dst_chunk));
  return *this;
}

// @brief 2つのカバーの論理積を計算する．
AlgBase::Chunk
AlgCover::product(
  SizeType num2,
  const Chunk& chunk2,
  SizeType& dst_num
) const
{
  // 単純には答の積項数は2つの積項数の積だが
  // 相反するリテラルを含む積は数えない．
  SizeType num1 = cube_num();
  auto dst_chunk = _new_chunk(num1 * num2);
  auto dst_list = _cube_list(dst_chunk);
  auto cube1_list = _cube_list(chunk(), 0, num1);
  auto cube2_list = _cube_list(chunk2, 0, num2);
  for ( auto cube1: cube1_list ) {
    for ( auto cube2: cube2_list ) {
      auto dst_cube = dst_list.back();
      if ( _cube_product(dst_cube, cube1, cube2) ) {
	dst_list.inc();
      }
    }
  }
  dst_num = dst_list.num();
  AlgSorter sorter{variable_num()};
  sorter.sort(dst_num, dst_chunk);
  return dst_chunk;
}

// @brief カバーとリテラルとの論理積を計算する．
AlgBase::Chunk
AlgCover::product(
  Literal lit,
  SizeType& dst_num
) const
{
  auto varid = lit.varid();
  auto inv = lit.is_negative();
  auto blk = _block_pos(varid);
  auto pat1 = _get_mask(varid, inv);
  auto npat1 = _get_mask(varid, !inv);

  // 単純には答の積項数は2つの積項数の積だが
  // 相反するリテラルを含む積は数えない．
  SizeType num1 = cube_num();
  auto dst_chunk = _new_chunk(num1);
  auto dst_list = _cube_list(dst_chunk);
  auto cube1_list = _cube_list(chunk(), 0, num1);
  for ( auto cube1: cube1_list ) {
    auto src1_p = cube1 + blk;
    if ( (*src1_p & npat1) == 0 ) {
      auto dst_cube = dst_list.back();
      _cube_copy(dst_cube, cube1);
      auto dst_p = dst_cube + blk;
      *dst_p |= pat1;
      dst_list.inc();
    }
  }
  dst_num = dst_list.num();
  // 同じ位置にパタンが足されるだけなので
  // 順序関係は変わらない．
  return dst_chunk;
}

END_NAMESPACE_YM_ALG
