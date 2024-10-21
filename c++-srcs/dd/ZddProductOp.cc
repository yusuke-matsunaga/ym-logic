
/// @file ZddProductOp.cc
/// @brief ZddProductOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Zdd.h"
#include "dd/DdEdge.h"
#include "dd/ZddMgrImpl.h"
#include "dd/DdNode.h"
#include "ZddProductOp.h"


BEGIN_NAMESPACE_YM_DD

/// @brief PRODUCT 演算を行う．
Zdd
ZddMgrImpl::product(
  const Zdd& left,
  const Zdd& right
)
{
  left._check_valid();
  right._check_valid();
  auto tmp0 = copy(left);
  auto tmp1 = copy(right);
  ZddProductOp op(*this);
  auto e = op.prod_step(_edge(tmp0), _edge(tmp1));
  return _zdd(e);
}


//////////////////////////////////////////////////////////////////////
// クラス ZddProductOp
//////////////////////////////////////////////////////////////////////

// @brief PRODUCT 演算を行う．
DdEdge
ZddProductOp::prod_step(
  DdEdge left,
  DdEdge right
)
{
  // trivial case のチェック

  // case 1: どちらかが が 0 なら 0 を返す．
  if ( left.is_zero() ) {
    return DdEdge::zero();
  }
  if ( right.is_zero() ) {
    return DdEdge::zero();
  }

  // case 2: どちらかが 1 なら他方を返す．
  if ( left.is_one() ) {
    return right;
  }
  if ( right.is_one() ) {
    return left;
  }

  // case 3: 等しかったら自分自身を返す．
  if ( left == right ) {
    return left;
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
  auto top_index = decomp(left, right, left0, left1, right0, right1);

  auto e00 = prod_step(left0, right0);
  auto e01 = prod_step(left0, right1);
  auto e10 = prod_step(left1, right0);
  auto e11 = prod_step(left1, right1);
  auto tmp = mCupOp.cup_step(e01, e10);
  auto sum = mCupOp.cup_step(tmp, e11);
  auto result = new_node(top_index, e00, sum);
  mTable.emplace(key, result);
  return result;
}

END_NAMESPACE_YM_DD
