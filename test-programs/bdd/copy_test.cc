
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

// @brief 真理値表形式の文字列からBDDを作る．
Bdd
from_truth(
  BddMgr& mgr,
  const char* str
)
{
  auto len = strlen(str);
  SizeType ni = 0;
  while ( (1 << ni) < len ) {
    ++ ni;
  }
  if ( ni > 0 ) {
    mgr.variable(ni - 1);
  }
  auto var_list = mgr.variable_list();
  return mgr.from_truth(var_list, str);
}

void
test()
{
  BddMgr mgr;
  auto bdd = from_truth(mgr, "1101");

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
