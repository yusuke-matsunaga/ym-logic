
/// @file Zdd_dump.cc
/// @brief Zdd::dump の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Zdd.h"
#include "ym/ZddMgr.h"
#include "ym/ZddInfo.h"
#include "ZddEdge.h"
#include "ZddMgrImpl.h"
#include "ym/BinEnc.h"
#include "ym/BinDec.h"


BEGIN_NAMESPACE_YM_ZDD

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

SizeType
restore_edge(
  BinDec& s,
  SizeType id
)
{
  auto val = s.read_vint();
  auto delta = ZddInfo::edge2node(val);
  if ( delta == 0 ) {
    return val;
  }
  auto node = id - delta;
  return node * 2;
}

END_NONAMESPACE

// @brief 独自形式でバイナリダンプする．
void
Zdd::dump(
  BinEnc& s
) const
{
  dump(s, {*this});
}

// @brief 複数のZDDを独自形式でバイナリダンプする．
void
Zdd::dump(
  BinEnc& s,
  const vector<Zdd>& bdd_list
)
{
  SizeType n = bdd_list.size();
  // シグネチャ
  s.write_signature(ZDD_SIG);
  // 要素数
  s.write_vint(n);

  if ( n == 0 ) {
    return;
  }

  vector<SizeType> root_edge_list;
  auto node_list = node_info(bdd_list, root_edge_list);
  // 根の枝
  for ( auto root_edge: root_edge_list ) {
    s.write_vint(root_edge);
  }
  // ノードリスト
  SizeType id = 1;
  for ( auto& node: node_list ) {
    // インデックス
    s.write_vint(node.index());
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

ZddEdge
decode(
  SizeType edge_info,
  const vector<ZddEdge>& edge_list
)
{
  if ( edge_info == 0 ) {
    return ZddEdge::zero();
  }
  if ( edge_info == 1 ) {
    return ZddEdge::one();
  }
  auto node = ZddInfo::edge2node(edge_info);
  return edge_list[node - 1];
}

END_NONAMESPACE

// @brief バイナリダンプから復元する．
vector<Zdd>
ZddMgr::restore(
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

  vector<ZddEdge> edge_list;
  for ( SizeType id = 1; ; ++ id ) {
    SizeType index = s.read_vint();
    SizeType edge0_info = restore_edge(s, id);
    SizeType edge1_info = restore_edge(s, id);
    if ( index == 0 && edge0_info == 0 && edge1_info == 0 ) {
      break;
    }
    auto edge0 = decode(edge0_info, edge_list);
    auto edge1 = decode(edge1_info, edge_list);
    auto edge = mImpl->new_node(index, edge0, edge1);
    edge_list.push_back(edge);
  }

  vector<Zdd> bdd_list(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    SizeType root_info = root_info_list[i];
    auto edge = decode(root_info, edge_list);
    bdd_list[i] = Zdd{impl(), edge};
  }
  return bdd_list;
}

END_NAMESPACE_YM_ZDD
