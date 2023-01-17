
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

  EXPECT_FALSE( lit.is_valid() );

  ostringstream oss;
  oss << lit;
  EXPECT_EQ( string("-X-"), oss.str() );
}

TEST(LiteralTest, simple_constr)
{
  SizeType var{3};
  Literal lit{var, false};

  EXPECT_TRUE( lit.is_valid() );

  auto var1 = lit.varid();
  EXPECT_EQ( var, var1 );

  EXPECT_TRUE( lit.is_positive() );
  EXPECT_FALSE( lit.is_negative() );

  auto lit_n = ~lit;
  EXPECT_NE( lit, lit_n );

  EXPECT_EQ( lit.varid(), lit_n.varid() );
  EXPECT_FALSE( lit_n.is_positive() );
  EXPECT_TRUE( lit_n.is_negative() );

  EXPECT_EQ( lit, lit.make_positive() );
  EXPECT_NE( lit, lit.make_negative() );

  EXPECT_NE( lit_n, lit_n.make_positive() );
  EXPECT_EQ( lit_n, lit_n.make_negative() );

  {
    ostringstream oss;
    oss << lit;
    EXPECT_EQ( string("3"), oss.str() );
  }
  {
    ostringstream oss;
    oss << lit_n;
    EXPECT_EQ( string("3'"), oss.str() );
  }
}

TEST(LiteralTest, compare)
{
  SizeType var1{1};
  SizeType var2{2};
  Literal lit1p{var1, false};
  Literal lit1n{var1, true};
  Literal lit2p{var2, false};
  Literal lit2n{var2, true};

  // lit1p vs lit1p
  EXPECT_TRUE(  lit1p == lit1p );
  EXPECT_FALSE( lit1p != lit1p );
  EXPECT_FALSE( lit1p <  lit1p );
  EXPECT_FALSE( lit1p >  lit1p );
  EXPECT_TRUE(  lit1p <= lit1p );
  EXPECT_TRUE(  lit1p >= lit1p );

  // lit1p vs lit1n
  EXPECT_FALSE( lit1p == lit1n );
  EXPECT_TRUE(  lit1p != lit1n );
  EXPECT_TRUE( lit1p <  lit1n );
  EXPECT_FALSE(  lit1p >  lit1n );
  EXPECT_TRUE( lit1p <= lit1n );
  EXPECT_FALSE(  lit1p >= lit1n );

  // lit1n vs lit2p
  EXPECT_FALSE( lit1n == lit2p );
  EXPECT_TRUE(  lit1n != lit2p );
  EXPECT_TRUE(  lit1n <  lit2p );
  EXPECT_FALSE( lit1n >  lit2p );
  EXPECT_TRUE(  lit1n <= lit2p );
  EXPECT_FALSE( lit1n >= lit2p );

  // lit1p vs lit2n
  EXPECT_FALSE( lit1p == lit2n );
  EXPECT_TRUE(  lit1p != lit2n );
  EXPECT_TRUE(  lit1p <  lit2n );
  EXPECT_FALSE( lit1p >  lit2n );
  EXPECT_TRUE(  lit1p <= lit2n );
  EXPECT_FALSE( lit1p >= lit2n );

  // lit2p vs lit1n
  EXPECT_FALSE( lit2p == lit1n );
  EXPECT_TRUE(  lit2p != lit1n );
  EXPECT_FALSE( lit2p <  lit1n );
  EXPECT_TRUE(  lit2p >  lit1n );
  EXPECT_FALSE( lit2p <= lit1n );
  EXPECT_TRUE(  lit2p >= lit1n );
}

END_NAMESPACE_YM
