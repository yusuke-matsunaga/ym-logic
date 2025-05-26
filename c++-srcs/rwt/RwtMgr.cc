
/// @file RwtMgr.cc
/// @brief RwtMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "RwtMgr.h"
#include "PatMgr.h"
#include "PatNode.h"
#include "CutMgr.h"
#include "aig/AigMgrImpl.h"


BEGIN_NAMESPACE_YM_AIG

BEGIN_NONAMESPACE

int
calc_merit_dfs(
  AigNode* node,
  std::unordered_map<SizeType, int>& count_dict
)
{
  if ( count_dict.count(node->id()) == 0 ) {
    count_dict.emplace(node->id(), 0);
  }
  auto& count = count_dict.at(node->id());
  ++ count;
  int ans = 0;
  if ( count == node->ref_count() ) {
    if ( node->is_and() ) {
      ++ ans;
      ans += calc_merit_dfs(node->fanin0_node(), count_dict);
      ans += calc_merit_dfs(node->fanin1_node(), count_dict);
    }
  }
  return ans;
}

// 削除されるノード数を数える．
int
calc_merit(
  const Cut* cut,
  Cut::Tv4Type tv
)
{
  // 大まかには cut の葉になっているノードよりも根のノード側にある
  // MFFC(Maximal Fanout Free Cone)のサイズを求めればよい．
  // めんどくさいのは見かけ上は葉のノードが実際には使われていない
  // 場合があるということ．
  // 例えば a & a' という式に対応するノードがあったら，実際には
  // 定数0なので a のノードは不要となる．

  auto root = cut->root();
  if ( !root->is_and() ) {
    return 0;
  }

  // ノード番号をキーにしてカウントを保持する辞書
  std::unordered_map<SizeType, int> count_dict;

  // サポートになっている葉のノードに印を付ける．
  auto sup = Npn4::get_support(tv);
  for ( SizeType i = 0; i < cut->leaf_size(); ++ i ) {
    if ( sup & (1 << i) ) {
      auto node = cut->leaf(i);
      count_dict.emplace(node->id(), -1);
    }
  }
  int ans = 1;
  ans += calc_merit_dfs(root->fanin0_node(), count_dict);
  ans += calc_merit_dfs(root->fanin1_node(), count_dict);
  return ans;
}

int
calc_cost_dfs(
  const PatNode* node,
  AigMgrImpl* aig_mgr,
  const Cut* cut,
  const Npn4& npn,
  std::unordered_map<SizeType, AigEdge>& node_dict
)
{
  auto id = node->id();
  if ( id == 0 ) {
    // 定数ノードはありえない
    throw std::invalid_argument{"constant node in pat graph"};
  }
  if ( id < 4 ) {
    // 葉のノード
    auto iinv = npn.iinv(id);
    auto iperm = npn.iperm(id);
    auto aig_node = cut->leaf(iperm);
    auto aig_edge = AigEdge(aig_node, iinv);
    node_dict.emplace(id, aig_edge);
    return 0;
  }
  int ans = 0;
  auto node0 = node->child0();
  auto aig_edge0 = AigEdge::zero();
  if ( node_dict.count(node0->id()) == 0 ) {
    ans += calc_cost_dfs(node0, aig_mgr, cut, npn, node_dict);
  }
  else {
    aig_edge0 = node_dict.at(node0->id());
  }
  auto node1 = node->child1();
  auto aig_edge1 = AigEdge::zero();
  if ( node_dict.count(node1->id()) == 0 ) {
    ans += calc_cost_dfs(node1, aig_mgr, cut, npn, node_dict);
  }
  else {
    aig_edge1 = node_dict.at(node1->id());
  }
  if ( node->is_and() ) {
    if ( !aig_edge0.is_zero() && !aig_edge1.is_zero() ) {
      auto aig_node = aig_mgr->find_and(aig_edge0, aig_edge1);
      if ( aig_node != nullptr ) {
	auto aig_edge = AigEdge(aig_node, false);
	node_dict.emplace(id, aig_edge);
	return 0;
      }
    }
    node_dict.emplace(id, AigEdge::zero());
    ans += 1;
  }
  else {
    // XOR ノード
    node_dict.emplace(id, AigEdge::zero());
    ans += 3;
  }
  return ans;
}

// pat で新規に追加されるノード数を数える．
int
calc_cost(
  const Cut* cut,
  AigMgrImpl* aig_mgr,
  const Npn4& rep_npn,
  const PatGraph& pat
)
{
  auto pat_root = pat.root;
  if ( pat_root->id() == 0 ) {
    // 定数ノードならコストは0
    return 0;
  }

  // pat を cut の関数に変換するためのNPN変換
  auto npn1 = pat.npn * rep_npn;
  // PatNode のノード番号をキーにして対応する AigEdge を保持する辞書
  // 対応する AigEdge がない場合には AigEdge::zero() を用いる．
  std::unordered_map<SizeType, AigEdge> node_dict;
  return calc_cost_dfs(pat_root, aig_mgr, cut, npn1, node_dict);
}

END_NONAMESPACE

// @brief パタンの置き換えを行う．
void
RwtMgr::rewrite(
  AigMgrImpl* mgr ///< [in] AIGマネージャ
)
{
  PatMgr pat_mgr;
  for ( bool go_on = true; go_on; ) {
    CutMgr cut_mgr(4);
    auto node_list = mgr->and_list();
    bool changed = false;
    for ( auto node: node_list ) {
      int max_gain = -1;
      Cut* max_cut = nullptr;
      PatGraph max_pat;
      // node に対するカットを列挙する．
      auto& cut_list = cut_mgr.enum_cuts(node);
      for ( auto cut: cut_list ) {
	auto tv = cut->calc_tv();
	// カットの内部のノードのうち，削除されるノード数を数える．
	auto merit = calc_merit(cut, tv);
	// カットの関数にマッチするパタンを列挙する．
	Npn4 rep_npn;
	auto& pat_list = pat_mgr.get_pat(tv, rep_npn);
	for ( auto& pat: pat_list ) {
	  // cut を pat で置き換えた時の得失を計算する．
	  auto cost = calc_cost(cut, mgr, rep_npn, pat);
	  auto gain = merit - cost;
	  if ( max_gain < gain ) {
	    max_gain = gain;
	    max_cut = cut;
	    max_pat = pat;
	  }
	}
      }
      if ( max_gain > 0 ) {
	// max_cut を max_pat で置き換える．
	changed = true;
      }
    }
    if ( changed ) {
      mgr->sweep();
    }
    else {
      go_on = false;
    }
  }
}

END_NAMESPACE_YM_AIG
