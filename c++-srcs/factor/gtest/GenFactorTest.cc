
/// @file GenFactorTest.cc
/// @brief GenFactorTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/SopCover.h"
#include "ym/Expr.h"
#include "ym/TvFunc.h"


BEGIN_NAMESPACE_YM_SOP

class GenFactorTest:
  public ::testing::Test
{
public:

  void
  check(
    const SopCover& cover,
    const Expr& expr
  )
  {
    SizeType ni = cover.variable_num();
    auto cover_f = cover.tvfunc();
    auto expr_f = expr.tvfunc(ni);
    EXPECT_EQ( cover_f, expr_f );
  }

  Literal lit_a{0, false};
  Literal lit_b{1, false};
  Literal lit_c{2, false};
  Literal lit_d{3, false};
  Literal lit_e{4, false};
  Literal lit_f{5, false};
  Literal lit_g{6, false};
  Literal lit_h{7, false};
  Literal lit_i{8, false};
  Literal lit_j{9, false};

};

TEST_F(GenFactorTest, quick_factor)
{
  // abg + acg + adf + aef + afg + bd + ce + be + cd
  auto cover1 = SopCover{7, { { lit_a, lit_b, lit_g },
			      { lit_a, lit_c, lit_g },
			      { lit_a, lit_d, lit_f },
			      { lit_a, lit_f, lit_g },
			      { lit_b, lit_d },
			      { lit_c, lit_e },
			      { lit_b, lit_e },
			      { lit_c, lit_d } } };

  auto expr1 = cover1.quick_factor();

  ostringstream tmp;
  tmp << expr1;

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

TEST_F(GenFactorTest, good_factor)
{
  // abg + acg + adf + aef + afg + bd + ce + be + cd
  auto cover1 = SopCover{7, { { lit_a, lit_b, lit_g },
			      { lit_a, lit_c, lit_g },
			      { lit_a, lit_d, lit_f },
			      { lit_a, lit_f, lit_g },
			      { lit_b, lit_d },
			      { lit_c, lit_e },
			      { lit_b, lit_e },
			      { lit_c, lit_d } } };

  auto expr1 = cover1.good_factor();

  ostringstream tmp;
  tmp << expr1;

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

TEST_F(GenFactorTest, bool_factor0)
{
  auto nv = 4U;

  auto cover1 = SopCover{nv, { {lit_a, lit_c},
			       {lit_a, lit_d},
			       {lit_b, lit_c},
			       {lit_b, lit_d} }};
  auto expr1 = cover1.bool_factor();

  cout << expr1 << endl;
}

TEST_F(GenFactorTest, bool_factor)
{
  // abg + acg + adf + aef + afg + bd + ce + be + cd
  auto cover1 = SopCover{7, { { lit_a, lit_b, lit_g },
			      { lit_a, lit_c, lit_g },
			      { lit_a, lit_d, lit_f },
			      { lit_a, lit_f, lit_g },
			      { lit_b, lit_d },
			      { lit_c, lit_e },
			      { lit_b, lit_e },
			      { lit_c, lit_d } } };

  auto expr1 = cover1.bool_factor();

  ostringstream tmp;
  tmp << expr1;

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
