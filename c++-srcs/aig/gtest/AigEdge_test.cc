
/// @file AigEdge_test.cc
/// @brief AigEdge_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "aig/AigEdge.h"
#include "aig/AigNode.h"


BEGIN_NAMESPACE_YM_AIG

TEST(AigEdgeTest, constr0)
{
  AigEdge h;
  // 内容は不定
  // ただコンストラクタが正常に動作することを確認
}

TEST(AigEdgeTest, constr1)
{
  // 入力ノード
  SizeType input_id = 234;
  AigNode node{1, input_id};

  bool inv = true;
  AigEdge h{&node, inv};

  AigEdge h_inv{&node, !inv};

  EXPECT_EQ( h_inv, ~h );
  EXPECT_EQ( h_inv, h * true );
  EXPECT_EQ( h, h * false );
  EXPECT_EQ( h_inv, h.positive_edge() );
  EXPECT_EQ( h, h.negative_edge() );

  EXPECT_EQ( &node, h.node() );
  EXPECT_EQ( inv, h.inv() );

  EXPECT_FALSE( h.is_zero() );
  EXPECT_FALSE( h.is_one() );
  EXPECT_FALSE( h.is_const() );
  EXPECT_TRUE( h.is_input() );
  EXPECT_EQ( input_id, h.input_id() );
  EXPECT_FALSE( h.is_and() );
  EXPECT_THROW( h.fanin(0),
		std::invalid_argument );
  EXPECT_THROW( h.fanin0(),
		std::invalid_argument );
  EXPECT_THROW( h.fanin1(),
		std::invalid_argument );
}

TEST(AigEdgeTest, constr2)
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

  bool inv = false;
  AigEdge h{&node2, inv};

  EXPECT_EQ( &node2, h.node() );
  EXPECT_EQ( inv, h.inv() );
  EXPECT_FALSE( h.is_zero() );
  EXPECT_FALSE( h.is_one() );
  EXPECT_FALSE( h.is_const() );
  EXPECT_FALSE( h.is_input() );
  EXPECT_THROW( h.input_id(),
		std::invalid_argument );
  EXPECT_TRUE( h.is_and() );
  EXPECT_EQ( edge0, h.fanin(0) );
  EXPECT_EQ( edge1, h.fanin(1) );
  EXPECT_EQ( edge0, h.fanin0() );
  EXPECT_EQ( edge1, h.fanin1() );
}

TEST(AigEdgeTest, zero_one)
{
  auto h0 = AigEdge::zero();
  auto h1 = AigEdge::one();

  EXPECT_EQ( h1, ~h0 );
  EXPECT_EQ( h0, h0.positive_edge() );
  EXPECT_EQ( h1, h0.negative_edge() );

  EXPECT_EQ( nullptr, h0.node() );
  EXPECT_FALSE( h0.inv() );
  EXPECT_TRUE( h0.is_zero() );
  EXPECT_FALSE( h0.is_one() );
  EXPECT_TRUE( h0.is_const() );
  EXPECT_FALSE( h0.is_input() );
  EXPECT_THROW( h0.input_id(),
		std::invalid_argument );
  EXPECT_FALSE( h0.is_and() );
  EXPECT_THROW( h0.fanin(0),
		std::invalid_argument );
  EXPECT_THROW( h0.fanin0(),
		std::invalid_argument );
  EXPECT_THROW( h0.fanin1(),
		std::invalid_argument );

  EXPECT_EQ( h0, ~h1 );
  EXPECT_EQ( h0, h1.positive_edge() );
  EXPECT_EQ( h1, h1.negative_edge() );
}

END_NAMESPACE_YM_AIG
