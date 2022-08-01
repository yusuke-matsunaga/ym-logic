
/// @file Bdd.cc
/// @brief Bdd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Bdd.h"
#include "ym/BddVarSet.h"
#include "ym/BddError.h"
#include "BddEdge.h"
#include "BddNode.h"
#include "SupOp.h"


BEGIN_NAMESPACE_YM_BDD

// @brief サポート集合のユニオンを計算して代入する．
Bdd&
Bdd::support_cup_int(
  const Bdd& right ///< [in] 第2オペランド
)
{
  ASSERT_COND( is_posicube() );
  ASSERT_COND( right.is_posicube() );
  SupOp op{mMgr};
  auto e = op.cup_step(mRoot, right.mRoot);
  change_root(e);
  return *this;
}

// @brief サポート集合のインターセクションを計算して代入する．
Bdd&
Bdd::support_cap_int(
  const Bdd& right
)
{
  ASSERT_COND( is_posicube() );
  ASSERT_COND( right.is_posicube() );
  SupOp op{mMgr};
  auto e = op.cap_step(mRoot, right.mRoot);
  change_root(e);
  return *this;
}

// @brief サポート集合の差を計算して代入する．
Bdd&
Bdd::support_diff_int(
  const Bdd& right ///< [in] 第2オペランド
)
{
  ASSERT_COND( is_posicube() );
  ASSERT_COND( right.is_posicube() );
  SupOp op{mMgr};
  auto e = op.diff_step(mRoot, right.mRoot);
  change_root(e);
  return *this;
}

// @brief サポート集合が共通部分を持つか調べる．
bool
Bdd::support_check_intersect(
  const Bdd& right
) const
{
  ASSERT_COND( is_posicube() );
  ASSERT_COND( right.is_posicube() );

  BddEdge e1{mRoot};
  BddEdge e2{right.mRoot};
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
  if ( mMgr == nullptr ) {
    return false;
  }
  BddEdge e{mRoot};
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
  if ( mMgr == nullptr ) {
    return false;
  }
  BddEdge e{mRoot};
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

// @brief サポート変数のリストを得る．
BddVarSet
Bdd::get_support() const
{
  ASSERT_COND( mMgr != nullptr );
  SupOp op{mMgr};
  auto e = op.get_step(mRoot);
  return BddVarSet{Bdd{mMgr, e}};
}

// @brief 変数のリストに変換する．
vector<VarId>
Bdd::to_varlist() const
{
  ASSERT_COND( mMgr != nullptr );
  if ( !is_posicube() ) {
    throw BddError{"Bdd::to_varlist(): Not a variable list."};
  }
  BddEdge edge{mRoot};
  vector<VarId> var_list;
  while ( !edge.is_const() ) {
    auto node = edge.node();
    var_list.push_back(VarId{node->index()});
    edge = node->edge1();
  }
  return var_list;
}

// @brief リテラルのリストの変換する．
vector<Literal>
Bdd::to_litlist() const
{
  ASSERT_COND( mMgr != nullptr );
  if ( !is_cube() ) {
    throw BddError{"Bdd::to_litlist(): Not a literal list."};
  }
  BddEdge edge{mRoot};
  vector<Literal> lit_list;
  while ( !edge.is_const() ) {
    auto node = edge.node();
    auto inv = edge.inv();
    auto e0 = node->edge0() ^ inv;
    auto e1 = node->edge1() ^ inv;
    VarId var{node->index()};
    if ( e0.is_zero() ) {
      lit_list.push_back(Literal{var, false});
      edge = e1;
    }
    else if ( e1.is_zero() ) {
      lit_list.push_back(Literal{var, true});
      edge = e0;
    }
  }
  return lit_list;
}


//////////////////////////////////////////////////////////////////////
// クラス SupOp
//////////////////////////////////////////////////////////////////////

// @brief サポートを表すBDDを返す．
BddEdge
SupOp::get_step(
  BddEdge edge
)
{
  if ( edge.is_const() ) {
    return BddEdge::one();
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
  auto result = new_node(index, BddEdge::zero(), tmp);
  mTable.emplace(node, result);
  return result;
}

// @brief サポートのユニオンを求める．
BddEdge
SupOp::cup_step(
  BddEdge edge0,
  BddEdge edge1
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
    return new_node(index0, BddEdge::zero(), tmp);
  }
  else if ( index0 == index1 ) {
    auto tmp = cup_step(node0->edge1(), node1->edge1());
    return new_node(index0, BddEdge::zero(), tmp);
  }
  else {
    auto tmp = cup_step(edge0, node1->edge1());
    return new_node(index1, BddEdge::zero(), tmp);
  }
}

// @brief サポートのインターセクションを求める．
BddEdge
SupOp::cap_step(
  BddEdge edge0,
  BddEdge edge1
)
{
  ASSERT_COND( !edge0.is_zero() );
  ASSERT_COND( !edge1.is_zero() );

  if ( edge0.is_one() || edge1.is_one() ) {
    return BddEdge::one();
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
    return new_node(index0, BddEdge::zero(), tmp);
  }
  else {
    return cap_step(edge0, node1->edge1());
  }
}

// @brief サポートのユニオンを求める．
BddEdge
SupOp::diff_step(
  BddEdge edge0,
  BddEdge edge1
)
{
  ASSERT_COND( !edge0.is_zero() );
  ASSERT_COND( !edge1.is_zero() );

  if ( edge0.is_one() ) {
    return BddEdge::one();
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
    return new_node(index0, BddEdge::zero(), tmp);
  }
  else if ( index0 == index1 ) {
    return diff_step(node0->edge1(), node1->edge1());
  }
  else { // index0 > index1
    return diff_step(edge0, node1->edge1());
  }
}

END_NAMESPACE_YM_BDD
