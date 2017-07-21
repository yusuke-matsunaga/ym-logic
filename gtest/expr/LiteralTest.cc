
/// @file LiteralTest.cc
/// @brief LiteralTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.


#include "gtest/gtest.h"
#include "ym/Literal.h"


BEGIN_NAMESPACE_YM

TEST(LiteralTest, empty_constr)
{
  Literal lit;

  EXPECT_EQ( kLiteralX, lit );
}

TEST(LiteralTest, simple_constr)
{
  VarId var(3);
  Literal lit(var);

  EXPECT_NE( kLiteralX, lit );

  VarId var1 = lit.varid();
  EXPECT_EQ( var, var1 );

  EXPECT_TRUE( lit.is_positive() );
  EXPECT_FALSE( lit.is_negative() );

  Literal lit_n = ~lit;
  EXPECT_NE( lit, lit_n );

  EXPECT_EQ( lit.varid(), lit_n.varid() );
  EXPECT_FALSE( lit_n.is_positive() );
  EXPECT_TRUE( lit_n.is_negative() );

  EXPECT_EQ( lit, lit.make_positive() );
  EXPECT_NE( lit, lit.make_negative() );

  EXPECT_NE( lit_n, lit_n.make_positive() );
  EXPECT_EQ( lit_n, lit_n.make_negative() );

}

END_NAMESPACE_YM
