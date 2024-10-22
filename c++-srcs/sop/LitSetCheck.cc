
/// @file LitSetCheck.cc
/// @brief LitSetCheck の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017, 2018, 2022, 2023 Yusuke Matsunaga
/// All rights reserved.

#include "LitSet.h"
#include "LitSetCheck.h"
#include "ym/Literal.h"


BEGIN_NAMESPACE_YM_SOP

//////////////////////////////////////////////////////////////////////
// クラス SopCube
//////////////////////////////////////////////////////////////////////

// @brief 該当するリテラルが含まれているか調べる．
bool
LitSet::is_in(
  Literal lit
) const
{
  LitSetCheck litset_check{mVariableNum};
  return litset_check(mBody, lit);
}


//////////////////////////////////////////////////////////////////////
// クラス LitSetCheck
//////////////////////////////////////////////////////////////////////

// @brief 要素のチェック
bool
LitSetCheck::operator()(
  SopBitVect* bv,
  Literal lit
)
{
  auto varid = lit.varid();
  auto inv = lit.is_negative();
  auto blk = _block_pos(varid);
  auto sft = _shift_num(varid);
  auto pat = bitvect(inv);
  auto mask = pat << sft;
  if ( bv[blk] & mask ) {
    return true;
  }
  else {
    return false;
  }
}

END_NAMESPACE_YM_SOP
