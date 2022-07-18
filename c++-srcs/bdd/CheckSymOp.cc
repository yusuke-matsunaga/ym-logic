
/// @file CheckSymOp.cc
/// @brief CheckSymOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "CheckSymOp.h"
#include "BddNode.h"


BEGIN_NAMESPACE_YM_BDD

// @brief 対称性のチェックを行う．
bool
CheckSymOp::op_step(
  BddEdge edge
)
{
  if ( edge.is_const() ) {
    return true;
  }

  auto node = edge.node();
  if ( mTable1.count(node) > 0 ) {
    return mTable1.at(node);
  }

  SizeType index = node->index();
  if ( index < mIndex1 ) {
    auto ans = op_step(node->edge0());
    if ( ans ) {
      ans = op_step(node->edge1());
    }
    mTable1.emplace(node, ans);
    return ans;
  }
  else if ( index == mIndex1 ) {
    auto ans = op_step2(node->edge0(), node->edge1());
    mTable1.emplace(node, ans);
    return ans;
  }
  else if ( index < mIndex2 ) {
    // mIndex1 に依存していなかったので
    // mIndex2 にも依存していないことを確認する．
    auto ans = op_step3(node->edge0());
    if ( ans ) {
      ans = op_step3(node->edge1());
    }
    mTable1.emplace(node, ans);
    return ans;
  }
  else if ( index == mIndex2 ) {
    // mIndex1 に依存していないが mIndex2 に依存している．
    return false;
  }
  else { // index > mIndex ) {
    // mIndex1 にも mIndex2 にも依存していない．
    return true;
  }
}

// @brief op_step の下請け関数
bool
CheckSymOp::op_step2(
  BddEdge edge0,
  BddEdge edge1
)
{
  if ( edge0.is_const() || edge1.is_const() ) {
    // ということは mIndex2 に依存していないので false
    return false;
  }

  Apply2Key key{edge0, edge1};
  if ( mTable2.count(key) > 0 ) {
    return mTable2.at(key);
  }

  BddEdge edge00, edge01;
  BddEdge edge10, edge11;
  auto top = decomp(edge0, edge1,
		    edge00, edge01,
		    edge10, edge11);
  if ( top < mIndex2 ) {
    auto ans = op_step2(edge00, edge10);
    if ( ans ) {
      ans = op_step2(edge01, edge11);
    }
    mTable2.emplace(key, ans);
    return ans;
  }
  else if ( top == mIndex2 ) {
    if ( mInv ) {
      return (edge00 == edge11);
    }
    else {
      return (edge01 == edge10);
    }
  }
  else { // top > mIndex2
    return false;
  }
}

// @brief op_step の下請け関数
bool
CheckSymOp::op_step3(
  BddEdge edge
)
{
  if ( edge.is_const() ) {
    return true;
  }
  auto node = edge.node();
  if ( mTable1.count(node) > 0 ) {
    return mTable1.at(node);
  }

  SizeType index = node->index();
  if ( index < mIndex2 ) {
    auto ans = op_step3(node->edge0());
    if ( ans ) {
      ans = op_step3(node->edge1());
    }
    mTable1.emplace(node, ans);
    return ans;
  }
  else if ( index == mIndex2 ) {
    return false;
  }
  else {
    return true;
  }
}

END_NAMESPACE_YM_BDD
