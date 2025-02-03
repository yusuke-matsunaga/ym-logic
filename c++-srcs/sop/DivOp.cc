
/// @file DivOp.cc
/// @brief DivOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "DivOp.h"


BEGIN_NAMESPACE_YM_SOP

//////////////////////////////////////////////////////////////////////
// クラス SopCover
//////////////////////////////////////////////////////////////////////

// @brief algebraic division を計算する．
SopCover
SopCover::operator/(
  const SopCover& right
) const
{
  if ( variable_num() != right.variable_num() ) {
    throw std::invalid_argument("variable_num() is different from each other");
  }

  DivOp divide{variable_num()};
  auto dst = divide(to_block(), right.to_block());
  return divide.make_cover(std::move(dst));
}

// @brief algebraic division を行って代入する．
SopCover&
SopCover::operator/=(
  const SopCover& right
)
{
  if ( variable_num() != right.variable_num() ) {
    throw std::invalid_argument("variable_num() is different from each other");
  }

  DivOp divide{variable_num()};
  auto dst = divide(to_block(), right.to_block());
  _set(std::move(dst));
  return *this;
}

// @brief キューブによる商を計算する．
SopCover
SopCover::operator/(
  const SopCube& right
) const
{
  if ( variable_num() != right.variable_num() ) {
    throw std::invalid_argument("variable_num() is different from each other");
  }

  DivOp divide{variable_num()};
  auto dst = divide(to_block(), right.body());
  return divide.make_cover(std::move(dst));
}

// @brief キューブによる商を計算する．
SopCover&
SopCover::operator/=(
  const SopCube& right
)
{
  if ( variable_num() != right.variable_num() ) {
    throw std::invalid_argument("variable_num() is different from each other");
  }

  DivOp divide{variable_num()};
  auto dst = divide(to_block(), right.body());
  _set(std::move(dst));
  return *this;
}

// @brief リテラルによる商を計算する．
SopCover
SopCover::operator/(
  Literal lit
) const
{
  DivOp divide{variable_num()};
  auto dst = divide(to_block(), lit);
  return divide.make_cover(std::move(dst));
}

// @brief リテラルによる商を計算して代入する．
SopCover&
SopCover::operator/=(
  Literal lit
)
{
  DivOp divide{variable_num()};
  auto dst = divide(to_block(), lit);
  _set(std::move(dst));
  return *this;
}


//////////////////////////////////////////////////////////////////////
// クラス DivOp
//////////////////////////////////////////////////////////////////////

// @brief カバーの代数的除算を行う．
SopBlock
DivOp::operator()(
  const SopBlockRef& block1,
  const SopBlockRef& block2
)
{
  auto& src1_bv = block1.body;
  SizeType src1_nc = block1.cube_num;
  auto& src2_bv = block2.body;
  SizeType src2_nc = block2.cube_num;

  // 作業領域のビットベクタを確保する．
  _resize_buff(src1_nc);

  // block1 の各キューブは高々1つのキューブでしか割ることはできない．
  // ただし，除数も被除数も algebraic expression の場合
  // ので bv1 の各キューブについて bv2 の各キューブで割ってみて
  // 成功した場合，その商を記録する．
  vector<bool> mark(src1_nc, false);
  for ( SizeType i = 0; i < src1_nc; ++ i ) {
    auto src1_cube = _cube_begin(src1_bv, i);
    auto dst_cube = _cube_begin(mTmpBlock.body, i);
    for ( SizeType j = 0; j < src2_nc; ++ j ) {
      auto src2_cube = _cube_begin(src2_bv, j);
      if ( cube_quotient(dst_cube, src1_cube, src2_cube) ) {
	mark[i] = true;
	break;
      }
    }
  }

  // 商のキューブは mTmpBlock 中に block2.cube_num 回現れているはず．
  vector<SizeType> pos_list;
  pos_list.reserve(src1_nc);
  for ( SizeType i = 0; i < src1_nc; ++ i ) {
    if ( !mark[i] ) {
      // 対象ではない．
      continue;
    }
    SizeType c = 1;
    vector<SizeType> tmp_list;
    auto tmp1 = _cube_begin(mTmpBlock.body, i);
    for ( SizeType i2 = i + 1; i2 < src1_nc; ++ i2 ) {
      auto tmp2 = _cube_begin(mTmpBlock.body, i2);
      if ( mark[i2] && cube_compare(tmp1, tmp2) == 0 ) {
	++ c;
	// i 番目のキューブと等しかったキューブ位置を記録する．
	tmp_list.push_back(i2);
      }
    }
    if ( c == src2_nc ) {
      // 見つけた
      pos_list.push_back(i);
      // tmp_list に含まれるキューブはもう調べなくて良い．
      for ( int pos: tmp_list ) {
	mark[pos] = false;
      }
    }
  }

  SizeType nc = pos_list.size();
  auto dst = new_block(nc);
  dst.cube_num = nc;
  auto dst_cube = _cube_begin(dst.body);
  for ( auto pos: pos_list ) {
    auto src_cube = _cube_begin(mTmpBlock.body, pos);
    cube_copy(dst_cube, src_cube);
    dst_cube += _cube_size();
  }
  return dst;
}

// @brief カバーをキューブで割る．
SopBlock
DivOp::operator()(
  const SopBlockRef& block1,
  const SopBitVect& bv2
)
{
  auto& src1_bv = block1.body;
  SizeType src1_nc = block1.cube_num;
  auto dst = new_block(src1_nc);
  auto dst_cube = _cube_begin(dst.body);
  dst.cube_num = 0;
  auto src1_cube = _cube_begin(src1_bv);
  auto src1_end = _cube_end(src1_cube, src1_nc);
  auto src2_cube = _cube_begin(bv2);
  for ( ; src1_cube != src1_end; src1_cube += _cube_size() ) {
    if ( cube_quotient(dst_cube, src1_cube, src2_cube) ) {
      dst_cube += _cube_size();
      ++ dst.cube_num;
    }
  }
  return dst;
}

// @brief カバーをリテラルで割る．
SopBlock
DivOp::operator()(
  const SopBlockRef& block1,
  Literal lit
)
{
  auto varid = lit.varid();
  auto inv = lit.is_negative();
  auto blk = _block_pos(varid);
  auto sft = _shift_num(varid);
  auto pat1 = get_mask(varid, inv);
  auto mask = 3UL << sft;
  auto nmask = ~mask;
  SizeType src1_nc = block1.cube_num;
  auto dst = new_block(src1_nc);
  auto dst_cube = _cube_begin(dst.body);
  dst.cube_num = 0;
  auto src1_cube = _cube_begin(block1.body);
  auto src1_end = _cube_end(src1_cube, src1_nc);
  for ( ; src1_cube != src1_end; src1_cube += _cube_size() ) {
    auto src1_p = src1_cube + blk;
    if ( (*src1_p & mask) == pat1 ) {
      cube_copy(dst_cube, src1_cube);
      auto dst_p = dst_cube + blk;
      *dst_p &= nmask;
      dst_cube += _cube_size();
      ++ dst.cube_num;
    }
  }
  return dst;
}

END_NAMESPACE_YM_SOP
