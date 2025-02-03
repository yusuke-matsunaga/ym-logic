
/// @file DiffOp.cc
/// @brief DiffOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
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
  return diff_sub(right.to_block());
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
  diff_int_sub(right.to_block());
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
  return diff_sub(right.to_block());
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
  diff_int_sub(right.to_block());
  return *this;
}

// @brief diff の共通処理
SopCover
SopCover::diff_sub(
  const SopBlockRef& block2
) const
{
  DiffOp diff{variable_num()};
  auto dst = diff(to_block(), block2);
  return diff.make_cover(std::move(dst));
}

// @brief diff_int の共通処理
void
SopCover::diff_int_sub(
  const SopBlockRef& block2
)
{
  DiffOp diff{variable_num()};
  auto dst = diff(to_block(), block2);
  _set(std::move(dst));
}


//////////////////////////////////////////////////////////////////////
// クラス DiffOp
//////////////////////////////////////////////////////////////////////

// @brief 2つのカバーの差分を計算する．
SopBlock
DiffOp::operator()(
  const SopBlockRef& block1,
  const SopBlockRef& block2
)
{
  // 結果のキューブ数は block1 のキューブ数を超えない．
  auto dst = new_block(block1.cube_num);
  auto& src1_bv = block1.body;
  auto src1_cube = _cube_begin(src1_bv);
  auto src1_end = _cube_end(src1_cube, block1.cube_num);
  auto& src2_bv = block1.body;
  auto src2_cube = _cube_begin(src2_bv);
  auto src2_end = _cube_end(src2_cube, block2.cube_num);
  auto& dst_bv = dst.body;
  auto dst_cube = _cube_begin(dst_bv);
  dst.cube_num = 0;
  while ( src1_cube != src1_end && src2_cube != src2_end ) {
    int res = cube_compare(src1_cube, src2_cube);
    if ( res > 0 ) {
      // src1_cube のみ存在する．
      // そのまま結果に含める．
      cube_copy(dst_cube, src1_cube);
      src1_cube += _cube_size();
      dst_cube += _cube_size();
      ++ dst.cube_num;
    }
    else if ( res < 0 ) {
      // src2_cube のみ存在する．
      src2_cube += _cube_size();
    }
    else {
      // src1_cube == src2_cube だった．
      // つまり結果には含めない．
      src1_cube += _cube_size();
      src2_cube += _cube_size();
    }
  }
  while ( src1_cube != src1_end ) {
    cube_copy(dst_cube, src1_cube);
    src1_cube += _cube_size();
    dst_cube += _cube_size();
    ++ dst.cube_num;
  }

  return dst;
}

END_NAMESPACE_YM_SOP
