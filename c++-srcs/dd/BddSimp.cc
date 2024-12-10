
/// @file BddSimp.cc
/// @brief BddSimp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "BddSimp.h"
#include "ym/Bdd.h"


BEGIN_NAMESPACE_YM_DD

// @brief 簡単化を行う．
Bdd
BddMgrPtr::simplify(
  const Bdd& on,
  const Bdd& dc
) const
{
  on._check_valid();
  dc._check_valid();
  _check_mgr(on.mMgr);
  _check_mgr(dc.mMgr);
  BddSimp op{get()};
  auto edge = op.simp_step(on.root(), dc.root());
  return _bdd(edge);
}


//////////////////////////////////////////////////////////////////////
// クラス BddSimp
//////////////////////////////////////////////////////////////////////

DdEdge
BddSimp::simp_step(
  DdEdge on,
  DdEdge dc
)
{
  // trivial case のチェック

  // case 1: on が定数ならそれを返す．
  if ( on.is_const() ) {
    return on;
  }

  // case 2: dc が 0 なら on を返す．
  if ( dc.is_zero() ) {
    return on;
  }

  // case 3: dc が 1 なら 0 を返す．
  // これはトップレベルの場合のみ
  if ( dc.is_one() ) {
    return DdEdge::zero();
  }

  // 演算結果テーブルを調べる．
  Apply2Key key{on, dc};
  if ( mTable.count(key) > 0 ) {
    return mTable.at(key);
  }

  // 見つからなかったので実際に apply 演算を行う．
  // 先頭のインデックスで分解する．
  DdEdge on0, on1;
  DdEdge dc0, dc1;
  auto top = decomp(
    on, dc,
    on0, on1,
    dc0, dc1
  );

  DdEdge result;
  if ( dc0.is_one() ) {
    result = simp_step(on1, dc1);
  }
  else if ( dc1.is_one() ) {
    result = simp_step(on0, dc0);
  }
  else {
    auto ans0 = simp_step(on0, dc0);
    auto ans1 = simp_step(on1, dc1);
    result = new_node(top, ans0, ans1);
  }
  mTable.emplace(key, result);
  return result;
}

END_NAMESPACE_YM_DD
