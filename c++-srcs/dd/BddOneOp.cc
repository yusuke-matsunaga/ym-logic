
/// @file BddOneOp.cc
/// @brief BddOneOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Bdd.h"
#include "DdEdge.h"
#include "DdNode.h"
#include "BddOneOp.h"


BEGIN_NAMESPACE_YM_DD

// @brief 1となるパスを求める．
Bdd
Bdd::get_onepath() const
{
  _check_valid();

  if ( is_zero() ) {
    return Bdd{mMgr, DdEdge::zero()};
  }
  BddOneOp op{*mMgr};
  auto e = op.op_step(DdEdge{mRoot});
  return Bdd{mMgr, e};
}

// @brief 0となるパスを求める．
Bdd
Bdd::get_zeropath() const
{
  return invert().get_onepath();
}


//////////////////////////////////////////////////////////////////////
// クラス Bdd_OneOp
//////////////////////////////////////////////////////////////////////

// @brief 1に至るパスを求める．
DdEdge
BddOneOp::op_step(
  DdEdge edge
)
{
  if ( edge.is_one() ) {
    return DdEdge::one();
  }

  auto node = edge.node();
  auto inv = edge.inv();
  auto edge0 = node->edge0() ^ inv;
  auto edge1 = node->edge1() ^ inv;
  if ( !edge1.is_zero() ) {
    auto tmp = op_step(edge1);
    return new_node(node->index(), DdEdge::zero(), tmp);
  }
  if ( !edge0.is_zero() ) {
    auto tmp = op_step(edge0);
    return new_node(node->index(), tmp, DdEdge::zero());
  }
  ASSERT_NOT_REACHED;
  return DdEdge{};
}

END_NAMESPACE_YM_DD
