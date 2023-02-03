
/// @file CheckIntersect.cc
/// @brief CheckIntersect の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "CheckIntersect.h"


BEGIN_NAMESPACE_YM_SOP

//////////////////////////////////////////////////////////////////////
// クラス SopCube
//////////////////////////////////////////////////////////////////////

// @brief 2つのキューブに共通なリテラルがあれば true を返す．
bool
SopCube::check_intersect(
  const SopCube& right
) const
{
  if ( mVariableNum != right.mVariableNum ) {
    throw std::invalid_argument("variable_num() is different from each other");
  }
  CheckIntersect check_intersect{mVariableNum};
  return check_intersect(mBody, right.mBody);
}


//////////////////////////////////////////////////////////////////////
// クラス CheckIntersect
//////////////////////////////////////////////////////////////////////

// @brief ２つのキューブに共通なリテラルがあれば true を返す．
bool
CheckIntersect::operator()(
  const SopBitVect* bv1,
  const SopBitVect* bv2
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
