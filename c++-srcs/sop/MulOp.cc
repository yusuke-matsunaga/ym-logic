
/// @file MulOp.cc
/// @brief MulOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
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
  auto src1 = block();
  auto src2 = right.block();
  SizeType cap = src1.cube_num * src2.cube_num;
  auto dst = product.new_block(cap);
  product(dst, src1, src2);
  return product.make_cover(dst);
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
  auto src1 = block();
  auto src2 = right.block();
  SizeType cap = src1.cube_num * src2.cube_num;
  if ( src1.capacity < cap ) {
    auto dst = product.new_block(cap);
    product(dst, src1, src2);
    delete_body();
    _set(dst);
  }
  else {
    product(src1, src1, src2);
    _set(src1);
  }
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
  auto src1 = block();
  SizeType cap = src1.cube_num;
  auto dst = product.new_block(cap);
  product(dst, src1, right.block().body);
  return product.make_cover(dst);
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
  auto src1 = block();
  // 結果のキューブ数は増えない．
  product(src1, src1, right.block().body);
  _set(src1);
  return *this;
}

// @brief 論理積を計算する(リテラル版)．
SopCover
SopCover::operator*(
  Literal right
) const
{
  MulOp product{variable_num()};
  auto src1 = block();
  SizeType cap = src1.cube_num;
  auto dst = product.new_block(cap);
  product(dst, src1, right);
  return product.make_cover(dst);
}

// @brief 論理積を計算して代入する(リテラル版)．
SopCover&
SopCover::operator*=(
  Literal right
)
{
  MulOp product{variable_num()};
  auto src1 = block();
  // 結果のキューブ数は増えない．
  product(src1, src1, right);
  _set(src1);
  return *this;
}


//////////////////////////////////////////////////////////////////////
// クラス MulOp
//////////////////////////////////////////////////////////////////////

// @brief 2つのカバーの論理積を計算する．
void
MulOp::operator()(
  SopBlock& dst,
  const SopBlock& src1,
  const SopBlock& src2
)
{
  auto bv1 = src1.body;
  auto bv2_start = src2.body;
  SizeType cap = src1.cube_num * src2.cube_num;

  // 単純には答の積項数は2つの積項数の積だが
  // 相反するリテラルを含む積は数えない．
  SizeType nc = 0;
  auto dst_bv = dst.body;
  auto bv1_end = _calc_offset(bv1, src1.cube_num);
  auto bv2_end = _calc_offset(bv2_start, src2.cube_num);
  for ( ; bv1 != bv1_end; bv1 += _cube_size() ) {
    for ( auto bv2 = bv2_start; bv2 != bv2_end; bv2 += _cube_size() ) {
      if ( cube_product(dst_bv, bv1, bv2) ) {
	dst_bv += _cube_size();
	++ nc;
      }
    }
  }
  dst.cube_num = nc;
  sort(dst);
}

// @brief カバーとキューブの論理積を計算する．
void
MulOp::operator()(
  SopBlock& dst,
  const SopBlock& src1,
  const SopBitVect* bv2
)
{
  auto bv1 = src1.body;

  // 相反するリテラルを含む積は数えない．
  SizeType nc = 0;
  auto dst_body = dst.body;
  auto bv1_end = _calc_offset(bv1, src1.cube_num);
  for ( ; bv1 != bv1_end; bv1 += _cube_size() ) {
    if ( cube_product(_calc_offset(dst_body, nc), bv1, bv2) ) {
      ++ nc;
    }
  }
  dst.cube_num = nc;
  sort(dst);
}

// @brief カバーとリテラルとの論理積を計算する．
void
MulOp::operator()(
  SopBlock& dst,
  const SopBlock& src1,
  Literal lit
)
{
  auto var_id = lit.varid();
  auto blk = _block_pos(var_id);
  auto sft = _shift_num(var_id);
  auto pat = lit2bv(lit);
  auto npat = lit2bv(~lit);
  auto pat1 = pat << sft;
  auto npat1 = npat << sft;

  auto bv1 = src1.body;
  auto bv1_end = _calc_offset(bv1, src1.cube_num);
  // 単純には答の積項数は2つの積項数の積だが
  // 相反するリテラルを含む積は数えない．
  auto dst_body = dst.body;
  SizeType nc = 0;
  for ( ; bv1 != bv1_end; bv1 += _cube_size() ) {
    if ( (bv1[blk] & npat1) == 0 ) {
      auto dst1 = _calc_offset(dst_body, nc);
      cube_copy(dst1, bv1);
      dst1[blk] |= pat1;
      ++ nc;
    }
  }
  dst.cube_num = nc;
}

END_NAMESPACE_YM_SOP
