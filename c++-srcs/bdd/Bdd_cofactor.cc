
/// @file Bdd_cofactor.cc
/// @brief Bdd::cofactor の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Bdd.h"
#include "BddEdge.h"
#include "BddMgrImpl.h"
#include "CofactorOp.h"


BEGIN_NAMESPACE_YM_BDD

// @brief コファクターを計算する．
Bdd
Bdd::cofactor(
  VarId var,
  bool inv
) const
{
  ASSERT_COND( mMgr != nullptr );
  auto cedge = mMgr->make_literal(var.val()) ^ inv;
  Bdd cube{mMgr, cedge}; // GC 用にロックする必要がある．
  return cofactor(cube);
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
  ASSERT_COND( mMgr != nullptr );
  BddEdge redge = copy_edge(cube);
  CofactorOp op{mMgr};
  auto e = op.op_step(mRoot, redge);
  return Bdd{mMgr, e};
}

// @brief コファクターを計算して代入する．
Bdd&
Bdd::cofactor_int(
  VarId var,
  bool inv
)
{
  ASSERT_COND( mMgr != nullptr );
  auto cedge = mMgr->make_literal(var.val()) ^ inv;
  Bdd cbdd{mMgr, cedge}; // GC 用にロックする必要がある．
  return cofactor_int(cbdd);
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
  ASSERT_COND( mMgr != nullptr );
  BddEdge redge = copy_edge(cube);
  CofactorOp op{mMgr};
  auto e = op.op_step(mRoot, redge);
  change_root(e);
  return *this;
}

END_NAMESPACE_YM_BDD
