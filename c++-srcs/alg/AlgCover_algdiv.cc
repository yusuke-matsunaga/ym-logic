
/// @file AlgCover_algdiv.cc
/// @brief Algcover の algdiv 関係の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/AlgCover.h"
#include "ym/AlgCube.h"


BEGIN_NAMESPACE_YM_ALG

//////////////////////////////////////////////////////////////////////
// クラス AlgCover
//////////////////////////////////////////////////////////////////////

// @brief algebraic division を計算する．
AlgCover
AlgCover::operator/(
  const AlgCover& right
) const
{
  if ( variable_num() != right.variable_num() ) {
    throw std::invalid_argument("variable_num() is different from each other");
  }
  SizeType dst_num;
  auto dst_chunk = algdiv(right.cube_num(), right.chunk(), dst_num);
  return AlgCover{variable_num(), dst_num, std::move(dst_chunk)};
}

// @brief algebraic division を行って代入する．
AlgCover&
AlgCover::operator/=(
  const AlgCover& right
)
{
  if ( variable_num() != right.variable_num() ) {
    throw std::invalid_argument("variable_num() is different from each other");
  }
  SizeType dst_num;
  auto dst_chunk = algdiv(right.cube_num(), right.chunk(), dst_num);
  _set(dst_num, std::move(dst_chunk));
  return *this;
}

// @brief キューブによる商を計算する．
AlgCover
AlgCover::operator/(
  const AlgCube& right
) const
{
  if ( variable_num() != right.variable_num() ) {
    throw std::invalid_argument("variable_num() is different from each other");
  }
  SizeType dst_num;
  auto dst_chunk = algdiv(right.chunk(), dst_num);
  return AlgCover{variable_num(), dst_num, std::move(dst_chunk)};
}

// @brief キューブによる商を計算する．
AlgCover&
AlgCover::operator/=(
  const AlgCube& right
)
{
  if ( variable_num() != right.variable_num() ) {
    throw std::invalid_argument("variable_num() is different from each other");
  }
  SizeType dst_num;
  auto dst_chunk = algdiv(right.chunk(), dst_num);
  _set(dst_num, std::move(dst_chunk));
  return *this;
}

// @brief リテラルによる商を計算する．
AlgCover
AlgCover::operator/(
  Literal lit
) const
{
  SizeType dst_num;
  auto dst_chunk = algdiv(lit, dst_num);
  return AlgCover{variable_num(), dst_num, std::move(dst_chunk)};
}

// @brief リテラルによる商を計算して代入する．
AlgCover&
AlgCover::operator/=(
  Literal lit
)
{
  SizeType dst_num;
  auto dst_chunk = algdiv(lit, dst_num);
  _set(dst_num, std::move(dst_chunk));
  return *this;
}

// @brief カバーの代数的除算を行う．
AlgBase::Chunk
AlgCover::algdiv(
  SizeType num2,
  const Chunk& chunk2,
  SizeType& dst_num
) const
{
  SizeType num1 = cube_num();

  // 作業領域のビットベクタを確保する．
  auto tmp_chunk = _new_chunk(num1);

  // 被除数の各キューブは高々1つのキューブでしか割ることはできない．
  // ただし，除数も被除数も algebraic expression の場合
  // ので bv1 の各キューブについて bv2 の各キューブで割ってみて
  // 成功した場合，その商を記録する．
  vector<bool> mark(num1, false);
  {
    auto dst_list = _cube_list(tmp_chunk);
    for ( SizeType i1 = 0; i1 < num1; ++ i1 ) {
      auto cube1 = _cube(chunk(), i1);
      auto dst_cube = dst_list.back();
      for ( SizeType i2 = 0; i2 < num2; ++ i2 ) {
	auto cube2 = _cube(chunk2, i2);
	if ( _cube_quotient(dst_cube, cube1, cube2) ) {
	  mark[i1] = true;
	  break;
	}
      }
      dst_list.inc();
    }
  }

  // 商のキューブは tmp_chunk 中に num2 回現れているはず．
  vector<SizeType> pos_list;
  pos_list.reserve(num1);
  for ( SizeType i1 = 0; i1 < num1; ++ i1 ) {
    if ( !mark[i1] ) {
      // 対象ではない．
      continue;
    }
    SizeType c = 1;
    vector<SizeType> tmp_list;
    auto cube1 = _cube(tmp_chunk, i1);
    for ( SizeType i2 = i1 + 1; i2 < num1; ++ i2 ) {
      auto cube2 = _cube(tmp_chunk, i2);
      if ( mark[i2] && _cube_compare(cube1, cube2) == 0 ) {
	++ c;
	// i 番目のキューブと等しかったキューブ位置を記録する．
	tmp_list.push_back(i2);
      }
    }
    if ( c == num2 ) {
      // 見つけた
      pos_list.push_back(i1);
      // tmp_list に含まれるキューブはもう調べなくて良い．
      for ( auto pos: tmp_list ) {
	mark[pos] = false;
      }
    }
  }

  dst_num = pos_list.size();
  auto dst_chunk = _new_chunk(dst_num);
  auto dst_list = _cube_list(dst_chunk);
  for ( auto pos: pos_list ) {
    auto src_cube = _cube(tmp_chunk, pos);
    auto dst_cube = dst_list.back();
    _cube_copy(dst_cube, src_cube);
    dst_list.inc();
  }
  return dst_chunk;
}

// @brief カバーをキューブで割る．
AlgBase::Chunk
AlgCover::algdiv(
  const Chunk& chunk2,
  SizeType& dst_num
) const
{
  SizeType num1 = cube_num();
  auto dst_chunk = _new_chunk(num1);
  auto dst_list = _cube_list(dst_chunk);
  auto cube1_list = _cube_list(chunk(), 0, num1);
  auto cube2 = _cube(chunk2);
  for ( auto cube1: cube1_list ) {
    auto dst_cube = dst_list.back();
    if ( _cube_quotient(dst_cube, cube1, cube2) ) {
      dst_list.inc();
    }
  }
  dst_num = dst_list.num();
  return dst_chunk;
}

// @brief カバーをリテラルで割る．
AlgBase::Chunk
AlgCover::algdiv(
  Literal lit,
  SizeType& dst_num
) const
{
  auto varid = lit.varid();
  auto inv = lit.is_negative();
  auto blk = _block_pos(varid);
  auto sft = _shift_num(varid);
  auto pat1 = _get_mask(varid, inv);
  auto mask = 3UL << sft;
  auto nmask = ~mask;

  SizeType num1 = cube_num();
  auto dst_chunk = _new_chunk(num1);
  auto dst_list = _cube_list(dst_chunk);
  auto cube1_list = _cube_list(chunk(), 0, num1);
  for ( auto cube1: cube1_list ) {
    auto src1_p = cube1 + blk;
    if ( (*src1_p & mask) == pat1 ) {
      auto dst_cube = dst_list.back();
      _cube_copy(dst_cube, cube1);
      auto dst_p = dst_cube + blk;
      *dst_p &= nmask;
      dst_list.inc();
    }
  }
  dst_num = dst_list.num();
  return dst_chunk;
}

END_NAMESPACE_YM_ALG
