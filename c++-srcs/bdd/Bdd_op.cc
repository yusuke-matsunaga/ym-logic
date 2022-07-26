
/// @file Bdd.cc
/// @brief Bdd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Bdd.h"
#include "ym/BddError.h"
#include "BddEdge.h"
#include "BddMgrImpl.h"
#include "BddNode.h"


BEGIN_NAMESPACE_YM_BDD

// @brief 論理積を返す．
Bdd
Bdd::and_op(
  const Bdd& right
) const
{
  ASSERT_COND( mMgr != nullptr );
  BddEdge redge = copy_edge(right);
  auto e = mMgr->and_op(mRoot, redge);
  return Bdd{mMgr, e};
}

// @brief 論理和を返す．
Bdd
Bdd::or_op(
  const Bdd& right
) const
{
  ASSERT_COND( mMgr != nullptr );
  BddEdge redge = copy_edge(right);
  auto e = mMgr->or_op(mRoot, redge);
  return Bdd{mMgr, e};
}

// @brief 排他的論理和を返す．
Bdd
Bdd::xor_op(
  const Bdd& right
) const
{
  ASSERT_COND( mMgr != nullptr );
  BddEdge redge = copy_edge(right);
  auto e = mMgr->xor_op(mRoot, redge);
  return Bdd{mMgr, e};
}

// @brief 論理積を計算して代入する．
Bdd&
Bdd::and_int(
  const Bdd& right
)
{
  ASSERT_COND( mMgr != nullptr );
  BddEdge redge = copy_edge(right);
  auto e = mMgr->and_op(mRoot, redge);
  change_root(e);
  return *this;
}

// @brief 論理和を計算して代入する．
Bdd&
Bdd::or_int(
  const Bdd& right
)
{
  ASSERT_COND( mMgr != nullptr );
  BddEdge redge = copy_edge(right);
  auto e = mMgr->or_op(mRoot, redge);
  change_root(e);
  return *this;
}

// @brief 排他的論理和を計算して代入する．
Bdd&
Bdd::xor_int(
  const Bdd& right
)
{
  ASSERT_COND( mMgr != nullptr );
  BddEdge redge = copy_edge(right);
  auto e = mMgr->xor_op(mRoot, redge);
  change_root(e);
  return *this;
}

// @brief If-Then-Else 演算
Bdd
ite(
  const Bdd& cond,
  const Bdd& then_f,
  const Bdd& else_f
)
{
  auto mgr = cond.mMgr;
  ASSERT_COND( mgr != nullptr );
  auto then_edge = cond.copy_edge(then_f);
  auto else_edge = cond.copy_edge(else_f);
  auto e = mgr->ite_op(cond.mRoot, then_edge, else_edge);
  return Bdd{mgr, e};
}

END_NAMESPACE_YM_BDD
