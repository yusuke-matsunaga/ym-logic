
/// @file SopBase_cube.cc
/// @brief SopBase のキューブ関係の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/SopBase.h"


BEGIN_NAMESPACE_YM_SOP

//////////////////////////////////////////////////////////////////////
// クラス SopBase
//////////////////////////////////////////////////////////////////////

// @brief キューブの内容をリテラルのリストのリストに変換する．
vector<Literal>
SopBase::_cube_literal_list(
  Cube cube
) const
{
  vector<Literal> lit_list;
  lit_list.reserve(_cube_literal_num(cube));
#if 1
  auto cube_end = _cube_end(cube);
  for ( ; cube != cube_end; ++ cube ) {
    _word_literal_list(*cube, lit_list);
  }
#else
  for ( SizeType var = 0; var < variable_num(); ++ var ) {
    auto pat = _get_pat(cube, var);
    if ( pat == SopPat::_1 ) {
      lit_list.push_back(Literal{var, false});
    }
    else if ( pat == SopPat::_0 ) {
      lit_list.push_back(Literal{var, true});
    }
  }
#endif
  return lit_list;
}

END_NAMESPACE_YM_SOP
