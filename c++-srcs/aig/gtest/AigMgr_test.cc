
/// @file AigMgr_test.cc
/// @brief AigMgr_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/AigMgr.h"


BEGIN_NAMESPACE_YM

TEST(AigMgrTest, constr)
{
  AigMgr mgr;

  EXPECT_EQ( 0, mgr.node_num() );
  EXPECT_EQ( 0, mgr.input_num() );
}

TEST(AigMgrTest, make_zero)
{
  AigMgr mgr;

  auto h = mgr.make_zero();
  EXPECT_TRUE( h.is_zero() );
  EXPECT_FALSE( h.is_one() );
  EXPECT_TRUE( h.is_const() );
  EXPECT_FALSE( h.is_input() );
  EXPECT_FALSE( h.is_and() );

  EXPECT_EQ( 0, mgr.node_num() );
  EXPECT_EQ( 0, mgr.input_num() );
  EXPECT_EQ( 0, mgr.and_num() );
}

TEST(AigMgrTest, make_one)
{
  AigMgr mgr;

  auto h = mgr.make_one();
  EXPECT_FALSE( h.is_zero() );
  EXPECT_TRUE( h.is_one() );
  EXPECT_TRUE( h.is_const() );
  EXPECT_FALSE( h.is_input() );
  EXPECT_FALSE( h.is_and() );

  EXPECT_EQ( 0, mgr.node_num() );
  EXPECT_EQ( 0, mgr.input_num() );
  EXPECT_EQ( 0, mgr.and_num() );
}

TEST(AigMgrTest, make_input)
{
  AigMgr mgr;

  auto h = mgr.make_input();
  EXPECT_FALSE( h.is_zero() );
  EXPECT_FALSE( h.is_one() );
  EXPECT_FALSE( h.is_const() );
  EXPECT_TRUE( h.is_input() );
  EXPECT_EQ( 0, h.input_id() );
  EXPECT_FALSE( h.is_and() );

  EXPECT_EQ( 1, mgr.node_num() );
  EXPECT_EQ( 1, mgr.input_num() );
  EXPECT_EQ( 0, mgr.and_num() );

  auto mgr2 = h.mgr();
  EXPECT_EQ( mgr, mgr2 );
}

TEST(AigMgrTest, and_op2)
{
  AigMgr mgr;

  auto h1 = mgr.make_input();
  auto h2 = mgr.make_input();

  auto ho = mgr.and_op({h1, h2});
  EXPECT_FALSE( ho.is_zero() );
  EXPECT_FALSE( ho.is_one() );
  EXPECT_FALSE( ho.is_const() );
  EXPECT_FALSE( ho.is_input() );
  EXPECT_TRUE( ho.is_and() );

  EXPECT_EQ( 3, mgr.node_num() );
  EXPECT_EQ( 1, mgr.and_num() );

  vector<AigBitVect> input_vals{0xCC, 0xAA};
  auto bv = ho.eval(input_vals);
  EXPECT_EQ( 0x88, bv );

  auto fanin_list = ho.ex_fanin_list();
  EXPECT_EQ( 2, fanin_list.size() );
  EXPECT_EQ( h1, fanin_list[0] );
  EXPECT_EQ( h2, fanin_list[1] );
}

TEST(AigMgrTest, and_op3)
{
  AigMgr mgr;

  auto h1 = mgr.make_input();
  auto h2 = mgr.make_input();
  auto h3 = mgr.make_input();

  auto ho = mgr.and_op({h1, h2, h3});
  EXPECT_FALSE( ho.is_zero() );
  EXPECT_FALSE( ho.is_one() );
  EXPECT_FALSE( ho.is_const() );
  EXPECT_FALSE( ho.is_input() );
  EXPECT_TRUE( ho.is_and() );

  EXPECT_EQ( 5, mgr.node_num() );
  EXPECT_EQ( 2, mgr.and_num() );

  vector<AigBitVect> input_vals{0xF0, 0xCC, 0xAA};
  auto bv = ho.eval(input_vals);
  EXPECT_EQ( 0x80, bv );

  auto fanin_list = ho.ex_fanin_list();
  EXPECT_EQ( 3, fanin_list.size() );
  EXPECT_EQ( h1, fanin_list[0] );
  EXPECT_EQ( h2, fanin_list[1] );
  EXPECT_EQ( h3, fanin_list[2] );
}

TEST(AigMgrTest, and_op4)
{
  AigMgr mgr;

  auto h1 = mgr.make_input();
  auto h2 = mgr.make_input();
  auto h3 = mgr.make_input();
  auto h4 = mgr.make_input();

  auto ho = mgr.and_op({h1, h2, h3, h4});
  EXPECT_TRUE( ho.is_and() );

  EXPECT_EQ( 7, mgr.node_num() );
  EXPECT_EQ( 3, mgr.and_num() );

  vector<AigBitVect> input_vals{0xFF00, 0xF0F0, 0xCCCC, 0xAAAA};
  auto bv = ho.eval(input_vals);
  EXPECT_EQ( 0x8000, bv );

  auto fanin_list = ho.ex_fanin_list();
  EXPECT_EQ( 4, fanin_list.size() );
  EXPECT_EQ( h1, fanin_list[0] );
  EXPECT_EQ( h2, fanin_list[1] );
  EXPECT_EQ( h3, fanin_list[2] );
  EXPECT_EQ( h4, fanin_list[3] );
}

TEST(AigMgrTest, and_op8)
{
  AigMgr mgr;

  auto h1 = mgr.make_input();
  auto h2 = mgr.make_input();
  auto h3 = mgr.make_input();
  auto h4 = mgr.make_input();
  auto h5 = mgr.make_input();
  auto h6 = mgr.make_input();
  auto h7 = mgr.make_input();
  auto h8 = mgr.make_input();

  auto ho = mgr.and_op({h1, h2, h3, h4, h5, h6, h7, h8});
  EXPECT_TRUE( ho.is_and() );

  EXPECT_EQ( 15, mgr.node_num() );
  EXPECT_EQ( 7, mgr.and_num() );

  auto fanin_list = ho.ex_fanin_list();
  ASSERT_EQ( 8, fanin_list.size() );
  EXPECT_EQ( h1, fanin_list[0] );
  EXPECT_EQ( h2, fanin_list[1] );
  EXPECT_EQ( h3, fanin_list[2] );
  EXPECT_EQ( h4, fanin_list[3] );
  EXPECT_EQ( h5, fanin_list[4] );
  EXPECT_EQ( h6, fanin_list[5] );
  EXPECT_EQ( h7, fanin_list[6] );
  EXPECT_EQ( h8, fanin_list[7] );
}

TEST(AigMgrTest, or_op3)
{
  AigMgr mgr;

  auto h1 = mgr.make_input();
  auto h2 = mgr.make_input();
  auto h3 = mgr.make_input();

  auto ho = mgr.or_op({h1, h2, h3});
  EXPECT_TRUE( ho.is_and() );

  EXPECT_EQ( 5, mgr.node_num() );
  EXPECT_EQ( 2, mgr.and_num() );

  vector<AigBitVect> input_vals{0xF0, 0xCC, 0xAA};
  auto bv = ho.eval(input_vals);
  EXPECT_EQ( 0xFE, bv );

  auto fanin_list = ho.ex_fanin_list();
  EXPECT_EQ( 3, fanin_list.size() );
  EXPECT_EQ( ~h1, fanin_list[0] );
  EXPECT_EQ( ~h2, fanin_list[1] );
  EXPECT_EQ( ~h3, fanin_list[2] );
}

TEST(AigMgrTest, or_op4)
{
  AigMgr mgr;

  auto h1 = mgr.make_input();
  auto h2 = mgr.make_input();
  auto h3 = mgr.make_input();
  auto h4 = mgr.make_input();

  auto ho = mgr.or_op({h1, h2, h3, h4});
  EXPECT_TRUE( ho.is_and() );

  EXPECT_EQ( 7, mgr.node_num() );
  EXPECT_EQ( 3, mgr.and_num() );

  vector<AigBitVect> input_vals{0xFF00, 0xF0F0, 0xCCCC, 0xAAAA};
  auto bv = ho.eval(input_vals);
  EXPECT_EQ( 0xFFFE, bv );

  auto fanin_list = ho.ex_fanin_list();
  EXPECT_EQ( 4, fanin_list.size() );
  EXPECT_EQ( ~h1, fanin_list[0] );
  EXPECT_EQ( ~h2, fanin_list[1] );
  EXPECT_EQ( ~h3, fanin_list[2] );
  EXPECT_EQ( ~h4, fanin_list[3] );
}

TEST(AigMgrTest, or_op8)
{
  AigMgr mgr;

  auto h1 = mgr.make_input();
  auto h2 = mgr.make_input();
  auto h3 = mgr.make_input();
  auto h4 = mgr.make_input();
  auto h5 = mgr.make_input();
  auto h6 = mgr.make_input();
  auto h7 = mgr.make_input();
  auto h8 = mgr.make_input();

  auto ho = mgr.or_op({h1, h2, h3, h4, h5, h6, h7, h8});
  EXPECT_TRUE( ho.is_and() );

  EXPECT_EQ( 15, mgr.node_num() );
  EXPECT_EQ( 7, mgr.and_num() );

  auto fanin_list = ho.ex_fanin_list();
  ASSERT_EQ( 8, fanin_list.size() );
  EXPECT_EQ( ~h1, fanin_list[0] );
  EXPECT_EQ( ~h2, fanin_list[1] );
  EXPECT_EQ( ~h3, fanin_list[2] );
  EXPECT_EQ( ~h4, fanin_list[3] );
  EXPECT_EQ( ~h5, fanin_list[4] );
  EXPECT_EQ( ~h6, fanin_list[5] );
  EXPECT_EQ( ~h7, fanin_list[6] );
  EXPECT_EQ( ~h8, fanin_list[7] );
}

TEST(AigMgrTest, xor_op3)
{
  AigMgr mgr;

  auto h1 = mgr.make_input();
  auto h2 = mgr.make_input();
  auto h3 = mgr.make_input();

  auto ho = mgr.xor_op({h1, h2, h3});
  EXPECT_TRUE( ho.is_and() );

  EXPECT_EQ( 9, mgr.node_num() );
  EXPECT_EQ( 6, mgr.and_num() );

  vector<AigBitVect> input_vals{0xF0, 0xCC, 0xAA};
  auto bv = ho.eval(input_vals);
  EXPECT_EQ( 0x96, bv );

  auto fanin_list = ho.ex_fanin_list();
  EXPECT_EQ( 2, fanin_list.size() );
}

TEST(AigMgrTest, xor_op4)
{
  AigMgr mgr;

  auto h1 = mgr.make_input();
  auto h2 = mgr.make_input();
  auto h3 = mgr.make_input();
  auto h4 = mgr.make_input();

  auto ho = mgr.xor_op({h1, h2, h3, h4});
  EXPECT_TRUE( ho.is_and() );

  EXPECT_EQ( 13, mgr.node_num() );
  EXPECT_EQ( 9, mgr.and_num() );

  vector<AigBitVect> input_vals{0xFF00, 0xF0F0, 0xCCCC, 0xAAAA};
  auto bv = ho.eval(input_vals);
  EXPECT_EQ( 0x6996, bv );

  auto fanin_list = ho.ex_fanin_list();
  EXPECT_EQ( 2, fanin_list.size() );
}

TEST(AigMgrTest, ex_fanin_list)
{
  AigMgr mgr;

  auto h1 = mgr.make_input();
  auto h2 = mgr.make_input();
  auto h3 = mgr.make_input();

  auto t1 = h1 & h2;
  auto t2 = h2 & h3;
  auto t3 = h3 & h1;

  auto s1 = t1 & t2;
  auto s2 = t2 & t3;

  auto o = s1 & s2;

  auto fanin_list = o.ex_fanin_list();
  EXPECT_EQ( 3, fanin_list.size() );
}

TEST(AigMgrTest, from_expr_zero)
{
  auto expr = Expr::zero();

  AigMgr mgr;

  auto h = mgr.from_expr(expr);
  EXPECT_TRUE( h.is_zero() );
}

TEST(AigMgrTest, from_expr_one)
{
  auto expr = Expr::one();

  AigMgr mgr;

  auto h = mgr.from_expr(expr);
  EXPECT_TRUE( h.is_one() );
}

TEST(AigMgrTest, from_expr_literal)
{
  SizeType varid = 10;
  bool inv = true;
  auto expr = Expr::literal(varid, inv);

  AigMgr mgr;

  auto h = mgr.from_expr(expr);
  EXPECT_TRUE( h.is_input() );
  EXPECT_EQ( varid, h.input_id() );
  EXPECT_EQ( inv, h.inv() );
}

TEST(AigMgrTest, from_expr_and)
{
  auto lit0 = Expr::literal(0);
  auto lit1 = Expr::literal(1);
  auto expr = lit0 & lit1;

  AigMgr mgr;

  auto h = mgr.from_expr(expr);
  EXPECT_FALSE( h.inv() );
  EXPECT_TRUE( h.is_and() );

  auto h0 = h.fanin0();
  EXPECT_FALSE( h0.inv() );
  EXPECT_TRUE( h0.is_input() );
  EXPECT_EQ( 0, h0.input_id() );

  auto h1 = h.fanin1();
  EXPECT_FALSE( h1.inv() );
  EXPECT_TRUE( h1.is_input() );
  EXPECT_EQ( 1, h1.input_id() );
}

TEST(AigMgrTest, from_expr_or)
{
  auto lit0 = Expr::literal(0);
  auto lit1 = Expr::literal(1);
  auto expr = lit0 | lit1;

  AigMgr mgr;

  auto h = mgr.from_expr(expr);
  EXPECT_TRUE( h.inv() );
  EXPECT_TRUE( h.is_and() );

  auto h0 = h.fanin0();
  EXPECT_TRUE( h0.inv() );
  EXPECT_TRUE( h0.is_input() );
  EXPECT_EQ( 0, h0.input_id() );

  auto h1 = h.fanin1();
  EXPECT_TRUE( h1.inv() );
  EXPECT_TRUE( h1.is_input() );
  EXPECT_EQ( 1, h1.input_id() );
}

TEST(AigMgrTest, from_expr_xor)
{
  auto lit0 = Expr::literal(0);
  auto lit1 = Expr::literal(1);
  auto expr = lit0 ^ lit1;

  AigMgr mgr;

  auto h = mgr.from_expr(expr);

  mgr.print(cout);

  vector<AigBitVect> input_vals{0xC, 0xA};
  auto bv = h.eval(input_vals);
  EXPECT_EQ( 0x6, bv );
}

TEST(AigMgrTest, sweep)
{
  AigMgr mgr;

  auto h1 = mgr.make_input();
  auto h2 = mgr.make_input();
  auto h3 = mgr.make_input();

  {
    auto ho = mgr.and_op({h1, h2, h3});
    EXPECT_TRUE( ho.is_and() );

    EXPECT_EQ( 5, mgr.node_num() );
    EXPECT_EQ( 2, mgr.and_num() );
  }
  mgr.sweep();
  EXPECT_EQ( 3, mgr.node_num() );
  EXPECT_EQ( 0, mgr.and_num() );
  {
    auto ho = mgr.and_op({h1, h2, h3});
    EXPECT_TRUE( ho.is_and() );

    mgr.print(cout);
    EXPECT_EQ( 5, mgr.node_num() );
    EXPECT_EQ( 2, mgr.and_num() );
  }
}

TEST(AigMgrTest, ref_count)
{
  AigMgr mgr;

  AigHandle ho;
  {
    auto h1 = mgr.make_input();
    auto h2 = mgr.make_input();
    auto h3 = mgr.make_input();

    ho = mgr.and_op({h1, h2, h3});
  }
  mgr.print(cout);
}

END_NAMESPACE_YM
