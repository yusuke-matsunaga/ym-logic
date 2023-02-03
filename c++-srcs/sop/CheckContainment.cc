
/// @file CheckContainment.cc
/// @brief CheckContainment の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "CheckContainment.h"


BEGIN_NAMESPACE_YM_SOP

//////////////////////////////////////////////////////////////////////
// クラス SopCube
//////////////////////////////////////////////////////////////////////

// @brief オペランドのキューブに含まれていたら true を返す．
bool
SopCube::check_containment(
  const SopCube& right
) const
{
  if ( mVariableNum != right.mVariableNum ) {
    throw std::invalid_argument("variable_num() is different from each other");
  }
  CheckContainment check_containment{mVariableNum};
  return check_containment(mBody, right.mBody);
}


//////////////////////////////////////////////////////////////////////
// クラス CheckContainment
//////////////////////////////////////////////////////////////////////

// @brief 一方のキューブが他方のキューブに含まれているか調べる．
bool
CheckContainment::operator()(
  const SopBitVect* bv1,
  const SopBitVect* bv2
)
{
  auto bv1_end = _calc_offset(bv1, 1);
  while ( bv1 != bv1_end ) {
    if ( (~(*bv1) & *bv2) != 0ULL ) {
      return false;
    }
    ++ bv1;
    ++ bv2;
  }
  return true;
}

END_NAMESPACE_YM_SOP
