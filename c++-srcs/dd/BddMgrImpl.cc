
/// @file BddMgrImpl.cc
/// @brief BddMgrImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Bdd.h"
#include "dd/BddMgrImpl.h"
#include "CopyOp.h"
#include "NodeCollector.h"
#include "dd/DdInfo.h"
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
  SizeType index,   ///< [in] インデックス
  const Bdd& edge0, ///< [in] 0枝
  const Bdd& edge1  ///< [in] 1枝
)
{
  edge0._check_valid();
  edge1._check_valid();
  auto tmp0 = copy(edge0);
  auto tmp1 = copy(edge1);
  auto e = new_node(index, _edge(tmp0), _edge(tmp1));
  return _bdd(e);
}

// @brief BDD をコピーする．
Bdd
BddMgrImpl::copy(
  const Bdd& src
)
{
  if ( src.mMgr == nullptr ) {
    // 不正な ZDD はそのまま
    return src;
  }
  if ( src.mMgr == this ) {
    // 自分自身に属している場合もそのまま
    return src;
  }
  CopyOp op{*this};
  auto e = op.copy_step(_edge(src));
  return _bdd(e);
}

// @brief リテラル関数を作る．
Bdd
BddMgrImpl::literal(
  SizeType var,
  bool inv
)
{
  return bdd(var, zero(), one()) ^ inv;
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
  vector<SizeType> root_list;
  auto node_list = node_info(bdd_list, root_list);
  return node_list.size();
}

// @brief DdEdge を Bdd に変換する．
Bdd
BddMgrImpl::_bdd(
  DdEdge edge
)
{
  return Bdd{this, edge};
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
  SizeType index,
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

  auto node = DdNodeMgr::new_node(index, edge0, edge1);

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
vector<DdInfo>
BddMgrImpl::node_info(
  const vector<Bdd>& bdd_list,
  vector<SizeType>& root_edge_list
)
{
  auto edge_list = root_list(bdd_list);
  NodeCollector nc{edge_list};

  root_edge_list.clear();
  root_edge_list.reserve(bdd_list.size());
  for ( auto root: nc.root_list() ) {
    root_edge_list.push_back(nc.edge2int(root));
  }

  vector<DdInfo> node_list;
  node_list.reserve(nc.node_list().size());
  for ( auto node: nc.node_list() ) {
    SizeType id = nc.node_id(node);
    ASSERT_COND( id == node_list.size() + 1 );
    SizeType index = node->index();
    SizeType edge0 = nc.edge2int(node->edge0());
    SizeType edge1 = nc.edge2int(node->edge1());
    node_list.push_back(DdInfo{index, edge0, edge1});
  }
  return node_list;
}

// @brief 複数のBDDの内容を出力する．
void
BddMgrImpl::display(
  ostream& s,
  const vector<Bdd>& bdd_list
)
{
  vector<SizeType> root_edge_list;
  auto node_list = node_info(bdd_list, root_edge_list);

  DdInfo::display(s, root_edge_list, node_list);
}

// @brief 構造を表す整数配列を作る．
vector<SizeType>
BddMgrImpl::rep_data(
  const vector<Bdd>& bdd_list
)
{
  vector<SizeType> root_edge_list;
  auto node_list = node_info(bdd_list, root_edge_list);
  return DdInfo::rep_data(root_edge_list, node_list);
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
  SizeType n = bdd_list.size();
  vector<Bdd> tmp_list(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    auto& bdd = bdd_list[i];
    bdd._check_valid();
    auto tmp = copy(bdd);
    tmp_list[i] = tmp;
  }

  // シグネチャ
  s.write_signature(BDD_SIG);
  // 要素数
  s.write_vint(n);

  if ( n == 0 ) {
    return;
  }

  vector<SizeType> root_edge_list;
  auto node_list = node_info(tmp_list, root_edge_list);
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

  vector<SizeType> root_info_list(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    root_info_list[i] = s.read_vint();
  }

  vector<DdEdge> edge_list;
  for ( SizeType id = 1; ; ++ id ) {
    SizeType index = s.read_vint();
    SizeType edge0_info = restore_edge(s, id);
    SizeType edge1_info = restore_edge(s, id);
    if ( index == 0 && edge0_info == 0 && edge1_info == 0 ) {
      break;
    }
    auto edge0 = decode(edge0_info, edge_list);
    auto edge1 = decode(edge1_info, edge_list);
    auto edge = new_node(index, edge0, edge1);
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
