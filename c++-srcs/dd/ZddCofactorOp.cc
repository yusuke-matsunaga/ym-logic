
/// @file ZddCofactorOp.cc
/// @brief ZddCofactorOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Zdd.h"
#include "ym/ZddMgrPtr.h"
#include "ZddCofactorOp.h"


BEGIN_NAMESPACE_YM_DD

// @brief 変数を含む集合を求める．
Zdd
ZddMgrPtr::onset(
  const Zdd& dd,
  const ZddItem& item
) const
{
  dd._check_valid();
  item._check_valid();
  _check_mgr(dd);
  _check_mgr(item);
  ZddCofactorOp op(get(), item.level(), true);
  auto edge = op.cofactor_step(dd.root());
  return _zdd(edge);
}

// @brief 変数を含まない集合を求める．
Zdd
ZddMgrPtr::offset(
  const Zdd& dd,
  const ZddItem& item
) const
{
  dd._check_valid();
  item._check_valid();
  _check_mgr(dd);
  _check_mgr(item);
  ZddCofactorOp op(get(), item.level(), false);
  auto edge = op.cofactor_step(dd.root());
  return _zdd(edge);
}


//////////////////////////////////////////////////////////////////////
// クラス ZddCofactorOp
//////////////////////////////////////////////////////////////////////

// @brief コファクターを計算する．
DdEdge
ZddCofactorOp::cofactor_step(
  DdEdge edge
)
{
  // 終端ならそのまま返す．
  if ( edge.is_const() ) {
    return edge;
  }

  if ( mTable.count(edge) > 0 ) {
    return mTable.at(edge);
  }

  auto node = edge.node();
  auto level = node->level();
  auto edge0 = node->edge0();
  auto edge1 = node->edge1();

  DdEdge result;
  if ( level == mLevel ) {
    if ( mPhase ) {
      result = cofactor_step(edge1);
    }
    else {
      result = cofactor_step(edge0);
    }
  }
  else if ( level < mLevel ) {
    auto ans0 = cofactor_step(edge0);
    auto ans1 = cofactor_step(edge1);
    result = new_node(level, ans0, ans1);;
  }
  else { // level > mLevel
    result = edge;
  }
  mTable.emplace(edge, result);
  return result;
}

END_NAMESPACE_YM_DD
