
/// @file dot_test.cc
/// @brief dot_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include <libgen.h>
#include "ym/Bdd.h"
#include "ym/BddMgr.h"


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
dot_test(
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
    vector<Bdd> func_list;
    while ( getline(s, buf) ) {
      auto f = mgr.from_truth(buf);
      func_list.push_back(f);
    }
    Bdd::gen_dot(cout, func_list);
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
  return YM_NAMESPACE::dot_test(argc, argv);
}
