
/// @file loop_test.cc
/// @brief loop_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/BddMgr.h"


BEGIN_NAMESPACE_YM

void
loop_test()
{
  for ( int i = 0; i < 1000000; ++ i ) {
    BddMgr mgr;
    SizeType n = 1000;
    vector<Bdd> bdd_list(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      bdd_list[i] = mgr.variable(i);
    }
  }
}

END_NAMESPACE_YM


int
main(
  int argc,
  char** argv
)
{
  YM_NAMESPACE::loop_test();

  return 0;
}
