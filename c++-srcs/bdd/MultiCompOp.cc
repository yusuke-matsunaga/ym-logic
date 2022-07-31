
/// @file MultiCompOp.cc
/// @brief MultiCompOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "MultiCompOp.h"


BEGIN_NAMESPACE_YM_BDD

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

  while ( mCompList[pos].first < index ) {
    ++ pos;
    if ( pos == mCompList.size() ) {
      return edge;
    }
  }
  auto cindex = mCompList[pos].first;

  BddEdge result;
  if ( mTable.count(node) > 0 ) {
    result = mTable.at(node);
  }
  else {
    auto edge0 = node->edge0();
    auto edge1 = node->edge1();
    if ( index < cindex ) {
      auto r0 = mcomp_step(edge0, pos);
      auto r1 = mcomp_step(edge1, pos);
      result = new_node(index, r0, r1);
    }
    else if ( index == cindex ) {
      auto r0 = mcomp_step(edge0, pos + 1);
      auto r1 = mcomp_step(edge1, pos + 1);
      auto cedge = mCompList[pos].second;
      result = mgr()->ite_op(cedge, r0, r1);
    }
    else { // index > cindex
      // 上の while 分の条件なのでここに来ることはない．
      ASSERT_NOT_REACHED;
    }
    mTable.emplace(node, result);
  }
  auto inv = edge.inv();
  return result ^ inv;
}

END_NAMESPACE_YM_BDD
