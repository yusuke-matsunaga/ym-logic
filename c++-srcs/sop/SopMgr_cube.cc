
/// @file SopMgr_cube.cc
/// @brief SopMgr のキューブ関係の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
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
  SopBitVectConstIter cube
)
{
  auto cube_end = _cube_end(cube);
  for ( ; cube != cube_end; ++ cube ) {
    if ( *cube != 0UL ) {
      return false;
    }
  }
  return true;
}

// @brief 2つのキューブの積を計算する．
bool
SopMgr::cube_product(
  SopBitVectIter dst_iter,
  SopBitVectConstIter src1_iter,
  SopBitVectConstIter src2_iter
)
{
  const SopPatWord mask1 = 0x5555555555555555ULL;
  const SopPatWord mask2 = 0xAAAAAAAAAAAAAAAAULL;
  auto dst_end = _cube_end(dst_iter);
  for ( ; dst_iter != dst_end; ++ dst_iter, ++ src1_iter, ++ src2_iter ) {
    auto tmp = *src1_iter | *src2_iter;
    auto tmp1 = tmp & mask1;
    auto tmp2 = tmp & mask2;
    if ( (tmp1 & (tmp2 >> 1)) != 0ULL ) {
      // 同じ変数の異なる極性のリテラルがあった．
      return false;
    }
    *dst_iter = tmp;
  }
  return true;
}

// @brief キューブとリテラルの積を計算する．
bool
SopMgr::cube_product(
  SopBitVectIter dst_iter,
  SopBitVectConstIter src_iter,
  Literal lit
)
{
  auto varid = lit.varid();
  auto inv = lit.is_negative();
  auto blk = _block_pos(varid);
  auto sft = _shift_num(varid);
  auto pat1 = get_mask(varid, inv);
  auto mask = 3UL << sft;

  // 単純には答の積項数は2つの積項数の積だが
  // 相反するリテラルを含む積は数えない．
  auto src_p = src_iter + blk;
  auto tmp = *src_p | pat1;
  if ( (tmp & mask) == mask ) {
    // 相反するリテラルがあった．
    return false;
  }
  cube_copy(dst_iter, src_iter);
  auto dst_p = dst_iter + blk;
  *dst_p = tmp;
  return true;
}

// @brief キューブとリテラルの積を計算する．
bool
SopMgr::cube_product_int(
  SopBitVectIter dst_iter,
  Literal lit
)
{
  auto varid = lit.varid();
  auto inv = lit.is_negative();
  auto blk = _block_pos(varid);
  auto sft = _shift_num(varid);
  auto pat1 = get_mask(varid, inv);
  auto mask = 3UL << sft;

  // 単純には答の積項数は2つの積項数の積だが
  // 相反するリテラルを含む積は数えない．
  auto dst_p = dst_iter + blk;
  auto tmp = *dst_p | pat1;
  if ( (tmp & mask) == mask ) {
    // 相反するリテラルがあった．
    return false;
  }
  *dst_p = tmp;
  return true;
}

// @brief キューブによる商を求める．
bool
SopMgr::cube_quotient(
  SopBitVectIter dst_iter,
  SopBitVectConstIter src1_iter,
  SopBitVectConstIter src2_iter
)
{
  auto dst_end = _cube_end(dst_iter);
  for ( ; dst_iter != dst_end; ++ dst_iter, ++ src1_iter, ++ src2_iter ) {
    auto pat1 = *src1_iter;
    auto pat2 = *src2_iter;
    if ( (~pat1 & pat2) != 0ULL ) {
      // この場合の dst の値は不定
      return false;
    }
    *dst_iter = pat1 & ~pat2;
  }
  return true;
}

// @brief リテラルによる商を求める．
bool
SopMgr::cube_quotient(
  SopBitVectIter dst_iter,
  SopBitVectConstIter src_iter,
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
  auto src_p = src_iter + blk;
  auto dst_p = dst_iter + blk;
  if ( (*src_p & mask) == pat1 ) {
    cube_copy(dst_iter, src_iter);
    *dst_p &= nmask;
    return true;
  }
  return false;
}

// @brief キューブ(を表すビットベクタ)の比較を行う．
int
SopMgr::cube_compare(
  SopBitVectConstIter src1_iter,
  SopBitVectConstIter src2_iter
)
{
  auto src1_end = _cube_end(src1_iter);
  for ( ; src1_iter != src1_end; ++ src1_iter, ++ src2_iter ) {
    auto pat1 = *src1_iter;
    auto pat2 = *src2_iter;
    if ( pat1 < pat2 ) {
      return -1;
    }
    else if ( pat1 > pat2 ) {
      return 1;
    }
  }
  return 0;
}

END_NAMESPACE_YM_SOP
