
/// @file BddShiftOp.cc
/// @brief BddShiftOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Bdd.h"
#include "dd/DdEdge.h"
#include "dd/DdNode.h"
#include "BddShiftOp.h"


BEGIN_NAMESPACE_YM_DD

// @brief 変数シフト演算
Bdd
BddMgrImpl::shift_var(
  const Bdd& bdd,
  SizeType var
)
{
  BddShiftOp op{*this, var};
  auto e = op.shift_step(DdEdge{bdd.mRoot});
  return _bdd(e);
}


//////////////////////////////////////////////////////////////////////
// クラス Bdd_ShiftOp
//////////////////////////////////////////////////////////////////////

BEGIN_NONAMESPACE

inline
void
decomp1(
  DdEdge edge,
  SizeType index0,
  DdEdge& edge0,
  DdEdge& edge1
)
{
  if ( edge.is_const() ) {
    edge0 = edge;
    edge1 = edge;
  }
  auto node = edge.node();
  auto inv = edge.inv();
  auto index = node->index();
  if ( index == index0 + 1 ) {
    edge0 = node->edge0() ^ inv;
    edge1 = node->edge1() ^ inv;
  }
  else {
    edge0 = edge;
    edge1 = edge;
  }
}

END_NONAMESPACE

// @brief シフト演算の結果を表すBDDを返す．
DdEdge
BddShiftOp::shift_step(
  DdEdge edge
)
{
  if ( edge.is_const() ) {
    return DdEdge::one();
  }

  auto node = edge.node();
  auto inv = edge.inv();
  auto index = node->index();
  if ( index + 1 == mVar ) {
    auto edge0 = node->edge0();
    auto edge1 = node->edge1();
    auto result = new_node(index, edge0, edge1);
    return result ^ inv;
  }
  else if ( index > mVar ) {
    // mVar より下のBDDはそのまま
    return edge;
  }

  if ( mTable.count(node) > 0 ) {
    auto result = mTable.at(node);
    return result ^ inv;
  }

  auto edge0 = node->edge0();
  auto edge1 = node->edge1();
  DdEdge r0;
  DdEdge r1;
  if ( index == mVar ) {
    DdEdge edge00;
    DdEdge edge01;
    decomp1(edge0, index, edge00, edge01);
    DdEdge edge10;
    DdEdge edge11;
    decomp1(edge1, index, edge10, edge10);
    r0 = new_node(index + 1, edge00, edge10);
    r1 = new_node(index + 1, edge01, edge11);
  }
  else { // index < mVar
    r0 = shift_step(edge0);
    r1 = shift_step(edge1);
  }
  auto result = new_node(index, r0, r1);
  mTable.emplace(node, result);
  return result ^ inv;
}

END_NAMESPACE_YM_DD
