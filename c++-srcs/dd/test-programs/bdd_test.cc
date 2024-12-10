
/// @file simple_test.cc
/// @brief simple_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Bdd.h"
#include "ym/BddMgr.h"


BEGIN_NAMESPACE_YM

int
simple_test(
  int argc,
  char** argv
)
{
  BddMgr mgr;

  //auto bdd = mgr.from_truth("10010110");
  cout << "_A" << endl;
  auto bdd = mgr.variable(0);
  cout << "A" << endl;
  bdd.display(cout);
  cout << "B" << endl;
  auto bdd2 = mgr.variable(1);
  cout << "C" << endl;
  bdd2.display(cout);
  cout << "D" << endl;

#if 1
  auto bdd3 = bdd & bdd2;
  cout << "E" << endl;
  bdd3.display(cout);
  cout << "F" << endl;
#endif

  return 0;
}

END_NAMESPACE_YM


int
main(
  int argc,
  char** argv
)
{
  return YM_NAMESPACE::simple_test(argc, argv);
}
