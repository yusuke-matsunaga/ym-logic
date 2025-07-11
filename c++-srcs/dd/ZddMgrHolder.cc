
/// @file ZddMgrHolder.cc
/// @brief ZddMgrHolder の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/ZddMgrHolder.h"
#include "ym/ZddMgr.h"
#include "ZddMgrImpl.h"
#include "DdInfoMgr.h"


BEGIN_NAMESPACE_YM_DD

//////////////////////////////////////////////////////////////////////
// クラス ZddMgrHolder
//////////////////////////////////////////////////////////////////////

// @brief 生のポインタを指定したコンストラクタ
ZddMgrHolder::ZddMgrHolder(
  ZddMgrImpl* ptr
) : mPtr{ptr}
{
}

// @brief コピーコンストラクタ
ZddMgrHolder::ZddMgrHolder(
  const ZddMgrHolder& src
) : mPtr{src.mPtr}
{
}

// @brief コピー代入演算子
ZddMgrHolder&
ZddMgrHolder::operator=(
  const ZddMgrHolder& src
)
{
  mPtr = src.mPtr;
  return *this;
}

// @brief デストラクタ
ZddMgrHolder::~ZddMgrHolder()
{
}

// @brief 親のマネージャを返す．
ZddMgr
ZddMgrHolder::mgr() const
{
  return ZddMgr{*this};
}

// @brief Zdd を作る．
Zdd
ZddMgrHolder::_zdd(
  DdEdge edge
) const
{
  return Zdd{*this, edge};
}

// @brief ZddItem を作る．
ZddItem
ZddMgrHolder::_item(
  DdEdge edge
) const
{
  return ZddItem{*this, edge};
}

// @brief 枝のリストを Zdd のリストに変換する．
std::vector<Zdd>
ZddMgrHolder::conv_to_zddlist(
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
ZddMgrHolder::conv_to_itemlist(
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
