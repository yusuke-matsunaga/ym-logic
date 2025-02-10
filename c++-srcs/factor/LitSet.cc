
/// @file LitSet.cc
/// @brief LitSet の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "LitSet.h"
#include "ym/SopCube.h"


BEGIN_NAMESPACE_YM_FACTOR

//////////////////////////////////////////////////////////////////////
// クラス LitSet
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LitSet::LitSet(
  SizeType var_num
) : SopBase{var_num},
    mChunk(_cube_size(), 0ULL)
{
}

// @brief コンストラクタ
LitSet::LitSet(
  SizeType var_num,
  Literal lit
) : LitSet{var_num}
{
  auto cube = mChunk.begin();
  _cube_set_literal(cube, lit);
}

// @brief コンストラクタ
LitSet::LitSet(
  SizeType var_num,
  const vector<Literal>& lit_list
) : LitSet{var_num}
{
  auto cube = mChunk.begin();
  _cube_set_literals(cube, lit_list);
}

// @brief コンストラクタ
LitSet::LitSet(
  SizeType var_num,
  std::initializer_list<Literal>& lit_list
) : LitSet{var_num}
{
  auto cube = mChunk.begin();
  _cube_set_literals(cube, lit_list);
}

// @brief コピーコンストラクタ
LitSet::LitSet(
  const LitSet& src
) : SopBase{src},
    mChunk{src.mChunk}
{
}

// @brief 代入演算子
LitSet&
LitSet::operator=(
  const LitSet& src
)
{
  if ( &src != this ) {
    SopBase::operator=(src);
    mChunk = src.mChunk;
  }

  return *this;
}

// @brief ムーブコンストラクタ
LitSet::LitSet(
  LitSet&& src
) : SopBase{std::move(src)},
    mChunk{std::move(src.mChunk)}
{
}

// @brief ムーブ代入演算子
LitSet&
LitSet::operator=(
  LitSet&& src
)
{
  SopBase::operator=(std::move(src));
  std::swap(mChunk, src.mChunk);

  return *this;
}

// @brief 要素を足す．
LitSet&
LitSet::operator+=(
  Literal lit
)
{
  auto cube = mChunk.begin();
  _cube_set_literal(cube, lit);
  return *this;
}

// @brief ユニオン演算付き代入
LitSet&
LitSet::operator+=(
  const LitSet& right
)
{
  auto dst_cube = _dst_cube(mChunk);
  auto dst_end = _cube_end(dst_cube);
  auto src_cube = _cube(right.mChunk);
  for ( ; dst_cube != dst_end; ++ dst_cube, ++ src_cube ) {
    *dst_cube |= *src_cube;
  }
  return *this;
}

// @brief 該当するリテラルが含まれているか調べる．
bool
LitSet::is_in(
  Literal lit
) const
{
  auto varid = lit.varid();
  auto inv = lit.is_negative();
  auto blk = _block_pos(varid);
  auto mask = _get_mask(varid, inv);
  auto cube = _cube(mChunk);
  if ( *(cube + blk) & mask ) {
    return true;
  }
  else {
    return false;
  }
}

// @brief 引数のキューブ中のリテラルをひとつでも含んでいたら true を返す．
bool
LitSet::check_intersect(
  const SopCube& right
) const
{
  if ( variable_num() != right.variable_num() ) {
    throw std::invalid_argument{"variable_num() mismatch"};
  }

  auto cube1 = _cube(mChunk);
  auto end1 = _cube_end(cube1);
  auto cube2 = _cube(right.chunk());
  for ( ; cube1 != end1; ++ cube1, ++ cube2 ) {
    if ( (*cube1 & *cube2) != 0ULL ) {
      return true;
    }
  }
  return false;
}

END_NAMESPACE_YM_FACTOR
