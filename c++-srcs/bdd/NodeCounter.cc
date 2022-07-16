
/// @file NodeCounter.cc
/// @brief NodeCounter の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "NodeCounter.h"
#include "BddEdge.h"
#include "BddNode.h"


BEGIN_NAMESPACE_YM_BDD

// @brief ノード数を数える．
SizeType
NodeCounter::count(
  const vector<BddEdge>& root_list
)
{
  mNodeSet.clear();
  for ( auto edge: root_list ) {
    count_step(edge);
  }
  return mNodeSet.size();
}

// @brief count の下請け関数
void
NodeCounter::count_step(
  BddEdge edge
)
{
  if ( edge.is_const() ) {
    return;
  }
  auto node = edge.node();
  if ( mNodeSet.count(node) == 0 ) {
    mNodeSet.emplace(node);
    count_step(node->edge0());
    count_step(node->edge1());
  }
}

END_NAMESPACE_YM_BDD
