
/// @file BddMgr.cc
/// @brief BddMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/BddMgr.h"
#include "ym/BddVar.h"
#include "ym/Bdd.h"
#include "BddMgrImpl.h"


BEGIN_NAMESPACE_YM

// @brief コンストラクタ
BddMgr::BddMgr(
) : mImpl{new BddMgrImpl}
{
}

// @brief デストラクタ
BddMgr::~BddMgr()
{
  delete mImpl;
}

// @brief 変数を割り当てる．
BddVar
BddMgr::new_variable(
  const string& name
)
{
  return mImpl->new_variable(name);
}

// @brief 恒儀関数を作る．
Bdd
BddMgr::make_zero()
{
  return Bdd{mImpl, BddEdge::make_zero()};
}

// @brief 恒新関数を作る．
Bdd
BddMgr::make_one()
{
  return Bdd{mImpl, BddEdge::make_one()};
}

// @brief リテラル関数を作る．
Bdd
BddMgr::make_literal(
  BddVar var,
  bool inv
)
{
  auto e = mImpl->new_node(var.index(), BddEdge::make_zero(), BddEdge::make_one());
  return Bdd{mImpl, e * inv};
}

// @brief 肯定のリテラル関数を作る．
Bdd
BddMgr::make_posi_literal(
  BddVar var
)
{
  return make_literal(var, false);
}

// @brief 否定のリテラル関数を作る．
Bdd
BddMgr::make_nega_literal(
  BddVar var
)
{
  return make_literal(var, true);
}

END_NAMESPACE_YM
