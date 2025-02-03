
/// @file LitSetUnion.cc
/// @brief LitSetUnion の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
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
  LitSetUnion litset_union{variable_num()};
  litset_union(mBody, right.mBody);

  return *this;
}


//////////////////////////////////////////////////////////////////////
// クラス LitSetUnion
//////////////////////////////////////////////////////////////////////

// @brief ユニオン演算
void
LitSetUnion::operator()(
  SopBitVect& dst_bv,
  const SopBitVect& src_bv
)
{
  auto dst_iter = _cube_begin(dst_bv);
  auto dst_end = _cube_end(dst_iter);
  auto src_iter = _cube_begin(src_bv);
  for ( ; dst_iter != dst_end; ++ dst_iter, ++ src_iter ) {
    *dst_iter |= *src_iter;
  }
}

END_NAMESPACE_YM_SOP
