
/// @file Bdd_test.cc
/// @brief Bdd のユニットテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/Bdd.h"
#include "ym/BddMgr.h"
#include "BddTest.h"


BEGIN_NAMESPACE_YM

TEST_F(BddTest, invalid1)
{
  // 不正値になっているはず．
  Bdd bdd;

  EXPECT_TRUE( bdd.is_invalid() );
  EXPECT_FALSE( bdd.is_valid() );
  EXPECT_FALSE( bdd.is_zero() );
  EXPECT_FALSE( bdd.is_one() );
}

TEST_F(BddTest, invalid2)
{
  // 不正値になっているはず．
  Bdd bdd = Bdd::invalid();

  EXPECT_TRUE( bdd.is_invalid() );
  EXPECT_FALSE( bdd.is_valid() );
  EXPECT_FALSE( bdd.is_zero() );
  EXPECT_FALSE( bdd.is_one() );
}

TEST_F(BddTest, copy_constructor)
{
  const char* exp_str = "1101";
  Bdd bdd = from_truth(exp_str);

  Bdd bdd1{bdd};
  check(bdd1, exp_str);
}

TEST_F(BddTest, copy_assignment)
{
  const char* exp_str = "1101";
  Bdd bdd = from_truth(exp_str);

  Bdd bdd1;
  bdd1 = bdd;
  check(bdd1, exp_str);
}

TEST_F(BddTest, invert1)
{
  const char* src_str = "1101";
  Bdd bdd = from_truth(src_str);

  Bdd bdd1 = bdd.invert();
  const char* exp_str = "0010";
  check(bdd1, exp_str);
}

TEST_F(BddTest, invert_int1)
{
  const char* src_str = "1101";
  Bdd bdd = from_truth(src_str);

  bdd.invert_int();
  const char* exp_str = "0010";
  check(bdd, exp_str);
}

TEST_F(BddTest, invert2)
{
  const char* src_str = "1101";
  Bdd bdd = from_truth(src_str);

  Bdd bdd1 = ~bdd;
  const char* exp_str = "0010";
  check(bdd1, exp_str);
}

TEST_F(BddTest, pol_inv1)
{
  const char* src_str = "1101";
  Bdd bdd = from_truth(src_str);

  Bdd bdd1 = bdd * true;
  const char* exp_str = "0010";
  check(bdd1, exp_str);
}

TEST_F(BddTest, pol_inv_int1)
{
  const char* src_str = "1101";
  Bdd bdd = from_truth(src_str);

  bdd *= true;
  const char* exp_str = "0010";
  check(bdd, exp_str);
}

TEST_F(BddTest, pol_inv2)
{
  const char* src_str = "1101";
  Bdd bdd = from_truth(src_str);

  Bdd bdd1 = bdd * false;
  check(bdd1, src_str);
}

TEST_F(BddTest, pol_inv_int2)
{
  const char* src_str = "1101";
  Bdd bdd = from_truth(src_str);

  bdd *= false;
  check(bdd, src_str);
}

TEST_F(BddTest, and_op1)
{
  Bdd var1 = literal(0);
  Bdd var2 = literal(1);
  Bdd bdd = var1.and_op(var2);

  check(bdd, "1000");
}

TEST_F(BddTest, and_int1)
{
  Bdd var1 = literal(0);
  Bdd var2 = literal(1);
  Bdd bdd = var1.and_int(var2);

  check(var1, "1000");
  check(bdd, "1000");
}

TEST_F(BddTest, and_op2)
{
  Bdd var1 = literal(0);
  Bdd var2 = literal(1);
  Bdd bdd = var1 & var2;

  check(bdd, "1000");
}

TEST_F(BddTest, and_int2)
{
  Bdd var1 = literal(0);
  Bdd var2 = literal(1);
  Bdd bdd = var1 &= var2;

  check(var1, "1000");
  check(bdd, "1000");
}

TEST_F(BddTest, and_op3)
{

  Bdd var1 = literal(0);
  BddMgr mgr2;
  Bdd var2 = mgr2.literal(VarId{1});
  Bdd bdd = var1.and_op(var2);

  check(bdd, "1000");
}

TEST_F(BddTest, and_int3)
{

  Bdd var1 = literal(0);
  BddMgr mgr2;
  Bdd var2 = mgr2.literal(VarId{1});
  Bdd bdd = var1.and_int(var2);

  check(var1, "1000");
  check(bdd, "1000");
}

TEST_F(BddTest, and_op4)
{

  Bdd var1 = literal(0);
  BddMgr mgr2;
  Bdd var2 = mgr2.literal(VarId{1});
  Bdd bdd = var1 & var2;

  check(bdd, "1000");
}

TEST_F(BddTest, and_int4)
{

  Bdd var1 = literal(0);
  BddMgr mgr2;
  Bdd var2 = mgr2.literal(VarId{1});
  Bdd bdd = var1 &= var2;

  check(var1, "1000");
  check(bdd, "1000");
}

TEST_F(BddTest, or_op1)
{
  Bdd var1 = literal(0);
  Bdd var2 = literal(1);
  Bdd bdd = var1.or_op(var2);

  check(bdd, "1110");
}

TEST_F(BddTest, or_int1)
{
  Bdd var1 = literal(0);
  Bdd var2 = literal(1);
  Bdd bdd = var1.or_int(var2);

  check(var1, "1110");
  check(bdd, "1110");
}

TEST_F(BddTest, or_op2)
{
  Bdd var1 = literal(0);
  Bdd var2 = literal(1);
  Bdd bdd = var1 | var2;

  check(bdd, "1110");
}

TEST_F(BddTest, or_int2)
{
  Bdd var1 = literal(0);
  Bdd var2 = literal(1);
  Bdd bdd = var1 |= var2;

  check(var1, "1110");
  check(bdd, "1110");
}

TEST_F(BddTest, or_op3)
{
  Bdd var1 = literal(0);
  BddMgr mgr2;
  Bdd var2 = mgr2.literal(VarId{1});
  Bdd bdd = var1.or_op(var2);

  check(bdd, "1110");
}

TEST_F(BddTest, or_int3)
{
  Bdd var1 = literal(0);
  BddMgr mgr2;
  Bdd var2 = mgr2.literal(VarId{1});
  Bdd bdd = var1.or_int(var2);

  check(var1, "1110");
  check(bdd, "1110");
}

TEST_F(BddTest, or_op4)
{
  Bdd var1 = literal(0);
  BddMgr mgr2;
  Bdd var2 = mgr2.literal(VarId{1});
  Bdd bdd = var1 | var2;

  check(bdd, "1110");
}

TEST_F(BddTest, or_int4)
{
  Bdd var1 = literal(0);
  BddMgr mgr2;
  Bdd var2 = mgr2.literal(VarId{1});
  Bdd bdd = var1 |= var2;

  check(var1, "1110");
  check(bdd, "1110");
}

TEST_F(BddTest, xor_op1)
{
  Bdd var1 = literal(0);
  Bdd var2 = literal(1);
  Bdd bdd = var1.xor_op(var2);

  check(bdd, "0110");
}

TEST_F(BddTest, xor_int1)
{
  Bdd var1 = literal(0);
  Bdd var2 = literal(1);
  Bdd bdd = var1.xor_int(var2);

  check(var1, "0110");
  check(bdd, "0110");
}

TEST_F(BddTest, xor_op2)
{
  Bdd var1 = literal(0);
  Bdd var2 = literal(1);
  Bdd bdd = var1 ^ var2;

  check(bdd, "0110");
}

TEST_F(BddTest, xor_int2)
{
  Bdd var1 = literal(0);
  Bdd var2 = literal(1);
  Bdd bdd = var1 ^= var2;

  check(var1, "0110");
  check(bdd, "0110");
}

TEST_F(BddTest, xor_op3)
{
  Bdd var1 = literal(0);
  BddMgr mgr2;
  Bdd var2 = mgr2.literal(VarId{1});
  Bdd bdd = var1.xor_op(var2);

  check(bdd, "0110");
}

TEST_F(BddTest, xor_int3)
{
  Bdd var1 = literal(0);
  BddMgr mgr2;
  Bdd var2 = mgr2.literal(VarId{1});
  Bdd bdd = var1.xor_int(var2);

  check(var1, "0110");
  check(bdd, "0110");
}

TEST_F(BddTest, xor_op4)
{
  Bdd var1 = literal(0);
  BddMgr mgr2;
  Bdd var2 = mgr2.literal(VarId{1});
  Bdd bdd = var1 ^ var2;

  check(bdd, "0110");
}

TEST_F(BddTest, xor_int4)
{
  Bdd var1 = literal(0);
  BddMgr mgr2;
  Bdd var2 = mgr2.literal(VarId{1});
  Bdd bdd = var1 ^= var2;

  check(var1, "0110");
  check(bdd, "0110");
}

TEST_F(BddTest, complex_expr1)
{
  Bdd var1 = literal(0);
  Bdd var2 = literal(1);
  Bdd var3 = literal(2);
  Bdd bdd = (var1 & ~var2) | var3;

  check(bdd, "10111010");
}

TEST_F(BddTest, complex_expr2)
{
  Bdd var1 = literal(0);
  Bdd var2 = literal(1);
  Bdd var3 = literal(2);
  BddMgr mgr2;
  Bdd var4 = mgr2.literal(VarId{1});
  Bdd bdd = (var1 & ~var2) | (var3 & ~var4);

  check(bdd, "00110010");
}

TEST_F(BddTest, ite1)
{
  Bdd bdd1 = from_truth("10101010");
  Bdd bdd2 = from_truth("11001100");
  Bdd bdd3 = from_truth("11110000");

  Bdd bdd = ite(bdd1, bdd2, bdd3);

  Bdd exp_bdd = bdd1 & bdd2 | ~bdd1 & bdd3;
  EXPECT_EQ( exp_bdd, bdd );
}

TEST_F(BddTest, cofactor1)
{
  const char* src_str = "10010111";
  Bdd bdd = from_truth(src_str);

  Bdd bdd1 = bdd.cofactor(VarId{0}, false);

  const char* exp_str = "10011001";
  check(bdd1, exp_str);
}

TEST_F(BddTest, cofactor_int1)
{
  const char* src_str = "10010111";
  Bdd bdd = from_truth(src_str);

  Bdd bdd1 = bdd.cofactor_int(VarId{0}, false);

  const char* exp_str = "10011001";
  check(bdd, exp_str);
  check(bdd1, exp_str);
}

TEST_F(BddTest, cofactor2)
{
  const char* src_str = "10010111";
  Bdd bdd = from_truth(src_str);

  Literal lit{VarId{0}, true};
  Bdd bdd1 = bdd.cofactor(lit);

  const char* exp_str = "01110111";
  check(bdd1, exp_str);
}

TEST_F(BddTest, cofactor_int2)
{
  const char* src_str = "10010111";
  Bdd bdd = from_truth(src_str);

  Literal lit{VarId{0}, true};
  Bdd bdd1 = bdd.cofactor_int(lit);

  const char* exp_str = "01110111";
  check(bdd, exp_str);
  check(bdd1, exp_str);
}

TEST_F(BddTest, cofactor3)
{
  const char* src_str = "10010111";
  Bdd bdd = from_truth(src_str);

  Literal lit{VarId{0}, true};

  Bdd bdd1 = bdd / lit;

  const char* exp_str = "01110111";
  check(bdd1, exp_str);
}

TEST_F(BddTest, cofactor_int3)
{
  const char* src_str = "10010111";
  Bdd bdd = from_truth(src_str);

  Literal lit{VarId{0}, true};
  Bdd bdd1 = bdd /= lit;

  const char* exp_str = "01110111";
  check(bdd, exp_str);
  check(bdd1, exp_str);
}

TEST_F(BddTest, cofactor4)
{
  const char* src_str = "10010111";
  Bdd bdd = from_truth(src_str);


  Bdd lit1 = literal(0);
  Bdd cube = lit1;

  Bdd bdd1 = bdd / cube;

  const char* exp_str = "10011001";
  check(bdd1, exp_str);
}

TEST_F(BddTest, cofactor_int4)
{
  const char* src_str = "10010111";
  Bdd bdd = from_truth(src_str);


  Bdd lit1 = literal(0);
  Bdd cube = lit1;
  Bdd bdd1 = bdd /= cube;

  const char* exp_str = "10011001";
  check(bdd, exp_str);
  check(bdd1, exp_str);
}

TEST_F(BddTest, cofactor5)
{
  const char* src_str = "10010111";
  Bdd bdd = from_truth(src_str);

  BddMgr mgr2;
  Bdd lit1 = mgr2.literal(VarId{0});
  Bdd lit2 = mgr2.literal(VarId{2});
  Bdd cube = ~lit1 & lit2;

  Bdd bdd1 = bdd.cofactor(cube);

  const char* exp_str = "00110011";
  check(bdd1, exp_str);
}

TEST_F(BddTest, cofactor_int5)
{
  const char* src_str = "10010111";
  Bdd bdd = from_truth(src_str);

  BddMgr mgr2;
  Bdd lit1 = mgr2.literal(VarId{0});
  Bdd lit2 = mgr2.literal(VarId{2});
  Bdd cube = ~lit1 & lit2;

  Bdd bdd1 = bdd.cofactor_int(cube);

  const char* exp_str = "00110011";
  check(bdd, exp_str);
  check(bdd1, exp_str);
}

TEST_F(BddTest, cofactor7)
{
  const char* src_str = "10010111";
  Bdd bdd = from_truth(src_str);

  Bdd lit1 = literal(0);
  Bdd lit2 = literal(2);
  Bdd cube = ~lit1 & lit2;
  Bdd bdd1 = bdd / cube;
  const char* exp_str = "00110011";
  check(bdd1, exp_str);
}

TEST_F(BddTest, cofactor_int7)
{
  const char* src_str = "10010111";
  Bdd bdd = from_truth(src_str);

  Bdd lit1 = literal(0);
  Bdd lit2 = literal(2);
  Bdd cube = ~lit1 & lit2;

  Bdd bdd1 = bdd /= cube;

  const char* exp_str = "00110011";
  check(bdd, exp_str);
  check(bdd1, exp_str);
}

TEST_F(BddTest, cofactor8)
{
  const char* src_str = "10010111";
  Bdd bdd = from_truth(src_str);

  BddMgr mgr2;
  Bdd lit1 = mgr2.literal(VarId{0});
  Bdd lit2 = mgr2.literal(VarId{2});
  Bdd cube = ~lit1 & lit2;
  Bdd bdd1 = bdd / cube;
  const char* exp_str = "00110011";
  check(bdd1, exp_str);
}

TEST_F(BddTest, cofactor_int8)
{
  const char* src_str = "10010111";
  Bdd bdd = from_truth(src_str);

  BddMgr mgr2;
  Bdd lit1 = mgr2.literal(VarId{0});
  Bdd lit2 = mgr2.literal(VarId{2});
  Bdd cube = ~lit1 & lit2;

  Bdd bdd1 = bdd /= cube;

  const char* exp_str = "00110011";
  check(bdd, exp_str);
  check(bdd1, exp_str);
}

TEST_F(BddTest, support_cup1)
{
  Bdd lit1 = literal(0);
  Bdd lit2 = literal(1);
  Bdd lit3 = literal(2);
  Bdd lit4 = literal(3);

  Bdd bdd1 = lit1 & lit2 & lit3;
  Bdd bdd2 = lit2 & lit4;

  Bdd bdd = support_cup(bdd1, bdd2);

  const char* exp_str = "1000000000000000";
  check(bdd, exp_str);
}

TEST_F(BddTest, support_cup_int1)
{
  Bdd lit1 = literal(0);
  Bdd lit2 = literal(1);
  Bdd lit3 = literal(2);
  Bdd lit4 = literal(3);

  Bdd bdd1 = lit1 & lit2 & lit3;
  Bdd bdd2 = lit2 & lit4;

  Bdd bdd = bdd1.support_cup_int(bdd2);

  const char* exp_str = "1000000000000000";
  check(bdd1, exp_str);
  check(bdd, exp_str);
}

TEST_F(BddTest, support_cap1)
{
  Bdd lit1 = literal(0);
  Bdd lit2 = literal(1);
  Bdd lit3 = literal(2);
  Bdd lit4 = literal(3);

  Bdd bdd1 = lit1 & lit2 & lit3;
  Bdd bdd2 = lit2 & lit4;

  Bdd bdd = support_cap(bdd1, bdd2);

  const char* exp_str = "1111000011110000";
  check(bdd, exp_str);
}

TEST_F(BddTest, support_cap_int1)
{
  Bdd lit1 = literal(0);
  Bdd lit2 = literal(1);
  Bdd lit3 = literal(2);
  Bdd lit4 = literal(3);

  Bdd bdd1 = lit1 & lit2 & lit3;
  Bdd bdd2 = lit2 & lit4;

  Bdd bdd = bdd1.support_cap_int(bdd2);

  const char* exp_str = "1111000011110000";
  check(bdd1, exp_str);
  check(bdd, exp_str);
}

TEST_F(BddTest, support_diff1)
{
  Bdd lit1 = literal(0);
  Bdd lit2 = literal(1);
  Bdd lit3 = literal(2);
  Bdd lit4 = literal(3);

  Bdd bdd1 = lit1 & lit2 & lit3;
  Bdd bdd2 = lit2 & lit4;

  Bdd bdd = support_diff(bdd1, bdd2);

  const char* exp_str = "1100110000000000";
  check(bdd, exp_str);
}

TEST_F(BddTest, support_diff_int1)
{
  Bdd lit1 = literal(0);
  Bdd lit2 = literal(1);
  Bdd lit3 = literal(2);
  Bdd lit4 = literal(3);

  Bdd bdd1 = lit1 & lit2 & lit3;
  Bdd bdd2 = lit2 & lit4;

  Bdd bdd = bdd1.support_diff_int(bdd2);

  const char* exp_str = "1100110000000000";
  check(bdd1, exp_str);
  check(bdd, exp_str);
}

TEST_F(BddTest, is_cube1)
{
  Bdd lit1 = literal(0);
  Bdd lit2 = literal(1);
  Bdd lit3 = literal(2);

  Bdd bdd1 = lit1 & lit2 & lit3;

  EXPECT_TRUE( bdd1.is_cube() );
}

TEST_F(BddTest, is_cube2)
{
  Bdd lit1 = literal(0);
  Bdd lit2 = literal(1);
  Bdd lit3 = literal(2);

  Bdd bdd1 = lit1 & lit2 | lit3;

  EXPECT_FALSE( bdd1.is_cube() );
}

TEST_F(BddTest, is_posicube1)
{
  Bdd lit1 = literal(0);
  Bdd lit2 = literal(1);
  Bdd lit3 = literal(2);

  Bdd bdd1 = lit1 & lit2 & lit3;

  EXPECT_TRUE( bdd1.is_posicube() );
}

TEST_F(BddTest, is_posicube2)
{
  Bdd lit1 = literal(0);
  Bdd lit2 = literal(1);
  Bdd lit3 = literal(2);

  Bdd bdd1 = lit1 & lit2 & ~lit3;

  EXPECT_FALSE( bdd1.is_posicube() );
}

TEST_F(BddTest, is_posicube3)
{
  Bdd lit1 = literal(0);
  Bdd lit2 = literal(1);
  Bdd lit3 = literal(2);

  Bdd bdd1 = lit1 & lit2 | lit3;

  EXPECT_FALSE( bdd1.is_posicube() );
}

TEST_F(BddTest, check_sup1)
{
  Bdd bdd = from_truth("1100");

  test_check_sup(bdd, VarId{0});
  test_check_sup(bdd, VarId{1});
}

TEST_F(BddTest, check_sym1)
{
  Bdd bdd = from_truth("111010101");

  test_check_sym(bdd, VarId{0}, VarId{1});
  test_check_sym(bdd, VarId{0}, VarId{2});
  test_check_sym(bdd, VarId{1}, VarId{2});
}

TEST_F(BddTest, check_sym2)
{
  Bdd bdd = from_truth("01001011");

  test_check_sym(bdd, VarId{0}, VarId{1});
  test_check_sym(bdd, VarId{0}, VarId{2});
  test_check_sym(bdd, VarId{1}, VarId{2});
}

TEST_F(BddTest, check_sym3)
{
  Bdd bdd = from_truth("0110");

  test_check_sym(bdd, VarId{0}, VarId{1});
}

TEST_F(BddTest, check_sym4)
{
  Bdd bdd = from_truth("01100101");

  test_check_sym(bdd, VarId{0}, VarId{1});
  test_check_sym(bdd, VarId{0}, VarId{2});
  test_check_sym(bdd, VarId{1}, VarId{2});
}

TEST_F(BddTest, check_sym5)
{
  Bdd bdd = from_truth("0100");

  test_check_sym(bdd, VarId{0}, VarId{1});
}

TEST_F(BddTest, get_support1)
{
  Bdd lit1 = literal(1);
  Bdd lit3 = literal(3);
  Bdd lit5 = literal(5);
  Bdd lit7 = literal(7);

  Bdd bdd = (lit1 & lit3) | (~lit1 & lit5) | (lit3 & ~lit7);

  Bdd sup = bdd.get_support();

  test_support(bdd, sup, 8);
}

TEST_F(BddTest, get_support2)
{
  Bdd lit1 = literal(1);
  Bdd lit2 = literal(2);
  Bdd lit3 = literal(3);

  Bdd bdd = (lit1 & lit2) | (~lit1 & lit2) | lit3;

  Bdd sup = bdd.get_support();

  test_support(bdd, sup, 4);
}

TEST_F(BddTest, get_onepath1)
{
  Bdd lit1 = literal(0);
  Bdd lit2 = literal(1);
  Bdd lit3 = literal(2);

  Bdd bdd = lit1 & ~lit2 & lit3;

  Bdd bdd1 = bdd.get_onepath();

  EXPECT_EQ( bdd, bdd1 );
}

TEST_F(BddTest, get_onepath2)
{
  Bdd bdd = mMgr.zero();

  Bdd bdd1 = bdd.get_onepath();

  EXPECT_EQ( bdd, bdd1 );
}

TEST_F(BddTest, get_onepath3)
{
  Bdd lit1 = literal(0);
  Bdd lit2 = literal(1);
  Bdd lit3 = literal(2);

  Bdd bdd = lit1 & ~lit2 | lit3;

  Bdd bdd1 = bdd.get_onepath();

  test_onepath(bdd, bdd1);
}

TEST_F(BddTest, get_zeropath1)
{
  Bdd lit1 = literal(0);
  Bdd lit2 = literal(1);
  Bdd lit3 = literal(2);

  Bdd bdd = lit1 | ~lit2 | lit3;

  Bdd bdd1 = bdd.get_zeropath();

  Bdd exp_bdd = ~lit1 & lit2 & ~lit3;
  EXPECT_EQ( exp_bdd, bdd1 );
}

TEST_F(BddTest, get_zeropath2)
{
  Bdd bdd = mMgr.one();

  Bdd bdd1 = bdd.get_zeropath();

  Bdd exp_bdd = mMgr.zero();
  EXPECT_EQ( exp_bdd, bdd1 );
}

TEST_F(BddTest, get_zeropath3)
{
  Bdd lit1 = literal(0);
  Bdd lit2 = literal(1);
  Bdd lit3 = literal(2);

  Bdd bdd = lit1 & ~lit2 | lit3;

  Bdd bdd1 = bdd.get_zeropath();

  test_zeropath(bdd, bdd1);
}

TEST_F(BddTest, root_decomp1)
{
  Bdd bdd = from_truth("1000");

  Bdd bdd0;
  Bdd bdd1;
  VarId top = bdd.root_decomp(bdd0, bdd1);

  EXPECT_EQ( VarId{0}, top );
  EXPECT_TRUE( bdd0.is_zero() );

  Bdd bdd00;
  Bdd bdd01;
  VarId dummy = bdd0.root_decomp(bdd00, bdd01);
  EXPECT_EQ( VarId::illegal(), dummy );
  EXPECT_EQ( bdd0, bdd00 );
  EXPECT_EQ( bdd0, bdd01 );

  Bdd bdd10;
  Bdd bdd11;
  VarId second = bdd1.root_decomp(bdd10, bdd11);

  EXPECT_EQ( VarId{1}, second );
  EXPECT_TRUE( bdd10.is_zero() );
  EXPECT_TRUE( bdd11.is_one() );
}

TEST_F(BddTest, root_decomp2)
{
  Bdd bdd = from_truth("1000");

  VarId top = bdd.root_var();
  Bdd bdd0 = bdd.root_cofactor0();
  Bdd bdd1 = bdd.root_cofactor1();

  EXPECT_EQ( VarId{0}, top );
  EXPECT_TRUE( bdd0.is_zero() );

  VarId dummy = bdd0.root_var();
  Bdd bdd00 = bdd0.root_cofactor0();
  Bdd bdd01 = bdd0.root_cofactor1();

  EXPECT_EQ( VarId::illegal(), dummy );
  EXPECT_EQ( bdd0, bdd00 );
  EXPECT_EQ( bdd0, bdd01 );

  VarId second = bdd1.root_var();
  Bdd bdd10 = bdd1.root_cofactor0();
  Bdd bdd11 = bdd1.root_cofactor1();

  EXPECT_EQ( VarId{1}, second );
  EXPECT_TRUE( bdd10.is_zero() );
  EXPECT_TRUE( bdd11.is_one() );
}

TEST_F(BddTest, root_inv1)
{
  Bdd bdd = mMgr.zero();

  EXPECT_FALSE( bdd.root_inv() );
}

TEST_F(BddTest, root_inv2)
{
  Bdd bdd = mMgr.one();

  EXPECT_TRUE( bdd.root_inv() );
}

TEST_F(BddTest, root_inv3)
{
  Bdd bdd = from_truth("1110");

  EXPECT_FALSE( bdd.root_inv() );
}

TEST_F(BddTest, root_inv4)
{
  Bdd bdd = from_truth("0001");

  EXPECT_TRUE( bdd.root_inv() );
}

TEST_F(BddTest, size1)
{
  Bdd bdd = mMgr.zero();

  EXPECT_EQ( 0, bdd.size() );
}

TEST_F(BddTest, size2)
{
  Bdd bdd = mMgr.one();

  EXPECT_EQ( 0, bdd.size() );
}

TEST_F(BddTest, size3)
{
  Bdd bdd = from_truth("1000");

  EXPECT_EQ( 2, bdd.size() );
}

TEST_F(BddTest, size4)
{
  Bdd bdd1 = from_truth("1100");
  Bdd bdd2 = from_truth("1010");
  Bdd bdd3 = from_truth("1000");

  EXPECT_EQ( 1, bdd1.size() );
  EXPECT_EQ( 1, bdd2.size() );
  EXPECT_EQ( 2, bdd3.size() );
  EXPECT_EQ( 3, Bdd::size({bdd1, bdd2, bdd3}) );
}

END_NAMESPACE_YM
