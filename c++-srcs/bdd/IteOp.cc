
/// @file IteOp.cc
/// @brief IteOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "IteOp.h"


BEGIN_NAMESPACE_YM_BDD

// @brief ITE 演算を行う．
BddEdge
IteOp::ite_step(
  BddEdge e0,
  BddEdge e1,
  BddEdge e2
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
  // e1 と e2 が相補的であれば e0 と e1 の XNOR となる．
  if ( e1 == ~e2 ) {
    return ~xor_step(e0, e1);
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

  IteKey key{e0, e1, e2};
  if ( mIteTable.count(key) > 0 ) {
    return mIteTable.at(key);
  }

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

  BddEdge e00, e01;
  if ( index0 == top ) {
    e00 = node0->edge0(inv0);
    e01 = node0->edge1(inv0);
  }
  else {
    e00 = e01 = e0;
  }

  BddEdge e10, e11;
  if ( index1 == top ) {
    e10 = node1->edge0(inv1);
    e11 = node1->edge1(inv1);
  }
  else {
    e10 = e11 = e1;
  }

  BddEdge e20, e21;
  if ( index2 == top ) {
    e20 = node2->edge0(inv2);
    e21 = node2->edge1(inv2);
  }
  else {
    e20 = e21 = e2;
  }

  auto r0 = ite_step(e00, e10, e20);
  auto r1 = ite_step(e01, e11, e21);
  auto result = new_node(top, r0, r1);
  mIteTable.emplace(key, result);
  return result;
}

// @brief AND演算を行う．
BddEdge
IteOp::and_step(
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
  ApplyKey key{left, right};
  if ( mAndTable.count(key) > 0 ) {
    return mAndTable.at(key);
  }

  // 見つからなかったので実際に apply 演算を行う．
  // 先頭のインデックスで分解する．
  BddEdge left0, left1;
  BddEdge right0, right1;
  auto top = decomp(left, right,
		    left0, left1,
		    right0, right1);
  auto ans0 = and_step(left0, right0);
  auto ans1 = and_step(left1, right1);
  auto result = new_node(top, ans0, ans1);
  mAndTable.emplace(key, result);
  return result;
}

// @brief OR演算を行う．
BddEdge
IteOp::or_step(
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
  ApplyKey key{left, right};
  if ( mOrTable.count(key) > 0 ) {
    return mOrTable.at(key);
  }

  // 見つからなかったので実際に apply 演算を行う．
  // 先頭のインデックスで分解する．
  BddEdge left0, left1;
  BddEdge right0, right1;
  auto top = decomp(left, right,
		    left0, left1,
		    right0, right1);
  auto ans0 = or_step(left0, right0);
  auto ans1 = or_step(left1, right1);
  auto result = new_node(top, ans0, ans1);
  mOrTable.emplace(key, result);
  return result;
}

// @brief XOR演算を行う．
BddEdge
IteOp::xor_step(
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
  ApplyKey key{left, right};
  if ( mXorTable.count(key) > 0 ) {
    return mXorTable.at(key);
  }

  // 見つからなかったので実際に apply 演算を行う．
  // 先頭のインデックスで分解する．
  BddEdge left0, left1;
  BddEdge right0, right1;
  auto top = decomp(left, right,
		    left0, left1,
		    right0, right1);
  auto ans0 = xor_step(left0, right0);
  auto ans1 = xor_step(left1, right1);
  auto result = new_node(top, ans0, ans1);
  mXorTable.emplace(key, result);
  return result;
}

END_NAMESPACE_YM_BDD
