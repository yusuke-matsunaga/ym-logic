
/// @file LitSetIntersect.cc
/// @brief LitSetIntersect の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017, 2018, 2022, 2023 Yusuke Matsunaga
/// All rights reserved.

#include "LitSet.h"
#include "LitSetIntersect.h"
#include "ym/SopCube.h"


BEGIN_NAMESPACE_YM_SOP

//////////////////////////////////////////////////////////////////////
// クラス SopCube
//////////////////////////////////////////////////////////////////////

// @brief 引数のキューブ中のリテラルをひとつでも含んでいたら true を返す．
bool
LitSet::check_intersect(
  const SopCube& right
) const
{
  if ( mVariableNum != right.variable_num() ) {
    throw std::invalid_argument{"variable_num() mismatch"};
  }

  LitSetIntersect litset_intersect{mVariableNum};
  return litset_intersect(mBody, right.block().body);
}


//////////////////////////////////////////////////////////////////////
// クラス LitSetIntersect
//////////////////////////////////////////////////////////////////////

// @brief 要素のチェック
bool
LitSetIntersect::operator()(
  SopBitVect* bv1,
  SopBitVect* bv2
)
{
  auto bv1_end = _calc_offset(bv1, 1);
  while ( bv1 != bv1_end ) {
    if ( (*bv1 & *bv2) != 0ULL ) {
      return true;
    }
    ++ bv1;
    ++ bv2;
  }
  return false;
}

END_NAMESPACE_YM_SOP
