
/// @file Bdd_display.cc
/// @brief Bdd::display の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Bdd.h"
#include "BddEdge.h"
#include "NodeDisp.h"


BEGIN_NAMESPACE_YM_BDD

// @brief 内容を出力する．
void
Bdd::display(
  ostream& s
) const
{
  if ( mMgr == nullptr ) {
    s << "Invalid BDD" << endl;
  }
  else {
    NodeDisp nd{s};
    nd.write({mRoot});
  }
}

END_NAMESPACE_YM_BDD
