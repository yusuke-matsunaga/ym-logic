
/// @file SopMgr.cc
/// @brief SopMgr の実装ファイル
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

BEGIN_NONAMESPACE

// 表引きを使ってリテラル数の計算を行う．
int
_count(
  SopPatWord pat
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
  const SopBlockRef& block
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
SopMgr::literal_num(
  const SopBlockRef& block,
  SizeType varid,
  bool inv
) const
{
  auto blk = _block_pos(varid);
  auto mask = get_mask(varid, inv);
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
SopMgr::literal_list(
  const SopBlockRef& block ///< [in] 対象のブロック
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
      auto pat = get_pat(cube, var);
      if ( pat == SopPat::_1 ) {
	tmp_list.push_back(Literal{var, false});
      }
      else if ( pat == SopPat::_0 ) {
	tmp_list.push_back(Literal{var, true});
      }
    }
    cube_list.push_back(tmp_list);
  }

  return cube_list;
}

// @brief カバーからキューブを取り出す．
SopCube
SopMgr::get_cube(
  const SopBlockRef& block,
  SizeType cube_id
) const
{
  auto dst_bv = new_cube();
  auto dst_cube = _cube_begin(dst_bv);
  auto src_cube = _cube_begin(block.body, cube_id);
  cube_copy(dst_cube, src_cube);
  return make_cube(std::move(dst_bv));
}

// @brief ビットベクタからハッシュ値を計算する．
SizeType
SopMgr::hash(
  const SopBitVectRef& block
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
SopMgr::to_expr(
  const SopBitVectRef& block,
)
{
  auto ans = Expr::zero();
  auto start = _cube_begin(block.body());
  auto end = _cube_end(start, block.cube_num);
  for ( auto p = start; p != end; p += _cube_size() ) {
    auto prod = Expr::one();
    for ( SizeType var = 0; var < variable_num(); ++ var ) {
      auto pat = get_pat(p, var);
      if ( pat == SopPat::_1 ) {
	prod &= Expr::posi_literal(var);
      }
      else if ( pat == SopPat::_0 ) {
	prod &= Expr::nega_literal(var);
      }
    }
    ans |= prod;
  }
  return ans;
}

// @brief カバー/キューブの内容を出力する．
void
SopMgr::print(
  ostream& s,
  const SopBitVect& bv,
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

// @brief 内容を出力する(デバッグ用)．
void
SopMgr::debug_print(
  ostream& s,
  const SopBlockRef& block
)
{
  for ( SizeType i = 0; i < block.cube_num; ++ i ) {
    auto cube = _calc_offset(block.body, i);
    for ( SizeType j = 0; j < variable_num(); ++ j ) {
      s << " ";
      auto pat = get_pat(cube, j);
      switch ( pat ) {
      case SopPat::_X: s << "00"; break;
      case SopPat::_0: s << "01"; break;
      case SopPat::_1: s << "10"; break;
      default:         s << "--"; break;
      }
    }
    s << endl;
  }
}

// @brief mTmpBlock に必要な領域を確保する．
void
SopMgr::_resize_buff(
  SizeType req_size
)
{
  if ( req_size <= mTmpBlock.cube_num ) {
    return;
  }
  auto size = mTmpBlock.cube_num;
  if ( size == 0 ) {
    size = 1;
  }
  while ( mTmpBlock.cube_num < req_size ) {
    size <<= 1;
  }

  mTmpBlock = _new_body(size);
}

END_NAMESPACE_YM_SOP
