
/// @file Bdd.cc
/// @brief Bdd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Bdd.h"
#include "BddEdge.h"
#include "NodeCounter.h"


BEGIN_NAMESPACE_YM_BDD

// @brief ノード数を返す．
SizeType
Bdd::size() const
{
  if ( mMgr == nullptr ) {
    return 0;
  }
  NodeCounter nc;
  return nc.count({mRoot});
}

// @brief 複数のBDDのノード数を数える．
SizeType
Bdd::size(
  const vector<Bdd>& bdd_list
)
{
  if ( bdd_list.empty() ) {
    return 0;
  }
  vector<BddEdge> edge_list;
  (void) root_list(bdd_list, edge_list);
  NodeCounter nc;
  return nc.count(edge_list);
}

END_NAMESPACE_YM_BDD
