
/// @file ZddMgrImpl.cc
/// @brief ZddMgrImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Zdd.h"
#include "ZddMgrImpl.h"
#include "DdInfo.h"
#include "DdInfoMgr.h"
#include "ym/BinEnc.h"
#include "ym/BinDec.h"


BEGIN_NAMESPACE_YM_DD

// @brief コンストラクタ
ZddMgrImpl::ZddMgrImpl()
{
}

// @brief デストラクタ
ZddMgrImpl::~ZddMgrImpl()
{
}

// @brief 要素数を返す．
SizeType
ZddMgrImpl::item_num() const
{
  return mItemList.size();
}

// @brief 要素を返す．
ZddItem
ZddMgrImpl::item(
  SizeType elem_id
)
{
  while ( mItemList.size() <= elem_id ) {
    auto id1 = new_variable();
    auto level1 = varid_to_level(id1);
    auto item1 = level_to_item(level1);
    mItemList.push_back(item1);
  }
  return mItemList[elem_id];
}

// @brief 要素のリストを返す．
vector<ZddItem>
ZddMgrImpl::item_list() const
{
  return mItemList;
}

// @brief 空集合を作る．
Zdd
ZddMgrImpl::zero()
{
  return _zdd(DdEdge::zero());
}

// @brief ユニバースを作る．
Zdd
ZddMgrImpl::one()
{
  return _zdd(DdEdge::one());
}

// @brief ZDDを作る．
Zdd
ZddMgrImpl::zdd(
  SizeType level,
  const Zdd& edge0,
  const Zdd& edge1
)
{
  edge0._check_valid();
  edge1._check_valid();
  _check_mgr(edge0);
  _check_mgr(edge1);
  auto e = new_node(level, _edge(edge0), _edge(edge1));
  return _zdd(e);
}

// @brief 部分集合を作る．
Zdd
ZddMgrImpl::make_set(
  const vector<ZddItem>& item_list
)
{
  auto n = item_list.size();
  vector<SizeType> tmp_list;
  tmp_list.reserve(n);
  for ( auto& item: item_list ) {
    auto level = item.level();
    tmp_list.push_back(level);
  }
  sort(tmp_list.begin(), tmp_list.end(), [](int a, int b) {
    return a > b;
  });

  auto f0 = DdEdge::zero();
  auto f1 = DdEdge::one();
  for ( auto elem: tmp_list ) {
    f1 = new_node(elem, f0, f1);
  }
  return _zdd(f1);
}

// @brief 複数のZDDのノード数を数える．
SizeType
ZddMgrImpl::zdd_size(
  const vector<Zdd>& zdd_list
)
{
  if ( zdd_list.empty() ) {
    return 0;
  }
  auto info_mgr = node_info(zdd_list);
  return info_mgr.node_list().size();
}

// @brief ZDD を作る．
Zdd
ZddMgrImpl::_zdd(
  DdEdge edge
)
{
  return Zdd{this, edge};
}

// @brief Zdd から DdEdge を取り出す．
DdEdge
ZddMgrImpl::_edge(
  const Zdd& zdd
)
{
  return DdEdge{zdd.mRoot};
}

// @brief ノードを作る．
DdEdge
ZddMgrImpl::new_node(
  SizeType level,
  DdEdge edge0,
  DdEdge edge1
)
{
  // edge1 が 0 の時は新しいノードを作らない．
  if ( edge1.is_zero() ) {
    return edge0;
  }

  auto node = DdNodeMgr::new_node(level, edge0, edge1);
  return DdEdge{node};
}

// @brief garbage_collection() が呼ばれた後に呼び出される関数
void
ZddMgrImpl::after_gc()
{
}

// @brief BDDの根の枝のリストを作る．
vector<DdEdge>
ZddMgrImpl::root_list(
  const vector<Zdd>& zdd_list
)
{
  SizeType n = zdd_list.size();
  vector<DdEdge> edge_list(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    auto& zdd = zdd_list[i];
    zdd._check_valid();
    edge_list[i] = _edge(zdd);
  }
  return edge_list;
}

// @brief レベルを要素に変換する．
ZddItem
ZddMgrImpl::level_to_item(
  SizeType level
)
{
  auto e = new_node(level, DdEdge::zero(), DdEdge::one());
  return ZddItem{this, e};
}

// @brief 複数のZDDのノードの情報を取り出す．
DdInfoMgr
ZddMgrImpl::node_info(
  const vector<Zdd>& zdd_list
)
{
  auto edge_list = root_list(zdd_list);
  return DdInfoMgr{edge_list, this};
}

// @brief 複数のZDDの内容を出力する．
void
ZddMgrImpl::display(
  ostream& s,
  const vector<Zdd>& bdd_list
)
{
  auto info_mgr = node_info(bdd_list);
  info_mgr.display(s);
}

// @brief 構造を表す整数配列を作る．
vector<SizeType>
ZddMgrImpl::rep_data(
  const vector<Zdd>& bdd_list
)
{
  auto info_mgr = node_info(bdd_list);
  return info_mgr.rep_data();
}

BEGIN_NONAMESPACE

// ダンプ用のシグネチャ
const char* ZDD_SIG{"ym_zdd1.0"};

// 枝の情報をダンプする．
void
dump_edge(
  BinEnc& s,
  SizeType id,
  SizeType node
)
{
  if ( node == 0 ) { // 定数
    s.write_vint(0);
  }
  else {
    SizeType delta = id - node;
    SizeType val = (delta * 2);
    s.write_vint(val);
  }
}

END_NONAMESPACE

// @brief 複数のZDDを独自形式でバイナリダンプする．
void
ZddMgrImpl::dump(
  BinEnc& s,
  const vector<Zdd>& zdd_list
)
{
  SizeType n = zdd_list.size();
  // シグネチャ
  s.write_signature(ZDD_SIG);
  // 要素数
  s.write_vint(n);

  if ( n == 0 ) {
    return;
  }

  auto info_mgr = node_info(zdd_list);
  // 根の枝
  for ( auto root: info_mgr.root_list() ) {
    s.write_vint(root);
  }
  // ノードリスト
  SizeType id = 1;
  for ( auto& node: info_mgr.node_list() ) {
    // レベル
    s.write_vint(node.level());
    // 0枝
    dump_edge(s, id, node.edge0_node());
    // 1枝
    dump_edge(s, id, node.edge1_node());
    ++ id;
  }
  // end-marker
  s.write_vint(0);
  s.write_vint(0);
  s.write_vint(0);
}

BEGIN_NONAMESPACE

SizeType
restore_edge(
  BinDec& s,
  SizeType id
)
{
  auto val = s.read_vint();
  auto delta = DdInfo::edge2node(val);
  if ( delta == 0 ) {
    return val;
  }
  auto node = id - delta;
  return node * 2;
}

DdEdge
decode(
  SizeType edge_info,
  const vector<DdEdge>& edge_list
)
{
  if ( edge_info == 0 ) {
    return DdEdge::zero();
  }
  if ( edge_info == 1 ) {
    return DdEdge::one();
  }
  auto node = DdInfo::edge2node(edge_info);
  return edge_list[node - 1];
}

END_NONAMESPACE

// @brief バイナリダンプから復元する．
vector<Zdd>
ZddMgrImpl::restore(
  BinDec& s
)
{
  if ( !s.read_signature(ZDD_SIG) ) {
    throw std::invalid_argument{"Zdd::restore(): wrong signature"};
  }

  SizeType n = s.read_vint();
  if ( n == 0 ) {
    return {};
  }

  vector<SizeType> root_info_list(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    root_info_list[i] = s.read_vint();
  }

  vector<DdEdge> edge_list;
  for ( SizeType id = 1; ; ++ id ) {
    SizeType level = s.read_vint();
    SizeType edge0_info = restore_edge(s, id);
    SizeType edge1_info = restore_edge(s, id);
    if ( level == 0 && edge0_info == 0 && edge1_info == 0 ) {
      break;
    }
    auto edge0 = decode(edge0_info, edge_list);
    auto edge1 = decode(edge1_info, edge_list);
    auto edge = new_node(level, edge0, edge1);
    edge_list.push_back(edge);
  }

  vector<Zdd> zdd_list(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    SizeType root_info = root_info_list[i];
    auto edge = decode(root_info, edge_list);
    zdd_list[i] = _zdd(edge);
  }
  return zdd_list;
}

END_NAMESPACE_YM_DD
