
/// @file Bdd_dump.cc
/// @brief Bdd::dump の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Bdd.h"
#include "ym/BddMgr.h"
#include "ym/BddInfo.h"
#include "ym/BddError.h"
#include "BddEdge.h"
#include "BddMgrImpl.h"
#include "ym/BinEnc.h"
#include "ym/BinDec.h"


BEGIN_NAMESPACE_YM_BDD

BEGIN_NONAMESPACE

// ダンプ用のシグネチャ
const char* BDD_SIG{"ym_bdd1.0"};

// 枝の情報をダンプする．
void
dump_edge(
  BinEnc& s,
  SizeType id,
  SizeType edge
)
{
  auto node = BddInfo::edge2node(edge);
  if ( node == 0 ) { // 定数
    s.write_vint(edge);
  }
  else {
    auto inv = BddInfo::edge2inv(edge);
    SizeType delta = id - node;
    SizeType val = (delta * 2) + static_cast<SizeType>(inv);
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
  auto delta = BddInfo::edge2node(val);
  if ( delta == 0 ) {
    return val;
  }
  auto inv = BddInfo::edge2inv(val);
  auto node = id - delta;
  return node * 2 + static_cast<SizeType>(inv);
}

END_NONAMESPACE

// @brief 独自形式でバイナリダンプする．
void
Bdd::dump(
  BinEnc& s
) const
{
  dump(s, {*this});
}

// @brief 複数のBDDを独自形式でバイナリダンプする．
void
Bdd::dump(
  BinEnc& s,
  const vector<Bdd>& bdd_list
)
{
  SizeType n = bdd_list.size();
  // シグネチャ
  s.write_signature(BDD_SIG);
  // 要素数
  s.write_vint(n);

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
    s.write_vint(node.index);
    // 0枝
    dump_edge(s, id, node.edge0);
    // 1枝
    dump_edge(s, id, node.edge1);
    ++ id;
  }
  // end-marker
  s.write_vint(0);
  s.write_vint(0);
  s.write_vint(0);
}

BEGIN_NONAMESPACE

BddEdge
decode(
  SizeType edge_info,
  const vector<BddEdge>& edge_list
)
{
  if ( edge_info == 0 ) {
    return BddEdge::zero();
  }
  if ( edge_info == 1 ) {
    return BddEdge::one();
  }
  auto node = BddInfo::edge2node(edge_info);
  auto inv = BddInfo::edge2inv(edge_info);
  return edge_list[node - 1] ^ inv;
}

END_NONAMESPACE

// @brief バイナリダンプから復元する．
vector<Bdd>
BddMgr::restore(
  BinDec& s
)
{
  if ( !s.read_signature(BDD_SIG) ) {
    throw BddError{"Bdd::restore(): wrong signature"};
  }

  SizeType n = s.read_vint();
  vector<SizeType> root_info_list(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    root_info_list[i] = s.read_vint();
  }

  vector<BddEdge> edge_list;
  for ( SizeType id = 1; ; ++ id ) {
    SizeType index = s.read_vint();
    SizeType edge0_info = restore_edge(s, id);
    SizeType edge1_info = restore_edge(s, id);
    if ( index == 0 && edge0_info == 0 && edge1_info == 0 ) {
      break;
    }
    BddEdge edge0 = decode(edge0_info, edge_list);
    BddEdge edge1 = decode(edge1_info, edge_list);
    BddEdge edge = mImpl->new_node(index, edge0, edge1);
    edge_list.push_back(edge);
  }

  vector<Bdd> bdd_list(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    SizeType root_info = root_info_list[i];
    BddEdge edge = decode(root_info, edge_list);
    bdd_list[i] = Bdd{impl(), edge};
  }
  return bdd_list;
}

END_NAMESPACE_YM_BDD
