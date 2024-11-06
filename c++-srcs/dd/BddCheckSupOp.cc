
/// @file BddCheckSupOp.cc
/// @brief BddCheckSupOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Bdd.h"
#include "dd/DdNode.h"
#include "dd/BddMgrImpl.h"
#include "BddCheckSupOp.h"


BEGIN_NAMESPACE_YM_DD

// @brief 与えられた変数がサポートの時 true を返す．
bool
Bdd::check_sup(
  const BddVar& var
) const
{
  auto index = var.index();
  BddCheckSupOp op{index};
  return op.op_step(DdEdge{mRoot});
}


//////////////////////////////////////////////////////////////////////
// クラス BddCheckSupOp
//////////////////////////////////////////////////////////////////////

// @brief サポートチェックを行う．
bool
BddCheckSupOp::op_step(
  DdEdge edge
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

END_NAMESPACE_YM_DD
