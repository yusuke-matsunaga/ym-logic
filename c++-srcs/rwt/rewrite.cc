
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
#include "CalcMerit.h"
#include "CalcCost.h"
#include "aig/AigMgrImpl.h"


BEGIN_NAMESPACE_YM_AIG

// 削除されるノード数を数える．
int
CalcMerit::calc(
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

  // サポートになっている葉のノードに印を付ける．
  auto sup = Npn4::get_support(tv);
  for ( SizeType i = 0; i < cut->leaf_size(); ++ i ) {
    if ( sup & (1 << i) ) {
      auto node = cut->leaf(i);
      mCountDict.emplace(node->id(), -1);
    }
  }
  int ans = 1;
  ans += calc_sub(root->fanin0_node());
  ans += calc_sub(root->fanin1_node());
  return ans;
}

int
CalcMerit::calc_sub(
  AigNode* node
)
{
  if ( mCountDict.count(node->id()) == 0 ) {
    mCountDict.emplace(node->id(), 0);
  }
  auto& count = mCountDict.at(node->id());
  ++ count;
  int ans = 0;
  if ( count == node->ref_count() ) {
    if ( node->is_and() ) {
      ++ ans;
      ans += calc_sub(node->fanin0_node());
      ans += calc_sub(node->fanin1_node());
    }
  }
  return ans;
}

// pat で新規に追加されるノード数を数える．
int
CalcCost::calc(
  const Cut* cut,
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
  return calc_sub(pat_root, cut, npn1);
}

int
CalcCost::calc_sub(
  const PatNode* node,
  const Cut* cut,
  const Npn4& npn
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
    mNodeDict.emplace(id, aig_edge);
    return 0;
  }
  int ans = 0;
  auto node0 = node->child0();
  auto aig_edge0 = AigEdge::zero();
  if ( mNodeDict.count(node0->id()) == 0 ) {
    ans += calc_sub(node0, cut, npn);
  }
  else {
    aig_edge0 = mNodeDict.at(node0->id());
  }
  auto node1 = node->child1();
  auto aig_edge1 = AigEdge::zero();
  if ( mNodeDict.count(node1->id()) == 0 ) {
    ans += calc_sub(node1, cut, npn);
  }
  else {
    aig_edge1 = mNodeDict.at(node1->id());
  }
  if ( node->is_and() ) {
    if ( !aig_edge0.is_zero() && !aig_edge1.is_zero() ) {
      auto aig_node = mAigMgr->find_and(aig_edge0, aig_edge1);
      if ( aig_node != nullptr ) {
	auto aig_edge = AigEdge(aig_node, false);
	mNodeDict.emplace(id, aig_edge);
	return 0;
      }
    }
    mNodeDict.emplace(id, AigEdge::zero());
    ans += 1;
  }
  else {
    // XOR ノード
    mNodeDict.emplace(id, AigEdge::zero());
    ans += 3;
  }
  return ans;
}

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
	CalcMerit calc_merit;
	auto merit = calc_merit.calc(cut, tv);
	// カットの関数にマッチするパタンを列挙する．
	Npn4 rep_npn;
	auto& pat_list = pat_mgr.get_pat(tv, rep_npn);
	for ( auto& pat: pat_list ) {
	  // cut を pat で置き換えた時の得失を計算する．
	  CalcCost calc_cost(mgr);
	  auto cost = calc_cost.calc(cut, rep_npn, pat);
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
