
/// @file DivOp.cc
/// @brief DivOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
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
  auto src1 = block();
  auto src2 = right.block();
  SizeType cap = src1.cube_num;
  auto dst = divide.new_block(cap);
  divide(dst, src1, src2);
  return divide.make_cover(dst);
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
  auto src1 = block();
  auto src2 = right.block();
  divide(src1, src1, src2);
  _set(src1);
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
  auto src1 = block();
  auto src2 = right.block();
  SizeType cap = src1.cube_num;
  auto dst = divide.new_block(cap);
  divide(dst, src1, right.block().body);
  return divide.make_cover(dst);
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
  auto src1 = block();
  auto src2 = right.block();
  divide(src1, src1, right.block().body);
  _set(src1);
  return *this;
}

// @brief リテラルによる商を計算する．
SopCover
SopCover::operator/(
  Literal lit
) const
{
  DivOp divide{variable_num()};
  auto src1 = block();
  SizeType cap = src1.cube_num;
  auto dst = divide.new_block(cap);
  divide(dst, src1, lit);
  return divide.make_cover(dst);
}

// @brief リテラルによる商を計算して代入する．
SopCover&
SopCover::operator/=(
  Literal lit
)
{
  DivOp divide{variable_num()};
  auto src1 = block();
  divide(src1, src1, lit);
  _set(src1);
  return *this;
}


//////////////////////////////////////////////////////////////////////
// クラス DivOp
//////////////////////////////////////////////////////////////////////

// @brief カバーの代数的除算を行う．
void
DivOp::operator()(
  SopBlock& dst,
  const SopBlock& src1,
  const SopBlock& src2
)
{
  SizeType nc1 = src1.cube_num;
  SizeType nc2 = src2.cube_num;

  // 作業領域のビットベクタを確保する．
  _resize_buff(nc1);

  auto bv1 = src1.body;
  auto bv2 = src2.body;

  // bv1 の各キューブは高々1つのキューブでしか割ることはできない．
  // ただし，除数も被除数も algebraic expression の場合
  // ので bv1 の各キューブについて bv2 の各キューブで割ってみて
  // 成功した場合，その商を記録する．
  vector<bool> mark(nc1, false);
  for ( SizeType i = 0; i < nc1; ++ i ) {
    auto tmp1 = _calc_offset(bv1, i);
    for ( SizeType j = 0; j < nc2; ++ j ) {
      auto tmp2 = _calc_offset(bv2, j);
      auto dst1 = _calc_offset(mTmpBuff, i);
      if ( cube_quotient(dst1, tmp1, tmp2) ) {
	mark[i] = true;
	break;
      }
    }
  }

  // 商のキューブは tmp 中に nc2 回現れているはず．
  vector<SizeType> pos_list;
  pos_list.reserve(nc1);
  for ( SizeType i = 0; i < nc1; ++ i ) {
    if ( !mark[i] ) {
      // 対象ではない．
      continue;
    }
    auto tmp1 = _calc_offset(mTmpBuff, i);

    SizeType c = 1;
    vector<SizeType> tmp_list;
    for ( SizeType i2 = i + 1; i2 < nc1; ++ i2 ) {
      auto tmp2 = _calc_offset(mTmpBuff, i2);
      if ( mark[i2] && cube_compare(tmp1, tmp2) == 0 ) {
	++ c;
	// i 番目のキューブと等しかったキューブ位置を記録する．
	tmp_list.push_back(i2);
      }
    }
    if ( c == nc2 ) {
      // 見つけた
      pos_list.push_back(i);
      // tmp_list に含まれるキューブはもう調べなくて良い．
      for ( int pos: tmp_list ) {
	mark[pos] = false;
      }
    }
  }

  auto dst_bv = dst.body;
  for ( SizeType pos: pos_list ) {
    auto tmp = _calc_offset(mTmpBuff, pos);
    cube_copy(dst_bv, tmp);
    dst_bv += _cube_size();
  }
  dst.cube_num = pos_list.size();
}

// @brief カバーをキューブで割る．
void
DivOp::operator()(
  SopBlock& dst,
  const SopBlock& src1,
  const SopBitVect* bv2
)
{
  SizeType nc1 = src1.cube_num;
  auto bv1 = src1.body;
  auto bv1_end = _calc_offset(bv1, nc1);
  auto dst_bv = dst.body;
  auto nb = _cube_size();
  SizeType nc = 0;
  for ( ; bv1 != bv1_end; bv1 += _cube_size() ) {
    if ( cube_quotient(dst_bv, bv1, bv2) ) {
      dst_bv += nb;
      ++ nc;
    }
  }
  dst.cube_num = nc;
}

// @brief カバーをリテラルで割る．
void
DivOp::operator()(
  SopBlock& dst,
  const SopBlock& src1,
  Literal lit
)
{
  auto varid = lit.varid();
  auto inv = lit.is_negative();
  auto blk = _block_pos(varid);
  auto sft = _shift_num(varid);
  auto pat = bitvect(inv);
  auto pat1 = pat << sft;
  auto mask = 3UL << sft;
  auto nmask = ~mask;
  auto nb = _cube_size();
  auto bv1 = src1.body;
  auto bv1_end = _calc_offset(bv1, src1.cube_num);
  auto dst_body = dst.body;
  SizeType nc = 0;
  for ( ; bv1 != bv1_end; bv1 += nb ) {
    if ( (bv1[blk] & mask) == pat1 ) {
      auto dst1 = _calc_offset(dst_body, nc);
      cube_copy(dst1, bv1);
      dst1[blk] &= nmask;
      ++ nc;
    }
  }
  dst.cube_num = nc;
}

END_NAMESPACE_YM_SOP
