
/// @file BddIteOp.cc
/// @brief BddIteOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "BddIteOp.h"


BEGIN_NAMESPACE_YM_DD

// @brief AND 演算を行う．
Bdd
BddMgrImpl::and_op(
  const Bdd& left,
  const Bdd& right
)
{
  left._check_valid();
  right._check_valid();
  auto tmp0 = copy(left);
  auto tmp1 = copy(right);
  BddIteOp op{*this};
  auto e = op.and_step(_edge(tmp0), _edge(tmp1));
  return _bdd(e);
}

// @brief XOR 演算を行う．
Bdd
BddMgrImpl::xor_op(
  const Bdd& left,
  const Bdd& right
)
{
  left._check_valid();
  right._check_valid();
  auto tmp0 = copy(left);
  auto tmp1 = copy(right);
  BddIteOp op{*this};
  auto e = op.xor_step(_edge(tmp0), _edge(tmp1));
  return _bdd(e);
}

// @brief ITE 演算を行う．
Bdd
BddMgrImpl::ite(
  const Bdd& e0,
  const Bdd& e1,
  const Bdd& e2
)
{
  e0._check_valid();
  e1._check_valid();
  e2._check_valid();
  auto tmp0 = copy(e0);
  auto tmp1 = copy(e1);
  auto tmp2 = copy(e2);
  BddIteOp op{*this};
  auto e = op.ite_step(_edge(tmp0), _edge(tmp1), _edge(tmp2));
  return _bdd(e);
}


//////////////////////////////////////////////////////////////////////
// クラス BddIteOP
//////////////////////////////////////////////////////////////////////

DdEdge
BddIteOp::and_step(
  DdEdge left,
  DdEdge right
)
{
  // trivial case のチェック

  // case 1: 片方が 0 なら 0 を返す．
  if ( left.is_zero() || right.is_zero() ) {
    return DdEdge::zero();
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
    return DdEdge::zero();
  }

  // 正規化を行う．
  if ( left.body() > right.body() ) {
    std::swap(left, right);
  }

  // 演算結果テーブルを調べる．
  Apply3Key key{left, right, DdEdge::zero()};
  if ( mTable.count(key) > 0 ) {
    return mTable.at(key);
  }

  // 見つからなかったので実際に apply 演算を行う．
  // 先頭のインデックスで分解する．
  DdEdge left0, left1;
  DdEdge right0, right1;
  auto top = decomp(
    left, right,
    left0, left1,
    right0, right1
  );
  auto ans0 = and_step(left0, right0);
  auto ans1 = and_step(left1, right1);
  auto result = new_node(top, ans0, ans1);
  mTable.emplace(key, result);
  return result;
}

// @brief XOR 演算を行う．
DdEdge
BddIteOp::xor_step(
  DdEdge left,
  DdEdge right
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
    return DdEdge::zero();
  }

  // case 4: 極性違いで等しかったら 1 を返す．
  if ( left.node() == right.node() ) {
    return DdEdge::one();
  }

  // 正規化を行う．
  bool oinv = left.inv() ^ right.inv();
  left.make_positive();
  right.make_positive();
  if ( left.body() > right.body() ) {
    std::swap(left, right);
  }

  // 演算結果テーブルを調べる．
  Apply3Key key{left, ~right, right};
  if ( mTable.count(key) > 0 ) {
    return mTable.at(key) ^ oinv;
  }
  // 見つからなかったので実際に apply 演算を行う．
  // 先頭のインデックスで分解する．
  DdEdge left0, left1;
  DdEdge right0, right1;
  auto top = decomp(
    left, right,
    left0, left1,
    right0, right1
  );
  auto ans0 = xor_step(left0, right0);
  auto ans1 = xor_step(left1, right1);
  auto result = new_node(top, ans0, ans1);
  mTable.emplace(key, result);
  return result ^ oinv;
}

// @brief ITE 演算を行う．
DdEdge
BddIteOp::ite_step(
  DdEdge e0,
  DdEdge e1,
  DdEdge e2
)
{
  // trivial cases

  // e0 が定数なら答は決まっている．
  if ( e0.is_zero() ) {
    return e2;
  }
  if ( e0.is_one() ) {
    return e1;
  }
  // e1 が 0 の時は ~e0 & e2 となる．
  if ( e1.is_zero() ) {
    return and_step(~e0, e2);
  }
  // e1 が 1 の時は e0 | e2 となる．
  if ( e1.is_one() ) {
    return or_step(e0, e2);
  }
  // e2 が 0 の時は e0 & e1 となる．
  if ( e2.is_zero() ) {
    return and_step(e0, e1);
  }
  // e2 が 1 の時は ~e0 | e1 となる．
  if ( e2.is_one() ) {
    return or_step(~e0, e1);
  }

  // e1 と e2 が等しければ e0 と無関係に e1 を返す．
  if ( e1 == e2 ) {
    return e1;
  }
  // e1 と e2 が相補的であれば e0 と e2 の XOR となる．
  if ( e1 == ~e2 ) {
    return xor_step(e0, e2);
  }
  // e0 と e1 が等しければ e0 | e2 となる．
  if ( e0 == e1 ) {
    return or_step(e0, e2);
  }
  // e0 と e1 が相補的であれば e1 & e2 となる．
  if ( e0 == ~e1 ) {
    return and_step(e1, e2);
  }
  // e0 と e2 が等しければ e0 & e1 となる．
  if ( e0 == e2 ) {
    return and_step(e0, e1);
  }
  // e0 と e2 が相補的であれば ~e0 | e1 となる．
  if ( e0 == ~e2 ) {
    return or_step(~e0, e1);
  }

  // e0 が正極性になるように正規化する．
  if ( e0.inv() ) {
    e0.make_positive();
    std::swap(e1, e2);
  }
  Apply3Key key{e0, e1, e2};
  if ( mTable.count(key) > 0 ) {
    return mTable.at(key);
  }

  // e0, e1, e2 をトップの変数で分解する．

  auto node0 = e0.node();
  auto inv0 = e0.inv();
  auto index0 = node0->index();

  auto node1 = e1.node();
  auto inv1 = e1.inv();
  auto index1 = node1->index();

  auto node2 = e2.node();
  auto inv2 = e2.inv();
  auto index2 = node2->index();

  auto top = std::min(std::min(index0, index1), index2);

  // e0 -> e00, e01
  DdEdge e00, e01;
  if ( index0 == top ) {
    e00 = node0->edge0() ^ inv0;
    e01 = node0->edge1() ^ inv0;
  }
  else {
    e00 = e01 = e0;
  }

  // e1 -> e10, e11
  DdEdge e10, e11;
  if ( index1 == top ) {
    e10 = node1->edge0() ^ inv1;
    e11 = node1->edge1() ^ inv1;
  }
  else {
    e10 = e11 = e1;
  }

  // e2 -> e20, e21
  DdEdge e20, e21;
  if ( index2 == top ) {
    e20 = node2->edge0() ^ inv2;
    e21 = node2->edge1() ^ inv2;
  }
  else {
    e20 = e21 = e2;
  }

  auto r0 = ite_step(e00, e10, e20);
  auto r1 = ite_step(e01, e11, e21);
  auto result = new_node(top, r0, r1);
  mTable.emplace(key, result);
  return result;
}

END_NAMESPACE_YM_DD
