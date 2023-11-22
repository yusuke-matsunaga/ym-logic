
/// @file NodeCollector.cc
/// @brief NodeCollector の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Zdd.h"
#include "ym/ZddInfo.h"
#include "NodeCollector.h"
#include "ZddEdge.h"
#include "ZddNode.h"


BEGIN_NAMESPACE_YM_ZDD

// @brief ノードの情報を取り出す．
vector<ZddInfo>
Zdd::node_info(
  SizeType& root_edge ///< [out] 根の情報を格納する変数
) const
{
  vector<SizeType> redge_list;
  auto node_list = node_info({*this}, redge_list);
  ASSERT_COND( redge_list.size() == 1 );
  root_edge = redge_list[0];
  return node_list;
}

// @brief 複数のZDDのノードの情報を取り出す．
vector<ZddInfo>
Zdd::node_info(
  const vector<Zdd>& bdd_list,
  vector<SizeType>& root_edge_list
)
{
  vector<ZddEdge> edge_list;
  (void) root_list(bdd_list, edge_list);
  NodeCollector nc{edge_list};

  root_edge_list.clear();
  root_edge_list.reserve(bdd_list.size());
  for ( auto root: nc.root_list() ) {
    root_edge_list.push_back(nc.edge2int(root));
  }

  vector<ZddInfo> node_list;
  node_list.reserve(nc.node_list().size());
  for ( auto node: nc.node_list() ) {
    SizeType id = nc.node_id(node);
    ASSERT_COND( id == node_list.size() + 1 );
    SizeType index = node->index();
    SizeType edge0 = nc.edge2int(node->edge0());
    SizeType edge1 = nc.edge2int(node->edge1());
    node_list.push_back(ZddInfo{index, edge0, edge1});
  }
  return node_list;
}


//////////////////////////////////////////////////////////////////////
// クラス NodeCollector
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
NodeCollector::NodeCollector(
  const vector<ZddEdge>& root_list
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
  ZddEdge edge
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

// @brief ZddEdge の情報を整数値に変換する．
SizeType
NodeCollector::edge2int(
  ZddEdge edge
) const
{
  if ( edge.is_zero() ) {
    return 0;
  }
  if ( edge.is_one() ) {
    return 1;
  }
  auto node = edge.node();
  SizeType id = node_id(node);
  return id * 2;
}

END_NAMESPACE_YM_ZDD
