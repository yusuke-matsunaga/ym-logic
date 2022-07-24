
/// @file CopyOp.cc
/// @brief CopyOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "CopyOp.h"


BEGIN_NAMESPACE_YM_BDD

// @brief コピーする．
BddEdge
CopyOp::copy_step(
  BddEdge edge
)
{
  if ( edge.is_zero() ) {
    return BddEdge::zero();
  }
  if ( edge.is_one() ) {
    return BddEdge::one();
  }

  auto node = edge.node();
  auto inv = edge.inv();
  auto index = node->index();
  if ( mTable.count(node) > 0 ) {
    auto rnode = mTable.at(node);
    return BddEdge{rnode, inv};
  }
  auto edge0 = node->edge0(inv);
  auto edge1 = node->edge1(inv);
  auto redge0 = copy_step(edge0);
  auto redge1 = copy_step(edge1);
  auto redge = new_node(index, redge0, redge1);
  auto rnode = redge.node();
  ASSERT_COND( redge.inv() == inv );
  mTable.emplace(node, rnode);
  return redge;
}

END_NAMESPACE_YM_BDD
