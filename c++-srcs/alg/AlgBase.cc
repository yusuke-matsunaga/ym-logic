
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

// @brief キューブのリテラル数を数える．
SizeType
AlgBase::_cube_literal_num(
  Cube src_cube
) const
{
  // 8 ビットごとに区切って表引きで計算する．
  SizeType ans = 0;
  auto end = _cube_end(src_cube);
  for ( auto p = src_cube; p != end; ++ p ) {
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

BEGIN_NONAMESPACE

// @brief バイト中のリテラルをリストに追加する．
inline
void
byte_literal_list(
  std::uint8_t byte,
  SizeType base,
  vector<Literal>& lit_list
)
{
  static const int table[] = {
    // utils/gen_alglbit_tbl.py で生成
#include "alglbit_tbl.h"
  };

  while ( byte != 0x00U ) {
    auto pos = table[byte];
    SizeType varid = (pos >> 1) + base;
    bool inv = (pos % 2) == 1 ? true : false;
    auto lit = Literal{varid, inv};
    lit_list.push_back(lit);
    byte &= ~(1 << (7 - pos));
  }
}

END_NONAMESPACE

// @brief ワード中のリテラルをリストに追加する．
void
AlgBase::_word_literal_list(
  AlgPatWord word,
  SizeType base,
  vector<Literal>& lit_list
)
{
  const AlgPatWord H32MASK  = 0xFFFFFFFF00000000ULL;
  const AlgPatWord L32MASK  = 0x00000000FFFFFFFFULL;
  const AlgPatWord HH16MASK = 0xFFFF000000000000ULL;
  const AlgPatWord HL16MASK = 0x0000FFFF00000000ULL;
  const AlgPatWord LH16MASK = 0x00000000FFFF0000ULL;
  const AlgPatWord LL16MASK = 0x000000000000FFFFULL;
  const AlgPatWord HHH8MASK = 0xFF00000000000000ULL;
  const AlgPatWord HHL8MASK = 0x00FF000000000000ULL;
  const AlgPatWord HLH8MASK = 0x0000FF0000000000ULL;
  const AlgPatWord HLL8MASK = 0x000000FF00000000ULL;
  const AlgPatWord LHH8MASK = 0x00000000FF000000ULL;
  const AlgPatWord LHL8MASK = 0x0000000000FF0000ULL;
  const AlgPatWord LLH8MASK = 0x000000000000FF00ULL;
  const AlgPatWord LLL8MASK = 0x00000000000000FFULL;
  // 2分探索で非ゼロの8ビットを取り出して処理する．
  if ( (word & H32MASK) != ALG_ALL0 ) {
    if ( (word & HH16MASK) != ALG_ALL0 ) {
      if ( (word & HHH8MASK) != ALG_ALL0 ) {
	auto byte = static_cast<std::uint8_t>((word >> 56) & 0xFFU);
	byte_literal_list(byte, base, lit_list);
      }
      if ( (word & HHL8MASK) != ALG_ALL0 ) {
	auto byte = static_cast<std::uint8_t>((word >> 48) & 0xFFU);
	byte_literal_list(byte, base + 4, lit_list);
      }
    }
    if ( (word & HL16MASK) != ALG_ALL0 ) {
      if ( (word & HLH8MASK) != ALG_ALL0 ) {
	auto byte = static_cast<std::uint8_t>((word >> 40) & 0xFFU);
	byte_literal_list(byte, base + 8, lit_list);
      }
      if ( (word & HLL8MASK) != ALG_ALL0 ) {
	auto byte = static_cast<std::uint8_t>((word >> 32) & 0xFFU);
	byte_literal_list(byte, base + 12, lit_list);
      }
    }
  }
  if ( (word & L32MASK) != ALG_ALL0 ) {
    if ( (word & LH16MASK) != ALG_ALL0 ) {
      if ( (word & LHH8MASK) != ALG_ALL0 ) {
	auto byte = static_cast<std::uint8_t>((word >> 24) & 0xFFU);
	byte_literal_list(byte, base + 16, lit_list);
      }
      if ( (word & LHL8MASK) != ALG_ALL0 ) {
	auto byte = static_cast<std::uint8_t>((word >> 16) & 0xFFU);
	byte_literal_list(byte, base + 20, lit_list);
      }
    }
    if ( (word & LL16MASK) != ALG_ALL0 ) {
      if ( (word & LLH8MASK) != ALG_ALL0 ) {
	auto byte = static_cast<std::uint8_t>((word >> 8) & 0xFFU);
	byte_literal_list(byte, base + 24, lit_list);
      }
      if ( (word & LLL8MASK) != ALG_ALL0 ) {
	auto byte = static_cast<std::uint8_t>((word >> 0) & 0xFFU);
	byte_literal_list(byte, base + 28, lit_list);
      }
    }
  }
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
