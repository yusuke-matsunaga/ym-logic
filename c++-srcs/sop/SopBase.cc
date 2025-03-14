
/// @file SopBase.cc
/// @brief SopBase の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/SopBase.h"


BEGIN_NAMESPACE_YM_SOP

//////////////////////////////////////////////////////////////////////
// クラス SopBase
//////////////////////////////////////////////////////////////////////

BEGIN_NONAMESPACE

// 表引きを使ってリテラル数の計算を行う．
inline
int
_count(
  SopPatWord pat
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
SopBase::_literal_num(
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

// @brief キューブのリテラル数を返す．
SizeType
SopBase::_cube_literal_num(
  Cube src_cube
) const
{
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
    // utils/gen_soplbit_tbl.py で生成
#include "soplbit_tbl.h"
  };

  while ( byte != 0xFFU ) {
    auto pos = table[byte];
    SizeType varid = (pos >> 1) + base;
    bool inv = (pos % 2) == 1 ? true : false;
    auto lit = Literal{varid, inv};
    lit_list.push_back(lit);
    byte |= (1 << (7 - pos));
  }
}

END_NONAMESPACE

// @brief ワード中のリテラルをリストに追加する．
void
SopBase::_word_literal_list(
  SopPatWord word,
  SizeType base,
  vector<Literal>& lit_list
)
{
  const SopPatWord H32MASK  = 0xFFFFFFFF00000000ULL;
  const SopPatWord L32MASK  = 0x00000000FFFFFFFFULL;
  const SopPatWord HH16MASK = 0xFFFF000000000000ULL;
  const SopPatWord HL16MASK = 0x0000FFFF00000000ULL;
  const SopPatWord LH16MASK = 0x00000000FFFF0000ULL;
  const SopPatWord LL16MASK = 0x000000000000FFFFULL;
  const SopPatWord HHH8MASK = 0xFF00000000000000ULL;
  const SopPatWord HHL8MASK = 0x00FF000000000000ULL;
  const SopPatWord HLH8MASK = 0x0000FF0000000000ULL;
  const SopPatWord HLL8MASK = 0x000000FF00000000ULL;
  const SopPatWord LHH8MASK = 0x00000000FF000000ULL;
  const SopPatWord LHL8MASK = 0x0000000000FF0000ULL;
  const SopPatWord LLH8MASK = 0x000000000000FF00ULL;
  const SopPatWord LLL8MASK = 0x00000000000000FFULL;
  // 2分探索で非ゼロの8ビットを取り出して処理する．
  if ( (word & H32MASK) != H32MASK ) {
    if ( (word & HH16MASK) != HH16MASK ) {
      if ( (word & HHH8MASK) != HHH8MASK ) {
	auto byte = static_cast<std::uint8_t>((word >> 56) & 0xFFU);
	byte_literal_list(byte, base, lit_list);
      }
      if ( (word & HHL8MASK) != HHL8MASK ) {
	auto byte = static_cast<std::uint8_t>((word >> 48) & 0xFFU);
	byte_literal_list(byte, base + 4, lit_list);
      }
    }
    if ( (word & HL16MASK) != HL16MASK ) {
      if ( (word & HLH8MASK) != HLH8MASK ) {
	auto byte = static_cast<std::uint8_t>((word >> 40) & 0xFFU);
	byte_literal_list(byte, base + 8, lit_list);
      }
      if ( (word & HLL8MASK) != HLL8MASK ) {
	auto byte = static_cast<std::uint8_t>((word >> 32) & 0xFFU);
	byte_literal_list(byte, base + 12, lit_list);
      }
    }
  }
  if ( (word & L32MASK) != L32MASK ) {
    if ( (word & LH16MASK) != LH16MASK ) {
      if ( (word & LHH8MASK) != LHH8MASK ) {
	auto byte = static_cast<std::uint8_t>((word >> 24) & 0xFFU);
	byte_literal_list(byte, base + 16, lit_list);
      }
      if ( (word & LHL8MASK) != LHL8MASK ) {
	auto byte = static_cast<std::uint8_t>((word >> 16) & 0xFFU);
	byte_literal_list(byte, base + 20, lit_list);
      }
    }
    if ( (word & LL16MASK) != LL16MASK ) {
      if ( (word & LLH8MASK) != LLH8MASK ) {
	auto byte = static_cast<std::uint8_t>((word >> 8) & 0xFFU);
	byte_literal_list(byte, base + 24, lit_list);
      }
      if ( (word & LLL8MASK) != LLL8MASK ) {
	auto byte = static_cast<std::uint8_t>((word >> 0) & 0xFFU);
	byte_literal_list(byte, base + 28, lit_list);
      }
    }
  }
}

// @brief Expr に変換する．
Expr
SopBase::_to_expr(
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
  auto cube_list = _literal_list(cube_num, chunk);
  auto ans = Expr::zero();
  for ( auto lits: cube_list ) {
    auto prod = Expr::one();
    for ( auto lit: lits ) {
      prod &= Expr::literal(lit);
    }
    ans |= prod;
  }
  return ans;
}

// @brief カバー/キューブの内容を出力する．
void
SopBase::_print(
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
SopBase::_print(
  ostream& s,
  Cube cube,
  const vector<string>& varname_list
) const
{
  auto lit_list = _cube_literal_list(cube);
  if ( lit_list.empty() ) {
    s << "{}";
  }
  else {
    const char* spc = "";
    for ( auto lit: lit_list ) {
      auto var = lit.varid();
      auto inv = lit.is_negative();
      string varname;
      if ( varname_list.size() > var ) {
	varname = varname_list[var];
      }
      else {
	ostringstream buf;
	buf << "v" << var;
	varname = buf.str();
      }
      s << spc << varname;
      spc = " ";
      if ( lit.is_negative() ) {
	s << "'";
      }
    }
  }
}

// @brief カバーの内容を blif のカバー形式で出力する．
void
SopBase::_write_cover(
  ostream& s,
  const Chunk& chunk,
  SizeType begin,
  SizeType end
) const
{
  auto cube_list = _cube_list(chunk, begin, end);
  for ( auto cube: cube_list ) {
    _write_cube(s, cube);
  }
}

// @brief キューブの内容を blif のカバー形式で出力する．
void
SopBase::_write_cube(
  ostream& s,
  Cube cube
) const
{
  for ( SizeType i = 0; i < variable_num(); ++ i ) {
    switch ( _get_pat(cube, i) ) {
    case SopPat::__: s << '?'; break;
    case SopPat::_1: s << '1'; break;
    case SopPat::_0: s << '0'; break;
    case SopPat::_X: s << '-'; break;
    }
  }
  s << endl;
}

// @brief 内容を出力する(デバッグ用)．
void
SopBase::_debug_print(
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
SopBase::_debug_print(
  ostream& s,
  Cube cube
) const
{
  for ( SizeType var = 0; var < variable_num(); ++ var ) {
    s << " ";
    auto pat = _get_pat(cube, var);
    switch ( pat ) {
    case SopPat::__: s << "00"; break;
    case SopPat::_0: s << "01"; break;
    case SopPat::_1: s << "10"; break;
    case SopPat::_X: s << "11"; break;
    }
  }
  s << endl;
}

END_NAMESPACE_YM_SOP
