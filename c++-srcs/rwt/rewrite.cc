
/// @file RwtMgr.cc
/// @brief RwtMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "aig/AigMgrImpl.h"
#include "PatMgr.h"
#include "PatNode.h"
#include "CutMgr.h"
#include "CalcMerit.h"
#include "Pat2Aig.h"
#include "ReplaceDict.h"

#define DEBUG_REWRITE 0
#define VERIFY_REWRITE 0
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

void
lock_dfs(
  AigEdge edge,
  std::unordered_set<SizeType>& lock_mark
)
{
  if ( edge.is_const() ) {
    return;
  }
  auto node = edge.node();
  if ( lock_mark.count(node->id()) > 0 ) {
    return;
  }
  lock_mark.emplace(node->id());
  if ( node->is_and() ) {
    lock_dfs(node->fanin0(), lock_mark);
    lock_dfs(node->fanin1(), lock_mark);
  }
}

END_NONAMESPACE

// @brief パタンの置き換えを行う．
void
AigMgrImpl::rewrite()
{
  PatMgr pat_mgr;
  for ( bool go_on = true; go_on; ) {
    bool changed = false;
    int acc_gain = 0;
    CutMgr cut_mgr(this, 4);
    auto& node_list = and_list();
    // 'ロック'の印
    std::unordered_set<SizeType> lock_mark;
    if ( debug > 0 ) {
      DOUT << "===================================================" << endl;
      print(DOUT);
    }
    // 置き換え結果を記録する辞書
    ReplaceDict replace_dict(this);
    // 入力からのトポロジカル順に処理を行う．
    // 置き換え結果は replace_dict に記録される．
    // トポロジカル順なので置き換えの影響を受けるノードは必ず
    // 後で処理される．
    auto before_num = and_num();
    bool reached = false;
    for ( auto node: node_list ) {
      if ( lock_mark.count(node->id()) > 0 ) {
	continue;
      }
      if ( debug > 1 ) {
	DOUT << "--------------------------------------------------" << endl
	     << "  pick up Node#" << node->id() << endl;
      }
      // 置き換えの結果を反映させる．
      auto fanin0 = replace_dict.get(node->fanin0());
      auto fanin1 = replace_dict.get(node->fanin1());
      AigEdge new_edge;
      if ( _special_case(fanin0, fanin1, new_edge) ) {
	replace_dict.add(node, new_edge);
	++ acc_gain;
	continue;
      }
      if ( fanin0 != node->fanin0() || fanin1 != node->fanin1() ) {
	_change_fanin(node, fanin0, fanin1);
      }
      int max_gain = -1;
      int max_merit = 0;
      int max_cost = 0;
      Cut* max_cut = nullptr;
      Npn4 max_npn;
      PatGraph max_pat;
      // node に対するカットを列挙する．
      auto& cut_list = cut_mgr.enum_cuts(node);
      for ( auto cut: cut_list ) {
	if ( cut->leaf_size() != 4 ) {
	  continue;
	}
	auto tv = cut->calc_tv();
	// カットの内部のノードのうち，削除されるノード数を数える．
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
	    max_gain = gain;
	    max_cut = cut;
	    max_npn = rep_npn;
	    max_pat = pat;
	    max_merit = merit;
	    max_cost = cost;
	  }
	}
      }
      if ( max_gain > 0 ) {
	// max_cut を max_pat で置き換える．
	changed = true;
	acc_gain += max_gain;
	if ( debug > 0 ) {
	  DOUT << "=====================" << endl
	       << "Node#" << node->id() << endl
	       << "max_gain = " << max_gain
	       << " (" << max_merit << " - " << max_cost << ")" << endl
	       << "max_cut:" << endl
	       << *max_cut
	       << "--------------------" << endl
	       << " =>" << endl;
	}
	Pat2Aig pat2aig(this);
	auto new_edge = pat2aig.new_aig(max_cut, max_npn, max_pat);
	replace_dict.add(node, new_edge);
	lock_dfs(new_edge, lock_mark);
	if ( debug > 0 ) {
	  print_new_edge(DOUT, new_edge, max_cut);
	}
	_deactivate(node);
#if VERIFY_REWITE
	_sanity_check();
#endif
      }
      else {
	lock_dfs(node->fanin0(), lock_mark);
	lock_dfs(node->fanin1(), lock_mark);
      }
    }
    if ( changed ) {
      // ハンドルの置き換えを行う．
      for ( auto handle: mHandleHash ) {
	auto old_edge = handle->_edge();
	if ( old_edge.is_const() ) {
	  continue;
	}
	auto new_edge = replace_dict.get(handle->_edge());
	if ( new_edge != handle->_edge() ) {
	  handle->_set_edge(new_edge);
	}
      }
      //sweep();
      if ( debug > 1 ) {
	print(DOUT);
      }
      if ( debug > 0 ) {
	print(DOUT);
	DOUT << "before:   " << before_num << endl
	     << "after:    " << and_num() << endl
	     << "expected: " << before_num - acc_gain << endl;
	if ( and_num() != (before_num - acc_gain) ) {
	  abort();
	}
      }
      break;
    }
    else {
      go_on = false;
    }
  }
}

END_NAMESPACE_YM_AIG
