
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

TEST_F(BddTest, copy)
{
  mMgr.variable(3);
  auto var_list = mMgr.variable_list();
  const char* exp_str = "1001010110111010";
  Bdd bdd = mMgr.from_truth(var_list, exp_str);

  BddMgr mgr1;
  Bdd bdd1 = mgr1.copy(bdd);
  EXPECT_TRUE( bdd1.is_valid() );

  auto bdd2 = mMgr.copy(bdd1);
  check(bdd2, exp_str);
}

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

TEST_F(BddTest, mgr_var_ordering)
{
  auto var0 = variable(0);
  auto var1 = variable(1);
  auto var2 = variable(2);
  auto var3 = variable(3);

  EXPECT_EQ( 0, var0.id() );
  EXPECT_EQ( 1, var1.id() );
  EXPECT_EQ( 2, var2.id() );
  EXPECT_EQ( 3, var3.id() );

  auto v_list = mMgr.variable_list();
  vector<BddVar> exp_v_list{var0, var1, var2, var3};
  EXPECT_EQ( exp_v_list, v_list );

  mMgr.set_variable_order({var3, var2, var1, var0});

  auto o_list = mMgr.variable_order();
  vector<BddVar> exp_o_list{var3, var2, var1, var0};
  EXPECT_EQ( exp_o_list, o_list );
}

END_NAMESPACE_YM
