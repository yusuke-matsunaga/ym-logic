
/// @file BddMgr_test.cc
/// @brief BddMgr のユニットテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/Bdd.h"
#include "ym/BddMgr.h"
#include "BddTest.h"


BEGIN_NAMESPACE_YM

TEST_F(BddTest, zero)
{
  Bdd bdd = mMgr.zero();

  EXPECT_FALSE( bdd.is_invalid() );
  EXPECT_TRUE( bdd.is_valid() );
  EXPECT_TRUE( bdd.is_zero() );
  EXPECT_FALSE( bdd.is_one() );

  check(bdd, "0");
}

TEST_F(BddTest, one)
{
  Bdd bdd = mMgr.one();

  EXPECT_FALSE( bdd.is_invalid() );
  EXPECT_TRUE( bdd.is_valid() );
  EXPECT_FALSE( bdd.is_zero() );
  EXPECT_TRUE( bdd.is_one() );

  check(bdd, "1");
}

TEST_F(BddTest, literal1)
{
  VarId var{0};
  Bdd bdd = mMgr.literal(var);

  EXPECT_FALSE( bdd.is_invalid() );
  EXPECT_TRUE( bdd.is_valid() );
  EXPECT_FALSE( bdd.is_zero() );
  EXPECT_FALSE( bdd.is_one() );

  check(bdd, "10");

  Bdd f0;
  Bdd f1;
  VarId var1 = bdd.root_decomp(f0, f1);

  EXPECT_EQ( var1, var );
  EXPECT_TRUE( f0.is_zero() );
  EXPECT_TRUE( f1.is_one() );
}

TEST_F(BddTest, literal2)
{
  VarId var{0};
  Bdd bdd = mMgr.literal(var, false);

  EXPECT_FALSE( bdd.is_invalid() );
  EXPECT_TRUE( bdd.is_valid() );
  EXPECT_FALSE( bdd.is_zero() );
  EXPECT_FALSE( bdd.is_one() );

  check(bdd, "10");

  Bdd f0;
  Bdd f1;
  VarId var1 = bdd.root_decomp(f0, f1);

  EXPECT_EQ( var1, var );
  EXPECT_TRUE( f0.is_zero() );
  EXPECT_TRUE( f1.is_one() );
}

TEST_F(BddTest, literal3)
{
  VarId var{0};
  Bdd bdd = mMgr.literal(var, true);

  EXPECT_FALSE( bdd.is_invalid() );
  EXPECT_TRUE( bdd.is_valid() );
  EXPECT_FALSE( bdd.is_zero() );
  EXPECT_FALSE( bdd.is_one() );

  check(bdd, "01");

  Bdd f0;
  Bdd f1;
  VarId var1 = bdd.root_decomp(f0, f1);

  EXPECT_EQ( var1, var );
  EXPECT_TRUE( f0.is_one() );
  EXPECT_TRUE( f1.is_zero() );
}

TEST_F(BddTest, posi_literal)
{
  VarId var{0};
  Bdd bdd = mMgr.posi_literal(var);

  EXPECT_FALSE( bdd.is_invalid() );
  EXPECT_TRUE( bdd.is_valid() );
  EXPECT_FALSE( bdd.is_zero() );
  EXPECT_FALSE( bdd.is_one() );

  check(bdd, "10");

  Bdd f0;
  Bdd f1;
  VarId var1 = bdd.root_decomp(f0, f1);

  EXPECT_EQ( var1, var );
  EXPECT_TRUE( f0.is_zero() );
  EXPECT_TRUE( f1.is_one() );
}

TEST_F(BddTest, nega_literal)
{
  VarId var{0};
  Bdd bdd = mMgr.nega_literal(var);

  EXPECT_FALSE( bdd.is_invalid() );
  EXPECT_TRUE( bdd.is_valid() );
  EXPECT_FALSE( bdd.is_zero() );
  EXPECT_FALSE( bdd.is_one() );

  check(bdd, "01");

  Bdd f0;
  Bdd f1;
  VarId var1 = bdd.root_decomp(f0, f1);

  EXPECT_EQ( var1, var );
  EXPECT_TRUE( f0.is_one() );
  EXPECT_TRUE( f1.is_zero() );
}

TEST_F(BddTest, from_truth1)
{
  const char* exp_str = "10010110";
  Bdd bdd = mMgr.from_truth(exp_str);
  check(bdd, exp_str);
}

TEST_F(BddTest, from_truth2)
{
  const char* exp_str = "10010101";
  Bdd bdd = mMgr.from_truth(exp_str);
  check(bdd, exp_str);
}

TEST_F(BddTest, copy)
{
  const char* exp_str = "1001010110111010";
  Bdd bdd = mMgr.from_truth(exp_str);

  BddMgr mgr1;
  Bdd bdd1 = mgr1.copy(bdd);
  check(bdd1, exp_str);

  Bdd bdd2 = mMgr.copy(bdd1);

  EXPECT_EQ( bdd, bdd2 );
}

END_NAMESPACE_YM
