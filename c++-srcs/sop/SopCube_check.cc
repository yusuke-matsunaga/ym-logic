
/// @file SopCube_check.cc
/// @brief SopCube の check_containment/intersect 関係の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/SopCube.h"


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
  auto cube1 = _cube();
  auto end1 = _cube_end(cube1);
  auto cube2 = right._cube();
  for ( ; cube1 != end1; ++ cube1, ++ cube2 ) {
    auto pat1 = *cube1;
    auto pat2 = *cube2;
    if ( (pat1 & pat2) != pat2 ) {
      return false;
    }
  }
  return true;
}

#if 0
// @brief 2つのキューブに共通なリテラルがあれば true を返す．
bool
SopCube::check_intersect(
  const SopCube& right
) const
{
  if ( variable_num() != right.variable_num() ) {
    throw std::invalid_argument("variable_num() is different from each other");
  }
  auto cube1 = _cube();
  auto end1 = _cube_end(cube1);
  auto cube2 = right._cube();
  for ( ; cube1 != end1; ++ cube1, ++ cube2 ) {
    if ( (*cube1 & *cube2) != 0ULL ) {
      return true;
    }
  }
  return false;
}
#endif

END_NAMESPACE_YM_SOP
