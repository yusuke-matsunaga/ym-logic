
/// @file AigMgrImpl_test.cc
/// @brief AigMgrImpl_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "aig/AigMgrImpl.h"


BEGIN_NAMESPACE_YM_AIG

TEST(AigMgrImplTest, constr)
{
  AigMgrImpl mgr;

  EXPECT_EQ( 0, mgr.node_num() );
  EXPECT_EQ( 0, mgr.input_num() );
}

TEST(AigMgrImplTest, make_input)
{
  AigMgrImpl mgr;

  auto e = mgr.make_input();
  EXPECT_FALSE( e.is_zero() );
  EXPECT_FALSE( e.is_one() );
  EXPECT_FALSE( e.is_const() );
  EXPECT_TRUE( e.is_input() );
  EXPECT_EQ( 0, e.input_id() );
  EXPECT_FALSE( e.is_and() );

  EXPECT_EQ( 1, mgr.node_num() );
  EXPECT_EQ( 1, mgr.input_num() );
  EXPECT_EQ( 0, mgr.and_num() );
}

TEST(AigMgrImplTest, and_op2)
{
  AigMgrImpl mgr;

  auto e1 = mgr.make_input();
  auto e2 = mgr.make_input();

  auto e = mgr.and_op(e1, e2);
  EXPECT_FALSE( e.is_zero() );
  EXPECT_FALSE( e.is_one() );
  EXPECT_FALSE( e.is_const() );
  EXPECT_FALSE( e.is_input() );
  EXPECT_TRUE( e.is_and() );

  EXPECT_EQ( 2, e.node()->id() );

  EXPECT_EQ( 3, mgr.node_num() );
  EXPECT_EQ( 1, mgr.and_num() );
}

TEST(AigMgrImplTest, and_op3)
{
  AigMgrImpl mgr;

  auto e1 = mgr.make_input();
  auto e2 = mgr.make_input();
  auto e3 = mgr.make_input();

  auto e = mgr.and_op({e1, e2, e3});
  EXPECT_FALSE( e.is_zero() );
  EXPECT_FALSE( e.is_one() );
  EXPECT_FALSE( e.is_const() );
  EXPECT_FALSE( e.is_input() );
  EXPECT_TRUE( e.is_and() );

  EXPECT_EQ( 4, e.node()->id() );

  EXPECT_EQ( 5, mgr.node_num() );
  EXPECT_EQ( 2, mgr.and_num() );
}

#if 0
TEST(AigMgrImplTest, sweep)
{
  AigMgrImpl mgr;

  auto e1 = mgr.make_input();
  auto e2 = mgr.make_input();
  auto e3 = mgr.make_input();

  {
    auto e = mgr.and_op({e1, e2, e3});
    EXPECT_TRUE( e.is_and() );

    EXPECT_EQ( 4, e.node()->id() );

    EXPECT_EQ( 5, mgr.node_num() );
    EXPECT_EQ( 2, mgr.and_num() );

    mgr.dec_ref(e);
  }
  mgr.sweep();
  EXPECT_EQ( 3, mgr.node_num() );
  EXPECT_EQ( 0, mgr.and_num() );
  {
    auto e = mgr.and_op({e1, e2, e3});
    mgr.inc_ref(e);
    EXPECT_TRUE( e.is_and() );

    EXPECT_EQ( 4, e.node()->id() );
    EXPECT_EQ( 5, mgr.node_num() );
    EXPECT_EQ( 2, mgr.and_num() );

    mgr.dec_ref(e);
  }
}
#endif

END_NAMESPACE_YM_AIG
