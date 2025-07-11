
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
) : ZddMgrHolder(new ZddMgrImpl)
{
}

// @brief ZddMgrHolder を指定したコンストラクタ
ZddMgr::ZddMgr(
  const ZddMgrHolder& holder
) : ZddMgrHolder(holder)
{
}

// @brief コピーコンストラクタ
ZddMgr::ZddMgr(
  const ZddMgr& src
) : ZddMgrHolder(src)
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
  return get()->item_num();
}

// @brief 要素を返す．
ZddItem
ZddMgr::item(
  SizeType item_id
)
{
  auto edge = get()->item(item_id);
  return _item(edge);
}

// @brief 要素のリストを返す．
std::vector<ZddItem>
ZddMgr::item_list() const
{
  return conv_to_itemlist(get()->item_list());
}

// @brief 空集合を作る．
Zdd
ZddMgr::zero()
{
  return _zdd(DdEdge::zero());
}

// @brief ユニバースを作る．
Zdd
ZddMgr::one()
{
  return _zdd(DdEdge::one());
}

// @brief 部分集合を作る．
Zdd
ZddMgr::make_set(
  const std::vector<ZddItem> item_list
)
{
  auto level_list = ZddItem::conv_to_levellist(item_list);
  auto edge = get()->make_set(level_list);
  return _zdd(edge);
}

// @brief バイナリダンプから復元する．
std::vector<Zdd>
ZddMgr::restore(
  BinDec& s
)
{
  auto edge_list = get()->restore(s);
  return conv_to_zddlist(edge_list);
}

// @brief ガーベージコレクションを行う．
void
ZddMgr::garbage_collection()
{
  get()->garbage_collection();
}

// @brief ノード数を返す．
SizeType
ZddMgr::node_num() const
{
  return get()->node_num();
}

// @brief GC を起動するしきい値を返す．
SizeType
ZddMgr::gc_limit() const
{
  return get()->gc_limit();
}

// @brief GC を起動するしきい値を設定する．
void
ZddMgr::set_gc_limit(
  SizeType limit
)
{
  get()->set_gc_limit(limit);
}

// @brief GC を許可する．
void
ZddMgr::enable_gc()
{
  get()->enable_gc();
}

// @brief GC を禁止する．
void
ZddMgr::disable_gc()
{
  get()->disable_gc();
}

// @brief 枝のリストをZddのリストに変換する．
std::vector<Zdd>
ZddMgr::conv_to_zddlist(
  const std::vector<DdEdge>& edge_list
) const
{
  std::vector<Zdd> zdd_list;
  zdd_list.reserve(edge_list.size());
  for ( auto edge: edge_list ) {
    auto zdd = _zdd(edge);
    zdd_list.push_back(zdd);
  }
  return zdd_list;
}

// @brief 枝のリストを要素のリストに変換する．
std::vector<ZddItem>
ZddMgr::conv_to_itemlist(
  const std::vector<DdEdge>& edge_list
) const
{
  std::vector<ZddItem> item_list;
  item_list.reserve(edge_list.size());
  for ( auto edge: edge_list ) {
    auto item = _item(edge);
    item_list.push_back(item);
  }
  return item_list;
}

END_NAMESPACE_YM_DD
