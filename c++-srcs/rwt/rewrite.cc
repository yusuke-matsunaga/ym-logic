
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


BEGIN_NAMESPACE_YM_AIG

// @brief パタンの置き換えを行う．
void
AigMgrImpl::rewrite()
{
  PatMgr pat_mgr;
  for ( bool go_on = true; go_on; ) {
    CutMgr cut_mgr(4);
    bool changed = false;
    for ( auto node: and_list() ) {
      if ( node->ref_count() == 0 ) {
	// 削除されたノード
	continue;
      }
      int max_gain = -1;
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
	  }
	}
      }
      if ( max_gain > 0 ) {
	cout << " max_gain = " << max_gain << endl;
	// max_cut を max_pat で置き換える．
	changed = true;
	Pat2Aig pat2aig(this);
	auto new_edge = pat2aig.new_aig(max_cut, max_npn, max_pat);
	replace(node, new_edge);
	cut_mgr.erase_cuts(node);
	_sanity_check();
      }
    }
    if ( changed ) {
      sweep();
      cout << and_num() << endl;
    }
    else {
      go_on = false;
    }
  }
}

END_NAMESPACE_YM_AIG
