
/// @file npn_test.cc
/// @brief npn_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.


#include "ym/TvFunc.h"
#include "NpnMgr.h"
#include "ym/StopWatch.h"
#include "ym/RandGen.h"


BEGIN_NAMESPACE_YM_LOGIC

int
npn_test(int argc,
	 char** argv)
{
  ymuint ni = 4;
  ymuint nf = 10000;
  ymuint tvcount = 0;
  ymuint tvmax = 0;
  ymuint redmax = 0;

  if ( argc != 2 && argc != 3 ) {
    cerr << "USAGE: npn_test <# of inputs> [<# of functions>]" << endl;
    return -1;
  }

  ni = atoi(argv[1]);
  if ( argc == 3 ) {
    nf = atoi(argv[2]);
  }

  StopWatch timer;
  RandGen rg;
  timer.start();
  ymuint ni_exp = 1U << ni;
  if ( ni <= 4 ) {
    // 全探索
    ymuint ni_exp_exp = 1U << ni_exp;
    nf = ni_exp_exp;
    for (ymuint p = 0; p < ni_exp_exp; ++ p) {
      vector<int> values(ni_exp);
      for (ymuint b = 0; b < ni_exp; ++ b) {
	if ( p & (1U << b) ) {
	  values[b] = 1;
	}
	else {
	  values[b] = 0;
	}
      }
      TvFunc f(ni, values);
      NpnMgr npnmgr;
      TvFunc cf = npnmgr.cannonical(f);
      ymuint tv = npnmgr.tvmax_count();
      vector<NpnMap> cmap_list;
      npnmgr.all_cmap(cmap_list);
      tvcount += tv;
      ymuint red_num = tv > 0 ? tv - cmap_list.size() : 0;
      if ( tvmax < tv ) {
	tvmax = tv;
      }
      if ( redmax < red_num ) {
	redmax = red_num;
      }
    }
  }
  else {
    // ランダムサンプリング
    for (ymuint i = 0; i < nf; ++ i) {
      vector<int> values(ni_exp);
      for (ymuint b = 0; b < ni_exp; ++ b) {
	if ( rg.int32() % 2 ) {
	  values[b] = 1;
	}
	else {
	  values[b] = 0;
	}
      }
      TvFunc f(ni, values);
      NpnMgr npnmgr;
      TvFunc cf = npnmgr.cannonical(f);
      ymuint tv = npnmgr.tvmax_count();
      vector<NpnMap> cmap_list;
      npnmgr.all_cmap(cmap_list);
      tvcount += tv;
      ymuint red_num = tv > 0 ? tv - cmap_list.size() : 0;
      if ( tvmax < tv ) {
	tvmax = tv;
      }
      if ( redmax < red_num ) {
	redmax = red_num;
      }
    }
  }

  timer.stop();
  USTime time = timer.time();
  cout << "# of inputs:     " << ni << endl
       << "# of functions:  " << nf << " functions" << endl
       << "CPU time:        " << time << endl
       << "Total recursion: " << tvcount << endl
       << "Max recursion:   " << tvmax << endl
       << "Max redundant:   " << redmax << endl;
  return 0;
}

END_NAMESPACE_YM_LOGIC

int
main(int argc,
     char** argv)
{
  return nsYm::nsLogic::npn_test(argc, argv);
}
