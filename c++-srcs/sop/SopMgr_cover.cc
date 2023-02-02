
/// @file AlgMgr_cover.cc
/// @brief AlgMgr のカバー関係の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017, 2018, 2022, 2023 Yusuke Matsunaga
/// All rights reserved.

#include "SopMgr.h"
#include "SopBlock.h"
#include "ym/Literal.h"


BEGIN_NAMESPACE_YM_SOP

//////////////////////////////////////////////////////////////////////
// クラス SopMgr
//////////////////////////////////////////////////////////////////////

// @brief 否定のカバーを求める．
SopBlock
SopMgr::cover_complement(
  const SopBlock& src
)
{
  auto bv1 = src.body;
  auto nc1 = src.cube_num;

  if ( nc1 == 0 ) {
    // 結果は tautology
    return new_cover(1);
  }

  for ( SizeType c = 0; c < nc1; ++ c ) {
    if ( cube_check_null(bv1, c) ) {
      // 結果は空
      return SopBlock{0, nullptr};
    }
  }

  auto cc = common_cube(src);
  SopBlock src1{src};
  SopBlock r{0, nullptr};
  if ( !cube_check_null(cc) ) {
    r = cube_complement(cc);
    src1 = cover_quotient(src, cc);
  }

  SizeType j = binate_select(src1);

  auto f0 = cover_cofactor(src1, Literal{j, true});
  auto f0_compl = cover_complement(f0);
  delete_cover(f0);
  auto f1 = cover_cofactor(src1, Literal{j, false});
  auto f1_compl = cover_complement(f1);
  delete_cover(f1);
  if ( src1.body != src.body ) {
    delete_cover(src1);
  }
  auto merge_ans = merge_with_identity(j, f0_compl, f1_compl);
  delete_cover(f0_compl);
  delete_cover(f1_compl);
  auto ans = cover_sum(r, merge_ans);
  delete_cover(r);
  delete_cover(merge_ans);
  return ans;
}

// @brief 2つのカバーの論理和を計算する．
SopBlock
SopMgr::cover_sum(
  const SopBlock& src1,
  const SopBlock& src2
)
{
  auto bv1 = src1.body;
  auto bv1_end = _calc_offset(bv1, src1.cube_num);

  auto bv2 = src2.body;
  auto bv2_end = _calc_offset(bv2, src2.cube_num);

  SizeType cap = src1.cube_num + src2.cube_num;
  auto dst_block = new_cover(cap);
  auto dst_bv = dst_block.body;

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

  dst_block.cube_num = nc;
  return dst_block;
}

// @brief 2つのカバーの差分を計算する．
void
SopMgr::_cover_diff_sub(
  SopBlock& dst,
  const SopBlock& src1,
  const SopBlock& src2
)
{
  auto bv1 = src1.body;
  auto bv1_end = _calc_offset(bv1, src1.cube_num);
  auto bv2 = src2.body;
  auto bv2_end = _calc_offset(bv2, src2.cube_num);
  auto dst_body = dst.body;
  SizeType nc = 0;
  while ( bv1 != bv1_end && bv2 != bv2_end ) {
    int res = cube_compare(bv1, bv2);
    if ( res > 0 ) {
      cube_copy(_calc_offset(dst_body, nc), bv1);
      bv1 += _cube_size();
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
    cube_copy(_calc_offset(dst_body, nc), bv1);
    bv1 += _cube_size();
    ++ nc;
  }
  dst.cube_num = nc;
}

// @brief 2つのカバーの論理積を計算する．
SopBlock
SopMgr::cover_product(
  const SopBlock& src1,
  const SopBlock& src2
)
{
  auto bv1 = src1.body;
  auto bv2_start = src2.body;
  SizeType cap = src1.cube_num * src2.cube_num;
  auto dst_block = new_cover(cap);

  // 単純には答の積項数は2つの積項数の積だが
  // 相反するリテラルを含む積は数えない．
  SizeType nc = 0;
  auto dst_body = dst_block.body;
  auto bv1_end = _calc_offset(bv1, src1.cube_num);
  auto bv2_end = _calc_offset(bv2_start, src2.cube_num);
  for ( ; bv1 != bv1_end; bv1 += _cube_size() ) {
    for ( auto bv2 = bv2_start; bv2 != bv2_end; bv2 += _cube_size() ) {
      if ( cube_product(_calc_offset(dst_body, nc), bv1, bv2) ) {
	++ nc;
      }
    }
  }
  _sort(dst_body, 0, nc);
  dst_block.cube_num = nc;
  return dst_block;
}

// @brief カバーとキューブの論理積を計算する．
void
SopMgr::_cover_product_sub(
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
  _sort(dst_body, 0, nc);
  dst.cube_num = nc;
}

// @brief カバーとリテラルとの論理積を計算する．
void
SopMgr::_cover_product_sub(
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

// @brief カバーの代数的除算を行う．
SopBlock
SopMgr::cover_quotient(
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
      if ( cube_quotient(_calc_offset(mTmpBuff, i), tmp1, tmp2) ) {
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

  SizeType n = pos_list.size();
  auto dst_block = new_cover(n);
  auto dst_body = dst_block.body;
  for ( SizeType i = 0; i < n; ++ i ) {
    SizeType pos = pos_list[i];
    auto tmp = _calc_offset(mTmpBuff, pos);
    cube_copy(_calc_offset(dst_body, i), tmp);
  }
  return dst_block;
}

// @brief カバーをキューブで割る．
void
SopMgr::_cover_quotient_sub(
  SopBlock& dst,
  const SopBlock& src1,
  const SopBitVect* bv2
)
{
  SizeType nc1 = src1.cube_num;
  auto bv1 = src1.body;
  auto dst_body = dst.body;
  SizeType nc = 0;
  for ( SizeType i = 0; i < nc1; ++ i ) {
    auto tmp1 = _calc_offset(bv1, i);
    if ( cube_quotient(_calc_offset(dst_body, nc), tmp1, bv2) ) {
      ++ nc;
    }
  }
  dst.cube_num = nc;
}

// @brief カバーをリテラルで割る．
void
SopMgr::_cover_quotient_sub(
  SopBlock& dst,
  const SopBlock& src1,
  Literal lit
)
{
  auto var_id = lit.varid();
  auto blk = _block_pos(var_id);
  auto sft = _shift_num(var_id);
  auto pat = lit2bv(lit);
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

// @brief コファクターを求める．
SopBlock
SopMgr::cover_cofactor(
  const SopBlock& src1,
  Literal lit
)
{
  auto var_id = lit.varid();
  auto blk = _block_pos(var_id);
  auto sft = _shift_num(var_id);
  //auto pat = lit2bv(lit);
  auto npat = lit2bv(~lit);
  //auto pat1 = pat << sft;
  auto npat1 = npat << sft;
  auto mask = 3UL << sft;
  auto nmask = ~mask;
  auto nb = _cube_size();
  auto bv1 = src1.body;
  auto bv1_end = _calc_offset(bv1, src1.cube_num);
  auto dst_block = new_cover(src1.cube_num);
  auto dst_body = dst_block.body;
  SizeType nc = 0;
  for ( ; bv1 != bv1_end; bv1 += nb ) {
    if ( (bv1[blk] & npat1) == 0UL ) {
      auto dst1 = _calc_offset(dst_body, nc);
      cube_copy(dst1, bv1);
      dst1[blk] &= nmask;
      ++ nc;
    }
  }
  dst_block.cube_num = nc;
  return dst_block;
}

// @brief カバー中のすべてのキューブの共通部分を求める．
SopBitVect*
SopMgr::common_cube(
  const SopBlock& src1
)
{
  auto bv1 = src1.body;
  auto bv1_end = _calc_offset(bv1, src1.cube_num);

  auto dst_body = new_cube();

  // 最初のキューブをコピーする．
  cube_copy(dst_body, bv1);

  // 2番目以降のキューブとの共通部分を求める．
  bv1 += _cube_size();
  while ( bv1 != bv1_end ) {
    SopBitVect tmp = 0ULL;
    auto dst_bv1 = dst_body;
    auto bv1_end1 = _calc_offset(bv1, 1);
    for ( ; bv1 != bv1_end1; ++ dst_bv1, ++ bv1) {
      *dst_bv1 &= *bv1;
      tmp |= *dst_bv1;
    }
    if ( tmp == 0ULL ) {
      // 空になった．
      break;
    }
  }
  return dst_body;
}

// @brief マージソートを行う下請け関数
void
SopMgr::_sort(
  SopBitVect* bv,
  SizeType start,
  SizeType end
)
{
  SizeType n = end - start;
  if ( n <= 1 ) {
    return;
  }
  if ( n == 2 ) {
    // (0, 1) と (1, 0) の2通りだけ
    auto bv0 = _calc_offset(bv, start + 0);
    auto bv1 = _calc_offset(bv, start + 1);
    if ( cube_compare(bv0, bv1) < 0 ) {
      // (1, 0) だったので交換する．
      _resize_buff(1);
      cube_swap(bv0, bv1);
    }
    return;
  }
  if ( n == 3 ) {
    // (0, 1, 2), (0, 2, 1), (1, 0, 2), (1, 2, 0), (2, 0, 1), (2, 1, 0)
    // の6通りなので虱潰し
    auto bv0 = _calc_offset(bv, start + 0);
    auto bv1 = _calc_offset(bv, start + 1);
    auto bv2 = _calc_offset(bv, start + 2);
    if ( cube_compare(bv0, bv1) < 0 ) {
      // (1, 0, 2), (1, 2, 0), (2, 1, 0)
      if ( cube_compare(bv0, bv2) < 0 ) {
	// (1, 2, 0), (2, 1, 0)
	if ( cube_compare(bv1, bv2) < 0 ) {
	  // (2, 1, 0)
	  // 0 と 2 を交換
	  _resize_buff(1);
	  cube_swap(bv0, bv2);
	}
	else {
	  // (1, 2, 0)
	  // 0 <- 1, 1 <- 2, 2 <- 0
	  _resize_buff(1);
	  cube_rotate3(bv0, bv1, bv2);
	}
      }
      else {
	// (1, 0, 2)
	// 0 <-> 1
	_resize_buff(1);
	cube_swap(bv0, bv1);
      }
    }
    else {
      // (0, 1, 2), (0, 2, 1), (2, 0, 1)
      if ( cube_compare(bv0, bv2) < 0 ) {
	// (2, 0, 1)
	// 0 <- 2, 2 <- 1, 1 <- 0
	_resize_buff(1);
	cube_rotate3(bv0, bv2, bv1);
      }
      else {
	// (0, 1, 2), (0, 2, 1)
	if ( cube_compare(bv1, bv2) < 0 ) {
	  // (0, 2, 1)
	  // 1 <-> 2
	  _resize_buff(1);
	  cube_swap(bv1, bv2);
	}
	else {
	  // (0, 1, 2)
	  // そのまま
	}
      }
    }
    return;
  }
  if ( n == 4 ) {
    auto bv0 = _calc_offset(bv, start + 0);
    auto bv1 = _calc_offset(bv, start + 1);
    auto bv2 = _calc_offset(bv, start + 2);
    auto bv3 = _calc_offset(bv, start + 3);
    _resize_buff(1);
    // 0 と 1 を整列
    if ( cube_compare(bv0, bv1) < 0 ) {
      cube_swap(bv0, bv1);
    }
    // 2 と 3 を整列
    if ( cube_compare(bv2, bv3) < 0 ) {
      cube_swap(bv2, bv3);
    }
    // 0 と 2 を比較
    if ( cube_compare(bv0, bv2) < 0 ) {
      if ( cube_compare(bv0, bv3) < 0 ) {
	// (0, 1) と (2, 3) を交換
	cube_swap(bv0, bv2);
	cube_swap(bv1, bv3);
      }
      else if ( cube_compare(bv1, bv3) < 0 ) {
	// 0 <- 2, 2 <- 3, 3 <- 1, 1 <- 0
	cube_rotate4(bv0, bv2, bv3, bv1);
      }
      else {
	// 0 <- 2, 2 <- 1, 1 <- 0
	cube_rotate3(bv0, bv2, bv1);
      }
    }
    else if ( cube_compare(bv1, bv2) < 0 ) {
      if ( cube_compare(bv1, bv3) < 0 ) {
	// 1 <- 2, 2 <- 3, 3 <- 1
	cube_rotate3(bv1, bv2, bv3);
      }
      else {
	// 1 <- 2, 2 <- 1
	cube_swap(bv1, bv2);
      }
    }
    else {
      // そのまま
    }
    return;
  }

  // 半分に分割してそれぞれソートする．
  SizeType hn = (n + 1) / 2;
  SizeType start1 = start;
  SizeType end1 = start + hn;
  SizeType start2 = end1;
  SizeType end2 = end;
  _sort(bv, start1, end1);
  _sort(bv, start2, end2);

  // trivial case
  // 前半部分の末尾が後半部分の先頭より大きければ
  // すでに整列している．
  auto bv_end1 = _calc_offset(bv, (end1 - 1));
  auto bv_start2 = _calc_offset(bv, start2);
  if ( cube_compare(bv_end1, bv_start2) > 0 ) {
    return;
  }

  // マージする．
  // 前半部分を一旦 mTmpBuff にコピーする．
  _resize_buff(hn);
  _copy(mTmpBuff, _calc_offset(bv,  start1), hn);
  auto bv1 = mTmpBuff;
  auto bv1_end = _calc_offset(mTmpBuff, hn);
  auto bv2 = _calc_offset(bv, start2);
  auto bv2_end = _calc_offset(bv, end2);
  auto dst_bv = _calc_offset(bv, start);
  while ( bv1 != bv1_end && bv2 != bv2_end ) {
    int comp_res = cube_compare(bv1, bv2);
    if ( comp_res > 0 ) {
      cube_copy(dst_bv, bv1);
      bv1 += _cube_size();
      dst_bv += _cube_size();
    }
    else if ( comp_res < 0 ) {
      cube_copy(dst_bv, bv2);
      bv2 += _cube_size();
      dst_bv += _cube_size();
    }
    else {
      // 重複したキューブはエラー
      ASSERT_NOT_REACHED;
    }
  }
  while ( bv1 != bv1_end ) {
    cube_copy(dst_bv, bv1);
    bv1 += _cube_size();
    dst_bv += _cube_size();
  }
  // 後半部分が残っている時はそのままでいいはず．
  ASSERT_COND( bv2 == dst_bv );
}

// @brief カバー(を表すビットベクタ)の比較を行う．
int
SopMgr::cover_compare(
  const SopBlock& src1,
  const SopBlock& src2
)
{
  auto bv1 = src1.body;
  auto bv1_end = _calc_offset(bv1, src1.cube_num);
  auto bv2 = src2.body;
  auto bv2_end = _calc_offset(bv2, src2.cube_num);
  for ( ; bv1 != bv1_end && bv2 != bv2_end; bv1 += _cube_size(), bv2 += _cube_size() ) {
    int res = cube_compare(bv1, bv2);
    if ( res != 0 ) {
      return res;
    }
  }
  if ( bv1 != bv1_end ) {
    // bv2 == bv2_end なので bv1 が大きい
    return 1;
  }
  if ( bv2 != bv2_end ) {
    // bv1 == bv1_end なので bv2 が大きい
    return -1;
  }
  // 等しい
  return 0;
}

END_NAMESPACE_YM_SOP
