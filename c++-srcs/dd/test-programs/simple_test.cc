
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

  auto bdd = mgr.from_truth("10010110");
  bdd.display(cout);

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
