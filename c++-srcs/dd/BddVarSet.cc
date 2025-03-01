
/// @file BddVarSet.cc
/// @brief BddVarSet の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022, 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/BddVarSet.h"
#include "ym/BddMgr.h"
#include "DdEdge.h"


BEGIN_NAMESPACE_YM_DD

// @brief サポート変数のサイズを返す．
SizeType
Bdd::support_size() const
{
  return get_support().size();
}

// @brief 内容を指定したコンストラクタ
BddVarSet::BddVarSet(
  const BddMgrHolder& holder,
  DdEdge root
) : Bdd(holder, root)
{
}

// @brief 要素のリストを指定したコンストラクタ
BddVarSet::BddVarSet(
  BddMgr& mgr,
  const vector<BddVar>& var_set
) : Bdd(mgr.one())
{
  for ( auto var: var_set ) {
    and_int(var);
  }
}

// @brief 和集合を求める．
BddVarSet
BddVarSet::operator+(
  const BddVarSet& right
) const
{
  auto edge = support_cup(right);
  return BddVarSet(*this, edge);
}

// @brief 和集合を求め代入する．
BddVarSet&
BddVarSet::operator+=(
  const BddVarSet& right
)
{
  auto edge = support_cup(right);
  _change_root(edge);
  return *this;
}

// @brief 差集合を求める．
BddVarSet
BddVarSet::operator-(
  const BddVarSet& right
) const
{
  auto edge = support_diff(right);
  return BddVarSet(*this, edge);
}

// @brief 差集合を求め代入する．
BddVarSet&
BddVarSet::operator-=(
  const BddVarSet& right
)
{
  auto edge = support_diff(right);
  _change_root(edge);
  return *this;
}

// @brief 共通集合を求める．
BddVarSet
BddVarSet::operator&(
  const BddVarSet& right
) const
{
  auto edge = support_cap(right);
  return BddVarSet(*this, edge);
}

// @brief 共通集合を求め代入する．
BddVarSet&
BddVarSet::operator&=(
  const BddVarSet& right
)
{
  auto edge = support_cap(right);
  _change_root(edge);
  return *this;
}

// @brief 共通部分を持つかチェックする．
bool
BddVarSet::operator&&(
  const BddVarSet& right
) const
{
  return support_check_intersect(right);
}

// @brief 等価比較
bool
BddVarSet::operator==(
  const BddVarSet& right
) const
{
  return Bdd::operator==(right);
}

// @brief サイズを返す．
SizeType
BddVarSet::size() const
{
  return Bdd::size();
}

// @brief 変数のリストに変換する．
vector<BddVar>
BddVarSet::to_varlist() const
{
  return Bdd::to_varlist();
}

// @brief 先頭の変数を返す．
BddVar
BddVarSet::top_var() const
{
  return Bdd::root_var();
}

END_NAMESPACE_YM_DD
