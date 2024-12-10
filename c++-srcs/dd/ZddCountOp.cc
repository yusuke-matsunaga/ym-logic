
/// @file ZddCountOp.cc
/// @brief ZddCountOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Zdd.h"
#include "DdEdge.h"
#include "DdNode.h"
#include "ZddCountOp.h"


BEGIN_NAMESPACE_YM_DD

// @brief 集合の要素数を数える．
SizeType
Zdd::count() const
{
  _check_valid();

  ZddCountOp op;
  return op.count_step(root());
}


//////////////////////////////////////////////////////////////////////
// クラス ZddCountOP
//////////////////////////////////////////////////////////////////////

SizeType
ZddCountOp::count_step(
  DdEdge edge
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
  auto c0 = count_step(edge0);
  auto c1 = count_step(edge1);
  auto result = c0 + c1;
  mTable.emplace(edge, result);
  return result;
}

END_NAMESPACE_YM_DD
