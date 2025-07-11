
/// @file BddVar_test.cc
/// @brief BddVar_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/BddVar.h"
#include "ym/BddLit.h"
#include "ym/BddMgr.h"


BEGIN_NAMESPACE_YM

TEST(BddVarTest, invalid)
{
  auto var = BddVar::invalid();

  EXPECT_TRUE( var.is_invalid() );
  EXPECT_FALSE( var.is_valid() );
}

TEST(BddVarTest, invalid2)
{
  BddVar var;

  EXPECT_TRUE( var.is_invalid() );
  EXPECT_FALSE( var.is_valid() );
}

TEST(BddVarTest, init)
{
  BddMgr mgr;

  auto var = mgr.variable(0);

  EXPECT_EQ( 0, var.id() );
}

TEST(BddVarTest, init2)
{
  BddMgr mgr;

  SizeType id = 10;
  auto var = mgr.variable(id);

  EXPECT_EQ( id, var.id() );
}

TEST(BddVarTest, copy)
{
  BddMgr mgr;

  SizeType id = 5;
  auto var = mgr.variable(id);

  auto var1{var};

  EXPECT_EQ( var, var1 );
}

TEST(BddVarTest, copy_assignment)
{
  BddMgr mgr;

  SizeType id = 5;
  auto var = mgr.variable(id);

  auto var1 = var;

  EXPECT_EQ( var, var1 );
}

TEST(BddVarTest, copy_assignment2)
{
  BddMgr mgr;

  SizeType ni = 5;
  auto var = mgr.variable(ni - 1);

  std::vector<BddVar> var_list(ni);
  for ( SizeType i = 0; i < ni; ++ i ) {
    auto var = mgr.variable(i);
    var_list[i] = var;
  }
  for ( SizeType i = 0; i < ni; ++ i ) {
    auto var = var_list[i];
    EXPECT_EQ( i, var.id() );
  }
}

TEST(BddVarTest, positive_literal)
{
  BddMgr mgr;

  SizeType id = 20;
  auto var = mgr.variable(id);

  auto lit = var.positive_literal();
  EXPECT_TRUE( lit.is_positive() );
  EXPECT_EQ( var, lit.var() );
}

TEST(BddVarTest, negative_literal)
{
  BddMgr mgr;

  SizeType id = 20;
  auto var = mgr.variable(id);

  auto lit = var.negative_literal();
  EXPECT_TRUE( lit.is_negative() );
  EXPECT_EQ( var, lit.var() );
}

END_NAMESPACE_YM
