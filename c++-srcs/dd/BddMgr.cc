
/// @file BddMgr.cc
/// @brief BddMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022, 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/BddMgr.h"
#include "ym/Bdd.h"
#include "BddMgrImpl.h"


BEGIN_NAMESPACE_YM_DD

// @brief コンストラクタ
BddMgr::BddMgr(
) : mImpl{new BddMgrImpl}
{
}

// @brief BddMgrImpl を指定したコンストラクタ
BddMgr::BddMgr(
  const BddMgrPtr& impl
) : mImpl{impl}
{
}

// @brief コピーコンストラクタ
BddMgr::BddMgr(
  const BddMgr& src
) : BddMgr{src.mImpl}
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
  return mImpl->variable_num();
}

// @brief 変数を返す．
BddVar
BddMgr::variable(
  SizeType varid
)
{
  return mImpl.variable(varid);
}

// @brief 変数のリストを返す．
vector<BddVar>
BddMgr::variable_list() const
{
  return mImpl.variable_list();
}

// @brief 変数順を表す変数のリストを返す．
vector<BddVar>
BddMgr::variable_order() const
{
  return mImpl.variable_order();
}

// @brief 変数順を設定する．
void
BddMgr::set_variable_order(
  const vector<BddVar>& order_list
)
{
  mImpl.set_variable_order(order_list);
}

// @brief BDD をコピーする．
Bdd
BddMgr::copy(
  const Bdd& src
)
{
  return mImpl.copy(src);
}

// @brief 恒儀関数を作る．
Bdd
BddMgr::zero()
{
  return mImpl.zero();
}

// @brief 恒新関数を作る．
Bdd
BddMgr::one()
{
  return mImpl.one();
}

// @brief 真理値表形式の文字列からBDDを作る．
Bdd
BddMgr::from_truth(
  const string& str,
  const vector<BddVar>& var_list
)
{
  return mImpl.from_truth(str, var_list);
}

// @brief ITE 演算を行う．
Bdd
BddMgr::ite(
  const Bdd& e0,
  const Bdd& e1,
  const Bdd& e2
)
{
  return mImpl.ite(e0, e1, e2);
}

// @brief 論理式から BDD を作る．
Bdd
BddMgr::from_expr(
  const Expr& expr,
  const vector<BddVar>& var_list
)
{
  return mImpl.from_expr(expr, var_list);
}

// @brief ドントケアを利用した簡単化を行う．
Bdd
BddMgr::simplify(
  const Bdd& on,  ///< [in] オンセット
  const Bdd& dc   ///< [in] ドントケアセット
)
{
  return mImpl.simplify(on, dc);
}

// @brief 複数のBDDのノード数を数える．
SizeType
BddMgr::bdd_size(
  const vector<Bdd>& bdd_list
)
{
  if ( bdd_list.empty() ) {
    return 0;
  }
  auto bdd0 = bdd_list.front();
  auto mgr = bdd0.mgr();
  return mgr.mImpl.bdd_size(bdd_list);
}

// @brief 複数のBDDの内容を出力する．
void
BddMgr::display(
  ostream& s,
  const vector<Bdd>& bdd_list
)
{
  if ( bdd_list.empty() ) {
    return;
  }
  auto bdd0 = bdd_list.front();
  auto mgr = bdd0.mgr();
  mgr.mImpl.display(s, bdd_list);
}

// @brief 複数のBDDを dot 形式で出力する．
void
BddMgr::gen_dot(
  ostream& s,
  const vector<Bdd>& bdd_list,
  const JsonValue& option
)
{
  if ( bdd_list.empty() ) {
    return;
  }
  auto bdd0 = bdd_list.front();
  auto mgr = bdd0.mgr();
  mgr.mImpl.gen_dot(s, bdd_list, option);
}

// @brief 構造を表す整数配列を作る．
vector<SizeType>
BddMgr::rep_data(
  const vector<Bdd>& bdd_list
)
{
  if ( bdd_list.empty() ) {
    return {};
  }
  auto bdd0 = bdd_list.front();
  auto mgr = bdd0.mgr();
  return mgr.mImpl.rep_data(bdd_list);
}

// @brief BDD の内容をバイナリダンプする．
void
BddMgr::dump(
  BinEnc& s,
  const vector<Bdd>& bdd_list
)
{
  if ( bdd_list.empty() ) {
    return;
  }

  auto bdd0 = bdd_list.front();
  auto mgr = bdd0.mgr();
  // sanity check
  for ( auto& bdd: bdd_list ) {
    bdd._check_mgr(mgr.mImpl);
  }
  mgr.mImpl.dump(s, bdd_list);
}

// @brief バイナリダンプから復元する．
vector<Bdd>
BddMgr::restore(
  BinDec& s
)
{
  return mImpl.restore(s);
}

// @brief ガーベージコレクションを行う．
void
BddMgr::garbage_collection()
{
  mImpl->garbage_collection();
}

// @brief ノード数を返す．
SizeType
BddMgr::node_num() const
{
  return mImpl->node_num();
}

// @brief GC を起動するしきい値を返す．
SizeType
BddMgr::gc_limit() const
{
  return mImpl->gc_limit();
}

// @brief GC を起動するしきい値を設定する．
void
BddMgr::set_gc_limit(
  SizeType limit
)
{
  mImpl->set_gc_limit(limit);
}

// @brief GC を許可する．
void
BddMgr::enable_gc()
{
  mImpl->enable_gc();
}

// @brief GC を禁止する．
void
BddMgr::disable_gc()
{
  mImpl->disable_gc();
}

END_NAMESPACE_YM_DD
