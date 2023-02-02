
/// @file AlgMgr_cube.cc
/// @brief AlgMgr のキューブ関係の実装ファイル
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

// @brief 空キューブかどうか調べる．
bool
SopMgr::cube_check_null(
  const SopBitVect* bv
)
{
  auto bv_end = _calc_offset(bv, 1);
  for ( ; bv != bv_end; ++ bv ) {
    auto pat = *bv;
    if ( pat != 0UL ) {
      return false;
    }
  }
  return true;
}

// @brief キューブを否定する．
SopBlock
SopMgr::cube_complement(
  const SopBitVect* bv
)
{
  SizeType nl = literal_num(bv, 1);
  auto dst_block = new_cover(nl);
  auto dst_body = dst_block.body;
  for ( SizeType var = 0; var < variable_num(); ++ var ) {
    auto pat = get_pat(bv, 0, var);
    if ( pat == SopPat::_1 ) {
      cube_set_literal(dst_body, var, Literal{var, false});
    }
    else if ( pat == SopPat::_0 ) {
      cube_set_literal(dst_body, var, Literal{var, true});
    }
  }
  return dst_block;
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
  auto dst_bv0 = dst_bv;
  if ( 0 ) {
    cout << "cube_product" << endl
	 << " bv1: ";
    debug_print(cout, bv1, 1);
    cout << " bv2: ";
    debug_print(cout, bv2, 1);
  }
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
  if ( 0 ) {
    cout << " =>" << endl;
    cout << "      ";
    debug_print(cout, dst_bv0, 1);
  }
  return true;
}

// @brief キューブとリテラルの積を計算する．
bool
SopMgr::cube_product(
  SopBitVect* dst_body,
  const SopBitVect* bv1,
  Literal lit
)
{
  auto var_id = lit.varid();
  auto blk = _block_pos(var_id);
  auto sft = _shift_num(var_id);
  auto pat = lit2bv(lit);
  auto pat1 = pat << sft;
  auto mask = 3UL << sft;

  // 単純には答の積項数は2つの積項数の積だが
  // 相反するリテラルを含む積は数えない．
  auto tmp = bv1[blk] | pat1;
  if ( (tmp & mask) == mask ) {
    // 相反するリテラルがあった．
    return false;
  }
  cube_copy(dst_body, bv1);
  dst_body[blk] = tmp;
  return true;
}

// @brief キューブとリテラルの積を計算する．
bool
SopMgr::cube_product_int(
  SopBitVect* dst_body,
  Literal lit
)
{
  auto var_id = lit.varid();
  auto blk = _block_pos(var_id);
  auto sft = _shift_num(var_id);
  auto pat = lit2bv(lit);
  auto pat1 = pat << sft;
  auto mask = 3UL << sft;

  // 単純には答の積項数は2つの積項数の積だが
  // 相反するリテラルを含む積は数えない．
  auto tmp = dst_body[blk] | pat1;
  if ( (tmp & mask) == mask ) {
    // 相反するリテラルがあった．
    return false;
  }
  dst_body[blk] = tmp;
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

// @brief リテラルによる商を求める．
bool
SopMgr::cube_quotient(
  SopBitVect* dst_bv,
  const SopBitVect* bv1,
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
  if ( (bv1[blk] & mask) == pat1 ) {
    cube_copy(dst_bv, bv1);
    dst_bv[blk] &= nmask;
    return true;
  }
  return false;
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

END_NAMESPACE_YM_SOP
