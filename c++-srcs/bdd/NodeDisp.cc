
/// @file NodeDisp.cc
/// @brief NodeDisp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "NodeDisp.h"
#include "BddEdge.h"
#include "BddNode.h"


BEGIN_NAMESPACE_YM_BDD

// @brief 出力する．
void
NodeDisp::write(
  const vector<BddEdge>& root_list
)
{
  for ( auto edge: root_list ) {
    get_node(edge);
  }
  SizeType i = 1;
  for ( auto edge: root_list ) {
    mS << "Root#" << i;
    ++ i;
    mS << ": ";
    write_edge(edge);
    mS << endl;
  }
  for ( auto node: node_list() ) {
    SizeType id = node_id(node);
    mS << setw(6) << id << ": "
      << setw(4) << node->index();
    auto edge0 = node->edge0();
    write_edge(edge0);
    mS << ": ";
    auto edge1 = node->edge1();
    write_edge(edge1);
    mS << endl;
  }
}

// @brief 枝の内容を出力する．
void
NodeDisp::write_edge(
  BddEdge edge
)
{
  if ( edge.is_zero() ) {
    mS << "   ZERO";
  }
  else if ( edge.is_one() ) {
    mS << "    ONE";
  }
  else {
    auto node = edge.node();
    auto inv = edge.inv();
    SizeType id = node_id(node);
    mS << setw(6) << id;
    if ( inv ) {
      mS << "~";
    }
    else {
      mS << " ";
    }
  }
}

END_NAMESPACE_YM_BDD
