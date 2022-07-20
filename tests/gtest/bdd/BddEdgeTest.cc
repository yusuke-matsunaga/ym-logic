
/// @file BddEdge_test.cc
/// @brief BddEdge_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "gtest/gtest.h"
#include "BddEdge.h"


BEGIN_NAMESPACE_YM_BDD

TEST(BddEdgeTest, zero)
{
  auto e = BddEdge::zero();

  EXPECT_TRUE( e.is_zero() );
  EXPECT_FALSE( e.is_one() );
  EXPECT_TRUE( e.is_const() );
  EXPECT_FALSE( e.is_invalid() );
}

TEST(BddEdgeTest, one)
{
  auto e = BddEdge::one();

  EXPECT_FALSE( e.is_zero() );
  EXPECT_TRUE( e.is_one() );
  EXPECT_TRUE( e.is_const() );
  EXPECT_FALSE( e.is_invalid() );
}

TEST(BddEdgeTest, invalid)
{
  auto e = BddEdge::invalid();

  EXPECT_FALSE( e.is_zero() );
  EXPECT_FALSE( e.is_one() );
  EXPECT_FALSE( e.is_const() );
  EXPECT_TRUE( e.is_invalid() );
}

END_NAMESPACE_YM_BDD
