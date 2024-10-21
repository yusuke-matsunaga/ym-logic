
/// @file ZddMgr.cc
/// @brief ZddMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/ZddMgr.h"
#include "ym/Zdd.h"
#include "dd/ZddMgrImpl.h"
#include "CopyOp.h"
#include "ZddInvOp.h"


BEGIN_NAMESPACE_YM_DD

// @brief コンストラクタ
ZddMgr::ZddMgr(
) : ZddMgr{new ZddMgrImpl}
{
}

// @brief ZddMgrImpl を指定したコンストラクタ
ZddMgr::ZddMgr(
  ZddMgrImpl* impl
) : mImpl{impl}
{
  mImpl->inc();
}

// @brief コピーコンストラクタ
ZddMgr::ZddMgr(
  const ZddMgr& src
) : ZddMgr{src.mImpl}
{
}

// @brief デストラクタ
ZddMgr::~ZddMgr()
{
  mImpl->dec();
}

// @brief ZDD をコピーする．
Zdd
ZddMgr::copy(
  const Zdd& src
)
{
  return impl()->copy(src);
}

// @brief 恒儀関数を作る．
Zdd
ZddMgr::zero()
{
  return impl()->zero();
}

// @brief 恒新関数を作る．
Zdd
ZddMgr::one()
{
  return impl()->one();
}

// @brief 部分集合を作る．
Zdd
ZddMgr::make_set(
  const vector<SizeType> elem_list
)
{
  return impl()->make_set(elem_list);
}

// @brief 複数のZDDのノード数を数える．
SizeType
ZddMgr::zdd_size(
  const vector<Zdd>& zdd_list
)
{
  return impl()->zdd_size(zdd_list);
}

// @brief 複数のZDDの内容を出力する．
void
ZddMgr::display(
  ostream& s,
  const vector<Zdd>& zdd_list
)
{
  impl()->display(s, zdd_list);
}

// @brief 複数のZDDを dot 形式で出力する．
void
ZddMgr::gen_dot(
  ostream& s,
  const vector<Zdd>& zdd_list,
  const unordered_map<string, string>& attr_dict
)
{
  impl()->gen_dot(s, zdd_list, attr_dict);
}

// @brief 構造を表す整数配列を作る．
vector<SizeType>
ZddMgr::rep_data(
  const vector<Zdd>& zdd_list
)
{
  return impl()->rep_data(zdd_list);
}

// @brief 複数のZDDを独自形式でバイナリダンプする．
void
ZddMgr::dump(
  BinEnc& s,
  const vector<Zdd>& zdd_list
)
{
  impl()->dump(s, zdd_list);
}

// @brief バイナリダンプから復元する．
vector<Zdd>
ZddMgr::restore(
  BinDec& s
)
{
  return impl()->restore(s);
}

// @brief ガーベージコレクションを行う．
void
ZddMgr::garbage_collection()
{
  impl()->garbage_collection();
}

// @brief ノード数を返す．
SizeType
ZddMgr::node_num() const
{
  return impl()->node_num();
}

// @brief GC を起動するしきい値を返す．
SizeType
ZddMgr::gc_limit() const
{
  return impl()->gc_limit();
}

// @brief GC を起動するしきい値を設定する．
void
ZddMgr::set_gc_limit(
  SizeType limit
)
{
  impl()->set_gc_limit(limit);
}

// @brief GC を許可する．
void
ZddMgr::enable_gc()
{
  impl()->enable_gc();
}

// @brief GC を禁止する．
void
ZddMgr::disable_gc()
{
  impl()->disable_gc();
}

END_NAMESPACE_YM_DD
