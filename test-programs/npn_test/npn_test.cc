
/// @file npn_test.cc
/// @brief npn_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.

#include "ym/TvFunc.h"
#include "npn/NpnMgr.h"
#include "ym/Range.h"
#include "ym/Timer.h"
#include <random>


BEGIN_NAMESPACE_YM_LOGIC

int
npn_test(
  int argc,
  char** argv
)
{
  int ni = 4;
  int nf = 10000;
  int tvcount = 0;
  int tvmax = 0;
  int redmax = 0;

  if ( argc != 2 && argc != 3 ) {
    std::cerr << "USAGE: npn_test <# of inputs> [<# of functions>]" << std::endl;
    return -1;
  }

  ni = atoi(argv[1]);
  if ( argc == 3 ) {
    nf = atoi(argv[2]);
  }

  Timer timer;
  std::mt19937 rg;
  timer.start();
  int ni_exp = 1U << ni;
  if ( ni <= 4 ) {
    // 全探索
    int ni_exp_exp = 1U << ni_exp;
    nf = ni_exp_exp;
    for ( int p: Range(ni_exp_exp) ) {
      std::vector<int> values(ni_exp);
      for ( int b: Range(ni_exp) ) {
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
      auto tv = npnmgr.tvmax_count();
      std::vector<NpnMap> cmap_list;
      npnmgr.all_cmap(cmap_list);
      tvcount += tv;
      auto red_num = tv > 0 ? tv - cmap_list.size() : 0;
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
    std::uniform_int_distribution<int> rd2(0, 1);
    for ( int i: Range(nf) ) {
      std::vector<int> values(ni_exp);
      for ( int b: Range(ni_exp) ) {
	if ( rd2(rg) ) {
	  values[b] = 1;
	}
	else {
	  values[b] = 0;
	}
      }
      TvFunc f(ni, values);
      NpnMgr npnmgr;
      TvFunc cf = npnmgr.cannonical(f);
      auto tv = npnmgr.tvmax_count();
      std::vector<NpnMap> cmap_list;
      npnmgr.all_cmap(cmap_list);
      tvcount += tv;
      auto red_num = tv > 0 ? tv - cmap_list.size() : 0;
      if ( tvmax < tv ) {
	tvmax = tv;
      }
      if ( redmax < red_num ) {
	redmax = red_num;
      }
    }
  }

  timer.stop();
  auto time = timer.get_time();
  std::cout << "# of inputs:     " << ni << std::endl
	    << "# of functions:  " << nf << " functions" << std::endl
	    << "CPU time:        " << time << std::endl
	    << "Total recursion: " << tvcount << std::endl
	    << "Max recursion:   " << tvmax << std::endl
	    << "Max redundant:   " << redmax << std::endl;
  return 0;
}

END_NAMESPACE_YM_LOGIC

int
main(
  int argc,
  char** argv
)
{
  return nsYm::nsLogic::npn_test(argc, argv);
}
