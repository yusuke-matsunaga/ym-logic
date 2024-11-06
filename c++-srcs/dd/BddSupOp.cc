
/// @file BddSupOp.cc
/// @brief BddSupOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Bdd.h"
#include "ym/BddVarSet.h"
#include "dd/DdEdge.h"
#include "dd/DdNode.h"
#include "BddSupOp.h"


BEGIN_NAMESPACE_YM_DD

// @brief サポート集合のユニオンを計算して代入する．
Bdd&
Bdd::support_cup_int(
  const Bdd& right ///< [in] 第2オペランド
)
{
  _check_posicube();
  right._check_posicube();

  BddSupOp op{*mMgr};
  auto e = op.cup_step(DdEdge{mRoot}, DdEdge{right.mRoot});
  change_root(e);
  return *this;
}

// @brief サポート集合のインターセクションを計算して代入する．
Bdd&
Bdd::support_cap_int(
  const Bdd& right
)
{
  _check_posicube();
  right._check_posicube();

  BddSupOp op{*mMgr};
  auto e = op.cap_step(DdEdge{mRoot}, DdEdge{right.mRoot});
  change_root(e);
  return *this;
}

// @brief サポート集合の差を計算して代入する．
Bdd&
Bdd::support_diff_int(
  const Bdd& right ///< [in] 第2オペランド
)
{
  _check_posicube();
  right._check_posicube();

  BddSupOp op{*mMgr};
  auto e = op.diff_step(DdEdge{mRoot}, DdEdge{right.mRoot});
  change_root(e);
  return *this;
}

// @brief サポート集合が共通部分を持つか調べる．
bool
Bdd::support_check_intersect(
  const Bdd& right
) const
{
  _check_posicube();
  right._check_posicube();

  auto e1 = DdEdge{mRoot};
  auto e2 = DdEdge{right.mRoot};
  while ( !e1.is_one() && !e2.is_one() ) {
    auto node1 = e1.node();
    auto node2 = e2.node();
    auto index1 = node1->index();
    auto index2 = node2->index();
    if ( index1 == index2 ) {
      return true;
    }
    else if ( index1 < index2 ) {
      e1 = node1->edge1();
    }
    else { // index > index2
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

  auto e = DdEdge{mRoot};
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

  auto e = DdEdge{mRoot};
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

  BddSupOp op{*mMgr};
  auto e = op.get_step(DdEdge{mRoot});
  return BddVarSet{Bdd{mMgr, e}};
}

// @brief サポート変数のリスト(vector)を得る．
vector<BddVar>
Bdd::get_support_list() const
{
  return get_support().to_varlist();
}

// @brief 変数のリストに変換する．
vector<BddVar>
Bdd::to_varlist() const
{
  _check_valid();
  _check_posicube();

  auto edge = DdEdge{mRoot};
  vector<BddVar> var_list;
  while ( !edge.is_const() ) {
    auto node = edge.node();
    auto var = mMgr->index_to_var(node->index());
    var_list.push_back(var);
    edge = node->edge1();
  }
  return var_list;
}

// @brief リテラルのリストの変換する．
vector<BddLit>
Bdd::to_litlist() const
{
  _check_valid();
  _check_cube();

  auto edge = DdEdge{mRoot};
  vector<BddLit> lit_list;
  while ( !edge.is_const() ) {
    auto node = edge.node();
    auto inv = edge.inv();
    auto e0 = node->edge0() ^ inv;
    auto e1 = node->edge1() ^ inv;
    auto var = mMgr->index_to_var(node->index());
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
  auto index0 = node0->index();
  auto index1 = node1->index();
  auto top = std::min(index0, index1);
  if ( index0 < index1 ) {
    return cap_step(node0->edge1(), edge1);
  }
  else if ( index0 == index1 ) {
    auto tmp = cap_step(node0->edge1(), node1->edge1());
    return new_node(index0, DdEdge::zero(), tmp);
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
  auto index0 = node0->index();
  auto index1 = node1->index();
  if ( index0 < index1 ) {
    auto tmp = diff_step(node0->edge1(), edge1);
    return new_node(index0, DdEdge::zero(), tmp);
  }
  else if ( index0 == index1 ) {
    return diff_step(node0->edge1(), node1->edge1());
  }
  else { // index0 > index1
    return diff_step(edge0, node1->edge1());
  }
}

END_NAMESPACE_YM_DD
