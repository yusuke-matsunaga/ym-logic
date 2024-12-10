
/// @file BddMgrImpl.cc
/// @brief BddMgrImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Bdd.h"
#include "BddMgrImpl.h"
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
  cout << "BddMgrImpl is destructed" << endl;
}

// @breif 変数の数を返す．
SizeType
BddMgrImpl::variable_num() const
{
  return mVarList.size();
}

// @brief 変数を返す．
BddVar
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
  return _bdd(mVarList[varid]);
}

// @brief 変数のリストを返す．
vector<BddVar>
BddMgrImpl::variable_list()
{
  vector<BddVar> var_list;
  var_list.reserve(variable_num());
  for ( auto edge: mVarList ) {
    auto var = _var(edge);
    var_list.push_back(var);
  }
  return var_list;
}

// @brief 恒偽関数を作る．
Bdd
BddMgrImpl::zero()
{
  return _bdd(DdEdge::zero());
}

// @brief 恒真関数を作る．
Bdd
BddMgrImpl::one()
{
  return _bdd(DdEdge::one());
}

// @brief BDD を作る．
Bdd
BddMgrImpl::bdd(
  SizeType level,
  const Bdd& bdd0,
  const Bdd& bdd1
)
{
  bdd0._check_valid();
  bdd1._check_valid();
  _check_mgr(bdd0);
  _check_mgr(bdd1);
  auto e = new_node(level, _edge(bdd0), _edge(bdd1));
  return _bdd(e);
}

// @brief 変数リストをインデックスリストに変換する．
vector<SizeType>
BddMgrImpl::level_list(
  const vector<BddVar>& var_list
) const
{
  vector<SizeType> ans;
  ans.reserve(var_list.size());
  for ( auto var: var_list ) {
    ans.push_back(var.level());
  }
  return ans;
}

// @brief インデックスを変数に変換する．
BddVar
BddMgrImpl::level_to_var(
  SizeType level
)
{
  auto e = new_node(level, DdEdge::zero(), DdEdge::one());
  return _var(e);
}

// @brief 変数順を表す変数のリストを返す．
vector<BddVar>
BddMgrImpl::variable_order()
{
  auto order_list = variable_list();
  sort(order_list.begin(), order_list.end(),
       [&](const BddVar& a, const BddVar& b){
	 auto level_a = varid_to_level(a.id());
	 auto level_b = varid_to_level(b.id());
	 return level_a < level_b;
       });
  return order_list;
}

// @brief 変数順を設定する．
void
BddMgrImpl::set_variable_order(
  const vector<BddVar>& order_list
)
{
  auto nv = variable_num();
  if ( order_list.size() != nv ) {
    throw std::invalid_argument{"order_list.size() mismatch"};
  }
  for ( SizeType i = 0; i < nv; ++ i ) {
    auto dst_level = nv - i - 1;
    auto var = order_list[dst_level];
    auto varid = var.id();
    auto level = varid_to_level(varid);
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

// @brief 複数のBDDのノード数を数える．
SizeType
BddMgrImpl::bdd_size(
  const vector<Bdd>& bdd_list
)
{
  if ( bdd_list.empty() ) {
    return 0;
  }
  auto info_mgr = node_info(bdd_list);
  return info_mgr.node_num();
}

// @brief DdEdge を Bdd に変換する．
Bdd
BddMgrImpl::_bdd(
  DdEdge edge
)
{
  return Bdd{ptr(), edge};
}

// @brief DdEdge を BddVar に変換する．
BddVar
BddMgrImpl::_var(
  DdEdge edge
)
{
  return BddVar{ptr(), edge};
}

// @brief Bdd から DdEdge を取り出す．
DdEdge
BddMgrImpl::_edge(
  const Bdd& bdd
)
{
  return DdEdge{bdd.mRoot};
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
  bool oinv{edge0.inv()};
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

// @brief BDDの根の枝のリストを作る．
vector<DdEdge>
BddMgrImpl::root_list(
  const vector<Bdd>& bdd_list
)
{
  SizeType n = bdd_list.size();
  vector<DdEdge> edge_list(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    auto& bdd = bdd_list[i];
    bdd._check_valid();
    edge_list[i] = _edge(bdd);
  }
  return edge_list;
}

// @brief 複数のBDDのノードの情報を取り出す．
DdInfoMgr
BddMgrImpl::node_info(
  const vector<Bdd>& bdd_list
)
{
  auto edge_list = root_list(bdd_list);
  return DdInfoMgr{edge_list, this};
}

// @brief 複数のBDDの内容を出力する．
void
BddMgrImpl::display(
  ostream& s,
  const vector<Bdd>& bdd_list
)
{
  auto info_mgr = node_info(bdd_list);
  info_mgr.display(s);
}

// @brief 構造を表す整数配列を作る．
vector<SizeType>
BddMgrImpl::rep_data(
  const vector<Bdd>& bdd_list
)
{
  auto info_mgr = node_info(bdd_list);
  return info_mgr.rep_data();
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

END_NONAMESPACE

// @brief BDD の内容をバイナリダンプする．
void
BddMgrImpl::dump(
  BinEnc& s,
  const vector<Bdd>& bdd_list
)
{
  // sanity check
  for ( auto& bdd: bdd_list ) {
    _check_mgr(bdd);
  }

  // シグネチャ
  s.write_signature(BDD_SIG);

  // 要素数
  auto n = bdd_list.size();
  s.write_vint(n);
  if ( n == 0 ) {
    return;
  }

  auto info_mgr = node_info(bdd_list);

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

BEGIN_NONAMESPACE

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

// @brief バイナリダンプから復元する．
vector<Bdd>
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
  vector<BddVar> var_list(nv);
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

  vector<Bdd> bdd_list(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    SizeType root_info = root_info_list[i];
    auto edge = decode(root_info, edge_list);
    bdd_list[i] = _bdd(edge);
  }
  return bdd_list;
}

END_NAMESPACE_YM_DD
