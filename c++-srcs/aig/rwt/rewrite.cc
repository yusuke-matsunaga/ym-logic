
/// @file RwtMgr.cc
/// @brief RwtMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "../AigMgrImpl.h"
#include "../ReplaceMgr.h"
#include "PatMgr.h"
#include "PatNode.h"
#include "CutMgr.h"
#include "CalcMerit.h"
#include "Pat2Aig.h"

#define DEBUG_REWRITE 0
#define DOUT std::cout


BEGIN_NAMESPACE_YM_AIG

BEGIN_NONAMESPACE

static
int debug = DEBUG_REWRITE;

void
print_dfs(
  std::ostream& s,
  AigNode* node,
  std::unordered_set<SizeType>& mark
)
{
  if ( node == nullptr ) {
    return;
  }
  if ( mark.count(node->id()) > 0 ) {
    return;
  }
  mark.emplace(node->id());
  node->print(s);
  if ( node->is_and() ) {
    print_dfs(s, node->fanin0_node(), mark);
    print_dfs(s, node->fanin1_node(), mark);
  }
}

void
print_new_edge(
  std::ostream& s,
  AigEdge root,
  const Cut* cut
)
{
  std::unordered_set<SizeType> mark;
  s << "Root: " << root << endl;
  for ( auto node: cut->leaf_list() ) {
    mark.emplace(node->id());
  }
  print_dfs(s, root.node(), mark);
}

END_NONAMESPACE

// @brief パタンの置き換えを行う．
void
AigMgrImpl::rewrite()
{
  SizeType i = 0;
  if ( debug > 1 ) {
    print(DOUT);
  }
  PatMgr pat_mgr;
  for ( bool go_on = true; go_on; ) {
    if ( debug > 0 ) {
      DOUT << "===================================================" << endl;
    }
    bool changed = false;
    int acc_gain = 0;
    CutMgr cut_mgr(this, 4);
    ReplaceMgr rep_mgr(this);
    // 入力からのトポロジカル順に処理を行う．
    auto node_list = and_list();
    auto before_num = and_num();
    for ( auto node: node_list ) {
      if ( debug > 2 ) {
	DOUT << "--------------------------------------------------" << endl
	     << "  pick up Node#" << node->id() << endl;
      }
      // node に対する置き換えパタンを求める．
      int max_gain = -1;
      int max_merit;
      int max_cost;
      Cut* max_cut = nullptr;
      Npn4 max_npn;
      PatGraph max_pat;
      // node に対するカットを列挙する．
      auto& cut_list = cut_mgr.enum_cuts(node);
      for ( auto cut: cut_list ) {
	if ( cut->leaf_size() != 4 ) {
	  continue;
	}
	{ // abc のヒューリスティック
	  // 葉のノードうちファンアウト数が1のものが
	  // 3つ以上あったらスキップする．
	  SizeType count = 0;
	  for ( auto leaf: cut->leaf_list() ) {
	    if ( leaf->ref_count() == 1 ) {
	      ++ count;
	    }
	  }
	  if ( count > 2 ) {
	    continue;
	  }
	}
	{
	  if ( !cut->check() ) {
	    cut->print(cout);
	    abort();
	  }
	}
	auto tv = cut->calc_tv();
	// カットの内部のノードのうち，削除されるノード数を数える．
	// calc_merit に削除されるノードの印が残る．
	CalcMerit calc_merit(cut, tv);
	int merit = calc_merit.merit();
	// カットの関数にマッチするパタンを列挙する．
	Npn4 rep_npn;
	auto& pat_list = pat_mgr.get_pat(tv, rep_npn);
	for ( auto& pat: pat_list ) {
	  // cut を pat で置き換えた時の得失を計算する．
	  Pat2Aig pat2aig(this);
	  auto cost = pat2aig.calc_cost(cut, rep_npn, pat, calc_merit);
	  auto gain = merit - cost;
	  if ( max_gain < gain ) {
	    // 最大ゲインとなるパタンを記録しておく．
	    max_gain = gain;
	    max_merit = merit;
	    max_cost = cost;
	    max_cut = cut;
	    max_npn = rep_npn;
	    max_pat = pat;
	  }
	}
      }
      if ( max_gain > 0 ) {
	// max_cut を max_pat で置き換える．
	changed = true;
	acc_gain += max_gain;
	if ( debug > 1 ) {
	  DOUT << "=====================" << endl
	       << "Node#" << node->id() << endl
	       << "max_gain = " << max_gain
	       << " ( " << max_merit
	       << " - " << max_cost << " )"
	       << endl
	       << "max_cut:" << endl
	       << *max_cut
	       << "--------------------" << endl
	       << " =>" << endl;
	}
	Pat2Aig pat2aig(this);
	auto new_edge = pat2aig.new_aig(max_cut, max_npn, max_pat);
	if ( debug > 1 ) {
	  print_new_edge(DOUT, new_edge, max_cut);
	}
	// node を new_edge で置き換える．
	cut_mgr.clear_cuts(node, rep_mgr);
	_replace(node, new_edge);
      }
    }
    if ( changed ) {
      if ( debug > 1 ) {
	print(DOUT);
      }
      if ( debug > 0 ) {
	DOUT << "before:   " << before_num << endl
	     << "after:    " << and_num() << endl;
      }
      if ( and_num() > (before_num - acc_gain) ) {
	DOUT << "expected: " << before_num - acc_gain << endl;
	abort();
      }
    }
    else {
      go_on = false;
    }
  }
  sweep();
}

END_NAMESPACE_YM_AIG
