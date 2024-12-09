
/// @file BddCheckSym.cc
/// @brief BddCheckSym の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Bdd.h"
#include "DdNode.h"
#include "BddCheckSymOp.h"
#include "BddMgrImpl.h"


BEGIN_NAMESPACE_YM_DD

// @brief 与えられた変数に対して対称の時 true を返す．
bool
Bdd::check_sym(
  const BddVar& var1,
  const BddVar& var2,
  bool inv
) const
{
  _check_mgr(var1);
  _check_mgr(var2);
  auto idx1 = var1.level();
  auto idx2 = var2.level();
  BddCheckSymOp op{idx1, idx2, inv};
  return op.op_step(DdEdge{mRoot});
}


//////////////////////////////////////////////////////////////////////
// クラス BddCheckSymOp
//////////////////////////////////////////////////////////////////////

// @brief 対称性のチェックを行う．
bool
BddCheckSymOp::op_step(
  DdEdge edge
)
{
  if ( edge.is_const() ) {
    return true;
  }

  auto node = edge.node();
  if ( mTable1.count(node) > 0 ) {
    return mTable1.at(node);
  }

  SizeType level = node->level();
  if ( level < mLevel1 ) {
    auto ans = op_step(node->edge0());
    if ( ans ) {
      ans = op_step(node->edge1());
    }
    mTable1.emplace(node, ans);
    return ans;
  }
  else if ( level == mLevel1 ) {
    auto ans = op_step2(node->edge0(), node->edge1());
    mTable1.emplace(node, ans);
    return ans;
  }
  else if ( level < mLevel2 ) {
    // mLevel1 に依存していなかったので
    // mLevel2 にも依存していないことを確認する．
    auto ans = op_step3(node->edge0());
    if ( ans ) {
      ans = op_step3(node->edge1());
    }
    mTable1.emplace(node, ans);
    return ans;
  }
  else if ( level == mLevel2 ) {
    // mLevel1 に依存していないが mLevel2 に依存している．
    return false;
  }
  else { // level > mLevel ) {
    // mLevel1 にも mLevel2 にも依存していない．
    return true;
  }
}

// @brief op_step の下請け関数
bool
BddCheckSymOp::op_step2(
  DdEdge edge0,
  DdEdge edge1
)
{
  if ( edge0.is_const() && edge1.is_const() ) {
    if ( edge0 == edge1 ) {
      return true;
    }
    else {
      // ということは mLevel2 に依存していないので false
      return false;
    }
  }

  Apply2Key key{edge0, edge1};
  if ( mTable2.count(key) > 0 ) {
    return mTable2.at(key);
  }

  DdEdge edge00, edge01;
  DdEdge edge10, edge11;
  SizeType top;

  if ( edge0.is_const() ) {
    edge00 = edge01 = edge0;
    ASSERT_COND( !edge1.is_const() );
    auto node1 = edge1.node();
    auto inv1 = edge1.inv();
    edge10 = node1->edge0() ^ inv1;
    edge11 = node1->edge1() ^ inv1;
    top = node1->level();
  }
  else if ( edge1.is_const() ) {
    edge10 = edge11 = edge1;
    ASSERT_COND( !edge0.is_const() );
    auto node0 = edge0.node();
    auto inv0 = edge0.inv();
    edge00 = node0->edge0() ^ inv0;
    edge01 = node0->edge1() ^ inv0;
    top = node0->level();
  }
  else {
    top = BddMgrImpl::decomp(
      edge0, edge1,
      edge00, edge01,
      edge10, edge11
    );
  }
  if ( top < mLevel2 ) {
    auto ans = op_step2(edge00, edge10);
    if ( ans ) {
      ans = op_step2(edge01, edge11);
    }
    mTable2.emplace(key, ans);
    return ans;
  }
  else if ( top == mLevel2 ) {
    if ( mInv ) {
      return (edge00 == edge11);
    }
    else {
      return (edge01 == edge10);
    }
  }
  else { // top > mLevel2
    return false;
  }
}

// @brief op_step の下請け関数
bool
BddCheckSymOp::op_step3(
  DdEdge edge
)
{
  if ( edge.is_const() ) {
    return true;
  }
  auto node = edge.node();
  if ( mTable1.count(node) > 0 ) {
    return mTable1.at(node);
  }

  SizeType level = node->level();
  if ( level < mLevel2 ) {
    auto ans = op_step3(node->edge0());
    if ( ans ) {
      ans = op_step3(node->edge1());
    }
    mTable1.emplace(node, ans);
    return ans;
  }
  else if ( level == mLevel2 ) {
    return false;
  }
  else {
    return true;
  }
}

END_NAMESPACE_YM_DD
