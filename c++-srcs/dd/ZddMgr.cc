
/// @file ZddMgr.cc
/// @brief ZddMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/ZddMgr.h"
#include "ym/Zdd.h"
#include "ZddMgrImpl.h"
#include "CopyOp.h"
#include "ZddInvOp.h"


BEGIN_NAMESPACE_YM_DD

// @brief コンストラクタ
ZddMgr::ZddMgr(
) : mImpl{new ZddMgrImpl}
{
}

// @brief デストラクタ
ZddMgr::~ZddMgr()
{
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
  return mImpl->make_set(elem_list);
}

// @brief バイナリダンプから復元する．
vector<Zdd>
ZddMgr::restore(
  BinDec& s
)
{
  return mImpl->restore(s);
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
