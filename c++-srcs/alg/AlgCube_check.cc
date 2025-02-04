
/// @file AlgCube_check.cc
/// @brief AlgCube の check_containment/intersect 関係の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/AlgCube.h"


BEGIN_NAMESPACE_YM_ALG

//////////////////////////////////////////////////////////////////////
// クラス AlgCube
//////////////////////////////////////////////////////////////////////

// @brief オペランドのキューブに含まれていたら true を返す．
bool
AlgCube::check_containment(
  const AlgCube& right
) const
{
  if ( variable_num() != right.variable_num() ) {
    throw std::invalid_argument("variable_num() is different from each other");
  }
  auto cube1 = _cube();
  auto end1 = _cube_end(cube1);
  auto cube2 = right._cube();
  for ( ; cube1 != end1; ++ cube1, ++ cube2 ) {
    if ( (~(*cube1) & *cube2) != 0ULL ) {
      return false;
    }
  }
  return true;
}

// @brief 2つのキューブに共通なリテラルがあれば true を返す．
bool
AlgCube::check_intersect(
  const AlgCube& right
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

END_NAMESPACE_YM_ALG
