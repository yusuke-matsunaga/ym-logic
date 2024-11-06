
/// @file truth_test.cc
/// @brief truth_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include <libgen.h>
#include "ym/Bdd.h"
#include "ym/BddMgr.h"
#include "ym/TvFunc.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

SizeType
log2(
  SizeType x
)
{
  SizeType y = 0;
  while ( (1 << y) < x ) {
    ++ y;
  }
  return y;
}

END_NONAMESPACE

int
truth_test(
  int argc,
  char** argv
)
{
  for ( int i = 1; i < argc; ++ i ) {
    string filename = argv[i];
    ifstream s{filename};
    if ( !s ) {
      cerr << filename << ": No such file." << endl;
      return 1;
    }

    BddMgr mgr;
    string buf;
    SizeType ni = 0;
    SizeType no = 0;
    SizeType nsingle = 0;
    SizeType nall = 0;
    vector<Bdd> func_list;
    vector<BddVar> var_list;
    while ( getline(s, buf) ) {
      TvFunc tvf{buf};
      ni = tvf.input_num();
      auto truth_str = tvf.str();
      auto f = mgr.from_truth(var_list, truth_str);
      func_list.push_back(f);
      nsingle += f.size();
    }
    no = func_list.size();
    nall = mgr.bdd_size(func_list);
    cout << basename(argv[i]) << ": #i " << ni << ": #o " << no
	 << ": #n " << nsingle << ": #a " << nall << endl;
  }
  return 0;
}

END_NAMESPACE_YM

int
main(
  int argc,
  char** argv
)
{
  return YM_NAMESPACE::truth_test(argc, argv);
}
