
/// @file Zdd.cc
/// @brief Zdd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022, 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Zdd.h"
#include "ZddEdge.h"
#include "ZddMgrImpl.h"
#include "ZddNode.h"
#include "ProductOp.h"
#include "RootHolder.h"
#include "CountOp.h"


BEGIN_NAMESPACE_YM_ZDD

// @brief CAP演算
Zdd
Zdd::cap_op(
  const Zdd& right
) const
{
  _check_valid();
  right._check_valid();

  auto redge = copy_edge(right);
  auto e = mMgr->cap_op(ZddEdge{mRoot}, redge);
  return Zdd{mMgr, e};
}

// @brief CUP演算
Zdd
Zdd::cup_op(
  const Zdd& right
) const
{
  _check_valid();
  right._check_valid();

  auto redge = copy_edge(right);
  auto e = mMgr->cup_op(ZddEdge{mRoot}, redge);
  return Zdd{mMgr, e};
}

// @brief DIFF演算
Zdd
Zdd::diff_op(
  const Zdd& right
) const
{
  _check_valid();
  right._check_valid();

  auto redge = copy_edge(right);
  auto e = mMgr->diff_op(ZddEdge{mRoot}, redge);
  return Zdd{mMgr, e};
}

// @brief PRODUCT演算
Zdd
Zdd::product_op(
  const Zdd& right
) const
{
  _check_valid();
  right._check_valid();

  auto redge = copy_edge(right);
  Zdd tmp_right{mMgr, redge};

  ProductOp op{mMgr};
  auto e = op.op_step(ZddEdge{mRoot}, ZddEdge{redge});
  return Zdd{mMgr, e};
}

// @brief CAP演算つき代入
Zdd&
Zdd::cap_int(
  const Zdd& right
)
{
  _check_valid();
  right._check_valid();

  auto redge = copy_edge(right);
  auto e = mMgr->cap_op(ZddEdge{mRoot}, redge);
  change_root(e);
  return *this;
}

// @brief CUP付き代入
Zdd&
Zdd::cup_int(
  const Zdd& right
)
{
  _check_valid();
  right._check_valid();

  auto redge = copy_edge(right);
  auto e = mMgr->cup_op(ZddEdge{mRoot}, redge);
  change_root(e);
  return *this;
}

// @brief DIFF付き代入
Zdd&
Zdd::diff_int(
  const Zdd& right
)
{
  _check_valid();
  right._check_valid();

  auto redge = copy_edge(right);
  auto e = mMgr->diff_op(ZddEdge{mRoot}, redge);
  change_root(e);
  return *this;
}

// @brief PRODUCT付き代入
Zdd&
Zdd::product_int(
  const Zdd& right
)
{
  _check_valid();
  right._check_valid();

  auto redge = copy_edge(right);
  Zdd tmp_right{mMgr, redge};

  ProductOp op{mMgr};
  auto e = op.op_step(ZddEdge{mRoot}, ZddEdge{redge});
  change_root(e);
  return *this;
}


// @brief PRODUCT 演算を行う．
ZddEdge
ProductOp::op_step(
  ZddEdge left,
  ZddEdge right
)
{
  // trivial case のチェック

  // case 1: どちらかが が 0 なら 0 を返す．
  if ( left.is_zero() ) {
    return ZddEdge::zero();
  }
  if ( right.is_zero() ) {
    return ZddEdge::zero();
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
  SizeType l_index;
  ZddEdge left0, left1;
  SizeType r_index;
  ZddEdge right0, right1;
  auto top_index = decomp(left, right, left0, left1, right0, right1);

  auto e00 = op_step(left0, right0);
  ZddEdge sum;
  {
    RootHolder lock00{mgr(), e00};
    auto e01 = op_step(left0, right1);
    RootHolder lock01{mgr(), e01};
    auto e10 = op_step(left1, right0);
    RootHolder lock10{mgr(), e10};
    auto e11 = op_step(left1, right1);
    RootHolder lock11{mgr(), e11};
    auto tmp = mgr()->cup_op(e01, e10);
    RootHolder lock_tmp{mgr(), tmp};
    sum = mgr()->cup_op(tmp, e11);
  }
  auto result = new_node(top_index, e00, sum);
  mTable.emplace(key, result);
  return result;
}

// @brief 集合の要素数を数える．
SizeType
Zdd::count() const
{
  _check_valid();

  CountOp op;
  return op.op_step(ZddEdge{mRoot});
}

SizeType
CountOp::op_step(
  ZddEdge edge
)
{
  if ( edge.is_zero() ) {
    return 0;
  }
  if ( edge.is_one() ) {
    return 1;
  }

  if ( mTable.count(edge) > 0 ) {
    return mTable.at(edge);
  }

  auto node = edge.node();
  auto edge0 = node->edge0();
  auto edge1 = node->edge1();
  auto c0 = op_step(edge0);
  auto c1 = op_step(edge1);
  auto result = c0 + c1;
  mTable.emplace(edge, result);
  return result;
}

END_NAMESPACE_YM_ZDD
