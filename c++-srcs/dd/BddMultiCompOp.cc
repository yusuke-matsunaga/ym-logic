
/// @file BddMultiCompOp.cc
/// @brief BddMultiCompOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Bdd.h"
#include "DdEdge.h"
#include "BddMgrImpl.h"
#include "DdNode.h"
#include "BddMultiCompOp.h"


BEGIN_NAMESPACE_YM_DD

// @brief compose 演算を行う．
Bdd
BddMgrImpl::compose(
  const Bdd& edge,
  SizeType index,
  const Bdd& cedge
)
{
  unordered_map<SizeType, Bdd> cmap{{index, cedge}};
  return multi_compose(edge, cmap);
}

// @brief 複合compose演算
Bdd
BddMgrImpl::multi_compose(
  const Bdd& bdd,
  const unordered_map<SizeType, Bdd>& compose_map
)
{
  bdd._check_valid();
  auto edge0 = copy(bdd);
  unordered_map<SizeType, DdEdge> cmap;
  vector<Bdd> tmp_list;
  for ( auto& p: compose_map ) {
    auto var = p.first;
    auto& bdd = p.second;
    bdd._check_valid();
    auto tmp = copy(bdd);
    tmp_list.push_back(tmp);
    auto cedge = _edge(tmp);
    cmap.emplace(var, cedge);
  }
  BddMultiCompOp op{*this, cmap};
  auto e = op.mcomp_op(_edge(edge0));
  return _bdd(e);
}

// @brief 変数順を入れ替える演算
Bdd
BddMgrImpl::remap_vars(
  const Bdd& bdd,
  const unordered_map<SizeType, Literal>& varmap
)
{
  bdd._check_valid();
  auto edge0 = copy(bdd);
  unordered_map<SizeType, DdEdge> cmap;
  vector<Bdd> tmp_list;
  for ( auto& p: varmap ) {
    auto var = p.first;
    auto lit = p.second;
    auto tmp = literal(lit);
    tmp_list.push_back(tmp);
    auto cedge = _edge(tmp);
    cmap.emplace(var, cedge);
  }
  BddMultiCompOp op{*this, cmap};
  auto e = op.mcomp_op(_edge(edge0));
  return _bdd(e);
}


//////////////////////////////////////////////////////////////////////
// クラス BddMultiCompOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
BddMultiCompOp::BddMultiCompOp(
  BddMgrImpl& mgr,
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
  auto index = node->index();

  DdEdge result;
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
      result = mIteOp.ite_step(cedge, r1, r0);
    }
    mTable.emplace(node, result);
  }
  auto inv = edge.inv();
  return result ^ inv;
}

END_NAMESPACE_YM_DD
