
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

// SizeType をダンプする．
void
dump_int(
  BinEnc& enc,
  SizeType num
)
{
  if ( num <= 127 ) {
    enc.write_8(static_cast<ymuint8>(num));
  }
  else {
    SizeType num1 = (num & 127) | 128;
    enc.write_8(static_cast<ymuint8>(num1));
    dump_int(enc, (num >> 7));
  }
}

// 枝の情報をダンプする．
void
dump_edge(
  BinEnc& enc,
  SizeType id,
  SizeType edge
)
{
  auto node = BddInfo::edge2node(edge);
  if ( node == 0 ) { // 定数
    dump_int(enc, edge);
  }
  else {
    auto inv = BddInfo::edge2inv(edge);
    SizeType delta = id - node;
    dump_int(enc, (delta * 2) + static_cast<SizeType>(inv));
  }
}

SizeType
restore_int(
  BinDec& dec
)
{
  SizeType num = 0;
  for ( SizeType weight = 1; ; weight *= 128 ) {
    SizeType c = dec.read_8();
    num += (c & 127) * weight;
    if ( (c & 128) == 0 ) {
      break;
    }
  }
  return num;
}

SizeType
restore_edge(
  BinDec& dec,
  SizeType id
)
{
  auto val = restore_int(dec);
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
  ostream& s
) const
{
  dump(s, {*this});
}

// @brief 複数のBDDを独自形式でバイナリダンプする．
void
Bdd::dump(
  ostream& s,
  const vector<Bdd>& bdd_list
)
{
  BinEnc enc{s};
  SizeType n = bdd_list.size();
  // シグネチャ
  enc.write_string(BDD_SIG);
  // 要素数
  enc.write_64(n);

  vector<SizeType> root_edge_list;
  auto node_list = node_info(bdd_list, root_edge_list);
  // 根の枝
  for ( auto root_edge: root_edge_list ) {
    dump_int(enc, root_edge);
  }
  // ノードリスト
  SizeType id = 1;
  for ( auto& node: node_list ) {
    // インデックス
    dump_int(enc, node.index);
    // 0枝
    dump_edge(enc, id, node.edge0);
    // 1枝
    dump_edge(enc, id, node.edge1);
    ++ id;
  }
  // end-marker
  dump_int(enc, 0);
  dump_int(enc, 0);
  dump_int(enc, 0);
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
  istream& s
)
{
  BinDec dec{s};

  string sig = dec.read_string();
  SizeType n = dec.read_64();
  if ( sig != BDD_SIG ) {
    throw BddError{"Bdd::restore(): wrong signature"};
  }

  vector<SizeType> root_info_list(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    root_info_list[i] = restore_int(dec);
  }

  vector<BddEdge> edge_list;
  for ( SizeType id = 1; ; ++ id ) {
    SizeType index = restore_int(dec);
    SizeType edge0_info = restore_edge(dec, id);
    SizeType edge1_info = restore_edge(dec, id);
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
