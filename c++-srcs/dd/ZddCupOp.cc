
/// @file ZddCupOp.cc
/// @brief ZdCupOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Zdd.h"
#include "dd/ZddMgrImpl.h"
#include "ZddCupOp.h"


BEGIN_NAMESPACE_YM_DD

// @brief CUP 演算を行う．
Zdd
ZddMgrImpl::cup(
  const Zdd& left,
  const Zdd& right
)
{
  left._check_valid();
  right._check_valid();

  auto tmp_left = copy(left);
  auto tmp_right = copy(right);
  ZddCupOp op{*this};
  auto e = op.cup_step(_edge(tmp_left), _edge(tmp_right));
  return _zdd(e);
}


//////////////////////////////////////////////////////////////////////
// クラス ZddCupOp
//////////////////////////////////////////////////////////////////////

// @brief CUP 演算を行う．
DdEdge
ZddCupOp::cup_step(
  DdEdge left,
  DdEdge right
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

  // case 2: 片方が 1 なら特別な処理を行う．
  if ( left.is_one() ) {
    return cup_step2(right);
  }
  if ( right.is_one() ) {
    return cup_step2(left);
  }

  // case 3: 等しかったら自分自身を返す．
  if ( left == right ) {
    return left;
  }

  // 正規化を行う．
  if ( left.body() > right.body() ) {
    std::swap(left, right);
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
  auto ans0 = cup_step(left0, right0);
  auto ans1 = cup_step(left1, right1);
  auto result = new_node(top, ans0, ans1);
  mTable.emplace(key, result);
  return result;
}

DdEdge
ZddCupOp::cup_step2(
  DdEdge left
)
{
  if ( left.is_const() ) {
    return DdEdge::one();
  }

  Apply2Key key{left, DdEdge::one()};
  if ( mTable.count(key) > 0 ) {
    return mTable.at(key);
  }
  auto node = left.node();
  auto level = node->level();
  auto edge0 = node->edge0();
  auto edge1 = node->edge1();
  auto ans0 = cup_step2(edge0);
  auto result = new_node(level, ans0, edge1);
  mTable.emplace(key, result);
  return result;
}

END_NAMESPACE_YM_DD
