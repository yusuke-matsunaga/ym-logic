
/// @file BddMgrImpl.cc
/// @brief BddMgrImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "BddMgrImpl.h"
#include "ym/Bdd.h"
#include "ym/BddVar.h"
#include "DdInfo.h"
#include "DdInfoMgr.h"
#include "ym/BinEnc.h"
#include "ym/BinDec.h"


BEGIN_NAMESPACE_YM_DD

// @brief コンストラクタ
BddMgrImpl::BddMgrImpl()
{
}

// @brief デストラクタ
BddMgrImpl::~BddMgrImpl()
{
}

// @breif 変数の数を返す．
SizeType
BddMgrImpl::variable_num() const
{
  return mVarList.size();
}

// @brief 変数を返す．
DdEdge
BddMgrImpl::variable(
  SizeType varid
)
{
  while ( mVarList.size() <= varid ) {
    auto varid1 = new_variable();
    auto level1 = varid_to_level(varid1);
    auto edge1 = new_node(level1, DdEdge::zero(), DdEdge::one());
    mVarList.push_back(edge1);
    activate(edge1);
  }
  return mVarList[varid];
}

// @brief 変数順を表す変数のリストを返す．
vector<DdEdge>
BddMgrImpl::variable_order()
{
  auto order_list = variable_list();
  sort(order_list.begin(), order_list.end(),
       [&](DdEdge a, DdEdge b) {
	 auto node_a = a.node();
	 auto node_b = b.node();
	 auto level_a = node_a->level();
	 auto level_b = node_b->level();
	 return level_a < level_b;
       });
  return order_list;
}

// @brief 変数順を設定する．
void
BddMgrImpl::set_variable_order(
  const vector<DdEdge>& order_list
)
{
  auto nv = variable_num();
  if ( order_list.size() != nv ) {
    throw std::invalid_argument{"order_list.size() mismatch"};
  }
  for ( SizeType i = 0; i < nv; ++ i ) {
    auto dst_level = nv - i - 1;
    auto edge = order_list[dst_level];
    auto node = edge.node();
    auto level = node->level();
    // level を dst_level に移動する．
    for ( ; level < dst_level; ++ level ) {
      swap_level(level);
    }
  }
}

// @brief sifting を用いて変数順の最適化を行う．
void
BddMgrImpl::dvo_sift()
{
  auto nv = variable_num();
  vector<bool> lock_array(nv, false);
  while ( true ) {
    // ノード数が最大の変数を選ぶ．
    SizeType max_num = 0;
    SizeType max_varid = 0;
    SizeType max_level = 0;
    for ( SizeType varid = 0; varid < nv; ++ varid ) {
      if ( lock_array[varid] ) {
	// ロックされていたらスキップする．
	continue;
      }
      auto level = varid_to_level(varid);
      auto n = node_num(level);
      if ( max_num < n ) {
	max_num = n;
	max_varid = varid;
	max_level = level;
      }
    }
    if ( max_num == 0 ) {
      break;
    }
    dvo_sub(max_level);
    lock_array[max_varid] = true;
  }
}

// @brief dvo_sift() の下請け関数
void
BddMgrImpl::dvo_sub(
  SizeType level
)
{
  auto best_num = node_num();
  auto best_level = level;
  // level を最下部まで移動させる．
  auto bottom_level = variable_num() - 1;
  for ( SizeType level1 = level; level1 < bottom_level; ++ level1 ) {
    swap_level(level1);
    auto num = node_num();
    if ( best_num > num ) {
      best_num = num;
      best_level = level1 + 1;
    }
  }
  // 最下部のインデックスを level まで移動させる．
  for ( SizeType level1 = variable_num(); level1 > level; -- level1 ) {
    swap_level(level1 - 1);
  }
  // level を最上部まで移動させる．
  for ( SizeType level1 = level; level1 > 0; -- level1 ) {
    swap_level(level1 - 1);
    auto num = node_num();
    if ( best_num > num ) {
      best_num = num;
      best_level = level1 - 1;
    }
  }
  // 最上部のインデックスを best_level まで移動させる．
  for ( SizeType level1 = 0; level1 < best_level; ++ level1 ) {
    swap_level(level1);
  }
}

BEGIN_NONAMESPACE

inline
bool
_decomp(
  DdEdge edge,
  SizeType level1,
  DdEdge& edge0,
  DdEdge& edge1
)
{
  if ( !edge.is_const() ) {
    auto node = edge.node();
    auto inv = edge.inv();
    auto level = node->level();
    if ( level == level1 ) {
      edge0 = node->edge0() ^ inv;
      edge1 = node->edge1() ^ inv;
      return true;
    }
  }
  edge0 = edge;
  edge1 = edge;
  return false;
}

END_NONAMESPACE

// @brief 隣り合うインデックスを交換する．
void
BddMgrImpl::swap_level(
  SizeType level
)
{
  // まず単純に隣り合うレベルのテーブルを入れ替える．
  DdNodeMgr::swap_level(level);

  // 問題は level + 1 の子供を持つ level のノード
  //
  //      level                level
  //     /     \              /     \
  // level+1 level+1  ==> level+1 level+1
  //  /   \   /   \        /   \    /   \
  // e00 e01 e10 e11      e00 e10  e01 e11
  //
  // 他のノードから参照されているので同じノードの
  // 中身を入れ替える．
  auto level2 = level + 1;
  scan(level, [&](DdNode* node) {
    DdEdge e0 = node->edge0();
    DdEdge e00, e01;
    auto d0 = _decomp(e0, level, e00, e01);
    DdEdge e1 = node->edge1();
    DdEdge e10, e11;
    auto d1 = _decomp(e1, level, e10, e11);
    if ( d0 || d1 ) {
      // 2つのレベルのインデックスを入れ替える．
      auto new_e0 = new_node(level2, e00, e10);
      auto new_e1 = new_node(level2, e01, e11);
      node->chg_level(level);
      node->chg_edges(new_e0, new_e1);
      activate(new_e0);
      activate(new_e1);
      deactivate(e0);
      deactivate(e1);
      reg_node(node);
      return true;
    }
    return false;
  });
  garbage_collection(level);
}

// @brief ノードを作る．
DdEdge
BddMgrImpl::new_node(
  SizeType level,
  DdEdge edge0,
  DdEdge edge1
)
{
  // 2つの枝が等しい時は新しいノードを作らない．
  if ( edge0 == edge1 ) {
    return edge0;
  }

  // 極性の正規化を行う．
  auto oinv = edge0.inv();
  edge0 ^= oinv;
  edge1 ^= oinv;

  auto node = DdNodeMgr::new_node(level, edge0, edge1);

  return DdEdge{node, oinv};
}

// @brief garbage_collection() が呼ばれた後に呼び出される関数
void
BddMgrImpl::after_gc()
{
}

BEGIN_NONAMESPACE

// ダンプ用のシグネチャ
const char* BDD_SIG{"ym_bdd1.0"};

// 枝の情報をダンプする．
void
dump_edge(
  BinEnc& s,
  SizeType id,
  SizeType node,
  bool inv
)
{
  if ( node == 0 ) { // 定数
    s.write_vint(static_cast<SizeType>(inv));
  }
  else {
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
  auto delta = DdInfo::edge2node(val);
  if ( delta == 0 ) {
    return val;
  }
  auto inv = DdInfo::edge2inv(val);
  auto node = id - delta;
  return node * 2 + static_cast<SizeType>(inv);
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
  auto inv = DdInfo::edge2inv(edge_info);
  return edge_list[node - 1] ^ inv;
}

END_NONAMESPACE

// @brief BDD の内容をバイナリダンプする．
void
BddMgrImpl::dump(
  BinEnc& s,
  const DdInfoMgr& info_mgr
)
{
  // シグネチャ
  s.write_signature(BDD_SIG);

  // 要素数
  auto n = info_mgr.root_list().size();
  s.write_vint(n);
  if ( n == 0 ) {
    return;
  }

  // 変数順
  auto nv = info_mgr.max_level();
  s.write_vint(nv);
  for ( SizeType i = 0; i < nv; ++ i ) {
    auto varid = info_mgr.level_to_varid(i);
    s.write_vint(varid);
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
    dump_edge(s, id, node.edge0_node(), node.edge0_inv());
    // 1枝
    dump_edge(s, id, node.edge1_node(), node.edge1_inv());
    ++ id;
  }
  // end-marker
  s.write_vint(0);
  s.write_vint(0);
  s.write_vint(0);
}

// @brief バイナリダンプから復元する．
vector<DdEdge>
BddMgrImpl::restore(
  BinDec& s
)
{
  if ( !s.read_signature(BDD_SIG) ) {
    throw std::invalid_argument{"Bdd::restore(): wrong signature"};
  }

  SizeType n = s.read_vint();
  if ( n == 0 ) {
    return {};
  }

  SizeType nv = variable_num();
  SizeType nv1 = s.read_vint();
  nv = std::max(nv, nv1);
  vector<DdEdge> var_list(nv);
  for ( SizeType i = 0; i < nv1; ++ i ) {
    auto varid = s.read_vint();
    var_list[i] = variable(varid);
  }
  for ( SizeType i = nv1; i < nv; ++ i ) {
    var_list[i] = variable(i);
  }
  set_variable_order(var_list);

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
