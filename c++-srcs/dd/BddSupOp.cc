
/// @file BddSupOp.cc
/// @brief BddSupOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Bdd.h"
#include "ym/BddVar.h"
#include "ym/BddVarSet.h"
#include "ym/BddLit.h"
#include "DdEdge.h"
#include "DdNode.h"
#include "BddSupOp.h"


BEGIN_NAMESPACE_YM_DD

// @brief サポート集合のユニオンを計算して代入する．
DdEdge
Bdd::support_cup(
  const Bdd& right
) const
{
  _check_posicube();
  right._check_posicube();

  BddSupOp op(get());
  auto ledge = root();
  auto redge = right.root();
  auto edge = op.cup_step(ledge, redge);
  return edge;
}

// @brief サポート集合のインターセクションを計算して代入する．
DdEdge
Bdd::support_cap(
  const Bdd& right
) const
{
  _check_posicube();
  right._check_posicube();

  BddSupOp op(get());
  auto ledge = root();
  auto redge = right.root();
  auto edge = op.cap_step(ledge, redge);
  return edge;
}

// @brief サポート集合の差を計算して代入する．
DdEdge
Bdd::support_diff(
  const Bdd& right
) const
{
  _check_posicube();
  right._check_posicube();

  BddSupOp op(get());
  auto ledge = root();
  auto redge = right.root();
  auto edge = op.diff_step(ledge, redge);
  return edge;
}

// @brief サポート集合が共通部分を持つか調べる．
bool
Bdd::support_check_intersect(
  const Bdd& right
) const
{
  _check_posicube();
  right._check_posicube();

  auto e1 = root();
  auto e2 = right.root();
  while ( !e1.is_one() && !e2.is_one() ) {
    auto node1 = e1.node();
    auto node2 = e2.node();
    auto level1 = node1->level();
    auto level2 = node2->level();
    if ( level1 == level2 ) {
      return true;
    }
    else if ( level1 < level2 ) {
      e1 = node1->edge1();
    }
    else { // level > level2
      e2 = node2->edge1();
    }
  }
  return false;
}

// @brief 積項の時 true を返す．
bool
Bdd::is_cube() const
{
  if ( is_invalid() ) {
    return false;
  }

  auto e = root();
  if ( e.is_zero() ) {
    return false;
  }
  while ( !e.is_one() ) {
    auto node = e.node();
    auto inv = e.inv();
    auto e0 = node->edge0() ^ inv;
    auto e1 = node->edge1() ^ inv;
    if ( e0.is_zero() ) {
      e = e1;
    }
    else if ( e1.is_zero() ) {
      e = e0;
    }
    else {
      return false;
    }
  }
  return true;
}

// @brief 正リテラルの積項の時 true を返す．
bool
Bdd::is_posicube() const
{
  if ( is_invalid() ) {
    return false;
  }

  auto e = root();
  if ( e.is_zero() ) {
    return false;
  }
  while ( !e.is_one() ) {
    auto node = e.node();
    auto inv = e.inv();
    auto e0 = node->edge0() ^ inv;
    if ( e0.is_zero() ) {
      e = node->edge1() ^ inv;
    }
    else {
      return false;
    }
  }
  return true;
}

// @brief サポートを表すBDDを返す．
BddVarSet
Bdd::get_support() const
{
  _check_valid();

  BddSupOp op(get());
  auto edge = op.get_step(root());
  return BddVarSet(_bdd(edge));
}

// @brief サポート変数のリスト(vector)を得る．
std::vector<BddVar>
Bdd::get_support_list() const
{
  return get_support().to_varlist();
}

// @brief 変数のリストに変換する．
std::vector<BddVar>
Bdd::to_varlist() const
{
  _check_valid();
  _check_posicube();

  auto edge = root();
  std::vector<BddVar> var_list;
  while ( !edge.is_const() ) {
    auto node = edge.node();
    auto var = _level_to_var(node->level());
    var_list.push_back(var);
    edge = node->edge1();
  }
  return var_list;
}

// @brief リテラルのリストの変換する．
std::vector<BddLit>
Bdd::to_litlist() const
{
  _check_valid();
  _check_cube();

  auto edge = root();
  std::vector<BddLit> lit_list;
  while ( !edge.is_const() ) {
    auto node = edge.node();
    auto inv = edge.inv();
    auto e0 = node->edge0() ^ inv;
    auto e1 = node->edge1() ^ inv;
    auto edge = get()->new_node(node->level(), DdEdge::zero(), DdEdge::one());
    auto var = _var(edge);
    if ( e0.is_zero() ) {
      lit_list.push_back(BddLit{var, false});
      edge = e1;
    }
    else if ( e1.is_zero() ) {
      lit_list.push_back(BddLit{var, true});
      edge = e0;
    }
  }
  return lit_list;
}


//////////////////////////////////////////////////////////////////////
// クラス Bdd_SupOp
//////////////////////////////////////////////////////////////////////

// @brief サポートを表すBDDを返す．
DdEdge
BddSupOp::get_step(
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
  auto level = node->level();
  auto edge0 = node->edge0();
  auto edge1 = node->edge1();
  auto r0 = get_step(edge0);
  auto r1 = get_step(edge1);
  auto tmp = cup_step(r0, r1);
  auto result = new_node(level, DdEdge::zero(), tmp);
  mTable.emplace(node, result);
  return result;
}

// @brief サポートのユニオンを求める．
DdEdge
BddSupOp::cup_step(
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
  auto level0 = node0->level();
  auto level1 = node1->level();
  auto top = std::min(level0, level1);
  if ( level0 < level1 ) {
    auto tmp = cup_step(node0->edge1(), edge1);
    return new_node(level0, DdEdge::zero(), tmp);
  }
  else if ( level0 == level1 ) {
    auto tmp = cup_step(node0->edge1(), node1->edge1());
    return new_node(level0, DdEdge::zero(), tmp);
  }
  else {
    auto tmp = cup_step(edge0, node1->edge1());
    return new_node(level1, DdEdge::zero(), tmp);
  }
}

// @brief サポートのインターセクションを求める．
DdEdge
BddSupOp::cap_step(
  DdEdge edge0,
  DdEdge edge1
)
{
  ASSERT_COND( !edge0.is_zero() );
  ASSERT_COND( !edge1.is_zero() );

  if ( edge0.is_one() || edge1.is_one() ) {
    return DdEdge::one();
  }

  auto node0 = edge0.node();
  auto node1 = edge1.node();
  auto level0 = node0->level();
  auto level1 = node1->level();
  auto top = std::min(level0, level1);
  if ( level0 < level1 ) {
    return cap_step(node0->edge1(), edge1);
  }
  else if ( level0 == level1 ) {
    auto tmp = cap_step(node0->edge1(), node1->edge1());
    return new_node(level0, DdEdge::zero(), tmp);
  }
  else {
    return cap_step(edge0, node1->edge1());
  }
}

// @brief サポートのユニオンを求める．
DdEdge
BddSupOp::diff_step(
  DdEdge edge0,
  DdEdge edge1
)
{
  ASSERT_COND( !edge0.is_zero() );
  ASSERT_COND( !edge1.is_zero() );

  if ( edge0.is_one() ) {
    return DdEdge::one();
  }
  if ( edge1.is_one() ) {
    return edge0;
  }

  auto node0 = edge0.node();
  auto node1 = edge1.node();
  auto level0 = node0->level();
  auto level1 = node1->level();
  if ( level0 < level1 ) {
    auto tmp = diff_step(node0->edge1(), edge1);
    return new_node(level0, DdEdge::zero(), tmp);
  }
  else if ( level0 == level1 ) {
    return diff_step(node0->edge1(), node1->edge1());
  }
  else { // level0 > level1
    return diff_step(edge0, node1->edge1());
  }
}

END_NAMESPACE_YM_DD
