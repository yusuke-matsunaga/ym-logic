
/// @file Bdd.cc
/// @brief Bdd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Bdd.h"
#include "ym/BddError.h"
#include "BddEdge.h"
#include "BddNode.h"
#include "SupOp.h"
#include "OneOp.h"


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
    auto e0 = node->edge0() * inv;
    auto e1 = node->edge1() * inv;
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
    auto e0 = node->edge0() * inv;
    if ( e0.is_zero() ) {
      e = node->edge1() * inv;
    }
    else {
      return false;
    }
  }
  return true;
}

// @brief サポート変数のリストを得る．
Bdd
Bdd::get_support() const
{
  ASSERT_COND( mMgr != nullptr );
  SupOp op{mMgr};
  auto e = op.get_step(mRoot);
  return Bdd{mMgr, e};
}

// @brief 1となるパスを求める．
Bdd
Bdd::get_onepath() const
{
  ASSERT_COND( mMgr != nullptr );
  OneOp op{mMgr};
  auto e = op.op_step(mRoot);
  return Bdd{mMgr, e};
}

// @brief 0となるパスを求める．
Bdd
Bdd::get_zeropath() const
{
  ASSERT_COND( mMgr != nullptr );
  OneOp op{mMgr};
  auto e = op.op_step(~BddEdge{mRoot});
  return Bdd{mMgr, e};
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
    auto e0 = node->edge0() * inv;
    auto e1 = node->edge1() * inv;
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

END_NAMESPACE_YM_BDD
