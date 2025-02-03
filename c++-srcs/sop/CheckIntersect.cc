
/// @file CheckIntersect.cc
/// @brief CheckIntersect の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
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
  if ( variable_num() != right.variable_num() ) {
    throw std::invalid_argument("variable_num() is different from each other");
  }
  CheckIntersect check_intersect{variable_num()};
  return check_intersect(_cube_begin(mBody),
			 _cube_begin(right.mBody));
}


//////////////////////////////////////////////////////////////////////
// クラス CheckIntersect
//////////////////////////////////////////////////////////////////////

// @brief ２つのキューブに共通なリテラルがあれば true を返す．
bool
CheckIntersect::operator()(
  SopBitVectConstIter src1_cube,
  SopBitVectConstIter src2_cube
)
{
  auto src1_end = _cube_end(src1_cube);
  for ( ; src1_cube != src1_end; ++ src1_cube, ++ src2_cube ) {
    if ( (*src1_cube & *src2_cube) != 0ULL ) {
      return true;
    }
  }
  return false;
}

END_NAMESPACE_YM_SOP
