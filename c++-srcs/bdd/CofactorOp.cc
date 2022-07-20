
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
  BddEdge edge,
  BddEdge cedge
)
{
  // 終端ならそのまま返す．
  if ( edge.is_const() ) {
    return edge;
  }

  // cedge が 0 なら 0 を返す．
  if ( cedge.is_zero() ) {
    return BddEdge::zero();
  }
  // cedge が 1 なら edge を返す．
  if ( cedge.is_one() ) {
    return BddEdge::one();
  }

  // 正規化する．
  bool oinv = edge.inv();
  edge.make_positive();

  Apply2Key key{edge, cedge};
  if ( mTable.count(key) > 0 ) {
    return mTable.at(key) * oinv;
  }

  auto node = edge.node();
  auto index = node->index();
  auto edge0 = node->edge0();
  auto edge1 = node->edge1();

  auto cnode = cedge.node();
  auto cindex = cnode->index();
  auto cinv = cedge.inv();
  BddEdge result;
  if ( index == cindex ) {
    if ( cnode->edge0(cinv).is_zero() ) {
      result = op_step(edge1, cnode->edge1(cinv));
    }
    else if ( cnode->edge1(cinv).is_zero() ) {
      result = op_step(edge0, cnode->edge0(cinv));
    }
    else {
      ASSERT_NOT_REACHED;
    }
  }
  else if ( index < cindex ) {
    auto ans0 = op_step(edge0, cedge);
    auto ans1 = op_step(edge1, cedge);
    result = new_node(index, ans0, ans1);;
  }
  else { // index > cindex
    if ( cnode->edge0(cinv).is_zero() ) {
      cedge = cnode->edge1(cinv);
    }
    else if ( cnode->edge1(cinv).is_zero() ) {
      cedge = cnode->edge1(cinv);
    }
    else {
      ASSERT_NOT_REACHED;
    }
    result = op_step(edge, cedge);
  }
  mTable.emplace(key, result);
  return result * oinv;
}

END_NAMESPACE_YM_BDD
