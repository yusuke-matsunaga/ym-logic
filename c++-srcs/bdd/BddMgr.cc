
/// @file BddMgr.cc
/// @brief BddMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/BddMgr.h"
#include "ym/BddVar.h"
#include "ym/Bdd.h"
#include "ym/BddError.h"
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

// @brief 真理値表形式の文字列からBDDを作る．
Bdd
BddMgr::from_truth(
  const string& str,
  const vector<BddVar>& var_list
)
{
  SizeType ni = var_list.size();
  if ( (1 << ni) != str.size() ) {
    throw BddError{"BddMgr::from_truth(): wrong string length"};
  }
  auto e = mImpl->from_truth(str, var_list);
  return Bdd{mImpl, e};
}

END_NAMESPACE_YM
