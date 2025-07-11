
/// @file BddMgr.cc
/// @brief BddMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/BddMgr.h"
#include "ym/Bdd.h"
#include "BddMgrImpl.h"


BEGIN_NAMESPACE_YM_DD

// @brief コンストラクタ
BddMgr::BddMgr(
) : BddMgrHolder(new BddMgrImpl)
{
}

// @brief BddMgrImpl を指定したコンストラクタ
BddMgr::BddMgr(
  const BddMgrHolder& holder
) : BddMgrHolder(holder)
{
}

// @brief コピーコンストラクタ
BddMgr::BddMgr(
  const BddMgr& src
) : BddMgrHolder(src)
{
}

// @brief デストラクタ
BddMgr::~BddMgr()
{
}

// @breif 変数の数を返す．
SizeType
BddMgr::variable_num() const
{
  return get()->variable_num();
}

// @brief 変数を返す．
BddVar
BddMgr::variable(
  SizeType varid
)
{
  auto edge = get()->variable(varid);
  return _var(edge);
}

// @brief 変数のリストを返す．
std::vector<BddVar>
BddMgr::variable_list() const
{
  auto& edge_list = get()->variable_list();
  return conv_to_varlist(edge_list);
}

// @brief 変数順を表す変数のリストを返す．
std::vector<BddVar>
BddMgr::variable_order() const
{
  auto edge_list = get()->variable_order();
  return conv_to_varlist(edge_list);
}

// @brief 変数順を設定する．
void
BddMgr::set_variable_order(
  const std::vector<BddVar>& order_list
)
{
  auto edge_list = BddVar::conv_to_edgelist(order_list);
  get()->set_variable_order(edge_list);
}

// @brief 恒儀関数を作る．
Bdd
BddMgr::zero()
{
  return _bdd(DdEdge::zero());
}

// @brief 恒新関数を作る．
Bdd
BddMgr::one()
{
  return _bdd(DdEdge::one());
}

// @brief バイナリダンプから復元する．
std::vector<Bdd>
BddMgr::restore(
  BinDec& s
)
{
  auto edge_list = get()->restore(s);
  return conv_to_bddlist(edge_list);
}

// @brief ガーベージコレクションを行う．
void
BddMgr::garbage_collection()
{
  get()->garbage_collection();
}

// @brief ノード数を返す．
SizeType
BddMgr::node_num() const
{
  return get()->node_num();
}

// @brief GC を起動するしきい値を返す．
SizeType
BddMgr::gc_limit() const
{
  return get()->gc_limit();
}

// @brief GC を起動するしきい値を設定する．
void
BddMgr::set_gc_limit(
  SizeType limit
)
{
  get()->set_gc_limit(limit);
}

// @brief GC を許可する．
void
BddMgr::enable_gc()
{
  get()->enable_gc();
}

// @brief GC を禁止する．
void
BddMgr::disable_gc()
{
  get()->disable_gc();
}

// @brief 枝のリストをBddのリストに変換する．
std::vector<Bdd>
BddMgr::conv_to_bddlist(
  const std::vector<DdEdge>& edge_list
) const
{
  std::vector<Bdd> bdd_list;
  bdd_list.reserve(edge_list.size());
  for ( auto edge: edge_list ) {
    auto bdd = _bdd(edge);
    bdd_list.push_back(bdd);
  }
  return bdd_list;
}

// @brief 枝のリストを変数のリストに変換する．
std::vector<BddVar>
BddMgr::conv_to_varlist(
  const std::vector<DdEdge>& edge_list
) const
{
  std::vector<BddVar> var_list;
  var_list.reserve(edge_list.size());
  for ( auto edge: edge_list ) {
    auto var = _var(edge);
    var_list.push_back(var);
  }
  return var_list;
}

END_NAMESPACE_YM_DD
