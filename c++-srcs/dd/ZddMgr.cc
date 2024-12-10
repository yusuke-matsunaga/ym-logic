
/// @file ZddMgr.cc
/// @brief ZddMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/ZddMgr.h"
#include "ym/Zdd.h"
#include "ZddMgrImpl.h"
#include "ZddInvOp.h"


BEGIN_NAMESPACE_YM_DD

// @brief コンストラクタ
ZddMgr::ZddMgr(
) : mImpl{new ZddMgrImpl}
{
}

// @brief ZddMgrPtr を指定したコンストラクタ
ZddMgr::ZddMgr(
  const ZddMgrPtr& impl
) : mImpl{impl}
{
}

// @brief コピーコンストラクタ
ZddMgr::ZddMgr(
  const ZddMgr& src
) : mImpl{src.mImpl}
{
}

// @brief デストラクタ
ZddMgr::~ZddMgr()
{
}

// @brief 要素数を返す．
SizeType
ZddMgr::item_num() const
{
  return mImpl->item_num();
}

// @brief 要素を返す．
ZddItem
ZddMgr::item(
  SizeType item_id
)
{
  return mImpl.item(item_id);
}

// @brief 要素のリストを返す．
vector<ZddItem>
ZddMgr::item_list() const
{
  return mImpl.item_list();
}

// @brief ZDD をコピーする．
Zdd
ZddMgr::copy(
  const Zdd& src
)
{
  return mImpl.copy(src);
}

// @brief 恒儀関数を作る．
Zdd
ZddMgr::zero()
{
  return mImpl.zero();
}

// @brief 恒新関数を作る．
Zdd
ZddMgr::one()
{
  return mImpl.one();
}

// @brief 部分集合を作る．
Zdd
ZddMgr::make_set(
  const vector<ZddItem> item_list
)
{
  return mImpl.make_set(item_list);
}

// @brief 複数のZDDのノード数を数える．
SizeType
ZddMgr::zdd_size(
  const vector<Zdd>& zdd_list
)
{
  if ( zdd_list.empty() ) {
    return 0;
  }
  auto zdd0 = zdd_list.front();
  auto mgr = zdd0.mgr();
  return mgr.mImpl.zdd_size(zdd_list);
}

// @brief 複数のZDDの内容を出力する．
void
ZddMgr::display(
  ostream& s,
  const vector<Zdd>& zdd_list
)
{
  if ( zdd_list.empty() ) {
    return;
  }
  auto zdd0 = zdd_list.front();
  auto mgr = zdd0.mgr();
  mgr.mImpl.display(s, zdd_list);
}

// @brief 複数のZDDを dot 形式で出力する．
void
ZddMgr::gen_dot(
  ostream& s,
  const vector<Zdd>& zdd_list,
  const JsonValue& option
)
{
  if ( zdd_list.empty() ) {
    return;
  }
  auto zdd0 = zdd_list.front();
  auto mgr = zdd0.mgr();
  mgr.mImpl.gen_dot(s, zdd_list, option);
}

// @brief 構造を表す整数配列を作る．
vector<SizeType>
ZddMgr::rep_data(
  const vector<Zdd>& zdd_list
)
{
  if ( zdd_list.empty() ) {
    return {};
  }
  auto zdd0 = zdd_list.front();
  auto mgr = zdd0.mgr();
  return mgr.mImpl.rep_data(zdd_list);
}

// @brief 複数のZDDを独自形式でバイナリダンプする．
void
ZddMgr::dump(
  BinEnc& s,
  const vector<Zdd>& zdd_list
)
{
  if ( zdd_list.empty() ) {
    return;
  }
  auto zdd0 = zdd_list.front();
  auto mgr = zdd0.mgr();
  mgr.mImpl.dump(s, zdd_list);
}

// @brief バイナリダンプから復元する．
vector<Zdd>
ZddMgr::restore(
  BinDec& s
)
{
  return mImpl.restore(s);
}

// @brief ガーベージコレクションを行う．
void
ZddMgr::garbage_collection()
{
  mImpl->garbage_collection();
}

// @brief ノード数を返す．
SizeType
ZddMgr::node_num() const
{
  return mImpl->node_num();
}

// @brief GC を起動するしきい値を返す．
SizeType
ZddMgr::gc_limit() const
{
  return mImpl->gc_limit();
}

// @brief GC を起動するしきい値を設定する．
void
ZddMgr::set_gc_limit(
  SizeType limit
)
{
  mImpl->set_gc_limit(limit);
}

// @brief GC を許可する．
void
ZddMgr::enable_gc()
{
  mImpl->enable_gc();
}

// @brief GC を禁止する．
void
ZddMgr::disable_gc()
{
  mImpl->disable_gc();
}

END_NAMESPACE_YM_DD
