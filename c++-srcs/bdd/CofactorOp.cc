
/// @file CofactorOp.cc
/// @brief CofactorOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "CofactorOp.h"
#include "BddNode.h"


BEGIN_NAMESPACE_YM_BDD

// @brief コファクターを計算する．
BddEdge
CofactorOp::op_step(
  BddEdge edge ///< [in] 枝
)
{
  // 終端ならそのまま返す．
  if ( edge.is_const() ) {
    return edge;
  }

  auto node = edge.node();
  auto index = node->index();
  auto inv = edge.inv();
  if ( index == mIndex ) {
    if ( mInv ) {
      return node->edge0(inv);
    }
    else {
      return node->edge1(inv);
    }
  }
  if ( index > mIndex ) {
    return edge;
  }

  BddEdge ans;
  if ( mTable.count(node) == 0 ) {
    auto edge0 = node->edge0();
    auto edge1 = node->edge1();
    auto ans0 = op_step(edge0);
    auto ans1 = op_step(edge1);
    ans = new_node(index, ans0, ans1);
    mTable.emplace(node, ans);
  }
  else {
    ans = mTable.at(node);
  }
  return ans * inv;
}

END_NAMESPACE_YM_BDD
