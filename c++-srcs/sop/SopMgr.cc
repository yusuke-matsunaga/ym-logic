
/// @file AlgMgr.cc
/// @brief AlgMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017, 2018, 2022, 2023 Yusuke Matsunaga
/// All rights reserved.

#include "SopMgr.h"
#include "SopBlock.h"
#include "ym/SopCube.h"
#include "ym/Literal.h"


BEGIN_NAMESPACE_YM_LOGIC

BEGIN_NONAMESPACE

inline
SopBitVect
lit2bv(
  Literal lit
)
{
  return static_cast<SopBitVect>(lit.is_positive() ? SopPat::_1 : SopPat::_0);
}

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス SopMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SopMgr::SopMgr(
  SizeType variable_num
) : mVarNum{variable_num}
{
  _init_buff();
}

// @brief デストラクタ
SopMgr::~SopMgr()
{
  delete_body(mTmpBuff, mTmpBuffSize);
}

BEGIN_NONAMESPACE

// 表引きを使ってリテラル数の計算を行う．
int
_count(
  SopBitVect pat
)
{
  int table[] = {
    // utils/gen_bitcount_tbl.py で生成
    0,  1,  1,  0,  1,  2,  2,  0,  1,  2,  2,  0,  0,  0,  0,  0,
    1,  2,  2,  0,  2,  3,  3,  0,  2,  3,  3,  0,  0,  0,  0,  0,
    1,  2,  2,  0,  2,  3,  3,  0,  2,  3,  3,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    1,  2,  2,  0,  2,  3,  3,  0,  2,  3,  3,  0,  0,  0,  0,  0,
    2,  3,  3,  0,  3,  4,  4,  0,  3,  4,  4,  0,  0,  0,  0,  0,
    2,  3,  3,  0,  3,  4,  4,  0,  3,  4,  4,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    1,  2,  2,  0,  2,  3,  3,  0,  2,  3,  3,  0,  0,  0,  0,  0,
    2,  3,  3,  0,  3,  4,  4,  0,  3,  4,  4,  0,  0,  0,  0,  0,
    2,  3,  3,  0,  3,  4,  4,  0,  3,  4,  4,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,

  };

  return table[pat];
}

END_NONAMESPACE

// @brief ビットベクタ上のリテラル数を数える．
SizeType
SopMgr::literal_num(
  const SopBlock& src
)
{
  // 8 ビットごとに区切って表引きで計算する．
  auto bv = src.bitvect();
  auto bv_end = _calc_offset(bv, src.cube_num());
  SizeType ans = 0;
  for ( ; bv != bv_end; ++ bv ) {
    auto pat = *bv;
    auto p1 = pat & 0xFFUL;
    ans += _count(p1);
    auto p2 = (pat >> 8) & 0xFFUL;
    ans += _count(p2);
    auto p3 = (pat >> 16) & 0xFFUL;
    ans += _count(p3);
    auto p4 = (pat >> 24) & 0xFFUL;
    ans += _count(p4);
    auto p5 = (pat >> 32) & 0xFFUL;
    ans += _count(p5);
    auto p6 = (pat >> 40) & 0xFFUL;
    ans += _count(p6);
    auto p7 = (pat >> 48) & 0xFFUL;
    ans += _count(p7);
    auto p8 = (pat >> 56);
    ans += _count(p8);
  }
  return ans;
}

// @brief ビットベクタ上の特定のリテラルの出現頻度を数える．
SizeType
SopMgr::literal_num(
  const SopBlock& src,
  Literal lit
)
{
  auto var_id = lit.varid();
  auto blk = _block_pos(var_id);
  auto sft = _shift_num(var_id);
  auto pat = lit2bv(lit);
  auto mask = pat << sft;
  auto bv = src.bitvect();
  auto bv_end = _calc_offset(bv, src.cube_num());
  SizeType ans = 0;
  for ( ; bv != bv_end; bv += _cube_size() ) {
    if ( (bv[blk] & mask) == mask ) {
      ++ ans;
    }
  }
  return ans;
}

// @brief キューブ/カバー用の領域を確保する．
SopBitVect*
SopMgr::new_body(
  SizeType cube_num
)
{
  auto size = _cube_size() * cube_num;
  auto body = new SopBitVect[size];
  for ( SizeType i = 0; i < size; ++ i ) {
    body[i] = 0ULL;
  }
  return body;
}

// @brief キューブ/カバー用の領域を削除する．
void
SopMgr::delete_body(
  SopBitVect* p,
  SizeType cube_num
)
{
  delete [] p;
}

// @brief 2つのカバーの論理和を計算する．
SizeType
SopMgr::cover_sum(
  SopBitVect* dst_bv,
  const SopBlock& src1,
  const SopBlock& src2
)
{
  auto bv1 = src1.bitvect();
  ASSERT_COND ( dst_bv != bv1 );
  auto bv1_end = _calc_offset(bv1, src1.cube_num());

  auto bv2 = src2.bitvect();
  ASSERT_COND ( dst_bv != bv2 );
  auto bv2_end = _calc_offset(bv2, src2.cube_num());

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

  return nc;
}

// @brief 2つのカバーの差分を計算する．
SizeType
SopMgr::cover_diff(
  SopBitVect* dst_bv,
  const SopBlock& src1,
  const SopBlock& src2
)
{
  auto bv1 = src1.bitvect();
  auto bv1_end = _calc_offset(bv1, src1.cube_num());
  auto bv2 = src2.bitvect();
  ASSERT_COND ( dst_bv != bv2 );
  auto bv2_end = _calc_offset(bv2, src2.cube_num());

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
  return nc;
}

// @brief 2つのカバーの論理積を計算する．
SizeType
SopMgr::cover_product(
  SopBitVect* dst_bv,
  const SopBlock& src1,
  const SopBlock& src2
)
{
  auto bv1 = src1.bitvect();
  ASSERT_COND( dst_bv != bv1 || src2.cube_num() == 1 );
  auto bv2_start = src2.bitvect();
  ASSERT_COND( dst_bv != bv2_start );

  // 単純には答の積項数は2つの積項数の積だが
  // 相反するリテラルを含む積は数えない．
  SizeType nc = 0;
  auto dst_bv0 = dst_bv;
  auto bv1_end = _calc_offset(bv1, src1.cube_num());
  auto bv2_end = _calc_offset(bv2_start, src2.cube_num());
  for ( ; bv1 != bv1_end; bv1 += _cube_size() ) {
    for ( auto bv2 = bv2_start; bv2 != bv2_end; bv2 += _cube_size() ) {
      if ( cube_product(dst_bv, bv1, bv2) ) {
	dst_bv += _cube_size();
	++ nc;
      }
    }
  }
  _sort(dst_bv0, 0, nc);
  return nc;
}

// @brief カバーとリテラルとの論理積を計算する．
SizeType
SopMgr::cover_product(
  SopBitVect* dst_bv,
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

  auto bv1 = src1.bitvect();
  auto bv1_end = _calc_offset(bv1, src1.cube_num());
  // 単純には答の積項数は2つの積項数の積だが
  // 相反するリテラルを含む積は数えない．
  SizeType nc = 0;
  for ( ; bv1 != bv1_end; bv1 += _cube_size() ) {
    auto tmp = bv1[blk] | pat1;
    if ( (tmp & mask) == mask ) {
      // 相反するリテラルがあった．
      continue;
    }
    cube_copy(dst_bv, bv1);
    dst_bv[blk] = tmp;
    dst_bv += _cube_size();
    ++ nc;
  }

  return nc;
}

// @brief カバーの代数的除算を行う．
SizeType
SopMgr::cover_quotient(
  SopBitVect* dst_bv,
  const SopBlock& src1,
  const SopBlock& src2
)
{
  SizeType nc1 = src1.cube_num();
  SizeType nc2 = src2.cube_num();

  // 作業領域のビットベクタを確保する．
  _resize_buff(nc1);

  auto bv1 = src1.bitvect();
  auto bv2 = src2.bitvect();

  // bv1 の各キューブは高々1つのキューブでしか割ることはできない．
  // ただし，除数も被除数も algebraic expression の場合
  // ので bv1 の各キューブについて bv2 の各キューブで割ってみて
  // 成功した場合，その商を記録する．
  vector<bool> mark(nc1, false);
  for ( SizeType i = 0; i < nc1; ++ i ) {
    for ( SizeType j = 0; j < nc2; ++ j ) {
      if ( cube_quotient(mTmpBuff, i, bv1, i, bv2, j) ) {
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

  for ( SizeType pos: pos_list ) {
    auto tmp = _calc_offset(mTmpBuff, pos);
    cube_copy(dst_bv, tmp);
    dst_bv += _cube_size();
  }

  return pos_list.size();
}

// @brief カバーをリテラルで割る．
SizeType
SopMgr::cover_quotient(
  SopBitVect* dst_bv,
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
  SizeType nc = 0;
  auto bv1 = src1.bitvect();
  auto bv1_end = _calc_offset(bv1, src1.cube_num());
  for ( ; bv1 != bv1_end; bv1 += nb ) {
    if ( (bv1[blk] & mask) == pat1 ) {
      cube_copy(dst_bv, bv1);
      dst_bv[blk] &= nmask;
      dst_bv += nb;
      ++ nc;
    }
  }

  return nc;
}

// @brief カバー中のすべてのキューブの共通部分を求める．
SopCube
SopMgr::common_cube(
  const SopBlock& src1
)
{
  auto bv1 = src1.bitvect();
  auto bv1_end = _calc_offset(bv1, src1.cube_num());

  auto dst_bv0 = new_body(1);
  auto dst_bv = dst_bv0;

  // 最初のキューブをコピーする．
  cube_copy(dst_bv, bv1);

  // 2番目以降のキューブとの共通部分を求める．
  bv1 += _cube_size();
  while ( bv1 != bv1_end ) {
    SopBitVect tmp = 0ULL;
    auto dst_bv1 = dst_bv;
    auto bv1_end1 = _calc_offset(bv1, 1);
    for ( ; bv1 != bv1_end1; ++ dst_bv1, ++ bv1) {
      *dst_bv &= *bv1;
      tmp |= *dst_bv;
    }
    if ( tmp == 0ULL ) {
      // 空になった．
      break;
    }
  }

  return SopCube(mVarNum, dst_bv0);
}

// @brief Literal のリストからキューブ(を表すビットベクタ)のコピーを行う．
void
SopMgr::cube_set(
  SopBitVect* dst_bv,
  const vector<Literal>& lit_list
)
{
  for ( auto lit: lit_list ) {
    auto var_id = lit.varid();
    auto blk = _block_pos(var_id);
    auto sft = _shift_num(var_id);
    auto pat = lit2bv(lit);
    auto mask = pat << sft;
    dst_bv[blk] |= mask;
  }
}

// @brief Literal のリストからキューブ(を表すビットベクタ)のコピーを行う．
void
SopMgr::cube_set(
  SopBitVect* dst_bv,
  std::initializer_list<Literal>& lit_list
)
{
  for ( auto lit: lit_list ) {
    auto var_id = lit.varid();
    auto blk = _block_pos(var_id);
    auto sft = _shift_num(var_id);
    auto pat = lit2bv(lit);
    auto mask = pat << sft;
    dst_bv[blk] |= mask;
  }
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
  cover_copy(mTmpBuff, _calc_offset(bv,  start1), hn);
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
  auto bv1 = src1.bitvect();
  auto bv1_end = _calc_offset(bv1, src1.cube_num());
  auto bv2 = src2.bitvect();
  auto bv2_end = _calc_offset(bv2, src2.cube_num());
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

// @brief ビットベクタからハッシュ値を計算する．
SizeType
SopMgr::hash(
  const SopBlock& src1
)
{
  // キューブは常にソートされているので
  // 順番は考慮する必要はない．
  // ただおなじキューブの中のビットに関しては
  // 本当は区別しなければならないがどうしようもないので
  // 16ビットに区切って単純に XOR を取る．
  SizeType ans = 0;
  auto bv = src1.bitvect();
  auto bv_end = _calc_offset(bv, src1.cube_num());
  for ( ; bv != bv_end; ++ bv ) {
    auto pat = *bv;
    ans ^= (pat & 0xFFFFULL); pat >>= 16;
    ans ^= (pat & 0xFFFFULL); pat >>= 16;
    ans ^= (pat & 0xFFFFULL); pat >>= 16;
    ans ^= pat;
  }
  return ans;
}

// @brief Expr に変換する．
Expr
SopMgr::to_expr(
  const SopBitVect* bv,
  SizeType cube_num
)
{
  auto ans = Expr::make_zero();
  for ( SizeType c = 0; c < cube_num; ++ c ) {
    auto prod = Expr::make_one();
    for ( SizeType var = 0; var < variable_num(); ++ var ) {
      auto pat = get_pat(bv, c, var);
      if ( pat == SopPat::_1 ) {
	prod &= Expr::make_posi_literal(var);
      }
      else if ( pat == SopPat::_0 ) {
	prod &= Expr::make_nega_literal(var);
      }
    }
    ans |= prod;
  }
  return ans;
}

// @brief キューブ(を表すビットベクタ)の比較を行う．
int
SopMgr::cube_compare(
  const SopBitVect* bv1,
  const SopBitVect* bv2
)
{
  auto bv1_end = _calc_offset(bv1, 1);
  while ( bv1 != bv1_end ) {
    auto pat1 = *bv1;
    auto pat2 = *bv2;
    if ( pat1 < pat2 ) {
      return -1;
    }
    else if ( pat1 > pat2 ) {
      return 1;
    }
    ++ bv1;
    ++ bv2;
  }
  return 0;
}

// @brief 2つのキューブの積が空でない時 true を返す．
bool
SopMgr::cube_check_conflict(
  const SopBitVect* bv1,
  const SopBitVect* bv2
)
{
  auto bv1_end = _calc_offset(bv1, 1);
  while ( bv1 != bv1_end ) {
    auto tmp = *bv1 | *bv2;
    const SopBitVect mask1 = 0x5555555555555555ULL;
    const SopBitVect mask2 = 0xAAAAAAAAAAAAAAAAULL;
    auto tmp1 = tmp & mask1;
    auto tmp2 = tmp & mask2;
    if ( (tmp1 & (tmp2 >> 1)) != 0ULL ) {
      // 同じ変数の異なる極性のリテラルがあった．
      return true;
    }
    ++ bv1;
    ++ bv2;
  }
  return false;
}

// @brief 一方のキューブが他方のキューブに含まれているか調べる．
bool
SopMgr::cube_check_containment(
  const SopBitVect* bv1,
  const SopBitVect* bv2
)
{
  auto bv1_end = _calc_offset(bv1, 1);
  while ( bv1 != bv1_end ) {
    if ( (~(*bv1) & *bv2) != 0ULL ) {
      return false;
    }
    ++ bv1;
    ++ bv2;
  }
  return true;
}

// @brief ２つのキューブに共通なリテラルがあれば true を返す．
bool
SopMgr::cube_check_intersect(
  const SopBitVect* bv1,
  const SopBitVect* bv2
)
{
  auto bv1_end = _calc_offset(bv1, 1);
  while ( bv1 != bv1_end ) {
    if ( (*bv1 & *bv2) != 0ULL ) {
      return true;
    }
    ++ bv1;
    ++ bv2;
  }
  return false;
}

// @brief キューブ(を表すビットベクタ)をクリアする．
void
SopMgr::cube_clear(
  SopBitVect* dst_bv
)
{
  auto dst_bv_end = _calc_offset(dst_bv, 1);
  while ( dst_bv != dst_bv_end ) {
    *dst_bv = 0ULL;
    ++ dst_bv;
  }
}

// @brief 2つのキューブの積を計算する．
bool
SopMgr::cube_product(
  SopBitVect* dst_bv,
  const SopBitVect* bv1,
  const SopBitVect* bv2
)
{
  auto dst_bv_end = _calc_offset(dst_bv, 1);
  while ( dst_bv != dst_bv_end ) {
    auto tmp = *bv1 | *bv2;
    const SopBitVect mask1 = 0x5555555555555555ULL;
    const SopBitVect mask2 = 0xAAAAAAAAAAAAAAAAULL;
    auto tmp1 = tmp & mask1;
    auto tmp2 = tmp & mask2;
    if ( (tmp1 & (tmp2 >> 1)) != 0ULL ) {
      // 同じ変数の異なる極性のリテラルがあった．
      return false;
    }
    *dst_bv = tmp;
    ++ dst_bv;
    ++ bv1;
    ++ bv2;
  }
  return true;
}

// @brief キューブによる商を求める．
bool
SopMgr::cube_quotient(
  SopBitVect* dst_bv,
  const SopBitVect* bv1,
  const SopBitVect* bv2
)
{
  auto dst_bv_end = _calc_offset(dst_bv, 1);
  while ( dst_bv != dst_bv_end ) {
    auto pat1 = *bv1;
    auto pat2 = *bv2;
    if ( (~pat1 & pat2) != 0ULL ) {
      // この場合の dst の値は不定
      return false;
    }
    *dst_bv = pat1 & ~pat2;
    ++ dst_bv;
    ++ bv1;
    ++ bv2;
  }
  return true;
}

// @brief 要素のチェック
bool
SopMgr::litset_check(
  SopBitVect* bv,
  Literal lit
)
{
  auto var_id = lit.varid();
  auto blk = _block_pos(var_id);
  auto sft = _shift_num(var_id);
  auto pat = lit2bv(lit);
  auto mask = pat << sft;
  if ( bv[blk] & mask ) {
    return true;
  }
  else {
    return false;
  }
}

// @brief ユニオン演算
void
SopMgr::litset_union(
  SopBitVect* dst_bv,
  const SopBitVect* src_bv
)
{
  auto dst_bv_end = _calc_offset(dst_bv, 1);
  for ( ; dst_bv != dst_bv_end; ++ dst_bv, ++ src_bv ) {
    *dst_bv |= *src_bv;
  }
}

// @brief カバー/キューブの内容を出力する．
void
SopMgr::print(
  ostream& s,
  const SopBitVect* bv,
  SizeType start,
  SizeType end,
  const vector<string>& varname_list
)
{
  const char* plus = "";
  for ( SizeType i = start; i < end; ++ i ) {
    s << plus;
    plus = " + ";
    const char* spc = "";
    for ( SizeType j = 0; j < variable_num(); ++ j ) {
      string varname;
      if ( varname_list.size() > j ) {
	varname = varname_list[j];
      }
      else {
	ostringstream buf;
	buf << "v" << j;
	varname = buf.str();
      }
      auto pat = get_pat(bv, i, j);
      if ( pat == SopPat::_1 ) {
	s << spc << varname;
	spc = " ";
      }
      else if ( pat == SopPat::_0 ) {
	s << spc << varname << "'";
	spc = " ";
      }
    }
  }
}

// @brief mTmpBuff を初期化する．
void
SopMgr::_init_buff()
{
  // とりあえず 128 を初期値とする．
  mTmpBuffSize = 128;
  mTmpBuff = new_body(mTmpBuffSize);
}

// @brief mTmpBuff に必要な領域を確保する．
// @param[in] req_size 要求するキューブ数
void
SopMgr::_resize_buff(
  SizeType req_size
)
{
  SizeType old_size = mTmpBuffSize;
  while ( mTmpBuffSize < req_size ) {
    mTmpBuffSize <<= 1;
  }

  if ( old_size < mTmpBuffSize ) {
    delete_body(mTmpBuff, old_size);
  }
  mTmpBuff = new_body(mTmpBuffSize);
}

END_NAMESPACE_YM_LOGIC
