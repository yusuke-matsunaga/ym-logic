
/// @file npn_test2.cc
/// @brief npn_test2 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.

#include "ym/TvFunc.h"
#include "npn/NpnMgr.h"
#include "ym/Timer.h"


BEGIN_NAMESPACE_YM_LOGIC

int
npn_test2(
  int argc,
  char** argv
)
{
  if ( argc != 2 ) {
    std::cerr << "USAGE: npn_test2 <filename>" << std::endl;
    return -1;
  }

  std::string filename = argv[1];
  std::ifstream fs(filename);
  if ( !fs ) {
    std::cerr << filename << ": no such file." << std::endl;
    return -1;
  }

  Timer timer;

  SizeType tvcount = 0;
  SizeType tvmax = 0;
  SizeType redmax = 0;

  std::string line;
  while ( getline(fs, line) ) {
    std::string ni_str;
    std::string func_str;
    bool head = true;
    for ( int i = 0; i < line.size(); ++ i) {
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
    int ni = atoi(ni_str.c_str());
    SizeType ni_exp = 1U << ni;
    if ( func_str.size() != ni_exp ) {
      std::cerr << "illegal func_str: " << func_str << std::endl
		<< "expected size = " << ni_exp << std::endl;
      continue;
    }
    std::vector<int> values(ni_exp);
    for (int i = 0; i < ni_exp; ++ i) {
      if ( func_str[i] == '0' ) {
	values[i] = 0;
      }
      else {
	values[i] = 1;
      }
    }
    TvFunc f(ni, values);
    std::cout << "F = " << f << std::endl;

    timer.start();
    NpnMgr npnmgr;
    TvFunc cf = npnmgr.cannonical(f);
    std::vector<NpnMap> cmap_list;
    npnmgr.all_cmap(cmap_list);
    timer.stop();
    std::cout << "cmap_list size: " << cmap_list.size() << std::endl;
    auto tv = npnmgr.tvmax_count();
    auto red_num = tv > 0 ? tv - cmap_list.size() : 0;
    if ( tvmax < tv ) {
      tvmax = tv;
    }
    if ( redmax < red_num ) {
      redmax = red_num;
    }
  }

  auto time = timer.get_time();
  std::cout << "CPU time:        " << time << std::endl
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
  return nsYm::nsLogic::npn_test2(argc, argv);
}
