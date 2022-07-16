
/// @file BddEdge_test.cc
/// @brief BddEdge_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "gtest/gtest.h"
#include "BddEdge.h"


BEGIN_NAMESPACE_YM_BDD

TEST(BddEdgeTest, make_zero)
{
  auto e = BddEdge::make_zero();

  EXPECT_TRUE( e.is_zero() );
  EXPECT_FALSE( e.is_one() );
  EXPECT_TRUE( e.is_const() );
  EXPECT_FALSE( e.is_invalid() );
}

TEST(BddEdgeTest, make_one)
{
  auto e = BddEdge::make_one();

  EXPECT_FALSE( e.is_zero() );
  EXPECT_TRUE( e.is_one() );
  EXPECT_TRUE( e.is_const() );
  EXPECT_FALSE( e.is_invalid() );
}

TEST(BddEdgeTest, make_invalid)
{
  auto e = BddEdge::make_invalid();

  EXPECT_FALSE( e.is_zero() );
  EXPECT_FALSE( e.is_one() );
  EXPECT_FALSE( e.is_const() );
  EXPECT_TRUE( e.is_invalid() );
}

END_NAMESPACE_YM_BDD
