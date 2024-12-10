
/// @file ZddMgrPtr.cc
/// @brief ZddMgrPtr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/ZddMgrPtr.h"
#include "ym/ZddMgr.h"
#include "ZddMgrImpl.h"
#include "DdInfoMgr.h"


BEGIN_NAMESPACE_YM_DD

//////////////////////////////////////////////////////////////////////
// クラス ZddMgrPtr
//////////////////////////////////////////////////////////////////////

// @brief 生のポインタを指定したコンストラクタ
ZddMgrPtr::ZddMgrPtr(
  ZddMgrImpl* ptr
) : mPtr{ptr}
{
}

// @brief コピーコンストラクタ
ZddMgrPtr::ZddMgrPtr(
  const ZddMgrPtr& src
) : mPtr{src.mPtr}
{
}

// @brief コピー代入演算子
ZddMgrPtr&
ZddMgrPtr::operator=(
  const ZddMgrPtr& src
)
{
  mPtr = src.mPtr;
  return *this;
}

// @brief デストラクタ
ZddMgrPtr::~ZddMgrPtr()
{
}

// @brief 要素を返す．
ZddItem
ZddMgrPtr::item(
  SizeType elem_id
) const
{
  auto edge = mPtr->item(elem_id);
  return _zdd(edge);
}

// @brief 要素のリストを返す．
vector<ZddItem>
ZddMgrPtr::item_list() const
{
  auto edge_list = mPtr->item_list();
  return conv_to_itemlist(edge_list);
}

// @brief 空集合を作る．
Zdd
ZddMgrPtr::zero() const
{
  return _zdd(DdEdge::zero());
}

// @brief ユニバースを作る．
Zdd
ZddMgrPtr::one() const
{
  return _zdd(DdEdge::one());
}

// @brief ZDDを作る．
Zdd
ZddMgrPtr::zdd(
  SizeType level,
  const Zdd& edge0,
  const Zdd& edge1
) const
{
  edge0._check_valid();
  edge1._check_valid();
  _check_mgr(edge0);
  _check_mgr(edge1);
  auto edge = mPtr->new_node(level, edge0.root(), edge1.root());
  return _zdd(edge);
}

// @brief 部分集合を作る．
Zdd
ZddMgrPtr::make_set(
  const vector<ZddItem>& item_list
) const
{
  auto n = item_list.size();
  vector<SizeType> level_list;
  level_list.reserve(n);
  for ( auto& item: item_list ) {
    auto level = item.level();
    level_list.push_back(level);
  }
  sort(level_list.begin(), level_list.end(),
       [](SizeType a, SizeType b) {
	 return a > b;
       });
  auto edge = mPtr->make_set(level_list);
  return _zdd(edge);
}

// @brief レベルを要素に変換する．
ZddItem
ZddMgrPtr::level_to_item(
  SizeType level
) const
{
  auto edge = mPtr->new_node(level, DdEdge::zero(), DdEdge::one());
  return _item(edge);
}

// @brief 複数のZDDの内容を出力する．
void
ZddMgrPtr::display(
  ostream& s,
  const vector<Zdd>& zdd_list
) const
{
  auto info_mgr = node_info(zdd_list);
  info_mgr.display(s);
}

// @brief 構造を表す整数配列を作る．
vector<SizeType>
ZddMgrPtr::rep_data(
  const vector<Zdd>& zdd_list ///< [in] ZDDのリスト
) const
{
  auto info_mgr = node_info(zdd_list);
  return info_mgr.rep_data();
}

// @brief 複数のZDDを独自形式でバイナリダンプする．
void
ZddMgrPtr::dump(
  BinEnc& s,
  const vector<Zdd>& zdd_list
) const
{
  auto info_mgr = node_info(zdd_list);
  mPtr->dump(s, info_mgr);
}

// @brief バイナリダンプから復元する．
vector<Zdd>
ZddMgrPtr::restore(
  BinDec& s
) const
{
  auto edge_list = mPtr->restore(s);
  return conv_to_zddlist(edge_list);
}

// @brief 複数のZDDのノード数を数える．
SizeType
ZddMgrPtr::zdd_size(
  const vector<Zdd>& zdd_list
) const
{
  if ( zdd_list.empty() ) {
    return 0;
  }
  auto info_mgr = node_info(zdd_list);
  return info_mgr.node_num();
}

// @brief 複数のBDDのノードの情報を取り出す．
DdInfoMgr
ZddMgrPtr::node_info(
  const vector<Zdd>& zdd_list
) const
{
  vector<DdEdge> edge_list;
  edge_list.reserve(zdd_list.size());
  for ( auto& zdd: zdd_list ) {
    auto edge = zdd.root();
    edge_list.push_back(edge);
  }
  return DdInfoMgr{edge_list, get()};
}

// @brief 親のマネージャを返す．
ZddMgr
ZddMgrPtr::mgr() const
{
  return ZddMgr{*this};
}

// @brief Zdd を作る．
Zdd
ZddMgrPtr::_zdd(
  DdEdge edge
) const
{
  return Zdd{*this, edge};
}

// @brief ZddItem を作る．
ZddItem
ZddMgrPtr::_item(
  DdEdge edge
) const
{
  return ZddItem{*this, edge};
}

// @brief 同じマネージャの要素かチェックする．
void
ZddMgrPtr::_check_mgr(
  const Zdd& zdd
) const
{
  _check_mgr(zdd.mMgr);
}

// @brief 枝のリストを Zdd のリストに変換する．
vector<Zdd>
ZddMgrPtr::conv_to_zddlist(
  const vector<DdEdge>& edge_list
) const
{
  vector<Zdd> zdd_list;
  zdd_list.reserve(edge_list.size());
  for ( auto edge: edge_list ) {
    auto zdd = _zdd(edge);
    zdd_list.push_back(zdd);
  }
  return zdd_list;
}

// @brief 枝のリストを要素のリストに変換する．
vector<ZddItem>
ZddMgrPtr::conv_to_itemlist(
  const vector<DdEdge>& edge_list
) const
{
  vector<ZddItem> item_list;
  item_list.reserve(edge_list.size());
  for ( auto edge: edge_list ) {
    auto item = _item(edge);
    item_list.push_back(item);
  }
  return item_list;
}

END_NAMESPACE_YM_DD
