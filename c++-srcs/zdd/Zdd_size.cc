
/// @file Zdd.cc
/// @brief Zdd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Zdd.h"


BEGIN_NAMESPACE_YM_ZDD

// @brief ノード数を返す．
SizeType
Zdd::size() const
{
  if ( mMgr == nullptr ) {
    return 0;
  }
  SizeType dummy;
  auto node_list = node_info(dummy);
  return node_list.size();
}

// @brief 複数のZDDのノード数を数える．
SizeType
Zdd::size(
  const vector<Zdd>& bdd_list
)
{
  if ( bdd_list.empty() ) {
    return 0;
  }
  vector<SizeType> root_list;
  auto node_list = node_info(bdd_list, root_list);
  return node_list.size();
}

END_NAMESPACE_YM_ZDD
