
/// @file BddVarSet.cc
/// @brief BddVarSet の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/BddVarSet.h"
#include "ym/BddMgr.h"


BEGIN_NAMESPACE_YM_BDD

// @brief 要素のリストを指定したコンストラクタ
BddVarSet::BddVarSet(
  BddMgr& mgr,
  const vector<VarId>& var_set
) : Bdd{mgr.one()}
{
  for ( auto var: var_set ) {
    auto lit = mgr.literal(var);
    and_int(lit);
  }
}

// @brief 和集合を求める．
BddVarSet
BddVarSet::operator+(
  const BddVarSet& right
) const
{
  return BddVarSet{*this}.operator+=(right);
}

// @brief 和集合を求め代入する．
BddVarSet&
BddVarSet::operator+=(
  const BddVarSet& right
)
{
  support_cup_int(right);
  return *this;
}

// @brief 差集合を求める．
BddVarSet
BddVarSet::operator-(
  const BddVarSet& right
) const
{
  return BddVarSet{*this}.operator-=(right);
}

// @brief 差集合を求め代入する．
BddVarSet&
BddVarSet::operator-=(
  const BddVarSet& right
)
{
  support_diff_int(right);
  return *this;
}

// @brief 共通集合を求める．
BddVarSet
BddVarSet::operator&(
  const BddVarSet& right
) const
{
  return BddVarSet{*this}.operator&=(right);
}

// @brief 共通集合を求め代入する．
BddVarSet&
BddVarSet::operator&=(
  const BddVarSet& right
)
{
  support_cap_int(right);
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
vector<VarId>
BddVarSet::to_varlist() const
{
  return Bdd::to_varlist();
}

// @brief 先頭の変数を返す．
VarId
BddVarSet::top_var() const
{
  return Bdd::root_var();
}

END_NAMESPACE_YM_BDD
