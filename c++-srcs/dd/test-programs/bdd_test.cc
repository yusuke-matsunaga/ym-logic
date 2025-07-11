
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
  std::cout << "_A" << std::endl;
  auto bdd = mgr.variable(0);
  std::cout << "A" << std::endl;
  bdd.display(std::cout);
  std::cout << "B" << std::endl;
  auto bdd2 = mgr.variable(1);
  std::cout << "C" << std::endl;
  bdd2.display(std::cout);
  std::cout << "D" << std::endl;

#if 1
  auto bdd3 = bdd & bdd2;
  std::cout << "E" << std::endl;
  bdd3.display(std::cout);
  std::cout << "F" << std::endl;
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
