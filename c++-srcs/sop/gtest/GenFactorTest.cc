
/// @file GenFactorTest.cc
/// @brief GenFactorTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018, 2023 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "GenFactor.h"
#include "ym/SopCover.h"
#include "ym/Expr.h"
#include "ym/Range.h"


BEGIN_NAMESPACE_YM_SOP

TEST(GenFactorTest, quick_factor)
{
  const int nv = 10;

  SizeType var0{0};
  SizeType var1{1};
  SizeType var2{2};
  SizeType var3{3};
  SizeType var4{4};
  SizeType var5{5};
  SizeType var6{6};
  SizeType var7{7};
  SizeType var8{8};
  SizeType var9{9};

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

  // abg + acg + adf + aef + afg + bd + ce + be + cd
  SopCover cover1{nv, { { lit_a, lit_b, lit_g },
			{ lit_a, lit_c, lit_g },
			{ lit_a, lit_d, lit_f },
			{ lit_a, lit_f, lit_g },
			{ lit_b, lit_d },
			{ lit_c, lit_e },
			{ lit_b, lit_e },
			{ lit_c, lit_d } } };

  QuickFactor factor;

  Expr expr = factor(cover1);

  ostringstream tmp;
  tmp << expr;

  cout << tmp.str() << endl;

#if 0
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
#endif
}

TEST(GenFactorTest, good_factor)
{
  const int nv = 10;

  SizeType var0{0};
  SizeType var1{1};
  SizeType var2{2};
  SizeType var3{3};
  SizeType var4{4};
  SizeType var5{5};
  SizeType var6{6};
  SizeType var7{7};
  SizeType var8{8};
  SizeType var9{9};

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

  // abg + acg + adf + aef + afg + bd + ce + be + cd
  SopCover cover1{nv, { { lit_a, lit_b, lit_g },
			{ lit_a, lit_c, lit_g },
			{ lit_a, lit_d, lit_f },
			{ lit_a, lit_f, lit_g },
			{ lit_b, lit_d },
			{ lit_c, lit_e },
			{ lit_b, lit_e },
			{ lit_c, lit_d } } };

  GoodFactor factor;

  Expr expr = factor(cover1);

  ostringstream tmp;
  tmp << expr;

  cout << tmp.str() << endl;

#if 0
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
#endif
}

END_NAMESPACE_YM_SOP
