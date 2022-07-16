
/// @file NodeCollector.cc
/// @brief NodeCollector の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "NodeCollector.h"
#include "BddEdge.h"
#include "BddNode.h"


BEGIN_NAMESPACE_YM_BDD

// @brief ノードを集める．
void
NodeCollector::get_node(
  BddEdge edge
)
{
  if ( edge.is_const() ) {
    return;
  }

  auto node = edge.node();
  if ( mNodeMap.count(node) == 0 ) {
    SizeType id = mNodeList.size();
    mNodeMap.emplace(node, id);
    mNodeList.push_back(node);
    get_node(node->edge0());
    get_node(node->edge1());
  }
}

END_NAMESPACE_YM_BDD
