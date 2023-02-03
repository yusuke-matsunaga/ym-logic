
/// @file DiffOp.cc
/// @brief DiffOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017, 2018, 2022, 2023 Yusuke Matsunaga
/// All rights reserved.

#include "DiffOp.h"


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
  auto src2 = right.block();
  return diff_sub(src2);
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
  auto src2 = right.block();
  diff_int_sub(src2);
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
  auto src2 = right.block();
  return diff_sub(src2);
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
  auto src2 = right.block();
  diff_int_sub(src2);
  return *this;
}

// @brief diff の共通処理
SopCover
SopCover::diff_sub(
  const SopBlock& src2
) const
{
  DiffOp diff{variable_num()};
  auto src1 = block();
  auto dst = diff.new_block(src1.cube_num + src2.cube_num);
  diff(dst, src1, src2);
  return diff.make_cover(dst);
}

// @brief diff_int の共通処理
void
SopCover::diff_int_sub(
  const SopBlock& src2
)
{
  DiffOp diff{variable_num()};
  auto src1 = block();
  // 結果のキューブ数は増えない．
  diff(src1, src1, src2);
  _set(src1);
}


//////////////////////////////////////////////////////////////////////
// クラス DiffOp
//////////////////////////////////////////////////////////////////////

// @brief 2つのカバーの差分を計算する．
void
DiffOp::operator()(
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
      dst_bv += _cube_size();
      ++ nc;
    }
    else if ( res < 0 ) {
      bv2 += _cube_size();
    }
    else {
      bv1 += _cube_size();
      bv2 += _cube_size();
    }
  }
  while ( bv1 != bv1_end ) {
    cube_copy(dst_bv, bv1);
    bv1 += _cube_size();
    dst_bv += _cube_size();
    ++ nc;
  }
  dst.cube_num = nc;
}

END_NAMESPACE_YM_SOP
