
/// @file ConcateOp.cc
/// @brief ConcateOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
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
  return concate_sub(right.to_block());
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
  concate_int_sub(right.to_block());
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
  return concate_sub(right.to_block());
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
  concate_int_sub(right.to_block());
  return *this;
}

// @brief concate の共通処理
SopCover
SopCover::concate_sub(
  const SopBlockRef& block2
) const
{
  ConcateOp concate{variable_num()};
  auto dst = concate(to_block(), block2);
  return concate.make_cover(std::move(dst));
}

// @brief concate_int の共通処理
void
SopCover::concate_int_sub(
  const SopBlockRef& block2
)
{
  ConcateOp concate{variable_num()};
  auto dst = concate(to_block(), block2);
  _set(std::move(dst));
}


//////////////////////////////////////////////////////////////////////
// クラス ConcateOp
//////////////////////////////////////////////////////////////////////

// @brief 2つのカバーの論理和を計算する．
SopBlock
ConcateOp::operator()(
  const SopBlockRef& block1,
  const SopBlockRef& block2
)
{
  auto dst = new_block(block1.cube_num + block2.cube_num);
  auto& dst_bv = dst.body;
  auto dst_iter = _cube_begin(dst_bv);
  dst.cube_num = 0;
  auto src1_iter = _cube_begin(block1.body);
  auto src1_end = _cube_end(src1_iter, block1.cube_num);
  auto src2_iter = _cube_begin(block2.body);
  auto src2_end = _cube_end(src2_iter, block2.cube_num);
  while ( src1_iter != src1_end && src2_iter != src2_end ) {
    int res = cube_compare(src1_iter, src2_iter);
    if ( res > 0 ) {
      cube_copy(dst_iter, src1_iter);
      src1_iter += _cube_size();
    }
    else if ( res < 0 ) {
      cube_copy(dst_iter, src2_iter);
      src2_iter += _cube_size();
    }
    else {
      cube_copy(dst_iter, src1_iter);
      src1_iter += _cube_size();
      src2_iter += _cube_size();
    }
    dst_iter += _cube_size();
    ++ dst.cube_num;
  }
  while ( src1_iter != src1_end ) {
    cube_copy(dst_iter, src1_iter);
    src1_iter += _cube_size();
    dst_iter += _cube_size();
    ++ dst.cube_num;
  }
  while ( src2_iter != src2_end ) {
    cube_copy(dst_iter, src2_iter);
    src2_iter += _cube_size();
    dst_iter += _cube_size();
    ++ dst.cube_num;
  }
  return dst;
}

END_NAMESPACE_YM_SOP
