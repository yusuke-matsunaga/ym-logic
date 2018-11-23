
/// @file AlgKernelTest.cc
/// @brief AlgKernelTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.


#include <gtest/gtest.h>
#include "AlgKernelGen.h"
#include "ym/Range.h"


BEGIN_NAMESPACE_YM_LOGIC

TEST(AlgKernelTest, test1)
{
  const int nv = 10;

  VarId var0(0);
  VarId var1(1);
  VarId var2(2);
  VarId var3(3);
  VarId var4(4);
  VarId var5(5);
  VarId var6(6);
  VarId var7(7);
  VarId var8(8);
  VarId var9(9);

  Literal lit_a(var0, false);
  Literal lit_b(var1, false);
  Literal lit_c(var2, false);
  Literal lit_d(var3, false);
  Literal lit_e(var4, false);
  Literal lit_f(var5, false);
  Literal lit_g(var6, false);
  Literal lit_h(var7, false);
  Literal lit_i(var8, false);
  Literal lit_j(var9, false);

  // adf + aef + bdf + bef + cdf + cef + bfg + h
  AlgCover cover1{nv, { { lit_a, lit_d, lit_f },
			{ lit_a, lit_e, lit_f },
			{ lit_b, lit_d, lit_f },
			{ lit_b, lit_e, lit_f },
			{ lit_c, lit_d, lit_f },
			{ lit_c, lit_e, lit_f },
			{ lit_b, lit_f, lit_g },
			{ lit_h } } };

  AlgKernelGen kg;

  vector<pair<AlgCover, AlgCover>> kernel_list;
  kg.all_kernel(cover1, kernel_list);

  vector<string> exp_list
    {
     "v3 + v4, v0 v5 + v2 v5",
     "v3 + v4 + v6, v1 v5",
     "v0 + v1 + v2, v3 v5 + v4 v5",
     "v0 v3 + v0 v4 + v1 v3 + v1 v4 + v1 v6 + v2 v3 + v2 v4, v5",
     "v0 v3 v5 + v0 v4 v5 + v1 v3 v5 + v1 v4 v5 + v1 v5 v6 + v2 v3 v5 + v2 v4 v5 + v7, "
    };

  int n = kernel_list.size();
  EXPECT_EQ( exp_list.size(), n );
  for ( int i: Range(n) ) {
    auto& ki = kernel_list[i];
    auto& kernel = ki.first;
    auto& cokernels = ki.second;
    ostringstream tmp;
    tmp << kernel << ", " << cokernels;
    string tmp_str = tmp.str();
    EXPECT_EQ( exp_list[i], tmp_str );
  }
}

END_NAMESPACE_YM_LOGIC
