
/// @file LitSetUnion.cc
/// @brief LitSetUnion の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017, 2018, 2022, 2023 Yusuke Matsunaga
/// All rights reserved.

#include "LitSet.h"
#include "LitSetUnion.h"


BEGIN_NAMESPACE_YM_SOP

//////////////////////////////////////////////////////////////////////
// クラス LitSet
//////////////////////////////////////////////////////////////////////

// @brief ユニオン演算付き代入
LitSet&
LitSet::operator+=(
  const LitSet& right
)
{
  LitSetUnion litset_union{mVariableNum};
  litset_union(mBody, right.mBody);

  return *this;
}


//////////////////////////////////////////////////////////////////////
// クラス LitSetUnion
//////////////////////////////////////////////////////////////////////

// @brief ユニオン演算
void
LitSetUnion::operator()(
  SopBitVect* dst_bv,
  const SopBitVect* src_bv
)
{
  auto dst_bv_end = _calc_offset(dst_bv, 1);
  for ( ; dst_bv != dst_bv_end; ++ dst_bv, ++ src_bv ) {
    *dst_bv |= *src_bv;
  }
}

END_NAMESPACE_YM_SOP
