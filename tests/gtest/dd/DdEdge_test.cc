
/// @file DdEdge_test.cc
/// @brief DdEdge_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "gtest/gtest.h"
#include "dd/DdEdge.h"


BEGIN_NAMESPACE_YM_DD

TEST(DdEdgeTest, zero)
{
  auto e = DdEdge::zero();

  EXPECT_TRUE( e.is_zero() );
  EXPECT_FALSE( e.is_one() );
  EXPECT_TRUE( e.is_const() );
  EXPECT_FALSE( e.is_invalid() );
}

TEST(DdEdgeTest, one)
{
  auto e = DdEdge::one();

  EXPECT_FALSE( e.is_zero() );
  EXPECT_TRUE( e.is_one() );
  EXPECT_TRUE( e.is_const() );
  EXPECT_FALSE( e.is_invalid() );
}

TEST(DdEdgeTest, invalid)
{
  auto e = DdEdge::invalid();

  EXPECT_FALSE( e.is_zero() );
  EXPECT_FALSE( e.is_one() );
  EXPECT_FALSE( e.is_const() );
  EXPECT_TRUE( e.is_invalid() );
}

END_NAMESPACE_YM_DD
