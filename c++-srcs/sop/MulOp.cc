
/// @file MulOp.cc
/// @brief MulOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "MulOp.h"


BEGIN_NAMESPACE_YM_SOP

//////////////////////////////////////////////////////////////////////
// クラス SopCover
//////////////////////////////////////////////////////////////////////

// @brief 論理積を計算する．
SopCover
SopCover::operator*(
  const SopCover& right
) const
{
  if ( variable_num() != right.variable_num() ) {
    throw std::invalid_argument("variable_num() is different from each other");
  }

  MulOp product{variable_num()};
  auto dst = product(to_block(), right.to_block());
  return product.make_cover(std::move(dst));
}

// @brief 論理積を計算して代入する．
SopCover&
SopCover::operator*=(
  const SopCover& right
)
{
  if ( variable_num() != right.variable_num() ) {
    throw std::invalid_argument("variable_num() is different from each other");
  }

  MulOp product{variable_num()};
  auto dst = product(to_block(), right.to_block());
  _set(std::move(dst));
  return *this;
}

// @brief 論理積を計算する(キューブ版)．
SopCover
SopCover::operator*(
  const SopCube& right
) const
{
  if ( variable_num() != right.variable_num() ) {
    throw std::invalid_argument("variable_num() is different from each other");
  }

  MulOp product{variable_num()};
  auto dst = product(to_block(), right.body());
  return product.make_cover(std::move(dst));
}

// @brief 論理積を計算して代入する(キューブ版)．
SopCover&
SopCover::operator*=(
  const SopCube& right
)
{
  if ( variable_num() != right.variable_num() ) {
    throw std::invalid_argument("variable_num() is different from each other");
  }

  MulOp product{variable_num()};
  auto dst = product(to_block(), right.body());
  _set(std::move(dst));
  return *this;
}

// @brief 論理積を計算する(リテラル版)．
SopCover
SopCover::operator*(
  Literal right
) const
{
  MulOp product{variable_num()};
  auto dst = product(to_block(), right);
  return product.make_cover(std::move(dst));
}

// @brief 論理積を計算して代入する(リテラル版)．
SopCover&
SopCover::operator*=(
  Literal right
)
{
  MulOp product{variable_num()};
  auto dst = product(to_block(), right);
  _set(std::move(dst));
  return *this;
}


//////////////////////////////////////////////////////////////////////
// クラス MulOp
//////////////////////////////////////////////////////////////////////

// @brief 2つのカバーの論理積を計算する．
SopBlock
MulOp::operator()(
  const SopBlockRef& block1,
  const SopBlockRef& block2
)
{
  auto& src1_bv = block1.body;
  auto& src2_bv = block2.body;
  SizeType src1_nc = block1.cube_num;
  SizeType src2_nc = block2.cube_num;

  auto dst = new_block(src1_nc + src2_nc);

  // 単純には答の積項数は2つの積項数の積だが
  // 相反するリテラルを含む積は数えない．
  auto dst_cube = _cube_begin(dst.body);
  dst.cube_num = 0;
  auto src1_begin = _cube_begin(src1_bv);
  auto src1_end = _cube_end(src1_begin, src1_nc);
  auto src2_begin = _cube_begin(src2_bv);
  auto src2_end = _cube_end(src2_begin, src2_nc);
  for ( auto src1_cube = src1_begin;
	src1_cube != src1_end;
	src1_cube += _cube_size() ) {
    for ( auto src2_cube = src2_begin;
	  src2_cube != src2_end;
	  src2_cube += _cube_size() ) {
      if ( cube_product(dst_cube, src1_cube, src2_cube) ) {
	dst_cube += _cube_size();
	++ dst.cube_num;
      }
    }
  }
  sort(dst);
  return dst;
}

// @brief カバーとリテラルとの論理積を計算する．
SopBlock
MulOp::operator()(
  const SopBlockRef& block1,
  Literal lit
)
{
  auto varid = lit.varid();
  auto inv = lit.is_negative();
  auto blk = _block_pos(varid);
  auto pat1 = get_mask(varid, inv);
  auto npat1 = get_mask(varid, !inv);

  SizeType src1_nc = block1.cube_num;
  auto src1_begin = _cube_begin(block1.body);
  auto src1_end = _cube_end(src1_begin, src1_nc);

  // 単純には答の積項数は2つの積項数の積だが
  // 相反するリテラルを含む積は数えない．
  auto dst = new_block(src1_nc);
  auto dst_begin = _cube_begin(dst.body);
  auto dst_cube = dst_begin;
  dst.cube_num = 0;
  for ( auto src1_cube = src1_begin;
	src1_cube != src1_end;
	src1_cube += _cube_size() ) {
    auto src1_p = src1_cube + blk;
    if ( (*src1_p & npat1) == 0 ) {
      cube_copy(dst_cube, src1_cube);
      auto dst_p = dst_cube + blk;
      *dst_cube |= pat1;
      dst_cube += _cube_size();
      ++ dst.cube_num;
    }
  }
  // 同じ位置にパタンが足されるだけなので
  // 順序関係は変わらない．
  return dst;
}

END_NAMESPACE_YM_SOP
