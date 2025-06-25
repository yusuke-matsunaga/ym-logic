
/// @file CutMgrTest.cc
/// @brief CutMgrTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "CutMgr.h"
#include "AigMgrImpl.h"


BEGIN_NAMESPACE_YM_AIG

TEST(CutMgrTest, input)
{
  AigMgrImpl mgr;

  auto e = mgr.input(0);
  auto node = e.node();

  SizeType cut_size = 4;
  CutMgr cut_mgr(&mgr, cut_size);
  auto cuts = cut_mgr.enum_cuts(node);

  EXPECT_EQ( 1, cuts.size() );
  auto cut = cuts.front();
  EXPECT_EQ( 1, cut->leaf_size() );
  EXPECT_EQ( node, cut->leaf_list().front() );
  EXPECT_EQ( 0, cut->node_list().size() );
}

TEST(CutMgrTest, and2)
{
  AigMgrImpl mgr;

  auto e0 = mgr.input(0);
  auto e1 = mgr.input(1);
  auto e2 = mgr.and_op(e0, e1);

  SizeType cut_size = 4;
  CutMgr cut_mgr(&mgr, cut_size);
  auto cuts = cut_mgr.enum_cuts(e2.node());

  ASSERT_EQ( 2, cuts.size() );

  auto cut0 = cuts[0];
  ASSERT_EQ( 1, cut0->leaf_size() );
  EXPECT_EQ( e2.node(), cut0->leaf_list().front() );
  EXPECT_EQ( 0, cut0->node_list().size() );

  auto cut1 = cuts[1];
  ASSERT_EQ( 2, cut1->leaf_size() );
  EXPECT_EQ( e0.node(), cut1->leaf_list()[0] );
  EXPECT_EQ( e1.node(), cut1->leaf_list()[1] );
  ASSERT_EQ( 1, cut1->node_list().size() );
  EXPECT_EQ( e2.node(), cut1->node_list()[0] );
}

TEST(CutMgrTest, and3)
{
  AigMgrImpl mgr;

  auto e0 = mgr.input(0);
  auto e1 = mgr.input(1);
  auto e2 = mgr.input(2);
  auto e3 = mgr.and_op(e0, e1);
  auto e4 = mgr.and_op(e3, e2);

  SizeType cut_size = 4;
  CutMgr cut_mgr(&mgr, cut_size);
  auto cuts = cut_mgr.enum_cuts(e4.node());

  ASSERT_EQ( 3, cuts.size() );

  auto cut0 = cuts[0];
  ASSERT_EQ( 1, cut0->leaf_size() );
  EXPECT_EQ( e4.node(), cut0->leaf_list().front() );
  EXPECT_EQ( 0, cut0->node_list().size() );

  auto cut1 = cuts[1];
  ASSERT_EQ( 2, cut1->leaf_size() );
  EXPECT_EQ( e3.node(), cut1->leaf_list()[0] );
  EXPECT_EQ( e2.node(), cut1->leaf_list()[1] );
  ASSERT_EQ( 1, cut1->node_list().size() );
  EXPECT_EQ( e4.node(), cut1->node_list()[0] );

  auto cut2 = cuts[2];
  ASSERT_EQ( 3, cut2->leaf_size() );
  EXPECT_EQ( e0.node(), cut2->leaf_list()[0] );
  EXPECT_EQ( e1.node(), cut2->leaf_list()[1] );
  EXPECT_EQ( e2.node(), cut2->leaf_list()[2] );
  ASSERT_EQ( 2, cut2->node_list().size() );
  EXPECT_EQ( e4.node(), cut2->node_list()[1] );
  EXPECT_EQ( e3.node(), cut2->node_list()[0] );
}

TEST(CutMgrTest, xor2)
{
  AigMgrImpl mgr;

  auto e0 = mgr.input(0);
  auto e1 = mgr.input(1);
  auto e5 = mgr.input(2);
  auto e6 = mgr.input(3);
  auto e2 = mgr.and_op(e0, ~e1);
  auto e3 = mgr.and_op(~e0, e1);
  auto e4 = mgr.and_op(~e2, ~e3);
  auto e7 = mgr.and_op(e5, e6);
  auto e8 = mgr.and_op(e4, e7);

  SizeType cut_size = 4;
  CutMgr cut_mgr(&mgr, cut_size);
  auto cuts = cut_mgr.enum_cuts(e8.node());

  mgr.print(cout);
  for ( auto cut: cuts ) {
    cut->print(cout);
    cout << endl;
  }
}

END_NAMESPACE_YM_AIG
