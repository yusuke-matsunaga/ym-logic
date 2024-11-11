
/// @file BddMgr_test.cc
/// @brief BddMgr のユニットテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022, 2023 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/Bdd.h"
#include "ym/BddMgr.h"
#include "ym/BddVar.h"
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
  auto var = mMgr.variable(0);

  EXPECT_FALSE( var.is_invalid() );
  EXPECT_TRUE( var.is_valid() );
  EXPECT_FALSE( var.is_zero() );
  EXPECT_FALSE( var.is_one() );

  check(var, "10");

  Bdd f0;
  Bdd f1;
  auto var1 = var.root_decomp(f0, f1);

  EXPECT_EQ( var1, var );
  EXPECT_TRUE( f0.is_zero() );
  EXPECT_TRUE( f1.is_one() );
}

TEST_F(BddTest, posi_literal)
{
  auto var = mMgr.variable(0);
  Bdd bdd = var.posilit();

  EXPECT_FALSE( bdd.is_invalid() );
  EXPECT_TRUE( bdd.is_valid() );
  EXPECT_FALSE( bdd.is_zero() );
  EXPECT_FALSE( bdd.is_one() );

  check(bdd, "10");

  Bdd f0;
  Bdd f1;
  auto var1 = bdd.root_decomp(f0, f1);

  EXPECT_EQ( var1, var );
  EXPECT_TRUE( f0.is_zero() );
  EXPECT_TRUE( f1.is_one() );
}

TEST_F(BddTest, nega_literal)
{
  auto var = mMgr.variable(0);
  Bdd bdd = var.negalit();

  EXPECT_FALSE( bdd.is_invalid() );
  EXPECT_TRUE( bdd.is_valid() );
  EXPECT_FALSE( bdd.is_zero() );
  EXPECT_FALSE( bdd.is_one() );

  check(bdd, "01");

  Bdd f0;
  Bdd f1;
  auto var1 = bdd.root_decomp(f0, f1);

  EXPECT_EQ( var1, var );
  EXPECT_TRUE( f0.is_one() );
  EXPECT_TRUE( f1.is_zero() );
}

TEST_F(BddTest, from_truth1)
{
  mMgr.variable(2);
  auto var_list = mMgr.variable_list();
  const char* exp_str = "10010110";
  Bdd bdd = mMgr.from_truth(var_list, exp_str);
  check(bdd, exp_str);
}

TEST_F(BddTest, from_truth2)
{
  mMgr.variable(2);
  auto var_list = mMgr.variable_list();
  const char* exp_str = "10010101";
  Bdd bdd = mMgr.from_truth(var_list, exp_str);
  check(bdd, exp_str);
}

#if 0
TEST_F(BddTest, copy)
{
  mMgr.variable(3);
  auto var_list = mMgr.variable_list();
  const char* exp_str = "1001010110111010";
  Bdd bdd = mMgr.from_truth(var_list, exp_str);

  BddMgr mgr1;
  //Bdd bdd1 = mgr1.copy(bdd);
  Bdd bdd1;
  check(bdd1, exp_str);

  //Bdd bdd2 = mMgr.copy(bdd1);
  Bdd bdd2;

  EXPECT_EQ( bdd, bdd2 );
}
#endif

TEST_F(BddTest, mgr_copy)
{
  BddMgr mgr1;
  auto var = mgr1.variable(0);

  auto bdd = var.posilit();

  auto mgr2 = bdd.mgr();

  EXPECT_EQ( mgr1, mgr2 );
}

TEST_F(BddTest, mgr_lifetime)
{
  Bdd bdd;
  {
    BddMgr mgr;
    auto var = mgr.variable(0);
    bdd = var.posilit();
  }

  EXPECT_TRUE( bdd.is_valid() );
  //check(bdd, "10");
}

END_NAMESPACE_YM
