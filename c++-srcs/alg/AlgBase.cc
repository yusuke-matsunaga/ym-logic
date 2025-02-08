
/// @file AlgBase.cc
/// @brief AlgBase の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/AlgBase.h"


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
  static const int table[] = {
    // utils/gen_litcount_tbl.py で生成
#include "litcount_tbl.h"
  };

  return table[pat];
}

END_NONAMESPACE

// @brief ビットベクタ上のリテラル数を数える．
SizeType
AlgBase::_literal_num(
  SizeType cube_num,
  const Chunk& chunk
) const
{
  // 8 ビットごとに区切って表引きで計算する．
  SizeType ans = 0;
  auto begin = _cube(chunk);
  auto end = _cube(chunk, cube_num);
  for ( auto p = begin; p != end; ++ p ) {
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
  SizeType cube_num,
  const Chunk& chunk,
  SizeType varid,
  bool inv
) const
{
  auto blk = _block_pos(varid);
  auto mask = _get_mask(varid, inv);
  SizeType ans = 0;
  auto cube_list = _cube_list(chunk, 0, cube_num);
  for ( auto cube: cube_list ) {
    auto word = _get_word(cube, blk);
    if ( (word & mask) == mask ) {
      ++ ans;
    }
  }
  return ans;
}

// @brief 内容をリテラルのリストのリストに変換する．
vector<vector<Literal>>
AlgBase::_literal_list(
  SizeType cube_num,
  const Chunk& chunk
) const
{
  vector<vector<Literal>> ans_list;
  ans_list.reserve(cube_num);

  auto cube_list = _cube_list(chunk, 0, cube_num);
  for ( auto cube: cube_list ) {
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
    ans_list.push_back(tmp_list);
  }

  return ans_list;
}

// @brief ビットベクタからハッシュ値を計算する．
SizeType
AlgBase::_hash(
  SizeType cube_num,
  const Chunk& chunk
) const
{
  // キューブは常にソートされているので
  // 順番は考慮する必要はない．
  // ただおなじキューブの中のビットに関しては
  // 本当は区別しなければならないがどうしようもないので
  // 16ビットに区切って単純に XOR を取る．
  SizeType ans = 0;
  auto begin = _cube(chunk);
  auto end = _cube(chunk, cube_num);
  for ( auto p = begin; p != end; ++ p ) {
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
  SizeType cube_num,
  const Chunk& chunk
) const
{
  // 特例
  if ( _cube_size() == 0 ) {
    if ( cube_num == 0 ) {
      return Expr::zero();
    }
    else {
      return Expr::one();
    }
  }
  auto ans = Expr::zero();
  auto cube_list = _cube_list(chunk, 0, cube_num);
  for ( auto cube: cube_list ) {
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
  const Chunk& chunk,
  SizeType begin,
  SizeType end,
  const vector<string>& varname_list
) const
{
  const char* plus = "";
  auto cube_list = _cube_list(chunk, begin, end);
  for ( auto cube: cube_list ) {
    s << plus;
    plus = " + ";
    _print(s, cube, varname_list);
  }
}

// @brief キューブの内容を出力する．
void
AlgBase::_print(
  ostream& s,
  Cube cube,
  const vector<string>& varname_list
) const
{
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

// @brief 内容を出力する(デバッグ用)．
void
AlgBase::_debug_print(
  ostream& s,
  SizeType cube_num,
  const Chunk& chunk
) const
{
  auto cube_list = _cube_list(chunk, 0, cube_num);
  for ( auto cube: cube_list ) {
    _debug_print(s, cube);
  }
}

// @brief 内容を出力する(デバッグ用)．
void
AlgBase::_debug_print(
  ostream& s,
  Cube cube
) const
{
  for ( SizeType var = 0; var < variable_num(); ++ var ) {
    s << " ";
    auto pat = _get_pat(cube, var);
    switch ( pat ) {
    case AlgPat::_X: s << "00"; break;
    case AlgPat::_0: s << "01"; break;
    case AlgPat::_1: s << "10"; break;
    default:         s << "--"; break;
    }
  }
  s << endl;
}

// @brief 内容を出力する(デバッグ用)．
void
AlgBase::_debug_print(
  ostream& s,
  DstCube cube
) const
{
  for ( SizeType var = 0; var < variable_num(); ++ var ) {
    s << " ";
    auto pat = _get_pat(cube, var);
    switch ( pat ) {
    case AlgPat::_X: s << "00"; break;
    case AlgPat::_0: s << "01"; break;
    case AlgPat::_1: s << "10"; break;
    default:         s << "--"; break;
    }
  }
  s << endl;
}

END_NAMESPACE_YM_ALG
