
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
DdEdge
ZddMgrImpl::item(
  SizeType elem_id
)
{
  while ( mItemList.size() <= elem_id ) {
    auto id1 = new_variable();
    auto level1 = varid_to_level(id1);
    auto edge1 = new_node(level1, DdEdge::zero(), DdEdge::one());
    mItemList.push_back(edge1);
    activate(edge1);
  }
  return mItemList[elem_id];
}

// @brief 部分集合を作る．
DdEdge
ZddMgrImpl::make_set(
  const vector<SizeType>& level_list
)
{
  // ボトムアップに作るので逆順のリストを用意する．
  auto tmp_list = level_list;
  std::sort(tmp_list.begin(), tmp_list.end(), std::greater<>());
  auto f0 = DdEdge::zero();
  auto f1 = DdEdge::one();
  for ( auto level: tmp_list ) {
    f1 = new_node(level, f0, f1);
  }
  return f1;
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
  const DdInfoMgr& info_mgr
)
{
  SizeType n = info_mgr.root_list().size();

  // シグネチャ
  s.write_signature(ZDD_SIG);
  // 要素数
  s.write_vint(n);

  if ( n == 0 ) {
    return;
  }

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
vector<DdEdge>
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

  vector<DdEdge> root_list(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    SizeType root_info = root_info_list[i];
    auto edge = decode(root_info, edge_list);
    root_list[i] = edge;
  }
  return root_list;
}

END_NAMESPACE_YM_DD
