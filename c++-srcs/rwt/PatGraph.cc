
/// @file PatGraph.cc
/// @brief PatGraph の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "PatGraph.h"
#include "PatNode.h"


BEGIN_NAMESPACE_YM_AIG

BEGIN_NONAMESPACE

PatGraph::Tv4
calc_dfs(
  const PatNode* node,
  std::unordered_map<SizeType, PatGraph::Tv4>& func_dict
)
{
  if ( func_dict.count(node->id()) > 0 ) {
    return func_dict.at(node->id());
  }
  // node は AND ノードのはず．
  auto node0 = node->child0();
  auto pat0 = calc_dfs(node0, func_dict);
  if ( node->inv0() ) {
    pat0 = ~pat0;
  }
  auto node1 = node->child1();
  auto pat1 = calc_dfs(node1, func_dict);
  if ( node->inv1() ) {
    pat1 = ~pat1;
  }
  auto pat = pat0 & pat1;
  func_dict.emplace(node->id(), pat);
  return pat;
}

END_NONAMESPACE

// @brief 関数を計算する．
PatGraph::Tv4
PatGraph::calc_tv() const
{
  // 入力の論理関数のパタン
  static Tv4 pat_list[] = {
    0xAAAA,
    0xCCCC,
    0xF0F0,
    0xFF00
  };
  // ノード番号をキーにして関数のパタンを保持する辞書
  std::unordered_map<SizeType, Tv4> func_dict;
  auto nl = leaf_num();
  for ( SizeType i = 0; i < nl; ++ i ) {
    auto node = leaf(i);
    auto pat = pat_list[i];
    if ( leaf_inv(i) ) {
      pat = ~pat;
    }
    func_dict.emplace(node->id(), pat);
  }
  auto pat = calc_dfs(root(), func_dict);
  if ( root_inv() ) {
    pat = ~pat;
  }
  return pat;
}

PatGraph
PatGraph::xform(
  const Npn4& npn
) const
{
  auto nl = leaf_num();
  std::vector<const PatNode*> xleaves(nl);
  std::vector<bool> xleaves_inv(nl, false);
  for ( SizeType i = 0; i < nl; ++ i ) {
    auto ipos = npn.iperm(i);
    xleaves[ipos] = leaf(i);
    xleaves_inv[ipos] = npn.iinv(i);
  }
  return PatGraph(root(), npn.oinv(), xleaves, xleaves_inv);
}

END_NAMESPACE_YM_AIG
