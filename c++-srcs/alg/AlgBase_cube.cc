
/// @file AlgBase_cube.cc
/// @brief AlgBase のキューブ関係の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/AlgBase.h"
#include "ym/Literal.h"


BEGIN_NAMESPACE_YM_ALG

//////////////////////////////////////////////////////////////////////
// クラス AlgBase
//////////////////////////////////////////////////////////////////////

// @brief 空キューブかどうか調べる．
bool
AlgBase::_cube_check_null(
  Cube cube
) const
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
AlgBase::_cube_product(
  DstCube dst_cube,
  Cube cube1,
  Cube cube2
) const
{
  const AlgPatWord mask1 = 0x5555555555555555ULL;
  const AlgPatWord mask2 = 0xAAAAAAAAAAAAAAAAULL;
  auto dst_end = _cube_end(dst_cube);
  for ( ; dst_cube != dst_end; ++ dst_cube, ++ cube1, ++ cube2 ) {
    auto tmp = *cube1 | *cube2;
    auto tmp1 = tmp & mask1;
    auto tmp2 = tmp & mask2;
    if ( (tmp1 & (tmp2 >> 1)) != 0ULL ) {
      // 同じ変数の異なる極性のリテラルがあった．
      return false;
    }
    *dst_cube = tmp;
  }
  return true;
}

// @brief キューブとリテラルの積を計算する．
bool
AlgBase::_cube_product(
  DstCube dst_cube,
  Cube src_cube,
  Literal lit
) const
{
  auto varid = lit.varid();
  auto inv = lit.is_negative();
  auto blk = _block_pos(varid);
  auto sft = _shift_num(varid);
  auto pat1 = _get_mask(varid, inv);
  auto mask = 3UL << sft;

  // 単純には答の積項数は2つの積項数の積だが
  // 相反するリテラルを含む積は数えない．
  auto src_p = src_cube + blk;
  auto tmp = *src_p | pat1;
  if ( (tmp & mask) == mask ) {
    // 相反するリテラルがあった．
    return false;
  }
  _cube_copy(dst_cube, src_cube);
  auto dst_p = dst_cube + blk;
  *dst_p = tmp;
  return true;
}

// @brief キューブとリテラルの積を計算する．
bool
AlgBase::_cube_product_int(
  DstCube dst_cube,
  Literal lit
) const
{
  auto varid = lit.varid();
  auto inv = lit.is_negative();
  auto blk = _block_pos(varid);
  auto sft = _shift_num(varid);
  auto pat1 = _get_mask(varid, inv);
  auto mask = 3UL << sft;

  // 単純には答の積項数は2つの積項数の積だが
  // 相反するリテラルを含む積は数えない．
  auto dst_p = dst_cube + blk;
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
AlgBase::_cube_quotient(
  DstCube dst_cube,
  Cube cube1,
  Cube cube2
) const
{
  auto dst_end = _cube_end(dst_cube);
  for ( ; dst_cube != dst_end; ++ dst_cube, ++ cube1, ++ cube2 ) {
    auto pat1 = *cube1;
    auto pat2 = *cube2;
    if ( (~pat1 & pat2) != 0ULL ) {
      // この場合の dst の値は不定
      return false;
    }
    *dst_cube = pat1 & ~pat2;
  }
  return true;
}

// @brief リテラルによる商を求める．
bool
AlgBase::_cube_quotient(
  DstCube dst_cube,
  Cube src_cube,
  Literal lit
) const
{
  auto varid = lit.varid();
  auto inv = lit.is_negative();
  auto blk = _block_pos(varid);
  auto sft = _shift_num(varid);
  auto pat1 = _get_mask(varid, inv);
  auto mask = 3UL << sft;
  auto nmask = ~mask;
  auto src_p = src_cube + blk;
  auto dst_p = dst_cube + blk;
  if ( (*src_p & mask) == pat1 ) {
    _cube_copy(dst_cube, src_cube);
    *dst_p &= nmask;
    return true;
  }
  return false;
}

// @brief キューブ(を表すビットベクタ)の比較を行う．
int
AlgBase::_cube_compare(
  Cube cube1,
  Cube cube2
) const
{
  auto src1_end = _cube_end(cube1);
  for ( ; cube1 != src1_end; ++ cube1, ++ cube2 ) {
    auto pat1 = *cube1;
    auto pat2 = *cube2;
    if ( pat1 < pat2 ) {
      return -1;
    }
    else if ( pat1 > pat2 ) {
      return 1;
    }
  }
  return 0;
}

END_NAMESPACE_YM_ALG
