
/// @file BddMgrImpl.cc
/// @brief BddMgrImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "BddMgrImpl.h"
#include "ym/BddVar.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// ApplyTable 用の定数
const SizeType AND_OP = 1;
const SizeType OR_OP  = 2;
const SizeType XOR_OP = 4;
const SizeType COF_OP = 8;

END_NONAMESPACE

// @brief コンストラクタ
BddMgrImpl::BddMgrImpl()
{
}

// @brief デストラクタ
BddMgrImpl::~BddMgrImpl()
{
}

BEGIN_NONAMESPACE

inline
SizeType
decomp(
  BddEdge left,
  BddEdge right,
  BddEdge& left0,
  BddEdge& left1,
  BddEdge& right0,
  BddEdge& right1
)
{
  auto l_node = left.node();
  auto l_inv = left.inv();
  auto l_index = l_node->index();
  auto r_node = right.node();
  auto r_inv = right.inv();
  auto r_index = r_node->index();
  auto top = std::min(l_index, r_index);
  if ( l_index == top ) {
    left0 = l_node->edge0(l_inv);
    left1 = l_node->edge1(l_inv);
  }
  else {
    left0 = left;
    left1 = left;
  }
  if ( r_index == top ) {
    right0 = r_node->edge0(r_inv);
    right1 = r_node->edge1(r_inv);
  }
  else {
    right0 = right;
    right1 = right;
  }
  return top;
}

END_NONAMESPACE

// @brief 変数を割り当てる．
BddVar
BddMgrImpl::new_variable(
  const string& name
)
{
  SizeType index = mVarList.size();
  mVarList.push_back(BddVar{index, name});
  return mVarList.back();
}

// @brief 論理積を計算する．
BddEdge
BddMgrImpl::and_op(
  BddEdge left,
  BddEdge right
)
{
  // trivial case のチェック

  // case 1: 片方が 0 なら 0 を返す．
  if ( left.is_zero() || right.is_zero() ) {
    return BddEdge::make_zero();
  }

  // case 2: 片方が 1 なら他方を返す．
  if ( left.is_one() ) {
    return right;
  }
  if ( right.is_one() ) {
    return left;
  }

  // case 3: 等しかったらそのまま返す．
  if ( left == right ) {
    return left;
  }

  // case 4: 極性違いで等しかったら 0 を返す．
  if ( left.node() == right.node() ) {
    return BddEdge::make_zero();
  }

  // 演算結果テーブルを調べる．
  BddEdge result;
  if ( !mApplyTable.find(AND_OP, left, right, result) ) {
    // 見つからなかったので実際に apply 演算を行う．
    // 先頭のインデックスで分解する．
    BddEdge left0, left1;
    BddEdge right0, right1;
    auto top = decomp(left, right,
		      left0, left1,
		      right0, right1);
    auto ans0 = and_op(left0, right0);
    auto ans1 = and_op(left1, right1);
    result = new_node(top, ans0, ans1);
    mApplyTable.put(AND_OP, left, right, result);
  }
  return result;
}

// @brief 論理和を計算する．
BddEdge
BddMgrImpl::or_op(
  BddEdge left,
  BddEdge right
)
{
  // trivial case のチェック

  // case 1: 片方が 1 なら 1 を返す．
  if ( left.is_one() || right.is_one() ) {
    return BddEdge::make_one();
  }

  // case 2: 片方が 0 なら他方を返す．
  if ( left.is_zero() ) {
    return right;
  }
  if ( right.is_zero() ) {
    return left;
  }

  // case 3: 等しかったらそのまま返す．
  if ( left == right ) {
    return left;
  }

  // case 4: 極性違いで等しかったら 1 を返す．
  if ( left.node() == right.node() ) {
    return BddEdge::make_one();
  }

  // 演算結果テーブルを調べる．
  BddEdge result;
  if ( !mApplyTable.find(OR_OP, left, right, result) ) {
    // 見つからなかったので実際に apply 演算を行う．
    // 先頭のインデックスで分解する．
    BddEdge left0, left1;
    BddEdge right0, right1;
    auto top = decomp(left, right,
		      left0, left1,
		      right0, right1);
    auto ans0 = or_op(left0, right0);
    auto ans1 = or_op(left1, right1);
    result = new_node(top, ans0, ans1);
    mApplyTable.put(OR_OP, left, right, result);
  }
  return result;
}

// @brief 排他的論理和を計算する．
BddEdge
BddMgrImpl::xor_op(
  BddEdge left,
  BddEdge right
)
{
  // trivial case のチェック

  // case 1: 片方が 0 なら他方を返す．
  if ( left.is_zero() ) {
    return right;
  }
  if ( right.is_zero() ) {
    return left;
  }

  // case 2: 片方が 1 なら他方の否定を返す．
  if ( left.is_one() ) {
    return ~right;
  }
  if ( right.is_one() ) {
    return ~left;
  }

  // case 3: 等しかったら 0 を返す．
  if ( left == right ) {
    return BddEdge::make_zero();
  }

  // case 4: 極性違いで等しかったら 1 を返す．
  if ( left.node() == right.node() ) {
    return BddEdge::make_one();
  }

  // 演算結果テーブルを調べる．
  BddEdge result;
  if ( !mApplyTable.find(XOR_OP, left, right, result) ) {
    // 見つからなかったので実際に apply 演算を行う．
    // 先頭のインデックスで分解する．
    BddEdge left0, left1;
    BddEdge right0, right1;
    auto top = decomp(left, right,
		      left0, left1,
		      right0, right1);
    auto ans0 = xor_op(left0, right0);
    auto ans1 = xor_op(left1, right1);
    result = new_node(top, ans0, ans1);
    mApplyTable.put(XOR_OP, left, right, result);
  }
  return result;
}

// @brief コファクターを計算する．
BddEdge
BddMgrImpl::cofactor_op(
  BddEdge edge,
  SizeType index,
  bool inv
)
{
  mCofacTable.clear();
  mCofacIndex = index;
  mCofacInv = inv;
  return cofactor_step(edge);
}

// @brief コファクターを計算する．
BddEdge
BddMgrImpl::cofactor_step(
  BddEdge edge
)
{
  // 終端ならそのまま返す．
  if ( edge.is_const() ) {
    return edge;
  }

  auto node = edge.node();
  auto index = node->index();
  auto inv = edge.inv();
  if ( index == mCofacIndex ) {
    if ( mCofacInv ) {
      return node->edge0(inv);
    }
    else {
      return node->edge1(inv);
    }
  }
  if ( index > mCofacIndex ) {
    return edge;
  }

  BddEdge ans;
  if ( mCofacTable.count(node) == 0 ) {
    auto edge0 = node->edge0();
    auto edge1 = node->edge1();
    auto ans0 = cofactor_step(edge0);
    auto ans1 = cofactor_step(edge1);
    ans = new_node(index, ans0, ans1);
    mCofacTable.emplace(node, ans);
  }
  else {
    ans = mCofacTable.at(node);
  }
  return ans * inv;
}

// @brief サポートチェックを行う．
bool
BddMgrImpl::check_sup(
  BddEdge edge,
  SizeType index
)
{
  mCofacTable.clear();
  mCofacIndex = index;
  return check_sup_step(edge);
}

// @brief サポートチェックを行う．
bool
BddMgrImpl::check_sup_step(
  BddEdge edge
)
{
  if ( edge.is_const() ) {
    return false;
  }

  auto node = edge.node();
  auto index = node->index();
  auto inv = edge.inv();
  if ( index == mCofacIndex ) {
    return true;
  }
  if ( index > mCofacIndex ) {
    return false;
  }

  BddEdge ans_edge;
  if ( mCofacTable.count(node) == 0 ) {
    auto edge0 = node->edge0();
    auto ans = check_sup_step(edge0);
    if ( !ans ) {
      auto edge1 = node->edge1();
      ans = check_sup_step(edge1);
    }
    if ( ans ) {
      ans_edge = BddEdge::make_one();
    }
    else {
      ans_edge = BddEdge::make_zero();
    }
    mCofacTable.emplace(node, ans_edge);
  }
  return ans_edge.inv();
}

// @brief 対称変数のチェックを行う．
bool
BddMgrImpl::check_sym(
  BddEdge edge,
  SizeType index1,
  SizeType index2,
  bool inv
)
{
}

BEGIN_NONAMESPACE

void
count_dfs(
  BddEdge e,
  unordered_set<BddNode*>& mark
)
{
  if ( e.is_const() ) {
    return;
  }
  auto node = e.node();
  if ( mark.count(node) == 0 ) {
    mark.emplace(node);
    count_dfs(node->edge0(), mark);
    count_dfs(node->edge1(), mark);
  }
}

END_NONAMESPACE

// @brief ノード数を数える．
SizeType
BddMgrImpl::count_size(
  const vector<BddEdge>& edge_list
)
{
  unordered_set<BddNode*> mark;
  for ( auto edge: edge_list ) {
    count_dfs(edge, mark);
  }
  return mark.size();
}

BEGIN_NONAMESPACE

void
gen_node_list(
  BddEdge e,
  unordered_map<BddNode*, SizeType>& node_map,
  vector<BddNode*>& node_list
)
{
  if ( e.is_const() ) {
    return;
  }
  auto node = e.node();
  if ( node_map.count(node) == 0 ) {
    SizeType id = node_list.size();
    node_map.emplace(node, id);
    node_list.push_back(node);
    gen_node_list(node->edge0(), node_map, node_list);
    gen_node_list(node->edge1(), node_map, node_list);
  }
}

void
dot_edge(
  ostream& s,
  BddEdge edge,
  const unordered_map<BddNode*, SizeType>& node_map,
  bool zero
)
{
  if ( edge.is_zero() ) {
    s << "const0";
  }
  else if ( edge.is_one() ) {
    s << "const1";
  }
  else {
    auto node = edge.node();
    auto inv = edge.inv();
    auto id = node_map.at(node);
    s << "node" << id
      << "[";
    const char* comma = "";
    if ( zero ) {
      s << "style=dotted";
      comma = ",";
    }
    if ( inv ) {
      s << comma << "dir=both,arrowtail=odot";
      comma = ",";
    }
    s << "]";
  }
  s << ";" << endl;
}

END_NONAMESPACE

// @brief 内容を dot 形式で出力する．
void
BddMgrImpl::gen_dot(
  ostream& s,
  const vector<BddEdge>& edge_list
)
{
  // ノードのリストを作る．
  unordered_map<BddNode*, SizeType> node_map;
  vector<BddNode*> node_list;
  for ( auto edge: edge_list ) {
    gen_node_list(edge, node_map, node_list);
  }

  // dot の開始
  s << "digraph bdd {" << endl
    << "  graph [rankdir = TB];" << endl;

  // 根のノードの定義
  SizeType i = 1;
  for ( auto edge: edge_list ) {
    s << "  root" << i
      << " [shape=box,label=\"BDD#"
      << i << "\"];" << endl;
    ++ i;
  }

  // ノードの定義
  SizeType max_index = 0;
  for ( auto node: node_list ) {
    auto id = node_map.at(node);
    s << "  node" << id
      << " [label=\""
      << node->index()
      << "\"];" << endl;
    if ( max_index < node->index() ) {
      max_index = node->index();
    }
  }

  // 終端の定義
  s << "  const0 [shape=box,label=\"0\"];" << endl;
  s << "  const1 [shape=box,label=\"1\"];" << endl;

  // 根の枝の定義
  i = 1;
  for ( auto edge: edge_list ) {
    s << "  root" << i << " -> ";
    dot_edge(s, edge, node_map, false);
    ++ i;
  }

  // 枝の定義
  vector<vector<SizeType>> rank_node_list(max_index + 1);
  for ( auto node: node_list ) {
    auto id = node_map.at(node);
    s << "  node" << id << " -> ";
    dot_edge(s, node->edge0(), node_map, true);
    s << "  node" << id << " -> ";
    dot_edge(s, node->edge1(), node_map, false);
    rank_node_list[node->index()].push_back(id);
  }

  // ランクの設定
  for ( auto& node_list: rank_node_list ) {
    s << "  { rank = same;";
    for ( auto id: node_list ) {
      s << " node" << id << ";";
    }
    s << "}" << endl;
  }

  // dot の終了
  s << "}" << endl;

}

// @brief 真理値表形式の文字列からBDDを作る．
BddEdge
BddMgrImpl::from_truth(
  const string& str,
  const vector<BddVar>& var_list
)
{
  mTruthTable.clear();
  mTruthVarList = var_list;
  return truth_step(str, 0);
}

// @brief from_truth の下請け関数
BddEdge
BddMgrImpl::truth_step(
  const string& str,
  SizeType pos
)
{
  if ( str == "0" ) {
    return BddEdge::make_zero();
  }
  if ( str == "1" ) {
    return BddEdge::make_one();
  }
  if ( mTruthTable.count(str) == 0 ) {
    SizeType n = str.size();
    SizeType h = n / 2;
    string str0 = str.substr(h);
    string str1 = str.substr(0, h);
    auto e0 = truth_step(str0, pos + 1);
    auto e1 = truth_step(str1, pos + 1);
    auto ans = new_node(pos, e0, e1);
    mTruthTable.emplace(str, ans);
    return ans;
  }
  else {
    return mTruthTable.at(str);
  }
}

// @brief ノードを作る．
BddEdge
BddMgrImpl::new_node(
  SizeType index,
  BddEdge edge0,
  BddEdge edge1
)
{
  // 2つの枝が等しい時は新しいノードを作らない．
  if ( edge0 == edge1 ) {
    return edge0;
  }

  // 極性の正規化を行う．
  bool oinv{edge0.inv()};
  edge0 *= oinv;
  edge1 *= oinv;

  // ノードテーブルを探す．
  auto node = mNodeTable.find(index, edge0, edge1);
  if ( node == nullptr ) {
    // なかったので新規に作る．
    node = new BddNode{index, edge0, edge1};
    // テーブルに登録する．
    mNodeTable.put(node);
  }
  return BddEdge{node, oinv};
}

// @brief ノード(枝)の参照回数を増やす．
void
BddMgrImpl::inc_ref(
  BddEdge edge
)
{
  auto node = edge.node();
  if ( node != nullptr ) {
    ++ node->mRefCount;
  }
}

// @brief ノード(枝)の参照回数を減らす．
void
BddMgrImpl::dec_ref(
  BddEdge edge
)
{
  auto node = edge.node();
  if ( node != nullptr ) {
    -- node->mRefCount;
  }
}

END_NAMESPACE_YM
