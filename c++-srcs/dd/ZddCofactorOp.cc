
/// @file ZddCofactorOp.cc
/// @brief ZddCofactorOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Zdd.h"
#include "ZddMgrImpl.h"
#include "ZddCofactorOp.h"


BEGIN_NAMESPACE_YM_DD

// @brief 変数を含む集合を求める．
Zdd
ZddMgrImpl::onset(
  const Zdd& dd,
  const ZddItem& item
)
{
  dd._check_valid();
  item._check_valid();
  _check_mgr(dd);
  _check_mgr(item);
  ZddCofactorOp op(*this, item.level(), true);
  auto e = op.cofactor_step(_edge(dd));
  return _zdd(e);
}

// @brief 変数を含まない集合を求める．
Zdd
ZddMgrImpl::offset(
  const Zdd& dd,
  const ZddItem& item
)
{
  dd._check_valid();
  item._check_valid();
  _check_mgr(dd);
  _check_mgr(item);
  ZddCofactorOp op(*this, item.level(), false);
  auto e = op.cofactor_step(_edge(dd));
  return _zdd(e);
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
