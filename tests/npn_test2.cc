
/// @file npn_test2.cc
/// @brief npn_test2 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.


#include "ym/TvFunc.h"
#include "NpnMgr.h"
#include "ym/StopWatch.h"


BEGIN_NAMESPACE_YM_LOGIC

int
npn_test2(int argc,
	  char** argv)
{
  if ( argc != 2 ) {
    cerr << "USAGE: npn_test2 <filename>" << endl;
    return -1;
  }

  string filename = argv[1];
  ifstream fs(filename);
  if ( !fs ) {
    cerr << filename << ": no such file." << endl;
    return -1;
  }

  StopWatch timer;

  ymuint tvcount = 0;
  ymuint tvmax = 0;
  ymuint redmax = 0;

  string line;
  while ( getline(fs, line) ) {
    string ni_str;
    string func_str;
    bool head = true;
    for (ymuint i = 0; i < line.size(); ++ i) {
      char c = line[i];
      if ( head ) {
	if ( c == ':' ) {
	  head = false;
	}
	else {
	  ni_str += c;
	}
      }
      else {
	func_str += c;
      }
    }
    ymuint ni = atoi(ni_str.c_str());
    ymuint ni_exp = 1U << ni;
    if ( func_str.size() != ni_exp ) {
      cerr << "illegal func_str: " << func_str << endl
	   << "expected size = " << ni_exp << endl;
      continue;
    }
    vector<int> values(ni_exp);
    for (ymuint i = 0; i < ni_exp; ++ i) {
      if ( func_str[i] == '0' ) {
	values[i] = 0;
      }
      else {
	values[i] = 1;
      }
    }
    TvFunc f(ni, values);
    cout << "F = " << f << endl;

    timer.start();
    NpnMgr npnmgr;
    TvFunc cf = npnmgr.cannonical(f);
    vector<NpnMap> cmap_list;
    npnmgr.all_cmap(cmap_list);
    timer.stop();
    cout << "cmap_list size: " << cmap_list.size() << endl;
    ymuint tv = npnmgr.tvmax_count();
    ymuint red_num = tv > 0 ? tv - cmap_list.size() : 0;
    if ( tvmax < tv ) {
      tvmax = tv;
    }
    if ( redmax < red_num ) {
      redmax = red_num;
    }
  }

  USTime time = timer.time();
  cout << "CPU time:        " << time << endl
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
  return nsYm::nsLogic::npn_test2(argc, argv);
}
