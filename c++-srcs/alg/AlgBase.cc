
/// @file AlgBase.cc
/// @brief AlgBase の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "AlgBase.h"
#include "AlgBlock.h"
#include "ym/Literal.h"


BEGIN_NAMESPACE_YM_ALG

//////////////////////////////////////////////////////////////////////
// クラス AlgBase
//////////////////////////////////////////////////////////////////////

BEGIN_NONAMESPACE

// 表引きを使ってリテラル数の計算を行う．
int
_count(
  AlgPatWord pat
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
AlgBase::_literal_num(
  const AlgBlockRef& block
) const
{
  // 8 ビットごとに区切って表引きで計算する．
  SizeType ans = 0;
  auto start = _cube_begin(block.body);
  auto end = _cube_end(start, block.cube_num);
  for ( auto p = start; p != end; ++ p ) {
    auto pat = *p;
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
AlgBase::_literal_num(
  const AlgBlockRef& block,
  SizeType varid,
  bool inv
) const
{
  auto blk = _block_pos(varid);
  auto mask = _get_mask(varid, inv);
  SizeType ans = 0;
  auto start = _cube_begin(block.body);
  auto end = _cube_end(start, block.cube_num);
  for ( auto p = start; p != end; p += _cube_size() ) {
    if ( (*(p + blk) & mask) == mask ) {
      ++ ans;
    }
  }
  return ans;
}

// @brief 内容をリテラルのリストのリストに変換する．
vector<vector<Literal>>
AlgBase::_literal_list(
  const AlgBlockRef& block ///< [in] 対象のブロック
) const
{
  vector<vector<Literal>> cube_list;
  cube_list.reserve(block.cube_num);

  auto begin = _cube_begin(block.body);
  auto end = _cube_end(begin, block.cube_num);
  for ( auto cube = begin;
	cube != end;
	cube += _cube_size() ) {
    vector<Literal> tmp_list;
    tmp_list.reserve(variable_num());
    for ( SizeType var = 0; var < variable_num(); ++ var ) {
      auto pat = _get_pat(cube, var);
      if ( pat == AlgPat::_1 ) {
	tmp_list.push_back(Literal{var, false});
      }
      else if ( pat == AlgPat::_0 ) {
	tmp_list.push_back(Literal{var, true});
      }
    }
    cube_list.push_back(tmp_list);
  }

  return cube_list;
}

// @brief カバーからキューブを取り出す．
AlgCube
AlgBase::_get_cube(
  const AlgBlockRef& block,
  SizeType cube_id
) const
{
  auto dst_bv = new_cube();
  auto dst_cube = _cube_begin(dst_bv);
  auto src_cube = _cube_begin(block.body, cube_id);
  _cube_copy(dst_cube, src_cube);
  return AlgCube{variable_num(), std::move(dst_bv)};
}

// @brief ビットベクタからハッシュ値を計算する．
SizeType
AlgBase::_hash(
  const AlgBitVectRef& block
)
{
  // キューブは常にソートされているので
  // 順番は考慮する必要はない．
  // ただおなじキューブの中のビットに関しては
  // 本当は区別しなければならないがどうしようもないので
  // 16ビットに区切って単純に XOR を取る．
  SizeType ans = 0;
  auto start = _cube_begin(block.body());
  auto end = _cube_end(start, block.cube_num);
  for ( auto p = start; p != end; ++ p ) {
    auto pat = *p;
    ans ^= (pat & 0xFFFFULL); pat >>= 16;
    ans ^= (pat & 0xFFFFULL); pat >>= 16;
    ans ^= (pat & 0xFFFFULL); pat >>= 16;
    ans ^= pat;
  }
  return ans;
}

// @brief Expr に変換する．
Expr
AlgBase::_to_expr(
  const AlgBitVectRef& block,
)
{
  auto ans = Expr::zero();
  auto start = _cube_begin(block.body());
  auto end = _cube_end(start, block.cube_num);
  for ( auto cube = start; cube != end; cube += _cube_size() ) {
    auto prod = Expr::one();
    for ( SizeType var = 0; var < variable_num(); ++ var ) {
      auto pat = _get_pat(cube, var);
      if ( pat == AlgPat::_1 ) {
	prod &= Expr::posi_literal(var);
      }
      else if ( pat == AlgPat::_0 ) {
	prod &= Expr::nega_literal(var);
      }
    }
    ans |= prod;
  }
  return ans;
}

// @brief カバー/キューブの内容を出力する．
void
AlgBase::_print(
  ostream& s,
  const AlgBitVect& bv,
  SizeType start,
  SizeType end,
  const vector<string>& varname_list
)
{
  const char* plus = "";
  auto cube_begin = _cube_begin(bv, start);
  auto cube_end = _cube_begin(bv, end);
  for ( auto cube = cube_begin; cube != cube_end; cube += _cube_size() ) {
    s << plus;
    plus = " + ";
    const char* spc = "";
    for ( SizeType var = 0; var < variable_num(); ++ var ) {
      string varname;
      if ( varname_list.size() > var ) {
	varname = varname_list[var];
      }
      else {
	ostringstream buf;
	buf << "v" << var;
	varname = buf.str();
      }
      auto pat = _get_pat(cube, var);
      if ( pat == AlgPat::_1 ) {
	s << spc << varname;
	spc = " ";
      }
      else if ( pat == AlgPat::_0 ) {
	s << spc << varname << "'";
	spc = " ";
      }
    }
  }
}

// @brief 内容を出力する(デバッグ用)．
void
AlgBase::_debug_print(
  ostream& s,
  const AlgBlockRef& block
)
{
  auto cube_begin = _cube_begin(block.body);
  auto cube_end = _cube_end(cube_begin, block.cube_num);
  for ( auto cube = cube_begin; cube != cube_end; cube += _cube_size() ) {
    for ( SizeType var = 0; var < variable_num(); ++ var ) {
      s << " ";
      auto pat = get_pat(cube, var);
      switch ( pat ) {
      case AlgPat::_X: s << "00"; break;
      case AlgPat::_0: s << "01"; break;
      case AlgPat::_1: s << "10"; break;
      default:         s << "--"; break;
      }
    }
    s << endl;
  }
}

END_NAMESPACE_YM_ALG
