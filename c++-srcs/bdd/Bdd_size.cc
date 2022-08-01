
/// @file Bdd.cc
/// @brief Bdd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Bdd.h"


BEGIN_NAMESPACE_YM_BDD

// @brief ノード数を返す．
SizeType
Bdd::size() const
{
  if ( mMgr == nullptr ) {
    return 0;
  }
  SizeType dummy;
  auto node_list = node_info(dummy);
  return node_list.size();
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
  vector<SizeType> root_list;
  auto node_list = node_info(bdd_list, root_list);
  return node_list.size();
}

END_NAMESPACE_YM_BDD
