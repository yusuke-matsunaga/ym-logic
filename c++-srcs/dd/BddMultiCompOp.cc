
/// @file BddMultiCompOp.cc
/// @brief BddMultiCompOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Bdd.h"
#include "ym/BddVar.h"
#include "ym/BddLit.h"
#include "ym/BddMgrHolder.h"
#include "DdEdge.h"
#include "DdNode.h"
#include "BddMultiCompOp.h"


BEGIN_NAMESPACE_YM_DD

// @brief 複合compose演算
DdEdge
Bdd::_multi_compose(
  const unordered_map<BddVar, Bdd>& compose_map
) const
{
  _check_valid();
  unordered_map<SizeType, DdEdge> cmap;
  for ( auto& p: compose_map ) {
    auto var = p.first;
    auto level = var.level();
    auto& bdd = p.second;
    bdd._check_valid();
    auto cedge = bdd.root();
    cmap.emplace(level, cedge);
  }
  BddMultiCompOp op(get(), cmap);
  auto edge = op.mcomp_op(root());
  return edge;
}

// @brief 変数順を入れ替える演算
DdEdge
Bdd::_remap_vars(
  const unordered_map<BddVar, BddLit>& varmap
) const
{
  _check_valid();
  unordered_map<SizeType, DdEdge> cmap;
  for ( auto& p: varmap ) {
    auto var = p.first;
    auto level = var.level();
    auto lit = p.second;
    auto cedge = lit.root();
    cmap.emplace(level, cedge);
  }
  BddMultiCompOp op(get(), cmap);
  auto edge = op.mcomp_op(root());
  return edge;
}


//////////////////////////////////////////////////////////////////////
// クラス BddMultiCompOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
BddMultiCompOp::BddMultiCompOp(
  BddMgrImpl* mgr,
  const unordered_map<SizeType, DdEdge>& comp_map
) : BddOpBase{mgr},
    mIteOp{mgr}
{
  mCompList.reserve(comp_map.size());
  for ( auto& p: comp_map ) {
    mCompList.push_back(p);
  }
  sort(mCompList.begin(), mCompList.end(),
       [](const pair<SizeType, DdEdge>& a,
	  const pair<SizeType, DdEdge>& b)
       { return a.first < b.first; });
}

// @brief 複数のcompose演算を行う．
DdEdge
BddMultiCompOp::mcomp_step(
  DdEdge edge,
  SizeType pos
)
{
  if ( edge.is_const() ) {
    return edge;
  }

  auto node = edge.node();
  auto level = node->level();

  DdEdge result;
  if ( mTable.count(node) > 0 ) {
    result = mTable.at(node);
  }
  else {
    auto clevel = mCompList[pos].first;
    while ( clevel < level ) {
      ++ pos;
      if ( pos == mCompList.size() ) {
	return edge;
      }
      clevel = mCompList[pos].first;
    }
    ASSERT_COND( clevel >= level );

    auto edge0 = node->edge0();
    auto edge1 = node->edge1();
    if ( level < clevel ) {
      auto r0 = mcomp_step(edge0, pos);
      auto r1 = mcomp_step(edge1, pos);
      result = new_node(level, r0, r1);
    }
    else { // level == clevel
      auto r0 = mcomp_step(edge0, pos + 1);
      auto r1 = mcomp_step(edge1, pos + 1);
      auto cedge = mCompList[pos].second;
      result = mIteOp.ite_step(cedge, r1, r0);
    }
    mTable.emplace(node, result);
  }
  auto inv = edge.inv();
  return result ^ inv;
}

END_NAMESPACE_YM_DD
