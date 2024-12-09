
/// @file BddCofactorOp.cc
/// @brief BddCofactorOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Bdd.h"
#include "DdEdge.h"
#include "BddMgrImpl.h"
#include "BddCofactorOp.h"


BEGIN_NAMESPACE_YM_DD

// @brief コファクターを計算する．
Bdd
BddMgrImpl::cofactor(
  const Bdd& bdd,
  const BddVar& var,
  bool inv
)
{
  bdd._check_valid();
  _check_mgr(bdd);
  auto cube = literal(var, inv);
  BddCofactorOp op{*this};
  auto e = op.op_step(_edge(bdd), _edge(cube));
  return _bdd(e);
}

// @brief コファクターを計算する．
Bdd
BddMgrImpl::cofactor(
  const Bdd& bdd,
  const Bdd& cube
)
{
  bdd._check_valid();
  cube._check_valid();
  cube._check_cube();
  _check_mgr(bdd);
  _check_mgr(cube);
  BddCofactorOp op{*this};
  auto e = op.op_step(_edge(bdd), _edge(cube));
  return _bdd(e);
}


//////////////////////////////////////////////////////////////////////
// クラス BddCofactorOp
//////////////////////////////////////////////////////////////////////

// @brief コファクターを計算する．
DdEdge
BddCofactorOp::op_step(
  DdEdge edge,
  DdEdge cedge
)
{
  // 終端ならそのまま返す．
  if ( edge.is_const() ) {
    return edge;
  }

  // cedge が 0 なら 0 を返す．
  if ( cedge.is_zero() ) {
    return DdEdge::zero();
  }
  // cedge が 1 なら edge を返す．
  if ( cedge.is_one() ) {
    return edge;
  }

  // 正規化する．
  bool oinv = edge.inv();
  edge.make_positive();

  Apply2Key key{edge, cedge};
  if ( mTable.count(key) > 0 ) {
    return mTable.at(key) ^ oinv;
  }

  auto node = edge.node();
  auto level = node->level();
  auto edge0 = node->edge0();
  auto edge1 = node->edge1();

  auto cnode = cedge.node();
  auto clevel = cnode->level();
  auto cinv = cedge.inv();
  DdEdge result;
  if ( level == clevel ) {
    auto cedge0 = cnode->edge0() ^ cinv;
    auto cedge1 = cnode->edge1() ^ cinv;
    if ( cedge0.is_zero() ) {
      result = op_step(edge1, cedge1);
    }
    else if ( cedge1.is_zero() ) {
      result = op_step(edge0, cedge0);
    }
    else {
      ASSERT_NOT_REACHED;
    }
  }
  else if ( level < clevel ) {
    auto ans0 = op_step(edge0, cedge);
    auto ans1 = op_step(edge1, cedge);
    result = new_node(level, ans0, ans1);;
  }
  else { // level > clevel
    auto cedge0 = cnode->edge0() ^ cinv;
    auto cedge1 = cnode->edge1() ^ cinv;
    if ( cedge0.is_zero() ) {
      cedge = cedge1;
    }
    else if ( cedge1.is_zero() ) {
      cedge = cedge0;
    }
    else {
      ASSERT_NOT_REACHED;
    }
    result = op_step(edge, cedge);
  }
  mTable.emplace(key, result);
  return result ^ oinv;
}

END_NAMESPACE_YM_DD
