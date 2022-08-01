
/// @file NodeCollector.cc
/// @brief NodeCollector の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Bdd.h"
#include "ym/BddInfo.h"
#include "NodeCollector.h"
#include "BddEdge.h"
#include "BddNode.h"


BEGIN_NAMESPACE_YM_BDD

// @brief ノードの情報を取り出す．
vector<BddInfo>
Bdd::node_info(
  SizeType& root_edge ///< [out] 根の情報を格納する変数
) const
{
  vector<SizeType> redge_list;
  auto node_list = node_info({*this}, redge_list);
  ASSERT_COND( redge_list.size() == 1 );
  root_edge = redge_list[0];
  return node_list;
}

// @brief 複数のBDDのノードの情報を取り出す．
vector<BddInfo>
Bdd::node_info(
  const vector<Bdd>& bdd_list,
  vector<SizeType>& root_edge_list
)
{
  vector<BddEdge> edge_list;
  (void) root_list(bdd_list, edge_list);
  NodeCollector nc{edge_list};

  root_edge_list.clear();
  root_edge_list.reserve(bdd_list.size());
  for ( auto root: nc.root_list() ) {
    root_edge_list.push_back(nc.edge2int(root));
  }

  vector<BddInfo> node_list;
  node_list.reserve(nc.node_list().size());
  for ( auto node: nc.node_list() ) {
    SizeType id = nc.node_id(node);
    SizeType index = node->index();
    SizeType edge0 = nc.edge2int(node->edge0());
    SizeType edge1 = nc.edge2int(node->edge1());
    node_list.push_back(BddInfo{id, index, edge0, edge1});
  }
  return node_list;
}


//////////////////////////////////////////////////////////////////////
// クラス NodeCollector
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
NodeCollector::NodeCollector(
  const vector<BddEdge>& root_list
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
  BddEdge edge
)
{
  if ( edge.is_const() ) {
    return;
  }

  auto node = edge.node();
  if ( mNodeMap.count(node) == 0 ) {
    SizeType id = mNodeList.size() + 1;
    mNodeMap.emplace(node, id);
    mNodeList.push_back(node);
    get_node(node->edge0());
    get_node(node->edge1());
  }
}

// @brief BddEdge の情報を整数値に変換する．
SizeType
NodeCollector::edge2int(
  BddEdge edge
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

END_NAMESPACE_YM_BDD
