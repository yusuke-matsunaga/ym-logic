
/// @file ZddDiffOp.cc
/// @brief ZdDiffOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Zdd.h"
#include "ZddMgrImpl.h"
#include "ZddDiffOp.h"


BEGIN_NAMESPACE_YM_DD

// @brief DIFF演算を行う．
Zdd
ZddMgrImpl::diff(
  const Zdd& left,
  const Zdd& right
)
{
  left._check_valid();
  right._check_valid();
  auto tmp_left = copy(left);
  auto tmp_right = copy(right);
  ZddDiffOp op{*this};
  auto e = op.diff_step(_edge(tmp_left), _edge(tmp_right));
  return _zdd(e);
}


//////////////////////////////////////////////////////////////////////
// クラス ZddDiffOp
//////////////////////////////////////////////////////////////////////

// @brief DIFF 演算を行う．
DdEdge
ZddDiffOp::diff_step(
  DdEdge left,
  DdEdge right
)
{
  // trivial case のチェック

  // case 1: left が 0 なら 0 を返す．
  if ( left.is_zero() ) {
    return DdEdge::zero();
  }

  // case 2: rightが 0 なら left を返す．
  if ( right.is_zero() ) {
    return left;
  }

  // case 3: left が 1 なら特別な処理1を行う．
  if ( left.is_one() ) {
    return diff_step2(right);
  }

  // case 4: right が 1 なら特別な処理2を行う．
  if ( right.is_one() ) {
    return diff_step3(left);
  }

  // case 5: 等しかったら 0 を返す．
  if ( left == right ) {
    return DdEdge::zero();
  }

  // 演算結果テーブルを調べる．
  Apply2Key key{left, right};
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
  auto ans0 = diff_step(left0, right0);
  auto ans1 = diff_step(left1, right1);
  auto result = new_node(top, ans0, ans1);
  mTable.emplace(key, result);
  return result;
}

DdEdge
ZddDiffOp::diff_step2(
  DdEdge right
)
{
  if ( right.is_zero() ) {
    return DdEdge::one();
  }
  if ( right.is_one() ) {
    return DdEdge::zero();
  }

  Apply2Key key{DdEdge::one(), right};
  if ( mTable.count(key) > 0 ) {
    return mTable.at(key);
  }
  auto node = right.node();
  auto index = node->index();
  auto edge0 = node->edge0();
  auto ans0 = diff_step2(edge0);
  auto result = new_node(index, ans0, DdEdge::zero());
  mTable.emplace(key, result);
  return result;
}

DdEdge
ZddDiffOp::diff_step3(
  DdEdge left
)
{
  if ( left.is_const() ) {
    return DdEdge::zero();
  }

  Apply2Key key{left, DdEdge::one()};
  if ( mTable.count(key) > 0 ) {
    return mTable.at(key);
  }
  auto node = left.node();
  auto index = node->index();
  auto edge0 = node->edge0();
  auto edge1 = node->edge1();
  auto ans0 = diff_step3(edge0);
  auto result = new_node(index, ans0, edge1);
  mTable.emplace(key, result);
  return result;
}

END_NAMESPACE_YM_DD
