
/// @file RwtMgr.cc
/// @brief RwtMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "RwtMgr.h"
#include "PatMgr.h"
#include "CutMgr.h"


BEGIN_NAMESPACE_YM_AIG

BEGIN_NONAMESPACE

// カットの葉に対応するノードのリストを作る．
std::vector<AigNode*>
get_leaf_list(
  const Cut* cut,
  const PatNode* pat_root
)
{
  std::vector<AigNode*> leaf_list;
  leaf_list.reserve(4);
  if ( pat_root->input0() ) {
    leaf_list.push_back(cut->leaf(0));
  }
  if ( pat_root->input1() ) {
    leaf_list.push_back(cut->leaf(1));
  }
  if ( pat_root->input2() ) {
    leaf_list.push_back(cut->leaf(2));
  }
  if ( pat_root->input3() ) {
    leaf_list.push_back(cut->leaf(3));
  }
  return leaf_list;
}

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
      ans += calc_merit_dfs(node->fsnin1_node(), count_dict);
    }
  }
  return ans;
}

// 削除されるノード数を数える．
int
calc_merit(
  const Cut* cut,
  const std::vector<AigNode*>& leaf_list
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
  // 葉のノードは削除できない
  for ( auto node: leaf_list ) {
    count_dict.emplace(node, -1);
  }
  int ans = 1;
  ans += calc_merit_dfs(root->fanin0_node(), count_dict);
  ans += calc_merit_dfs(root->fanin1_node(), count_dict);
  return ans;
}

// 追加されるノード数を数える．
int
calc_cost(
  const PatNode* pat_root,
  const Npn4& npn,
  const std::vector<AigNode*>& leaf_list
)
{
  // ノード番号をキーにして対応する AigEdge を保持する辞書
  std::unordered_set<SizeType, AigEdge> node_dict;
}

// cut を pat で置き換えた時のゲインを計算する．
int
calc_gain(
  const Cut* cut,
  const PatGraph& pat
)
{
  auto pat_root = pat.root;
  auto leaf_list = get_leaf_list(cut, pat_root);
  // cut のうち，削除されるノード数を数える．
  auto merit = calc_merit(cut, leaf_list);
  // pat で新規に追加されるノード数を数える．
  auto npn1 = pat.npn * rep_npn;
  auto cost = calc_cost(pat_root, npn1, leaf_list);
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
    CutMgr cut_mgr;
    auto node_list = mgr->and_list();
    bool changed = false;
    for ( auto node: node_list ) {
      int max_gain = -1;
      Cut* max_cut = nullptr;
      PatGraph max_pat;
      // node に対するカットを列挙する．
      auto& cut_list = cut_mgr.enum_cuts(node);
      for ( auto cut: cut_list ) {
	// カットの関数にマッチするパタンを列挙する．
	auto tv = cut->calc_tv(node);
	Npn4 rep_npn;
	auto& pat_list = cut_mgr.get_pat(tv, rep_npn);
	for ( auto& pat: pat_list ) {
	  // cut を pat で置き換えた時の得失を計算する．
	  int gain = calc_gain(cut, pat);
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
