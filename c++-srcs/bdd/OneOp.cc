
/// @file OneOp.cc
/// @brief OneOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "OneOp.h"


BEGIN_NAMESPACE_YM_BDD

// @brief 1に至るパスを求める．
BddEdge
OneOp::op_step(
  BddEdge edge
)
{
  if ( edge.is_one() ) {
    return BddEdge::one();
  }
  ASSERT_COND( !edge.is_const() );

  auto node = edge.node();
  auto inv = edge.inv();
  auto edge0 = node->edge0() * inv;
  auto edge1 = node->edge1() * inv;
  if ( !edge1.is_zero() ) {
    auto tmp = op_step(edge1);
    return new_node(node->index(), BddEdge::zero(), tmp);
  }
  else { // !edge0.is_zero()
    auto tmp = op_step(edge0);
    return new_node(node->index(), tmp, BddEdge::zero());
  }
}

END_NAMESPACE_YM_BDD
