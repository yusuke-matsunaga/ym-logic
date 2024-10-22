
/// @file SopMgr.cc
/// @brief SopMgr の実装ファイル
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

BEGIN_NONAMESPACE

// 表引きを使ってリテラル数の計算を行う．
int
_count(
  SopBitVect pat
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
  const SopBitVect* bv,
  SizeType cube_num
)
{
  // 8 ビットごとに区切って表引きで計算する．
  auto bv_end = _calc_offset(bv, cube_num);
  SizeType ans = 0;
  for ( ; bv != bv_end; ++ bv ) {
    auto pat = *bv;
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
  const SopBlock& block,
  SizeType varid,
  bool inv
)
{
  auto blk = _block_pos(varid);
  auto sft = _shift_num(varid);
  auto pat = bitvect(inv);
  auto mask = pat << sft;
  auto bv = block.body;
  auto bv_end = _calc_offset(bv, block.cube_num);
  SizeType ans = 0;
  for ( ; bv != bv_end; bv += _cube_size() ) {
    if ( (bv[blk] & mask) == mask ) {
      ++ ans;
    }
  }
  return ans;
}

// @brief ビットベクタからハッシュ値を計算する．
SizeType
SopMgr::hash(
  const SopBlock& src1
)
{
  // キューブは常にソートされているので
  // 順番は考慮する必要はない．
  // ただおなじキューブの中のビットに関しては
  // 本当は区別しなければならないがどうしようもないので
  // 16ビットに区切って単純に XOR を取る．
  SizeType ans = 0;
  auto bv = src1.body;
  auto bv_end = _calc_offset(bv, src1.cube_num);
  for ( ; bv != bv_end; ++ bv ) {
    auto pat = *bv;
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
  const SopBlock& block
)
{
  auto bv = block.body;
  auto ans = Expr::zero();
  for ( SizeType c = 0; c < block.cube_num; ++ c ) {
    auto prod = Expr::one();
    for ( SizeType var = 0; var < variable_num(); ++ var ) {
      auto pat = get_pat(bv, c, var);
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
  const SopBitVect* bv,
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
  const SopBitVect* bv,
  SizeType num
)
{
  for ( SizeType i = 0; i < num; ++ i ) {
    for ( SizeType j = 0; j < variable_num(); ++ j ) {
      s << " ";
      auto pat = get_pat(bv, i, j);
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

// @brief mTmpBuff に必要な領域を確保する．
// @param[in] req_size 要求するキューブ数
void
SopMgr::_resize_buff(
  SizeType req_size
)
{
  SizeType old_size = mTmpBuffSize;
  if ( mTmpBuffSize == 0 ) {
    mTmpBuffSize = 1;
  }
  while ( mTmpBuffSize < req_size ) {
    mTmpBuffSize <<= 1;
  }

  if ( old_size < mTmpBuffSize ) {
    _delete_body(mTmpBuff);
  }
  mTmpBuff = _new_body(mTmpBuffSize);
}

END_NAMESPACE_YM_SOP
