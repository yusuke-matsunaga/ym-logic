
/// @file copy_test.cc
/// @brief copy_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Bdd.h"
#include "ym/BddVar.h"
#include "ym/BddMgr.h"


BEGIN_NAMESPACE_YM

void
test()
{
  BddMgr mgr;
  auto bdd = mgr.from_truth("1101");

  Bdd bdd1{bdd};

  bdd1.display(cout);

  SizeType ni = 2;
  vector<BddVar> var_list(ni);
  for ( SizeType i = 0; i < ni; ++ i ) {
    auto var = mgr.variable(i);
    cout << "Var#" << i << endl;
    var.display(cout);
    var_list[i] = var;
    var_list[i].display(cout);
  }
  auto str = bdd.to_truth(var_list);
  cout << str << endl;
}

END_NAMESPACE_YM


int
main(
  int argc,
  char** argv
)
{
  YM_NAMESPACE::test();

  return 0;
}
