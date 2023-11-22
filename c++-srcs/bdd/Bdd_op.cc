
/// @file Bdd.cc
/// @brief Bdd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022, 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Bdd.h"
#include "BddEdge.h"
#include "BddMgrImpl.h"
#include "BddNode.h"
#include "MultiCompOp.h"


BEGIN_NAMESPACE_YM_BDD

// @brief 論理積を返す．
Bdd
Bdd::and_op(
  const Bdd& right
) const
{
  _check_valid();
  right._check_valid();

  BddEdge redge = copy_edge(right);
  auto e = mMgr->and_op(BddEdge{mRoot}, redge);
  return Bdd{mMgr, e};
}

// @brief 論理和を返す．
Bdd
Bdd::or_op(
  const Bdd& right
) const
{
  _check_valid();
  right._check_valid();

  BddEdge redge = copy_edge(right);
  auto e = mMgr->or_op(BddEdge{mRoot}, redge);
  return Bdd{mMgr, e};
}

// @brief 排他的論理和を返す．
Bdd
Bdd::xor_op(
  const Bdd& right
) const
{
  _check_valid();
  right._check_valid();

  BddEdge redge = copy_edge(right);
  auto e = mMgr->xor_op(BddEdge{mRoot}, redge);
  return Bdd{mMgr, e};
}

// @brief 論理積を計算して代入する．
Bdd&
Bdd::and_int(
  const Bdd& right
)
{
  _check_valid();
  right._check_valid();

  BddEdge redge = copy_edge(right);
  auto e = mMgr->and_op(BddEdge{mRoot}, redge);
  change_root(e);
  return *this;
}

// @brief 論理和を計算して代入する．
Bdd&
Bdd::or_int(
  const Bdd& right
)
{
  _check_valid();
  right._check_valid();

  BddEdge redge = copy_edge(right);
  auto e = mMgr->or_op(BddEdge{mRoot}, redge);
  change_root(e);
  return *this;
}

// @brief 排他的論理和を計算して代入する．
Bdd&
Bdd::xor_int(
  const Bdd& right
)
{
  _check_valid();
  right._check_valid();

  BddEdge redge = copy_edge(right);
  auto e = mMgr->xor_op(BddEdge{mRoot}, redge);
  change_root(e);
  return *this;
}

// @brief If-Then-Else 演算
Bdd
ite(
  const Bdd& cond,
  const Bdd& then_f,
  const Bdd& else_f
)
{
  cond._check_valid();
  then_f._check_valid();
  else_f._check_valid();

  auto mgr = cond.mMgr;
  auto then_edge = cond.copy_edge(then_f);
  auto else_edge = cond.copy_edge(else_f);
  auto e = mgr->ite_op(BddEdge{cond.mRoot}, then_edge, else_edge);
  return Bdd{mgr, e};
}

// @brief 複合compose演算
Bdd
Bdd::multi_compose(
  const unordered_map<SizeType, Bdd>& compose_map
) const
{
  auto e = mcomp_sub(compose_map);
  return Bdd{mMgr, e};
}

// @brief 複合compose演算を行って代入する．
Bdd&
Bdd::multi_compose_int(
  const unordered_map<SizeType, Bdd>& compose_map
)
{
  auto e = mcomp_sub(compose_map);
  change_root(e);
  return *this;
}

// @brief multi_compose() の共通関数
BddEdge
Bdd::mcomp_sub(
  const unordered_map<SizeType, Bdd>& compose_map
) const
{
  _check_valid();

  unordered_map<SizeType, BddEdge> cmap;
  for ( auto& p: compose_map ) {
    auto var = p.first;
    auto& bdd = p.second;
    bdd._check_valid();
    auto cedge = copy_edge(bdd);
    cmap.emplace(var, cedge);
  }
  MultiCompOp op{mMgr, cmap};
  auto e = op.mcomp_op(BddEdge{mRoot});
  return e;
}

// @brief 変数順を入れ替える演算
Bdd
Bdd::remap_vars(
  const unordered_map<SizeType, Literal>& varmap
) const
{
  _check_valid();

  unordered_map<SizeType, Bdd> compose_map;
  for ( auto& p: varmap ) {
    auto var = p.first;
    auto lit = p.second;
    auto e = mMgr->make_literal(lit.varid()) ^ lit.is_negative();
    Bdd bdd{mMgr, e};
    compose_map.emplace(var, bdd);
  }
  return multi_compose(compose_map);
}


//////////////////////////////////////////////////////////////////////
// クラス MultiCompOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MultiCompOp::MultiCompOp(
  BddMgrImpl* mgr,
  const unordered_map<SizeType, BddEdge>& comp_map
) : OpBase{mgr}
{
  mCompList.reserve(comp_map.size());
  for ( auto& p: comp_map ) {
    mCompList.push_back(p);
  }
  sort(mCompList.begin(), mCompList.end(),
       [](const pair<SizeType, BddEdge>& a,
	  const pair<SizeType, BddEdge>& b)
       { return a.first < b.first; });
}

// @brief 複数のcompose演算を行う．
BddEdge
MultiCompOp::mcomp_step(
  BddEdge edge,
  SizeType pos
)
{
  if ( edge.is_const() ) {
    return edge;
  }

  auto node = edge.node();
  auto index = node->index();

  BddEdge result;
  if ( mTable.count(node) > 0 ) {
    result = mTable.at(node);
  }
  else {
    auto cindex = mCompList[pos].first;
    while ( cindex < index ) {
      ++ pos;
      if ( pos == mCompList.size() ) {
	return edge;
      }
      cindex = mCompList[pos].first;
    }
    ASSERT_COND( cindex >= index );

    auto edge0 = node->edge0();
    auto edge1 = node->edge1();
    if ( index < cindex ) {
      auto r0 = mcomp_step(edge0, pos);
      auto r1 = mcomp_step(edge1, pos);
      result = new_node(index, r0, r1);
    }
    else { // index == cindex
      auto r0 = mcomp_step(edge0, pos + 1);
      auto r1 = mcomp_step(edge1, pos + 1);
      auto cedge = mCompList[pos].second;
      result = mgr()->ite_op(cedge, r1, r0);
    }
    mTable.emplace(node, result);
  }
  auto inv = edge.inv();
  return result ^ inv;
}

END_NAMESPACE_YM_BDD
