
/// @file CheckContainment.cc
/// @brief CheckContainment の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
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
  if ( variable_num() != right.variable_num() ) {
    throw std::invalid_argument("variable_num() is different from each other");
  }
  CheckContainment check_containment{variable_num()};
  return check_containment(mBody.begin(), right.mBody.begin());
}


//////////////////////////////////////////////////////////////////////
// クラス CheckContainment
//////////////////////////////////////////////////////////////////////

// @brief 一方のキューブが他方のキューブに含まれているか調べる．
bool
CheckContainment::operator()(
  SopBitVectConstIter cube1,
  SopBitVectConstIter cube2
)
{
  auto cube1_end = _cube_end(cube1);
  for ( ; cube1 != cube1_end; ++ cube1, ++ cube2 ) {
    if ( (~(*cube1) & *cube2) != 0ULL ) {
      return false;
    }
  }
  return true;
}

END_NAMESPACE_YM_SOP
