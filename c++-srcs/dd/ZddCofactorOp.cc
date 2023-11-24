
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
  SizeType var    ///< [in] 変数
)
{
  dd._check_valid();
  auto tmp_zdd = copy(dd);
  ZddCofactorOp op(*this, var, true);
  auto e = op.cofactor_step(_edge(tmp_zdd));
  return _zdd(e);
}

// @brief 変数を含まない集合を求める．
Zdd
ZddMgrImpl::offset(
  const Zdd& dd,
  SizeType var
)
{
  dd._check_valid();
  auto tmp_zdd = copy(dd);
  ZddCofactorOp op(*this, var, false);
  auto e = op.cofactor_step(_edge(tmp_zdd));
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
  auto index = node->index();
  auto edge0 = node->edge0();
  auto edge1 = node->edge1();

  DdEdge result;
  if ( index == mIndex ) {
    if ( mPhase ) {
      result = cofactor_step(edge1);
    }
    else {
      result = cofactor_step(edge0);
    }
  }
  else if ( index < mIndex ) {
    auto ans0 = cofactor_step(edge0);
    auto ans1 = cofactor_step(edge1);
    result = new_node(index, ans0, ans1);;
  }
  else { // index > mIndex
    result = edge;
  }
  mTable.emplace(edge, result);
  return result;
}

END_NAMESPACE_YM_DD
