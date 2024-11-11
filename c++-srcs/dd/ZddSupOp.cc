
/// @file ZddSupOp.cc
/// @brief ZddSupOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Zdd.h"
#include "dd/DdEdge.h"
#include "dd/DdNode.h"
#include "ZddSupOp.h"


BEGIN_NAMESPACE_YM_DD

// @brief サポート変数のリスト(vector)を得る．
vector<ZddItem>
Zdd::get_support_list() const
{
  _check_valid();

  ZddSupOp op{*mMgr};
  auto edge = op.get_step(DdEdge{mRoot});
  vector<ZddItem> item_list;
  while ( !edge.is_const() ) {
    auto node = edge.node();
    auto item = mMgr->index_to_item(node->index());
    item_list.push_back(item);
    edge = node->edge1();
  }
  return item_list;
}


//////////////////////////////////////////////////////////////////////
// クラス Zdd_SupOp
//////////////////////////////////////////////////////////////////////

// @brief サポートを表すBDDを返す．
DdEdge
ZddSupOp::get_step(
  DdEdge edge
)
{
  if ( edge.is_const() ) {
    return DdEdge::one();
  }

  auto node = edge.node();
  if ( mTable.count(node) > 0 ) {
    return mTable.at(node);
  }
  auto index = node->index();
  auto edge0 = node->edge0();
  auto edge1 = node->edge1();
  auto r0 = get_step(edge0);
  auto r1 = get_step(edge1);
  auto tmp = cup_step(r0, r1);
  auto result = new_node(index, DdEdge::zero(), tmp);
  mTable.emplace(node, result);
  return result;
}

// @brief サポートのユニオンを求める．
DdEdge
ZddSupOp::cup_step(
  DdEdge edge0,
  DdEdge edge1
)
{
  ASSERT_COND( !edge0.is_zero() );
  ASSERT_COND( !edge1.is_zero() );

  if ( edge0.is_one() ) {
    return edge1;
  }
  if ( edge1.is_one() ) {
    return edge0;
  }

  auto node0 = edge0.node();
  auto node1 = edge1.node();
  auto index0 = node0->index();
  auto index1 = node1->index();
  auto top = std::min(index0, index1);
  if ( index0 < index1 ) {
    auto tmp = cup_step(node0->edge1(), edge1);
    return new_node(index0, DdEdge::zero(), tmp);
  }
  else if ( index0 == index1 ) {
    auto tmp = cup_step(node0->edge1(), node1->edge1());
    return new_node(index0, DdEdge::zero(), tmp);
  }
  else {
    auto tmp = cup_step(edge0, node1->edge1());
    return new_node(index1, DdEdge::zero(), tmp);
  }
}

END_NAMESPACE_YM_DD
