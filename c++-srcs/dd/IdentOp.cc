
/// @file Bdd_ident.cc
/// @brief Bdd::is_identical() の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Bdd.h"
#include "ym/Zdd.h"
#include "dd/DdNode.h"
#include "IdentOp.h"


BEGIN_NAMESPACE_YM_DD

// @brief 同じ構造を持つか調べる．
bool
Bdd::is_identical(
  const Bdd& right
) const
{
  if ( mMgr == nullptr || right.mMgr == nullptr ) {
    return false;
  }
  if ( mMgr == right.mMgr ) {
    return mRoot == right.mRoot;
  }
  IdentOp op;
  return op.ident_step(DdEdge{mRoot}, DdEdge{right.mRoot});
}


// @brief 同じ構造を持つか調べる．
bool
Zdd::is_identical(
  const Zdd& right
) const
{
  if ( mMgr == nullptr || right.mMgr == nullptr ) {
    return false;
  }
  if ( mMgr == right.mMgr ) {
    return mRoot == right.mRoot;
  }
  IdentOp op;
  return op.ident_step(DdEdge{mRoot}, DdEdge{right.mRoot});
}


//////////////////////////////////////////////////////////////////////
// クラス IdentOp
//////////////////////////////////////////////////////////////////////

// @brief 同じ構造かチェックする．
bool
IdentOp::ident_step(
  DdEdge left,
  DdEdge right
)
{
  if ( left.is_const() ) {
    return left == right;
  }
  if ( right.is_const() ) {
    return false;
  }
  if ( left.inv() != right.inv() ) {
    return false;
  }

  auto l_node = left.node();
  auto r_node = right.node();
  if ( l_node->index() != r_node->index() ) {
    return false;
  }

  auto left0 = l_node->edge0();
  auto left1 = l_node->edge1();
  auto right0 = r_node->edge0();
  auto right1 = r_node->edge1();

  Apply2Key key{left, right};
  if ( mTable.count(key) > 0 ) {
    return mTable.at(key);
  }
  auto ans = ident_step(left0, right0);
  if ( ans ) {
    ans = ident_step(left1, right1);
  }
  mTable.emplace(key, ans);
  return ans;
}

END_NAMESPACE_YM_DD
