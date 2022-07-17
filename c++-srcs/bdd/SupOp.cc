
/// @file SupOp.cc
/// @brief SupOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "SupOp.h"


BEGIN_NAMESPACE_YM_BDD

// @brief サポートを表すBDDを返す．
BddEdge
SupOp::op_step(
  BddEdge edge
)
{
  if ( edge.is_const() ) {
    return BddEdge::make_one();
  }

  auto node = edge.node();
  if ( mTable.count(node) > 0 ) {
    return mTable.at(node);
  }
  auto index = node->index();
  auto edge0 = node->edge0();
  auto edge1 = node->edge1();
  auto r0 = op_step(edge0);
  auto r1 = op_step(edge1);
  auto tmp = merge_step(r0, r1);
  auto result = new_node(index, BddEdge::make_zero(), tmp);
  mTable.emplace(node, result);
  return result;
}

// @brief 2つのBDDをマージする．
BddEdge
SupOp::merge_step(
  BddEdge edge0,
  BddEdge edge1
)
{
  auto node0 = edge0.node();
  auto node1 = edge1.node();
  auto index0 = node0->index();
  auto index1 = node1->index();
  auto top = std::min(index0, index1);
  if ( index0 < index1 ) {
    auto tmp = merge_step(node0->edge1(), edge1);
    return new_node(index0, BddEdge::make_zero(), tmp);
  }
  else if ( index0 == index1 ) {
    auto tmp = merge_step(node0->edge1(), node1->edge1());
    return new_node(index0, BddEdge::make_zero(), tmp);
  }
  else {
    auto tmp = merge_step(edge0, node1->edge1());
    return new_node(index1, BddEdge::make_zero(), tmp);
  }
}

END_NAMESPACE_YM_BDD
