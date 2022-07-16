
/// @file CheckSupOp.cc
/// @brief CheckSupOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "CheckSupOp.h"
#include "BddNode.h"


BEGIN_NAMESPACE_YM_BDD

// @brief サポートチェックを行う．
bool
CheckSupOp::op_step(
  BddEdge edge
)
{
  if ( edge.is_const() ) {
    return false;
  }

  auto node = edge.node();
  auto index = node->index();
  auto inv = edge.inv();
  if ( index == mIndex ) {
    return true;
  }
  if ( index > mIndex ) {
    return false;
  }

  if ( mTable.count(node) > 0 ) {
    return mTable.at(node);
  }

  auto edge0 = node->edge0();
  auto ans = op_step(edge0);
  if ( !ans ) {
    auto edge1 = node->edge1();
    ans = op_step(edge1);
  }
  mTable.emplace(node, ans);
  return ans;
}

END_NAMESPACE_YM_BDD
