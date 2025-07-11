
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
    std::string filename = argv[i];
    std::ifstream s{filename};
    if ( !s ) {
      std::cerr << filename << ": No such file." << std::endl;
      return 1;
    }

    BddMgr mgr;
    std::string buf;
    SizeType ni = 0;
    SizeType no = 0;
    SizeType nsingle = 0;
    SizeType nall = 0;
    std::vector<Bdd> func_list;
    while ( getline(s, buf) ) {
      TvFunc tvf{buf};
      ni = tvf.input_num();
      auto truth_str = tvf.str();
      auto f = mgr.from_truth(truth_str);
      func_list.push_back(f);
      nsingle += f.size();
    }
    no = func_list.size();
    nall = Bdd::bdd_size(func_list);
    std::cout << basename(argv[i]) << ": #i " << ni << ": #o " << no
	      << ": #n " << nsingle << ": #a " << nall << std::endl;
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
