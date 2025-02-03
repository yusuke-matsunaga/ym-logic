
/// @file LitSetIntersect.cc
/// @brief LitSetIntersect の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "LitSet.h"
#include "LitSetIntersect.h"
#include "ym/SopCube.h"


BEGIN_NAMESPACE_YM_SOP

//////////////////////////////////////////////////////////////////////
// クラス LitSet
//////////////////////////////////////////////////////////////////////

// @brief 引数のキューブ中のリテラルをひとつでも含んでいたら true を返す．
bool
LitSet::check_intersect(
  const SopCube& right
) const
{
  if ( variable_num() != right.variable_num() ) {
    throw std::invalid_argument{"variable_num() mismatch"};
  }

  LitSetIntersect litset_intersect{variable_num()};
  return litset_intersect(mBody, right.body());
}


//////////////////////////////////////////////////////////////////////
// クラス LitSetIntersect
//////////////////////////////////////////////////////////////////////

// @brief 要素のチェック
bool
LitSetIntersect::operator()(
  const SopBitVect& bv1,
  const SopBitVect& bv2
)
{
  auto iter1 = bv1.begin();
  auto end1 = _cube_end(iter1);
  auto iter2 = bv2.begin();
  for ( ; iter1 != end1; ++ iter1, ++ iter2 ) {
    if ( (*iter1 & *iter2) != 0ULL ) {
      return true;
    }
  }
  return false;
}

END_NAMESPACE_YM_SOP
