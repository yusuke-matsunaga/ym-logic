
/// @file NodeCollector.cc
/// @brief NodeCollector の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "NodeCollector.h"
#include "DdInfo.h"
#include "DdEdge.h"
#include "DdNode.h"


BEGIN_NAMESPACE_YM_DD

//////////////////////////////////////////////////////////////////////
// クラス NodeCollector
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
NodeCollector::NodeCollector(
  const vector<DdEdge>& root_list
)
{
  mRootList.reserve(root_list.size());
  for ( auto root: root_list ) {
    get_node(root);
    mRootList.push_back(root);
  }
}

// @brief ノードを集める．
void
NodeCollector::get_node(
  DdEdge edge
)
{
  if ( edge.is_const() ) {
    return;
  }

  auto node = edge.node();
  if ( mNodeMap.count(node) == 0 ) {
    get_node(node->edge0());
    get_node(node->edge1());
    SizeType id = mNodeList.size() + 1;
    mNodeMap.emplace(node, id);
    mNodeList.push_back(node);
  }
}

// @brief DdEdge の情報を整数値に変換する．
SizeType
NodeCollector::edge2int(
  DdEdge edge
) const
{
  if ( edge.is_zero() ) {
    return 0;
  }
  if ( edge.is_one() ) {
    return 1;
  }
  auto node = edge.node();
  auto inv = edge.inv();
  SizeType id = node_id(node);
  return id * 2 + static_cast<SizeType>(inv);
}

END_NAMESPACE_YM_DD
