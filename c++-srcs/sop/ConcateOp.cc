
/// @file ConcateOp.cc
/// @brief ConcateOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017, 2018, 2022, 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ConcateOp.h"


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
  auto src2 = right.block();
  return concate_sub(src2);
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
  auto src2 = right.block();
  concate_int_sub(src2);
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
  auto src2 = right.block();
  return concate_sub(src2);
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
  auto src2 = right.block();
  concate_int_sub(src2);
  return *this;
}

// @brief concate の共通処理
SopCover
SopCover::concate_sub(
  const SopBlock& src2
) const
{
  ConcateOp concate{variable_num()};
  auto src1 = block();
  auto dst = concate.new_block(src1.cube_num + src2.cube_num);
  concate(dst, src1, src2);
  return concate.make_cover(dst);
}

// @brief concate_int の共通処理
void
SopCover::concate_int_sub(
  const SopBlock& src2
)
{
  ConcateOp concate{variable_num()};
  auto src1 = block();
  SizeType cap = src1.cube_num + src2.cube_num;
  if ( src1.capacity < cap ) {
    auto dst = concate.new_block(cap);
    concate(dst, src1, src2);
    delete_body();
    _set(dst);
  }
  else {
    concate(src1, src1, src2);
    _set(src1);
  }
}


//////////////////////////////////////////////////////////////////////
// クラス ConcateOp
//////////////////////////////////////////////////////////////////////

// @brief 2つのカバーの論理和を計算する．
void
ConcateOp::operator()(
  SopBlock& dst,
  const SopBlock& src1,
  const SopBlock& src2
)
{
  auto bv1 = src1.body;
  auto bv1_end = _calc_offset(bv1, src1.cube_num);

  auto bv2 = src2.body;
  auto bv2_end = _calc_offset(bv2, src2.cube_num);

  auto dst_bv = dst.body;

  SizeType nc = 0;
  while ( bv1 != bv1_end && bv2 != bv2_end ) {
    int res = cube_compare(bv1, bv2);
    if ( res > 0 ) {
      cube_copy(dst_bv, bv1);
      bv1 += _cube_size();
    }
    else if ( res < 0 ) {
      cube_copy(dst_bv, bv2);
      bv2 += _cube_size();
    }
    else {
      cube_copy(dst_bv, bv1);
      bv1 += _cube_size();
      bv2 += _cube_size();
    }
    dst_bv += _cube_size();
    ++ nc;
  }
  while ( bv1 != bv1_end ) {
    cube_copy(dst_bv, bv1);
    bv1 += _cube_size();
    dst_bv += _cube_size();
    ++ nc;
  }
  while ( bv2 != bv2_end ) {
    cube_copy(dst_bv, bv2);
    bv2 += _cube_size();
    dst_bv += _cube_size();
    ++ nc;
  }

  dst.cube_num = nc;
}

END_NAMESPACE_YM_SOP
