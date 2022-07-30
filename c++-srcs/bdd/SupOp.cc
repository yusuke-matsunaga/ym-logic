
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
SupOp::get_step(
  BddEdge edge
)
{
  if ( edge.is_const() ) {
    return BddEdge::one();
  }

  auto node = edge.node();
  if ( mTable.count(node) > 0 ) {
    return mTable.at(node);
  }
  auto index = node->index();
  auto edge0 = node->edge0();
  auto edge1 = node->edge1();
  auto r0 = get_step(edge0);
  auto r1 = get_step(edge1);
  auto tmp = cup_step(r0, r1);
  auto result = new_node(index, BddEdge::zero(), tmp);
  mTable.emplace(node, result);
  return result;
}

// @brief サポートのユニオンを求める．
BddEdge
SupOp::cup_step(
  BddEdge edge0,
  BddEdge edge1
)
{
  ASSERT_COND( !edge0.is_zero() );
  ASSERT_COND( !edge1.is_zero() );

  if ( edge0.is_one() ) {
    return edge1;
  }
  if ( edge1.is_one() ) {
    return edge0;
  }

  auto node0 = edge0.node();
  auto node1 = edge1.node();
  auto index0 = node0->index();
  auto index1 = node1->index();
  auto top = std::min(index0, index1);
  if ( index0 < index1 ) {
    auto tmp = cup_step(node0->edge1(), edge1);
    return new_node(index0, BddEdge::zero(), tmp);
  }
  else if ( index0 == index1 ) {
    auto tmp = cup_step(node0->edge1(), node1->edge1());
    return new_node(index0, BddEdge::zero(), tmp);
  }
  else {
    auto tmp = cup_step(edge0, node1->edge1());
    return new_node(index1, BddEdge::zero(), tmp);
  }
}

// @brief サポートのインターセクションを求める．
BddEdge
SupOp::cap_step(
  BddEdge edge0,
  BddEdge edge1
)
{
  ASSERT_COND( !edge0.is_zero() );
  ASSERT_COND( !edge1.is_zero() );

  if ( edge0.is_one() || edge1.is_one() ) {
    return BddEdge::one();
  }

  auto node0 = edge0.node();
  auto node1 = edge1.node();
  auto index0 = node0->index();
  auto index1 = node1->index();
  auto top = std::min(index0, index1);
  if ( index0 < index1 ) {
    return cap_step(node0->edge1(), edge1);
  }
  else if ( index0 == index1 ) {
    auto tmp = cap_step(node0->edge1(), node1->edge1());
    return new_node(index0, BddEdge::zero(), tmp);
  }
  else {
    return cap_step(edge0, node1->edge1());
  }
}

// @brief サポートのユニオンを求める．
BddEdge
SupOp::diff_step(
  BddEdge edge0,
  BddEdge edge1
)
{
  ASSERT_COND( !edge0.is_zero() );
  ASSERT_COND( !edge1.is_zero() );

  if ( edge0.is_one() ) {
    return BddEdge::one();
  }
  if ( edge1.is_one() ) {
    return edge0;
  }

  auto node0 = edge0.node();
  auto node1 = edge1.node();
  auto index0 = node0->index();
  auto index1 = node1->index();
  if ( index0 < index1 ) {
    auto tmp = diff_step(node0->edge1(), edge1);
    return new_node(index0, BddEdge::zero(), tmp);
  }
  else if ( index0 == index1 ) {
    return diff_step(node0->edge1(), node1->edge1());
  }
  else { // index0 > index1
    return diff_step(edge0, node1->edge1());
  }
}

END_NAMESPACE_YM_BDD
