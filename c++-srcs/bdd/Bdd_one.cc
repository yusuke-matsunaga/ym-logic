
/// @file Bdd.cc
/// @brief Bdd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022, 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Bdd.h"
#include "BddEdge.h"
#include "BddNode.h"
#include "OneOp.h"


BEGIN_NAMESPACE_YM_BDD

// @brief 1となるパスを求める．
Bdd
Bdd::get_onepath() const
{
  _check_valid();

  if ( is_zero() ) {
    return Bdd{mMgr, BddEdge::zero()};
  }
  OneOp op{mMgr};
  auto e = op.op_step(mRoot);
  return Bdd{mMgr, e};
}

// @brief 0となるパスを求める．
Bdd
Bdd::get_zeropath() const
{
  return invert().get_onepath();
}


//////////////////////////////////////////////////////////////////////
// クラス OneOp
//////////////////////////////////////////////////////////////////////

// @brief 1に至るパスを求める．
BddEdge
OneOp::op_step(
  BddEdge edge
)
{
  if ( edge.is_one() ) {
    return BddEdge::one();
  }

  auto node = edge.node();
  auto inv = edge.inv();
  auto edge0 = node->edge0() ^ inv;
  auto edge1 = node->edge1() ^ inv;
  if ( !edge1.is_zero() ) {
    auto tmp = op_step(edge1);
    return new_node(node->index(), BddEdge::zero(), tmp);
  }
  if ( !edge0.is_zero() ) {
    auto tmp = op_step(edge0);
    return new_node(node->index(), tmp, BddEdge::zero());
  }
  ASSERT_NOT_REACHED;
  return BddEdge{};
}

END_NAMESPACE_YM_BDD
