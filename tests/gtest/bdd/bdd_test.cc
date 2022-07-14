
/// @file bdd_test.cc
/// @brief bdd_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/Bdd.h"
#include "ym/BddMgr.h"


BEGIN_NAMESPACE_YM

TEST(BddTest, invalid1)
{
  // 不正値になっているはず．
  Bdd bdd;

  EXPECT_TRUE( bdd.is_invalid() );
  EXPECT_FALSE( bdd.is_valid() );
  EXPECT_FALSE( bdd.is_zero() );
  EXPECT_FALSE( bdd.is_one() );
}

TEST(BddTest, invalid2)
{
  // 不正値になっているはず．
  Bdd bdd = Bdd::make_invalid();

  EXPECT_TRUE( bdd.is_invalid() );
  EXPECT_FALSE( bdd.is_valid() );
  EXPECT_FALSE( bdd.is_zero() );
  EXPECT_FALSE( bdd.is_one() );
}

TEST(BddTest, zero)
{
  BddMgr mgr;

  Bdd bdd = mgr.make_zero();

  EXPECT_FALSE( bdd.is_invalid() );
  EXPECT_TRUE( bdd.is_valid() );
  EXPECT_TRUE( bdd.is_zero() );
  EXPECT_FALSE( bdd.is_one() );
}

TEST(BddTest, one)
{
  BddMgr mgr;

  Bdd bdd = mgr.make_one();

  EXPECT_FALSE( bdd.is_invalid() );
  EXPECT_TRUE( bdd.is_valid() );
  EXPECT_FALSE( bdd.is_zero() );
  EXPECT_TRUE( bdd.is_one() );
}

TEST(BddTest, lit1)
{
  BddMgr mgr;
  BddVar var = mgr.new_variable("var1");

  Bdd bdd = mgr.make_literal(var);

  EXPECT_FALSE( bdd.is_invalid() );
  EXPECT_TRUE( bdd.is_valid() );
  EXPECT_FALSE( bdd.is_zero() );
  EXPECT_FALSE( bdd.is_one() );

  Bdd f0;
  Bdd f1;
  BddVar var1 = bdd.root_decomp(f0, f1);

  EXPECT_EQ( var1, var );
  EXPECT_TRUE( f0.is_zero() );
  EXPECT_TRUE( f1.is_one() );
}

TEST(BddTest, lit2)
{
  BddMgr mgr;
  BddVar var = mgr.new_variable("var1");

  Bdd bdd = mgr.make_literal(var, false);

  EXPECT_FALSE( bdd.is_invalid() );
  EXPECT_TRUE( bdd.is_valid() );
  EXPECT_FALSE( bdd.is_zero() );
  EXPECT_FALSE( bdd.is_one() );

  Bdd f0;
  Bdd f1;
  BddVar var1 = bdd.root_decomp(f0, f1);

  EXPECT_EQ( var1, var );
  EXPECT_TRUE( f0.is_zero() );
  EXPECT_TRUE( f1.is_one() );
}

TEST(BddTest, lit3)
{
  BddMgr mgr;
  BddVar var = mgr.new_variable("var1");

  Bdd bdd = mgr.make_literal(var, true);

  EXPECT_FALSE( bdd.is_invalid() );
  EXPECT_TRUE( bdd.is_valid() );
  EXPECT_FALSE( bdd.is_zero() );
  EXPECT_FALSE( bdd.is_one() );

  Bdd f0;
  Bdd f1;
  BddVar var1 = bdd.root_decomp(f0, f1);

  EXPECT_EQ( var1, var );
  EXPECT_TRUE( f0.is_one() );
  EXPECT_TRUE( f1.is_zero() );
}

TEST(BddTest, lit4)
{
  BddMgr mgr;
  BddVar var = mgr.new_variable("var1");

  Bdd bdd = mgr.make_posi_literal(var);

  EXPECT_FALSE( bdd.is_invalid() );
  EXPECT_TRUE( bdd.is_valid() );
  EXPECT_FALSE( bdd.is_zero() );
  EXPECT_FALSE( bdd.is_one() );

  Bdd f0;
  Bdd f1;
  BddVar var1 = bdd.root_decomp(f0, f1);

  EXPECT_EQ( var1, var );
  EXPECT_TRUE( f0.is_zero() );
  EXPECT_TRUE( f1.is_one() );
}

TEST(BddTest, lit5)
{
  BddMgr mgr;
  BddVar var = mgr.new_variable("var1");

  Bdd bdd = mgr.make_nega_literal(var);

  EXPECT_FALSE( bdd.is_invalid() );
  EXPECT_TRUE( bdd.is_valid() );
  EXPECT_FALSE( bdd.is_zero() );
  EXPECT_FALSE( bdd.is_one() );

  Bdd f0;
  Bdd f1;
  BddVar var1 = bdd.root_decomp(f0, f1);

  EXPECT_EQ( var1, var );
  EXPECT_TRUE( f0.is_one() );
  EXPECT_TRUE( f1.is_zero() );
}

TEST(BddTest, and1)
{
  BddMgr mgr;
  BddVar var1 = mgr.new_variable("var1");
  BddVar var2 = mgr.new_variable("var2");

  Bdd bdd1 = mgr.make_posi_literal(var1);
  Bdd bdd2 = mgr.make_posi_literal(var2);

  EXPECT_EQ( var1, bdd1.root_var() );
  EXPECT_EQ( var2, bdd2.root_var() );

  Bdd bdd = bdd1 & bdd2;

  Bdd f0;
  Bdd f1;
  BddVar v1 = bdd.root_decomp(f0, f1);

  EXPECT_EQ( var1, v1 );
  EXPECT_EQ( var1.index(), v1.index() );
  EXPECT_TRUE( f0.is_zero() );

  Bdd f10;
  Bdd f11;
  BddVar v2 = f1.root_decomp(f10, f11);

  EXPECT_EQ( var2, v2 );
  EXPECT_TRUE( f10.is_zero() );
  EXPECT_TRUE( f11.is_one() );
}

END_NAMESPACE_YM
