
/// @file BddMgr.cc
/// @brief BddMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022, 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/BddMgr.h"
#include "ym/Bdd.h"
#include "dd/BddMgrImpl.h"
#include "CopyOp.h"


BEGIN_NAMESPACE_YM_DD

// @brief コンストラクタ
BddMgr::BddMgr(
) : BddMgr{new BddMgrImpl}
{
}

// @brief BddMgrImpl を指定したコンストラクタ
BddMgr::BddMgr(
  BddMgrImpl* impl
) : mImpl{impl}
{
  mImpl->inc();
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
  mImpl->dec();
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
  return mImpl->variable(varid);
}

// @brief 変数のリストを返す．
vector<BddVar>
BddMgr::variable_list() const
{
  return mImpl->variable_list();
}

// @brief BDD をコピーする．
Bdd
BddMgr::copy(
  const Bdd& src
)
{
  return impl()->copy(src);
}

// @brief 恒儀関数を作る．
Bdd
BddMgr::zero()
{
  return impl()->zero();
}

// @brief 恒新関数を作る．
Bdd
BddMgr::one()
{
  return impl()->one();
}

// @brief 真理値表形式の文字列からBDDを作る．
Bdd
BddMgr::from_truth(
  const vector<BddVar>& var_list,
  const string& str
)
{
  return impl()->from_truth(var_list, str);
}

// @brief ITE 演算を行う．
Bdd
BddMgr::ite(
  const Bdd& e0,
  const Bdd& e1,
  const Bdd& e2
)
{
  return impl()->ite(e0, e1, e2);
}

// @brief ドントケアを利用した簡単化を行う．
Bdd
BddMgr::simplify(
  const Bdd& on,  ///< [in] オンセット
  const Bdd& dc   ///< [in] ドントケアセット
)
{
  return impl()->simplify(on, dc);
}

// @brief 複数のBDDのノード数を数える．
SizeType
BddMgr::bdd_size(
  const vector<Bdd>& bdd_list
)
{
  return impl()->bdd_size(bdd_list);
}

// @brief 複数のBDDの内容を出力する．
void
BddMgr::display(
  ostream& s,
  const vector<Bdd>& bdd_list
)
{
  impl()->display(s, bdd_list);
}

// @brief 複数のBDDを dot 形式で出力する．
void
BddMgr::gen_dot(
  ostream& s,
  const vector<Bdd>& bdd_list,
  const unordered_map<string, string>& attr_dict
)
{
  impl()->gen_dot(s, bdd_list, attr_dict);
}

// @brief 構造を表す整数配列を作る．
vector<SizeType>
BddMgr::rep_data(
  const vector<Bdd>& bdd_list
)
{
  return impl()->rep_data(bdd_list);
}

// @brief BDD の内容をバイナリダンプする．
void
BddMgr::dump(
  BinEnc& s,
  const vector<Bdd>& bdd_list
)
{
  impl()->dump(s, bdd_list);
}

// @brief バイナリダンプから復元する．
vector<Bdd>
BddMgr::restore(
  BinDec& s
)
{
  return impl()->restore(s);
}

// @brief ガーベージコレクションを行う．
void
BddMgr::garbage_collection()
{
  impl()->garbage_collection();
}

// @brief ノード数を返す．
SizeType
BddMgr::node_num() const
{
  return impl()->node_num();
}

// @brief GC を起動するしきい値を返す．
SizeType
BddMgr::gc_limit() const
{
  return impl()->gc_limit();
}

// @brief GC を起動するしきい値を設定する．
void
BddMgr::set_gc_limit(
  SizeType limit
)
{
  impl()->set_gc_limit(limit);
}

// @brief GC を許可する．
void
BddMgr::enable_gc()
{
  impl()->enable_gc();
}

// @brief GC を禁止する．
void
BddMgr::disable_gc()
{
  impl()->disable_gc();
}

END_NAMESPACE_YM_DD
