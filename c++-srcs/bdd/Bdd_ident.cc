
/// @file Bdd_ident.cc
/// @brief Bdd::is_identical() の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Bdd.h"
#include "BddNode.h"
#include "IdentOp.h"


BEGIN_NAMESPACE_YM_BDD

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
  return op.ident_step(mRoot, right.mRoot);
}


//////////////////////////////////////////////////////////////////////
// クラス IdentOp
//////////////////////////////////////////////////////////////////////

// @brief 同じ構造かチェックする．
bool
IdentOp::ident_step(
  BddEdge left,
  BddEdge right
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

  Apply2Key key{left, right};
  if ( mTable.count(key) > 0 ) {
    return mTable.at(key);
  }

  BddEdge left0, left1;
  BddEdge right0, right1;
  auto top = decomp(left, right,
		    left0, left1,
		    right0, right1);
  auto ans = ident_step(left0, right0);
  if ( ans ) {
    ans = ident_step(left1, right1);
  }
  mTable.emplace(key, ans);
  return ans;
}

END_NAMESPACE_YM_BDD
