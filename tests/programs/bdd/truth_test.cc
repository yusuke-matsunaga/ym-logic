
/// @file truth_test.cc
/// @brief truth_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

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
    vector<BddVar> var_list;
    string buf;
    while ( getline(s, buf) ) {
      SizeType n = buf.size();
      SizeType ni = log2(n);
      ASSERT_COND( (1 << ni) == n );
      while ( var_list.size() < ni ) {
	var_list.push_back(mgr.new_variable());
      }
      auto f = mgr.from_truth(buf, var_list);
      f.display(cout);
    }
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
