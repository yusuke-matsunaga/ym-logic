
/// @file Pat2Aig.cc
/// @brief Pat2Aig の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "Pat2Aig.h"
#include "CutMgr.h"
#include "PatMgr.h"
#include "PatNode.h"
#include "Npn4.h"
#include "CalcMerit.h"
#include "../AigMgrImpl.h"


BEGIN_NAMESPACE_YM_AIG

bool
Pat2Aig::debug = false;

// @brief PatGraph を AIG に変換する．
AigEdge
Pat2Aig::new_aig(
  const Cut* cut,
  const PatGraph& pat
)
{
  AigEdge aig_edge;
  auto npn = pat.npn();
  auto pat_root = pat.root();
  if ( pat_root->id() == 0 ) {
    // 定数ノード
    aig_edge = AigEdge::zero();
  }
  else {
    mNodeDict.clear();
    // pat の葉のノードと cut の葉のノードの対応を取る．
    for ( SizeType i = 0; i < 4; ++ i ) {
      auto ipos = npn.iperm(i);
      if ( ipos < cut->leaf_size() ) {
	// 対応する cut の葉ノードを取ってくる．
	auto leaf_node = cut->leaf(ipos);
	auto edge = AigEdge(leaf_node, npn.iinv(i));
	// i 番目の入力のノード番号は i + 1
	auto id = i + 1;
	mNodeDict.emplace(id, edge);
      }
    }
    aig_edge = aig_sub(pat_root);
  }
  if ( npn.oinv() ) {
    aig_edge = ~aig_edge;
  }
  return aig_edge;
}

AigEdge
Pat2Aig::aig_sub(
  const PatNode* node
)
{
  if ( debug ) {
    cout << "aig_sub(Node#" << node->id() << ")" << endl;
  }
  if ( mNodeDict.count(node->id()) > 0 ) {
    if ( debug ) {
      cout << "  -> aig_sub(Node#" << node->id() << ")"
	   << " = " << mNodeDict.at(node->id()) << "[*]" << endl;
    }
    return mNodeDict.at(node->id());
  }

  auto node0 = node->child0();
  auto aig_edge0 = aig_sub(node0);
  if ( node->inv0() ) {
    aig_edge0 = ~aig_edge0;
  }

  auto node1 = node->child1();
  auto aig_edge1 = aig_sub(node1);
  if ( node->inv1() ) {
    aig_edge1 = ~aig_edge1;
  }

  auto aig_edge = mAigMgr->and_op(aig_edge0, aig_edge1);
  mNodeDict.emplace(node->id(), aig_edge);
  if ( debug ) {
    cout << "  -> aig_sub(Node#" << node->id() << ")"
	 << "  -> " << aig_edge0 << " & " << aig_edge1 << " = " << aig_edge << endl;
  }
  return aig_edge;
}

// pat で新規に追加されるノード数を数える．
int
Pat2Aig::calc_cost(
  const Cut* cut,
  const PatGraph& pat,
  const CalcMerit& calc_merit
)
{
  auto pat_root = pat.root();
  if ( pat_root->id() <= 4 ) {
    // 定数ノードか入力ノードならコストは0
    return 0;
  }

  mNodeDict.clear();
  mCount = 0;
  // pat の葉のノードと cut の葉のノードの対応を取る．
  auto npn = pat.npn();
  for ( SizeType i = 0; i < 4; ++ i ) {
    auto ipos = npn.iperm(i);
    if ( ipos < cut->leaf_size() ) {
      // 対応する cut の葉ノードを取ってくる．
      auto leaf_node = cut->leaf(ipos);
      auto edge = AigEdge(leaf_node, npn.iinv(i));
      // i 番目の入力のノード番号は i + 1
      auto id = i + 1;
      mNodeDict.emplace(id, edge);
    }
  }
  calc_sub(pat_root, calc_merit);
  return mCount;
}

AigEdge
Pat2Aig::calc_sub(
  const PatNode* node,
  const CalcMerit& calc_merit
)
{
  if ( debug ) {
    cout << "calc_sub(Node#" << node->id() << ")" << endl;
  }
  auto id = node->id();
  if ( mNodeDict.count(id) > 0 ) {
    if ( debug ) {
      cout << "  -> " << mNodeDict.at(id) << endl;
    }
    return mNodeDict.at(id);
  }
  if ( id == 0 ) {
    // 定数ノードはありえない
    throw std::invalid_argument{"constant node in pat graph"};
  }
  if ( id <= 4 ) {
    // 葉のノードはノーカウント
    return AigEdge::zero();
  }

  auto node0 = node->child0();
  auto aig_edge0 = calc_sub(node0, calc_merit);
  if ( node->inv0() ) {
    aig_edge0 = ~aig_edge0;
  }

  auto node1 = node->child1();
  auto aig_edge1 = calc_sub(node1, calc_merit);
  if ( node->inv1() ) {
    aig_edge1 = ~aig_edge1;
  }

  if ( !aig_edge0.is_const() && !aig_edge1.is_const() ) {
    auto aig_node = mAigMgr->find_and(aig_edge0, aig_edge1);
    // 同じ構造のノードが既に存在しているか調べる．
    if ( aig_node != nullptr ) {
      // ただし存在しても削除されるノードなら無視する．
      if ( !calc_merit.check(aig_node) && aig_node->ref_count() > 0 ) {
	auto aig_edge = AigEdge(aig_node, false);
	mNodeDict.emplace(id, aig_edge);
	if ( debug ) {
	  cout << "  -> " << aig_edge << "[*]" << endl;
	}
	return aig_edge;
      }
    }
  }
  ++ mCount;
  mNodeDict.emplace(id, AigEdge::zero());
  if ( debug ) {
    cout << "  -> zero" << endl;
  }
  return AigEdge::zero();
}

END_NAMESPACE_YM_AIG
