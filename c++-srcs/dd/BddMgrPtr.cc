
/// @file BddMgrPtr.cc
/// @brief BddMgrPtr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/BddMgrPtr.h"
#include "ym/BddLit.h"
#include "BddMgrImpl.h"
#include "DdInfoMgr.h"


BEGIN_NAMESPACE_YM_DD

//////////////////////////////////////////////////////////////////////
// クラス BddMgrPtr
//////////////////////////////////////////////////////////////////////

// @brief 生のポインタを指定したコンストラクタ
BddMgrPtr::BddMgrPtr(
  BddMgrImpl* ptr
) : mPtr{ptr}
{
}

// @brief コピーコンストラクタ
BddMgrPtr::BddMgrPtr(
  const BddMgrPtr& src
) : mPtr{src.mPtr}
{
}

// @brief コピー代入演算子
BddMgrPtr&
BddMgrPtr::operator=(
  const BddMgrPtr& src
)
{
  mPtr = src.mPtr;

  return *this;
}

// @brief デストラクタ
BddMgrPtr::~BddMgrPtr()
{
}

// @brief 変数を返す．
BddVar
BddMgrPtr::variable(
  SizeType varid
) const
{
  return _var(mPtr->variable(varid));
}

// @brief 変数のリストを返す．
vector<BddVar>
BddMgrPtr::variable_list() const
{
  auto& edge_list = mPtr->variable_list();
  return conv_to_varlist(edge_list);
}

// @brief 恒偽関数を作る．
Bdd
BddMgrPtr::zero() const
{
  return _bdd(DdEdge::zero());
}

// @brief 恒真関数を作る．
Bdd
BddMgrPtr::one() const
{
  return _bdd(DdEdge::one());
}

// @brief BDD を作る．
Bdd
BddMgrPtr::bdd(
  SizeType level,
  const Bdd& bdd0,
  const Bdd& bdd1
) const
{
  bdd0._check_valid();
  bdd1._check_valid();
  _check_mgr(bdd0.mMgr);
  _check_mgr(bdd1.mMgr);
  auto edge = mPtr->new_node(level, bdd0.root(), bdd1.root());
  return _bdd(edge);
}

// @brief リテラル関数を表すBDDを作る．
BddLit
BddMgrPtr::literal(
  const BddVar& var,
  bool inv
) const
{
  _check_mgr(var.mMgr);
  return BddLit{var, inv};
}

// @brief 肯定のリテラル関数を作る．
BddLit
BddMgrPtr::posi_literal(
  const BddVar& var
) const
{
  return literal(var, false);
}

// @brief 否定のリテラル関数を作る．
BddLit
BddMgrPtr::nega_literal(
  const BddVar& var
) const
{
  return literal(var, true);
}

// @brief 変数リストをインデックスリストに変換する．
vector<SizeType>
BddMgrPtr::level_list(
  const vector<BddVar>& var_list
) const
{
  vector<SizeType> ans;
  ans.reserve(var_list.size());
  for ( auto var: var_list ) {
    ans.push_back(var.level());
  }
  return ans;
}

// @brief レベルを変数に変換する．
BddVar
BddMgrPtr::level_to_var(
  SizeType level
) const
{
  auto edge = mPtr->new_node(level, DdEdge::zero(), DdEdge::one());
  return _var(edge);
}

// @brief 変数順を表す変数のリストを返す．
vector<BddVar>
BddMgrPtr::variable_order() const
{
  auto edge_list = mPtr->variable_order();
  return conv_to_varlist(edge_list);
}

// @brief 変数順を設定する．
void
BddMgrPtr::set_variable_order(
  const vector<BddVar>& order_list
) const
{
  vector<DdEdge> edge_list;
  edge_list.reserve(order_list.size());
  for ( auto& var: order_list ) {
    auto edge = var.root();
    edge_list.push_back(edge);
  }
  mPtr->set_variable_order(edge_list);
}

// @brief 複数のBDDのノード数を数える．
SizeType
BddMgrPtr::bdd_size(
  const vector<Bdd>& bdd_list
) const
{
  if ( bdd_list.empty() ) {
    return 0;
  }
  auto info_mgr = node_info(bdd_list);
  return info_mgr.node_num();
}

// @brief 複数のBDDの内容を出力する．
void
BddMgrPtr::display(
  ostream& s,
  const vector<Bdd>& bdd_list
) const
{
  auto info_mgr = node_info(bdd_list);
  info_mgr.display(s);
}

// @brief 構造を表す整数配列を作る．
vector<SizeType>
BddMgrPtr::rep_data(
  const vector<Bdd>& bdd_list
) const
{
  auto info_mgr = node_info(bdd_list);
  return info_mgr.rep_data();
}

// @brief BDD の内容をバイナリダンプする．
void
BddMgrPtr::dump(
  BinEnc& s,
  const vector<Bdd>& bdd_list
) const
{
  auto info_mgr = node_info(bdd_list);
  mPtr->dump(s, info_mgr);
}

// @brief バイナリダンプから復元する．
vector<Bdd>
BddMgrPtr::restore(
  BinDec& s
) const
{
  auto edge_list = mPtr->restore(s);
  return conv_to_bddlist(edge_list);
}

// @brief 複数のBDDのノードの情報を取り出す．
DdInfoMgr
BddMgrPtr::node_info(
  const vector<Bdd>& bdd_list
) const
{
  vector<DdEdge> edge_list;
  edge_list.reserve(bdd_list.size());
  for ( auto& bdd: bdd_list ) {
    auto edge = bdd.root();
    edge_list.push_back(edge);
  }
  return DdInfoMgr{edge_list, get()};
}

// @brief DdEdge を Bdd に変換する．
Bdd
BddMgrPtr::_bdd(
  DdEdge edge
) const
{
  return Bdd{*this, edge};
}

// @brief DdEdge を BddVar に変換する．
BddVar
BddMgrPtr::_var(
  DdEdge edge
) const
{
  return BddVar{*this, edge};
}

// @brief 同じマネージャの要素かチェックする．
void
BddMgrPtr::_check_mgr(
  const Bdd& bdd
) const
{
  _check_mgr(bdd.mMgr);
}

// @brief 枝のリストをBddのリストに変換する．
vector<Bdd>
BddMgrPtr::conv_to_bddlist(
  const vector<DdEdge>& edge_list
) const
{
  vector<Bdd> bdd_list;
  bdd_list.reserve(edge_list.size());
  for ( auto edge: edge_list ) {
    auto bdd = _bdd(edge);
    bdd_list.push_back(bdd);
  }
  return bdd_list;
}

// @brief 枝のリストを変数のリストに変換する．
vector<BddVar>
BddMgrPtr::conv_to_varlist(
  const vector<DdEdge>& edge_list
) const
{
  vector<BddVar> var_list;
  var_list.reserve(edge_list.size());
  for ( auto edge: edge_list ) {
    auto var = _var(edge);
    var_list.push_back(var);
  }
  return var_list;
}

END_NAMESPACE_YM_DD
