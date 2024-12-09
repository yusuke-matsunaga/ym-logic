
/// @file ZddInvOp.cc
/// @brief ZddInvOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Zdd.h"
#include "ZddMgrImpl.h"
#include "ZddInvOp.h"


BEGIN_NAMESPACE_YM_DD

// @brief ZDD を反転する．
Zdd
ZddMgrImpl::invert(
  const Zdd& src
)
{
  ZddInvOp op{*this};
  auto e = op.inv_step(_edge(src));
  return _zdd(e);
}


//////////////////////////////////////////////////////////////////////
// クラス Zdd_InvOp
//////////////////////////////////////////////////////////////////////

// @brief 補集合を作る．
DdEdge
ZddInvOp::inv_step(
  DdEdge edge
)
{
  if ( edge.is_zero() ) {
    return DdEdge::one();
  }
  if ( edge.is_one() ) {
    return DdEdge::zero();
  }

  auto node = edge.node();
  auto level = node->level();
  if ( mTable.count(node) > 0 ) {
    auto rnode = mTable.at(node);
    return DdEdge{rnode};
  }
  auto edge0 = node->edge0();
  auto edge1 = node->edge1();
  auto redge0 = inv_step(edge0);
  auto redge1 = inv_step(edge1);
  auto redge = new_node(level, redge0, redge1);
  auto rnode = redge.node();
  mTable.emplace(node, rnode);
  return redge;
}

END_NAMESPACE_YM_DD
