
/// @file LitSetCheck.cc
/// @brief LitSetCheck の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
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
  LitSetCheck litset_check{variable_num()};
  return litset_check(mBody, lit);
}


//////////////////////////////////////////////////////////////////////
// クラス LitSetCheck
//////////////////////////////////////////////////////////////////////

// @brief 要素のチェック
bool
LitSetCheck::operator()(
  const SopBitVect& bv,
  Literal lit
)
{
  auto varid = lit.varid();
  auto inv = lit.is_negative();
  auto blk = _block_pos(varid);
  auto mask = get_mask(varid, inv);
  if ( bv[blk] & mask ) {
    return true;
  }
  else {
    return false;
  }
}

END_NAMESPACE_YM_SOP
