
/// @file AigHandle_test.cc
/// @brief AigHandle_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/AigHandle.h"
#include "ym/AigMgr.h"


BEGIN_NAMESPACE_YM_AIG

TEST(AigHandleTest, constr0)
{
  AigHandle h;
  // 内容は不定
  // ただコンストラクタが正常に動作することを確認
}

TEST(AigHandleTest, zero)
{
  auto h = AigHandle::zero();

  EXPECT_FALSE( h.inv() );
  EXPECT_TRUE( h.is_zero() );
  EXPECT_FALSE( h.is_one() );
  EXPECT_TRUE( h.is_const() );
  EXPECT_FALSE( h.is_input() );
  EXPECT_THROW( h.input_id(),
		std::invalid_argument );
  EXPECT_FALSE( h.is_and() );
  EXPECT_THROW( h.fanin(0),
		std::invalid_argument );
  EXPECT_THROW( h.fanin0(),
		std::invalid_argument );
  EXPECT_THROW( h.fanin1(),
		std::invalid_argument );
  EXPECT_THROW( h.ex_fanin_list(),
		std::invalid_argument );
  EXPECT_EQ( 0, h.index() );

  auto ih = ~h;
  EXPECT_TRUE( ih.is_one() );

  EXPECT_EQ( h, h.positive_handle() );
  EXPECT_EQ( ih, h.negative_handle() );

  EXPECT_EQ( h, h * false );
  EXPECT_EQ( ih, h * true );
}

TEST(AigHandleTest, one)
{
  auto h = AigHandle::one();

  EXPECT_TRUE( h.inv() );
  EXPECT_FALSE( h.is_zero() );
  EXPECT_TRUE( h.is_one() );
  EXPECT_TRUE( h.is_const() );
  EXPECT_FALSE( h.is_input() );
  EXPECT_THROW( h.input_id(),
		std::invalid_argument );
  EXPECT_FALSE( h.is_and() );
  EXPECT_THROW( h.fanin(0),
		std::invalid_argument );
  EXPECT_THROW( h.fanin0(),
		std::invalid_argument );
  EXPECT_THROW( h.fanin1(),
		std::invalid_argument );
  EXPECT_THROW( h.ex_fanin_list(),
		std::invalid_argument );
  EXPECT_EQ( 1, h.index() );

  auto ih = ~h;
  EXPECT_TRUE( ih.is_zero() );

  EXPECT_EQ( ih, h.positive_handle() );
  EXPECT_EQ( h, h.negative_handle() );

  EXPECT_EQ( h, h * false );
  EXPECT_EQ( ih, h * true );
}

TEST(AigHandleTest, input)
{
  AigMgr mgr;

  auto h = mgr.input(0);

  EXPECT_FALSE( h.inv() );
  EXPECT_EQ( mgr, h.mgr() );
  EXPECT_FALSE( h.is_zero() );
  EXPECT_FALSE( h.is_one() );
  EXPECT_FALSE( h.is_const() );
  EXPECT_TRUE( h.is_input() );
  EXPECT_EQ( 0, h.input_id() );
  EXPECT_FALSE( h.is_and() );
  EXPECT_THROW( h.fanin(0),
		std::invalid_argument );
  EXPECT_THROW( h.fanin0(),
		std::invalid_argument );
  EXPECT_THROW( h.fanin1(),
		std::invalid_argument );
  EXPECT_THROW( h.ex_fanin_list(),
		std::invalid_argument );
  EXPECT_EQ( 2, h.index() );

}

TEST(AigHandleTest, and1)
{
  AigMgr mgr;

  auto h1 = mgr.input(0);
  auto h2 = mgr.input(1);
  auto h3 = h1 & h2;

  EXPECT_EQ( 3, mgr.node_num() );
  EXPECT_EQ( 1, mgr.and_num() );

  EXPECT_FALSE( h3.inv() );
  EXPECT_EQ( mgr, h3.mgr() );
  EXPECT_FALSE( h3.is_zero() );
  EXPECT_FALSE( h3.is_one() );
  EXPECT_FALSE( h3.is_const() );
  EXPECT_FALSE( h3.is_input() );
  EXPECT_THROW( h3.input_id(),
		std::invalid_argument );
  EXPECT_TRUE( h3.is_and() );
  auto i1 = mgr.input(0);
  auto i2 = mgr.input(1);
  EXPECT_EQ( i1, h3.fanin(0) );
  EXPECT_EQ( i2, h3.fanin(1) );
  EXPECT_EQ( i1, h3.fanin0() );
  EXPECT_EQ( i2, h3.fanin1() );

  auto fanin_list = h3.ex_fanin_list();
  EXPECT_EQ( 2, fanin_list.size() );
  EXPECT_EQ( h1, fanin_list[0] );
  EXPECT_EQ( h2, fanin_list[1] );

  std::unordered_map<SizeType, AigBitVect> ival_dict{{0, 0xC},
						     {1, 0xA}};
  auto output_val = h3.eval(ival_dict);
  EXPECT_EQ( 0x8, output_val );
}

TEST(AigHandleTest, and_bad1)
{
  AigMgr mgr;
  AigMgr mgr2;

  auto h1 = mgr.input(0);
  auto h2 = mgr2.input(1);

  EXPECT_THROW( h1 & h2,
		std::invalid_argument );
}

TEST(AigHandleTest, and_bad2)
{
  AigMgr mgr;
  AigMgr mgr2;

  auto h1 = mgr2.input(0);
  auto h2 = mgr.input(1);

  EXPECT_THROW( h1 & h2,
		std::invalid_argument );
}

TEST(AigHandleTest, and_zero1)
{
  AigMgr mgr;

  auto h1 = mgr.input(0);
  auto h2 = AigHandle::zero();

  auto h3 = h1 & h2;
  EXPECT_TRUE( h3.is_zero() );
}

TEST(AigHandleTest, and_zero2)
{
  AigMgr mgr;

  auto h1 = AigHandle::zero();
  auto h2 = mgr.input(0);

  auto h3 = h1 & h2;
  EXPECT_TRUE( h3.is_zero() );
}

TEST(AigHandleTest, and_one1)
{
  AigMgr mgr;

  auto h1 = mgr.input(0);
  auto h2 = AigHandle::one();

  auto h3 = h1 & h2;
  EXPECT_EQ( h1, h3 );
}

TEST(AigHandleTest, and_one2)
{
  AigMgr mgr;

  auto h1 = AigHandle::one();
  auto h2 = mgr.input(0);

  auto h3 = h1 & h2;
  EXPECT_EQ( h2, h3 );
}

TEST(AigHandleTest, and2)
{
  AigMgr mgr;

  auto h1 = mgr.input(0);
  auto h2 = mgr.input(1);

  auto h3 = h1 & h2;
  EXPECT_TRUE( h3.is_and() );

  auto h4 = h1 & h2;
  EXPECT_EQ( h3, h4 );

  EXPECT_EQ( 3, mgr.node_num() );
  EXPECT_EQ( 1, mgr.and_num() );
}

TEST(AigHandleTest, iand1)
{
  AigMgr mgr;

  auto h1 = mgr.input(0);
  auto h2 = mgr.input(1);
  h1 &= h2;

  EXPECT_FALSE( h1.inv() );
  EXPECT_EQ( mgr, h1.mgr() );
  EXPECT_FALSE( h1.is_zero() );
  EXPECT_FALSE( h1.is_one() );
  EXPECT_FALSE( h1.is_const() );
  EXPECT_FALSE( h1.is_input() );
  EXPECT_THROW( h1.input_id(),
		std::invalid_argument );
  EXPECT_TRUE( h1.is_and() );
  auto i1 = mgr.input(0);
  auto i2 = mgr.input(1);
  EXPECT_EQ( i1, h1.fanin(0) );
  EXPECT_EQ( i2, h1.fanin(1) );
  EXPECT_EQ( i1, h1.fanin0() );
  EXPECT_EQ( i2, h1.fanin1() );
}

TEST(AigHandleTest, or1)
{
  AigMgr mgr;

  auto h1 = mgr.input(0);
  auto h2 = mgr.input(1);
  auto h3 = h1 | h2;

  EXPECT_EQ( 3, mgr.node_num() );
  EXPECT_EQ( 1, mgr.and_num() );

  EXPECT_TRUE( h3.inv() );
  EXPECT_EQ( mgr, h3.mgr() );
  EXPECT_FALSE( h3.is_zero() );
  EXPECT_FALSE( h3.is_one() );
  EXPECT_FALSE( h3.is_const() );
  EXPECT_FALSE( h3.is_input() );
  EXPECT_THROW( h3.input_id(),
		std::invalid_argument );
  EXPECT_TRUE( h3.is_and() );
  auto i1 = mgr.input(0);
  auto i2 = mgr.input(1);
  EXPECT_EQ( ~i1, h3.fanin(0) );
  EXPECT_EQ( ~i2, h3.fanin(1) );
  EXPECT_EQ( ~i1, h3.fanin0() );
  EXPECT_EQ( ~i2, h3.fanin1() );

  auto fanin_list = h3.ex_fanin_list();
  EXPECT_EQ( 2, fanin_list.size() );
  EXPECT_EQ( ~h1, fanin_list[0] );
  EXPECT_EQ( ~h2, fanin_list[1] );

  std::unordered_map<SizeType, AigBitVect> ival_dict{{0, 0xC},
						     {1, 0xA}};
  auto bv = h3.eval(ival_dict);
  EXPECT_EQ( 0xE, bv );
}

TEST(AigHandleTest, or_zero1)
{
  AigMgr mgr;

  auto h1 = mgr.input(0);
  auto h2 = AigHandle::zero();

  auto h3 = h1 | h2;
  EXPECT_EQ( h1, h3 );
}

TEST(AigHandleTest, or_zero2)
{
  AigMgr mgr;

  auto h1 = AigHandle::zero();
  auto h2 = mgr.input(0);

  auto h3 = h1 | h2;
  EXPECT_EQ( h2, h3 );
}

TEST(AigHandleTest, or_one1)
{
  AigMgr mgr;

  auto h1 = mgr.input(0);
  auto h2 = AigHandle::one();

  auto h3 = h1 | h2;
  EXPECT_TRUE( h3.is_one() );
}

TEST(AigHandleTest, or_one2)
{
  AigMgr mgr;

  auto h1 = AigHandle::one();
  auto h2 = mgr.input(0);

  auto h3 = h1 | h2;
  EXPECT_TRUE( h3.is_one() );
}

TEST(AigHandleTest, or_bad1)
{
  AigMgr mgr;
  AigMgr mgr2;

  auto h1 = mgr.input(0);
  auto h2 = mgr2.input(1);

  EXPECT_THROW( h1 | h2,
		std::invalid_argument );
}

TEST(AigHandleTest, or_bad2)
{
  AigMgr mgr;
  AigMgr mgr2;

  auto h1 = mgr2.input(0);
  auto h2 = mgr.input(1);

  EXPECT_THROW( h1 | h2,
		std::invalid_argument );
}

TEST(AigHandleTest, or2)
{
  AigMgr mgr;

  auto h1 = mgr.input(0);
  auto h2 = mgr.input(1);

  auto h3 = h1 | h2;
  EXPECT_TRUE( h3.is_and() );

  auto h4 = h1 | h2;
  EXPECT_EQ( h3, h4 );

  EXPECT_EQ( 3, mgr.node_num() );
  EXPECT_EQ( 1, mgr.and_num() );
}

TEST(AigHandleTest, ior1)
{
  AigMgr mgr;

  auto h1 = mgr.input(0);
  auto h2 = mgr.input(1);
  h1 |= h2;

  EXPECT_TRUE( h1.inv() );
  EXPECT_EQ( mgr, h1.mgr() );
  EXPECT_FALSE( h1.is_zero() );
  EXPECT_FALSE( h1.is_one() );
  EXPECT_FALSE( h1.is_const() );
  EXPECT_FALSE( h1.is_input() );
  EXPECT_THROW( h1.input_id(),
		std::invalid_argument );
  EXPECT_TRUE( h1.is_and() );
  auto i1 = mgr.input(0);
  auto i2 = mgr.input(1);
  EXPECT_EQ( ~i1, h1.fanin(0) );
  EXPECT_EQ( ~i2, h1.fanin(1) );
  EXPECT_EQ( ~i1, h1.fanin0() );
  EXPECT_EQ( ~i2, h1.fanin1() );
}

TEST(AigHandleTest, xor1)
{
  AigMgr mgr;

  auto h1 = mgr.input(0);
  auto h2 = mgr.input(1);

  auto h3 = h1 ^ h2;
  EXPECT_TRUE( h3.is_and() );

  EXPECT_EQ( 5, mgr.node_num() );
  EXPECT_EQ( 3, mgr.and_num() );

  auto fanin_list = h3.ex_fanin_list();
  EXPECT_EQ( 2, fanin_list.size() );

  std::unordered_map<SizeType, AigBitVect> ival_dict{{0, 0xC},
						     {1, 0xA}};
  auto bv = h3.eval(ival_dict);
  EXPECT_EQ( 0x6, bv );
}

TEST(AigHandleTest, xor_zero1)
{
  AigMgr mgr;

  auto h1 = mgr.input(0);
  auto h2 = AigHandle::zero();

  auto h3 = h1 ^ h2;
  EXPECT_EQ( h1, h3 );
}

TEST(AigHandleTest, xor_zero2)
{
  AigMgr mgr;

  auto h1 = AigHandle::zero();
  auto h2 = mgr.input(0);

  auto h3 = h1 ^ h2;
  EXPECT_EQ( h2, h3 );
}

TEST(AigHandleTest, xor_one1)
{
  AigMgr mgr;

  auto h1 = mgr.input(0);
  auto h2 = AigHandle::one();

  auto h3 = h1 ^ h2;
  EXPECT_EQ( ~h1, h3 );
}

TEST(AigHandleTest, xor_one2)
{
  AigMgr mgr;

  auto h1 = AigHandle::one();
  auto h2 = mgr.input(0);

  auto h3 = h1 ^ h2;
  EXPECT_EQ( ~h2, h3 );
}

TEST(AigHandleTest, xor_bad1)
{
  AigMgr mgr;
  AigMgr mgr2;

  auto h1 = mgr.input(0);
  auto h2 = mgr2.input(1);

  EXPECT_THROW( h1 ^ h2,
		std::invalid_argument );
}

TEST(AigHandleTest, xor_bad2)
{
  AigMgr mgr;
  AigMgr mgr2;

  auto h1 = mgr2.input(0);
  auto h2 = mgr.input(1);

  EXPECT_THROW( h1 ^ h2,
		std::invalid_argument );
}

TEST(AigHandleTest, xor2)
{
  AigMgr mgr;

  auto h1 = mgr.input(0);
  auto h2 = mgr.input(1);

  auto h3 = h1 ^ h2;
  EXPECT_TRUE( h3.is_and() );

  auto h4 = h1 ^ h2;
  EXPECT_EQ( h3, h4 );

  EXPECT_EQ( 5, mgr.node_num() );
  EXPECT_EQ( 3, mgr.and_num() );
}

TEST(AigHandleTest, cofactor)
{

  auto lit0 = Expr::literal(0);
  auto lit1 = Expr::literal(1);
  auto lit2 = Expr::literal(2);

  auto expr = lit0 & lit1 | ~lit0 & lit2;

  AigMgr mgr;

  auto h = mgr.from_expr(expr);

  auto h_lit0 = mgr.from_expr(lit0);
  auto h1 = h.cofactor({h_lit0});

  auto h_lit1 = mgr.from_expr(lit1);
  EXPECT_EQ( h_lit1, h1 );
}

END_NAMESPACE_YM_AIG
