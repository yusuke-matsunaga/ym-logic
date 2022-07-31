
/// @file CompOp.cc
/// @brief CompOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "CompOp.h"


BEGIN_NAMESPACE_YM_BDD

// @brief compose 演算
BddEdge
CompOp::comp_step(
  BddEdge edge
)
{
  if ( edge.is_const() ) {
    return edge;
  }

  auto node = edge.node();
  auto index = node->index();
  if ( index > mIndex ) {
    return edge;
  }

  // 演算結果テーブルを調べる．
  BddEdge result;
  if ( mTable.count(node) > 0 ) {
    result = mTable.at(node);
  }
  else {
    auto edge0 = node->edge0();
    auto edge1 = node->edge1();
    auto r0 = comp_step(edge0);
    auto r1 = comp_step(edge1);
    if ( index == mIndex ) {
      result = mgr()->ite_op(mCedge, r0, r1);
    }
    else {
      result = new_node(index, r0, r1);
    }
    mTable.emplace(node, result);
  }
  auto inv = edge.inv();
  return result ^ inv;
}

END_NAMESPACE_YM_BDD
