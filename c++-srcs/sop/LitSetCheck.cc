
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
  auto var_id = lit.varid();
  auto blk = _block_pos(var_id);
  auto sft = _shift_num(var_id);
  auto pat = lit2bv(lit);
  auto mask = pat << sft;
  if ( bv[blk] & mask ) {
    return true;
  }
  else {
    return false;
  }
}

END_NAMESPACE_YM_SOP
