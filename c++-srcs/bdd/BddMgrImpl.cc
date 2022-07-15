
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

// @brief ノード数を数える．
SizeType
BddMgrImpl::count_size(
  BddEdge edge
)
{
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
  return truth_step(str, var_list.size() - 1);
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
    auto e0 = truth_step(str0, pos - 1);
    auto e1 = truth_step(str1, pos - 1);
    auto ans = new_node(mTruthVarList[pos].index(), e0, e1);
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