
/// @file AigNode_test.cc
/// @brief AigNode_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "AigNode.h"


BEGIN_NAMESPACE_YM_AIG

TEST(AigNodeTest, constr1)
{
  SizeType id = 123;
  SizeType input_id = 456;
  AigNode node{id, input_id};

  EXPECT_EQ( id, node.id() );
  EXPECT_TRUE( node.is_input() );
  EXPECT_FALSE( node.is_and() );

  EXPECT_EQ( input_id, node.input_id() );

  EXPECT_THROW( node.fanin_node(0),
		std::invalid_argument );
  EXPECT_THROW( node.fanin0_node(),
		std::invalid_argument );
  EXPECT_THROW( node.fanin1_node(),
		std::invalid_argument );
  EXPECT_THROW( node.fanin_inv(0),
		std::invalid_argument );
  EXPECT_THROW( node.fanin0_inv(),
		std::invalid_argument );
  EXPECT_THROW( node.fanin1_inv(),
		std::invalid_argument );
  EXPECT_THROW( node.fanin(0),
		std::invalid_argument );
  EXPECT_THROW( node.fanin0(),
		std::invalid_argument );
  EXPECT_THROW( node.fanin1(),
		std::invalid_argument );
}

TEST(AigNodeTest, constr2)
{
  SizeType id1 = 0;
  SizeType input_id1 = 123;
  AigNode node0{id1, input_id1};

  SizeType id2 = 1;
  SizeType input_id2 = 456;
  AigNode node1{id2, input_id2};

  SizeType id3 = 2;
  bool inv0 = true;
  AigEdge edge0{&node0, inv0};
  bool inv1 = false;
  AigEdge edge1{&node1, inv1};
  AigNode node2{id3, edge0, edge1};

  EXPECT_EQ( id3, node2.id() );
  EXPECT_FALSE( node2.is_input() );
  EXPECT_TRUE( node2.is_and() );

  EXPECT_THROW( node2.input_id(),
		std::invalid_argument );

  EXPECT_EQ( &node0, node2.fanin_node(0) );
  EXPECT_EQ( &node1, node2.fanin_node(1) );
  EXPECT_EQ( &node0, node2.fanin0_node() );
  EXPECT_EQ( &node1, node2.fanin1_node() );
  EXPECT_EQ( inv0, node2.fanin_inv(0) );
  EXPECT_EQ( inv1, node2.fanin_inv(1) );
  EXPECT_EQ( inv0, node2.fanin0_inv() );
  EXPECT_EQ( inv1, node2.fanin1_inv() );
  EXPECT_EQ( edge0, node2.fanin(0) );
  EXPECT_EQ( edge1, node2.fanin(1) );
  EXPECT_EQ( edge0, node2.fanin0() );
  EXPECT_EQ( edge1, node2.fanin1() );
}

END_NAMESPACE_YM_AIG
