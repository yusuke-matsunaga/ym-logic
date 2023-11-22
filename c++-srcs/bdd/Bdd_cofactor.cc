
/// @file Bdd_cofactor.cc
/// @brief Bdd::cofactor の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022, 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Bdd.h"
#include "BddEdge.h"
#include "BddMgrImpl.h"
#include "CofactorOp.h"


BEGIN_NAMESPACE_YM_BDD

// @brief コファクターを計算する．
Bdd
Bdd::cofactor(
  SizeType var,
  bool inv
) const
{
  _check_valid();

  auto cedge = mMgr->make_literal(var) ^ inv;
  Bdd cube{mMgr, cedge}; // GC 用にロックする必要がある．
  CofactorOp op{mMgr};
  auto e = op.op_step(BddEdge{mRoot}, cedge);
  return Bdd{mMgr, e};
}

// @brief コファクターを計算する．
Bdd
Bdd::cofactor(
  Literal lit
) const
{
  return cofactor(lit.varid(), lit.is_negative());
}

// @brief コファクターを計算する．
Bdd
Bdd::cofactor(
  const Bdd& cube
) const
{
  _check_valid();
  cube._check_valid();
  cube._check_cube();

  auto redge = copy_edge(cube);
  CofactorOp op{mMgr};
  auto e = op.op_step(BddEdge{mRoot}, redge);
  return Bdd{mMgr, e};
}

// @brief コファクターを計算して代入する．
Bdd&
Bdd::cofactor_int(
  SizeType var,
  bool inv
)
{
  _check_valid();

  auto cedge = mMgr->make_literal(var) ^ inv;
  Bdd cbdd{mMgr, cedge}; // GC 用にロックする必要がある．
  CofactorOp op{mMgr};
  auto e = op.op_step(BddEdge{mRoot}, cedge);
  change_root(e);
  return *this;
}

// @brief コファクターを計算して代入する．
Bdd&
Bdd::cofactor_int(
  Literal lit
)
{
  return cofactor_int(lit.varid(), lit.is_negative());
}

/// @brief コファクターを計算して代入する．
Bdd&
Bdd::cofactor_int(
  const Bdd& cube
)
{
  _check_valid();
  cube._check_valid();
  cube._check_cube();

  BddEdge redge = copy_edge(cube);
  CofactorOp op{mMgr};
  auto e = op.op_step(BddEdge{mRoot}, redge);
  change_root(e);
  return *this;
}


//////////////////////////////////////////////////////////////////////
// クラス CofactorOp
//////////////////////////////////////////////////////////////////////

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
  auto index = node->index();
  auto edge0 = node->edge0();
  auto edge1 = node->edge1();

  auto cnode = cedge.node();
  auto cindex = cnode->index();
  auto cinv = cedge.inv();
  BddEdge result;
  if ( index == cindex ) {
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
  else if ( index < cindex ) {
    auto ans0 = op_step(edge0, cedge);
    auto ans1 = op_step(edge1, cedge);
    result = new_node(index, ans0, ans1);;
  }
  else { // index > cindex
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

END_NAMESPACE_YM_BDD
