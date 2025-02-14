
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

TEST_F(GenFactorTest, gen_factor)
{
  // abg + acg + adf + aef + afg + bd + be + cd + ce
  auto cover1 = SopCover{7, { { lit_a, lit_b, lit_g },
			      { lit_a, lit_c, lit_g },
			      { lit_a, lit_d, lit_f },
			      { lit_a, lit_e, lit_f },
			      { lit_a, lit_f, lit_g },
			      { lit_b, lit_d },
			      { lit_c, lit_e },
			      { lit_b, lit_e },
			      { lit_c, lit_d } } };

  auto expr1 = cover1.quick_factor();

  cout << expr1 << endl;

  auto expr2 = cover1.good_factor();

  cout << expr2 << endl;

  auto expr3 = cover1.bool_factor();

  cout << expr3 << endl;
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

END_NAMESPACE_YM_SOP
