
/// @file Bdd_test.cc
/// @brief Bdd のユニットテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022, 2023 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/Bdd.h"
#include "ym/BddVarSet.h"
#include "ym/BddMgr.h"
#include "ym/BinEnc.h"
#include "ym/BinDec.h"
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

TEST_F(BddTest, variable1)
{
  auto bdd = literal(0);
  check(bdd, "10");
}

TEST_F(BddTest, variable2)
{
  auto bdd = ~literal(1);
  check(bdd, "0101");
}

TEST_F(BddTest, variable3)
{
  auto bdd = literal(2);
  check(bdd, "10101010");
}

TEST_F(BddTest, variable4)
{
  auto bdd = ~literal(3);
  check(bdd, "0101010101010101");
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

TEST_F(BddTest, invert_invalid)
{
  Bdd bdd; // 不正値となっているはず．

  EXPECT_TRUE( bdd.is_invalid() );

  Bdd bdd1 = ~bdd;
  EXPECT_TRUE( bdd1.is_invalid() );
  // 結果も不正値だが例外は送出されない．
}

TEST_F(BddTest, invert_int_invalid)
{
  Bdd bdd; // 不正値となっているはず．

  EXPECT_TRUE( bdd.is_invalid() );

  bdd.invert_int();
  EXPECT_TRUE( bdd.is_invalid() );
  // 結果も不正値だが例外は送出されない．
}

TEST_F(BddTest, pol_inv1)
{
  const char* src_str = "1101";
  Bdd bdd = from_truth(src_str);

  Bdd bdd1 = bdd ^ true;
  const char* exp_str = "0010";
  check(bdd1, exp_str);
}

TEST_F(BddTest, pol_inv_int1)
{
  const char* src_str = "1101";
  Bdd bdd = from_truth(src_str);

  bdd ^= true;
  const char* exp_str = "0010";
  check(bdd, exp_str);
}

TEST_F(BddTest, pol_inv1_invalid)
{
  Bdd bdd; // 不正値となっているはず．

  EXPECT_TRUE( bdd.is_invalid() );

  Bdd bdd1 = bdd ^ true;
  EXPECT_TRUE( bdd1.is_invalid() );
  // 結果も不正値だが例外は送出されない．
}

TEST_F(BddTest, pol_inv1_int_invalid)
{
  Bdd bdd; // 不正値となっているはず．

  EXPECT_TRUE( bdd.is_invalid() );

  bdd ^= true;
  EXPECT_TRUE( bdd.is_invalid() );
  // 結果も不正値だが例外は送出されない．
}

TEST_F(BddTest, pol_inv2)
{
  const char* src_str = "1101";
  Bdd bdd = from_truth(src_str);

  Bdd bdd1 = bdd ^ false;
  check(bdd1, src_str);
}

TEST_F(BddTest, pol_inv_int2)
{
  const char* src_str = "1101";
  Bdd bdd = from_truth(src_str);

  bdd ^= false;
  check(bdd, src_str);
}

TEST_F(BddTest, pol_inv2_invalid)
{
  Bdd bdd; // 不正値となっているはず．

  EXPECT_TRUE( bdd.is_invalid() );

  Bdd bdd1 = bdd ^ false;
  EXPECT_TRUE( bdd1.is_invalid() );
  // 結果も不正値だが例外は送出されない．
}

TEST_F(BddTest, pol_inv2_int_invalid)
{
  Bdd bdd; // 不正値となっているはず．

  EXPECT_TRUE( bdd.is_invalid() );

  bdd ^= false;
  EXPECT_TRUE( bdd.is_invalid() );
  // 結果も不正値だが例外は送出されない．
}

TEST_F(BddTest, and_op1)
{
  Bdd var1 = literal(0);
  Bdd var2 = literal(1);
  Bdd bdd = var1.and_op(var2);

  check(bdd, "1000");
}

TEST_F(BddTest, and_op2)
{
  Bdd var1 = literal(0);
  Bdd var2 = literal(1);
  Bdd bdd = var1 & var2;

  check(bdd, "1000");
}

TEST_F(BddTest, and_op_invalid1)
{
  Bdd var1; // 不正値
  Bdd var2 = literal(1);
  EXPECT_THROW( var1.and_op(var2), std::logic_error );
}

TEST_F(BddTest, and_op_invalid2)
{
  Bdd var1 = literal(0);
  Bdd var2; // 不正値
  EXPECT_THROW( var1.and_op(var2), std::logic_error );
}

TEST_F(BddTest, and_op_invalid3)
{

  Bdd var1 = literal(0);
  BddMgr mgr2;
  auto var2 = mgr2.variable(1);
  EXPECT_THROW( var1.and_op(var2), std::logic_error );
}

TEST_F(BddTest, and_op2_invalid1)
{
  Bdd var1; // 不正値
  Bdd var2 = literal(1);
  EXPECT_THROW( var1 & var2, std::logic_error );
}

TEST_F(BddTest, and_op2_invalid2)
{
  Bdd var1 = literal(0);
  Bdd var2; // 不正値
  EXPECT_THROW( var1 & var2, std::logic_error );
}

TEST_F(BddTest, and_op2_invalid3)
{
  Bdd var1 = literal(0);
  BddMgr mgr2;
  auto var2 = mgr2.variable(1);
  EXPECT_THROW( var1 & var2, std::logic_error );
}

TEST_F(BddTest, and_int1)
{
  Bdd var1 = literal(0);
  Bdd var2 = literal(1);
  Bdd bdd = var1.and_int(var2);

  check(var1, "1000");
  check(bdd, "1000");
}

TEST_F(BddTest, and_int_invalid1)
{
  Bdd var1; // 不正値
  Bdd var2 = literal(1);
  EXPECT_THROW( var1.and_int(var2), std::logic_error );
}

TEST_F(BddTest, and_int_invalid2)
{
  Bdd var1 = literal(0);
  Bdd var2; // 不正値
  EXPECT_THROW( var1.and_int(var2), std::logic_error );
}

TEST_F(BddTest, and_int_invalid3)
{
  Bdd var1 = literal(0);
  BddMgr mgr2;
  auto var2 = mgr2.variable(1);
  EXPECT_THROW( var1.and_int(var2), std::logic_error );
}

TEST_F(BddTest, and_int2)
{
  Bdd var1 = literal(0);
  Bdd var2 = literal(1);
  Bdd bdd = var1 &= var2;

  check(var1, "1000");
  check(bdd, "1000");
}

TEST_F(BddTest, and_int2_invalid1)
{
  Bdd var1; // 不正値
  Bdd var2 = literal(1);
  EXPECT_THROW( var1 &= var2, std::logic_error );
}

TEST_F(BddTest, and_int2_invalid2)
{
  Bdd var1 = literal(0);
  Bdd var2; // 不正値
  EXPECT_THROW( var1 &= var2, std::logic_error );
}

TEST_F(BddTest, and_int2_invalid3)
{
  Bdd var1 = literal(0);
  BddMgr mgr2;
  auto var2 = mgr2.variable(1);
  EXPECT_THROW( var1 &= var2, std::logic_error );
}

TEST_F(BddTest, or_op1)
{
  Bdd var1 = literal(0);
  Bdd var2 = literal(1);
  Bdd bdd = var1.or_op(var2);

  check(bdd, "1110");
}

TEST_F(BddTest, or_op_invalid1)
{
  Bdd var1; // 不正値
  Bdd var2 = literal(1);
  EXPECT_THROW( var1.or_op(var2), std::logic_error );
}

TEST_F(BddTest, or_op_invalid2)
{
  Bdd var1 = literal(0);
  Bdd var2; // 不正値
  EXPECT_THROW( var1.or_op(var2), std::logic_error );
}

TEST_F(BddTest, or_op_invalid3)
{
  Bdd var1 = literal(0);
  BddMgr mgr2;
  auto var2 = mgr2.variable(1);
  EXPECT_THROW( var1.or_op(var2), std::logic_error );
}

TEST_F(BddTest, or_int1)
{
  Bdd var1 = literal(0);
  Bdd var2 = literal(1);
  Bdd bdd = var1.or_int(var2);

  check(var1, "1110");
  check(bdd, "1110");
}

TEST_F(BddTest, or_int_invalid1)
{
  Bdd var1; // 不正値
  Bdd var2 = literal(1);
  EXPECT_THROW( var1.or_int(var2), std::logic_error );
}

TEST_F(BddTest, or_int_invalid2)
{
  Bdd var1 = literal(0);
  Bdd var2; // 不正値
  EXPECT_THROW( var1.or_int(var2), std::logic_error );
}

TEST_F(BddTest, or_int_invalid3)
{
  Bdd var1 = literal(0);
  BddMgr mgr2;
  auto var2 = mgr2.variable(1);
  EXPECT_THROW( var1.or_int(var2), std::logic_error );
}

TEST_F(BddTest, or_op2)
{
  Bdd var1 = literal(0);
  Bdd var2 = literal(1);
  Bdd bdd = var1 | var2;

  check(bdd, "1110");
}

TEST_F(BddTest, or_op2_invalid1)
{
  Bdd var1; // 不正値
  Bdd var2 = literal(1);
  EXPECT_THROW( var1 | var2, std::logic_error );
}

TEST_F(BddTest, or_op2_invalid2)
{
  Bdd var1 = literal(0);
  Bdd var2; // 不正値
  EXPECT_THROW( var1 | var2, std::logic_error );
}

TEST_F(BddTest, or_op2_invalid3)
{
  Bdd var1 = literal(0);
  BddMgr mgr2;
  auto var2 = mgr2.variable(1);
  EXPECT_THROW( var1 | var2, std::logic_error );
}

TEST_F(BddTest, or_int2)
{
  Bdd var1 = literal(0);
  Bdd var2 = literal(1);
  Bdd bdd = var1 |= var2;

  check(var1, "1110");
  check(bdd, "1110");
}

TEST_F(BddTest, or_int2_invalid1)
{
  Bdd var1; // 不正値
  Bdd var2 = literal(1);
  EXPECT_THROW( var1 |= var2, std::logic_error );
}

TEST_F(BddTest, or_int2_invalid2)
{
  Bdd var1 = literal(0);
  Bdd var2; // 不正値
  EXPECT_THROW( var1 |= var2, std::logic_error );
}

TEST_F(BddTest, or_int2_invalid3)
{
  Bdd var1 = literal(0);
  BddMgr mgr2;
  auto var2 = mgr2.variable(1);
  EXPECT_THROW( var1 |= var2, std::logic_error );
}

TEST_F(BddTest, xor_op1)
{
  Bdd var1 = literal(0);
  Bdd var2 = literal(1);
  Bdd bdd = var1.xor_op(var2);

  check(bdd, "0110");
}

TEST_F(BddTest, xor_op_invalid1)
{
  Bdd var1; // 不正値
  Bdd var2 = literal(1);
  EXPECT_THROW( var1.xor_op(var2), std::logic_error );
}

TEST_F(BddTest, xor_op_invalid2)
{
  Bdd var1 = literal(0);
  Bdd var2; // 不正値
  EXPECT_THROW( var1.xor_op(var2), std::logic_error );
}

TEST_F(BddTest, xor_op_invalid3)
{
  Bdd var1 = literal(0);
  BddMgr mgr2;
  auto var2 = mgr2.variable(1);
  EXPECT_THROW( var1.xor_op(var2), std::logic_error );
}

TEST_F(BddTest, xor_int1)
{
  Bdd var1 = literal(0);
  Bdd var2 = literal(1);
  Bdd bdd = var1.xor_int(var2);

  check(var1, "0110");
  check(bdd, "0110");
}

TEST_F(BddTest, xor_int_invalid1)
{
  Bdd var1; // 不正値
  Bdd var2 = literal(1);
  EXPECT_THROW( var1.xor_int(var2), std::logic_error );
}

TEST_F(BddTest, xor_int_invalid2)
{
  Bdd var1 = literal(0);
  Bdd var2; // 不正値
  EXPECT_THROW( var1.xor_int(var2), std::logic_error );
}

TEST_F(BddTest, xor_int_invalid3)
{
  Bdd var1 = literal(0);
  BddMgr mgr2;
  auto var2 = mgr2.variable(1);
  EXPECT_THROW( var1.xor_int(var2), std::logic_error );
}

TEST_F(BddTest, xor_op2)
{
  Bdd var1 = literal(0);
  Bdd var2 = literal(1);
  Bdd bdd = var1 ^ var2;

  check(bdd, "0110");
}

TEST_F(BddTest, xor_op2_invalid1)
{
  Bdd var1; // 不正値
  Bdd var2 = literal(1);
  EXPECT_THROW( var1 ^ var2, std::logic_error );
}

TEST_F(BddTest, xor_op2_invalid2)
{
  Bdd var1 = literal(0);
  Bdd var2; // 不正値
  EXPECT_THROW( var1 ^ var2, std::logic_error );
}

TEST_F(BddTest, xor_op2_invalid3)
{
  Bdd var1 = literal(0);
  BddMgr mgr2;
  auto var2 = mgr2.variable(1);
  EXPECT_THROW( var1 ^ var2, std::logic_error );
}

TEST_F(BddTest, xor_int2)
{
  Bdd var1 = literal(0);
  Bdd var2 = literal(1);
  Bdd bdd = var1 ^= var2;

  check(var1, "0110");
  check(bdd, "0110");
}

TEST_F(BddTest, xor_int2_invalid1)
{
  Bdd var1; // 不正値
  Bdd var2 = literal(1);
  EXPECT_THROW( var1 ^= var2, std::logic_error );
}

TEST_F(BddTest, xor_int2_invalid2)
{
  Bdd var1 = literal(0);
  Bdd var2; // 不正値
  EXPECT_THROW( var1 ^= var2, std::logic_error );
}

TEST_F(BddTest, xor_int2_invalid3)
{
  Bdd var1 = literal(0);
  BddMgr mgr2;
  auto var2 = mgr2.variable(1);
  EXPECT_THROW( var1 ^= var2, std::logic_error );
}

TEST_F(BddTest, xor3)
{
  Bdd var1 = literal(0);
  Bdd var2 = literal(1);
  Bdd var3 = literal(2);
  Bdd bdd = var1 ^ var2 ^ var3;

  check(bdd, "10010110");
}

TEST_F(BddTest, complex_expr1)
{
  Bdd var1 = literal(0);
  Bdd var2 = literal(1);
  Bdd var3 = literal(2);
  Bdd bdd = (var1 & ~var2) | var3;

  check(bdd, "10111010");
}

TEST_F(BddTest, complex_expr_invalid1)
{
  Bdd var1 = literal(0);
  Bdd var2 = literal(1);
  Bdd var3 = literal(2);
  BddMgr mgr2;
  auto var4 = mgr2.variable(1);
  EXPECT_THROW( (var1 & ~var2) | (var3 & ~var4), std::logic_error );
}

TEST_F(BddTest, ite1)
{
  Bdd bdd1 = from_truth("10101010");
  Bdd bdd2 = from_truth("11001100");
  Bdd bdd3 = from_truth("11110000");

  Bdd bdd = Bdd::ite(bdd1, bdd2, bdd3);

  Bdd exp_bdd = bdd1 & bdd2 | ~bdd1 & bdd3;
  EXPECT_EQ( exp_bdd, bdd );
}

TEST_F(BddTest, ite_invalid1)
{
  Bdd bdd1; // 不正値
  Bdd bdd2 = from_truth("11001100");
  Bdd bdd3 = from_truth("11110000");

  EXPECT_THROW( Bdd::ite(bdd1, bdd2, bdd3), std::logic_error );
}

TEST_F(BddTest, ite_invalid2)
{
  Bdd bdd1 = from_truth("10101010");
  Bdd bdd2; // 不正値
  Bdd bdd3 = from_truth("11110000");

  EXPECT_THROW( Bdd::ite(bdd1, bdd2, bdd3), std::logic_error );
}

TEST_F(BddTest, ite_invalid3)
{
  Bdd bdd1 = from_truth("10101010");
  Bdd bdd2 = from_truth("11001100");
  Bdd bdd3; // 不正値

  EXPECT_THROW( Bdd::ite(bdd1, bdd2, bdd3), std::logic_error );
}

TEST_F(BddTest, cofactor1)
{
  const char* src_str = "10010111";
  Bdd bdd = from_truth(src_str);

  auto var = variable(0);
  Bdd bdd1 = bdd.cofactor(var, false);

  const char* exp_str = "10011001";
  check(bdd1, exp_str);
}

TEST_F(BddTest, cofactor1_invalid1)
{
  Bdd bdd; // 不正値

  auto var = variable(0);
  EXPECT_THROW( bdd.cofactor(var, false), std::logic_error );
}

TEST_F(BddTest, cofactor_int1)
{
  const char* src_str = "10010111";
  Bdd bdd = from_truth(src_str);

  auto var = variable(0);
  Bdd bdd1 = bdd.cofactor_int(var, false);

  const char* exp_str = "10011001";
  check(bdd, exp_str);
  check(bdd1, exp_str);
}

TEST_F(BddTest, cofactor_int1_invalid1)
{
  Bdd bdd; // 不正値

  auto var = variable(0);
  EXPECT_THROW( bdd.cofactor_int(var, false), std::logic_error );
}

TEST_F(BddTest, cofactor2)
{
  const char* src_str = "10010111";
  Bdd bdd = from_truth(src_str);

  auto var = variable(0);
  auto lit = var.negalit();
  Bdd bdd1 = bdd.cofactor(lit);

  const char* exp_str = "01110111";
  check(bdd1, exp_str);
}

TEST_F(BddTest, cofactor2_invalid1)
{
  Bdd bdd; // 不正値

  auto var = variable(0);
  auto lit = var.negalit();
  EXPECT_THROW( bdd.cofactor(lit), std::logic_error );
}

TEST_F(BddTest, cofactor2_invalid2)
{
  const char* src_str = "10010111";
  Bdd bdd = from_truth(src_str);

  BddMgr mgr2;
  auto var1 = mgr2.variable(0);
  auto var2 = mgr2.variable(2);
  Bdd cube = ~var1 & var2;

  EXPECT_THROW( bdd.cofactor(cube), std::logic_error );
}

TEST_F(BddTest, cofactor_int2)
{
  const char* src_str = "10010111";
  Bdd bdd = from_truth(src_str);

  auto var = variable(0);
  auto lit = var.negalit();
  Bdd bdd1 = bdd.cofactor_int(lit);

  const char* exp_str = "01110111";
  check(bdd, exp_str);
  check(bdd1, exp_str);
}

TEST_F(BddTest, cofactor_int2_invalid1)
{
  Bdd bdd; // 不正値

  auto var = variable(0);
  auto lit = var.negalit();
  EXPECT_THROW( bdd.cofactor_int(lit), std::logic_error );
}

TEST_F(BddTest, cofactor_int2_invalid2)
{
  const char* src_str = "10010111";
  Bdd bdd = from_truth(src_str);

  BddMgr mgr2;
  auto var1 = mgr2.variable(0);
  auto var2 = mgr2.variable(2);
  Bdd cube = ~var1 & var2;

  EXPECT_THROW( bdd.cofactor_int(cube), std::logic_error );
}

TEST_F(BddTest, cofactor3)
{
  const char* src_str = "10010111";
  Bdd bdd = from_truth(src_str);

  auto var = variable(0);
  auto lit = var.negalit();

  Bdd bdd1 = bdd / lit;

  const char* exp_str = "01110111";
  check(bdd1, exp_str);
}

TEST_F(BddTest, cofactor3_invalid)
{
  Bdd bdd; // 不正値

  auto var = variable(0);
  auto lit = var.negalit();
  EXPECT_THROW( bdd / lit, std::logic_error );
}

TEST_F(BddTest, cofactor_int3)
{
  const char* src_str = "10010111";
  Bdd bdd = from_truth(src_str);

  auto var = variable(0);
  auto lit = var.negalit();
  Bdd bdd1 = bdd /= lit;

  const char* exp_str = "01110111";
  check(bdd, exp_str);
  check(bdd1, exp_str);
}

TEST_F(BddTest, cofactor_int3_invalid)
{
  Bdd bdd; // 不正値

  auto var = variable(0);
  auto lit = var.negalit();
  EXPECT_THROW( bdd /= lit, std::logic_error );
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

TEST_F(BddTest, cofactor4_invalid1)
{
  Bdd bdd; // 不正値

  Bdd lit1 = literal(0);
  Bdd cube = lit1;

  EXPECT_THROW( bdd / cube, std::logic_error );
}

TEST_F(BddTest, cofactor4_invalid2)
{
  const char* src_str = "10010111";
  Bdd bdd = from_truth(src_str);

  Bdd cube; // 不正値

  EXPECT_THROW( bdd / cube, std::logic_error );
}

TEST_F(BddTest, cofactor4_invalid3)
{
  const char* src_str = "10010111";
  Bdd bdd = from_truth(src_str);

  Bdd not_cube = literal(0) ^ literal(1); // キューブでない．

  EXPECT_THROW( bdd / not_cube, std::logic_error );
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

TEST_F(BddTest, cofactor_int4_invalid1)
{
  Bdd bdd; // 不正値

  Bdd lit1 = literal(0);
  Bdd cube = lit1;

  EXPECT_THROW( bdd /= cube, std::logic_error );
}

TEST_F(BddTest, cofactor_int4_invalid2)
{
  const char* src_str = "10010111";
  Bdd bdd = from_truth(src_str);

  Bdd cube; // 不正値

  EXPECT_THROW( bdd /= cube, std::logic_error );
}

TEST_F(BddTest, cofactor_int4_invalid3)
{
  const char* src_str = "10010111";
  Bdd bdd = from_truth(src_str);

  Bdd not_cube = literal(0) ^ literal(1); // キューブでない．

  EXPECT_THROW( bdd /= not_cube, std::logic_error );
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

TEST_F(BddTest, cofactor7_invalid1)
{
  const char* src_str = "10010111";
  Bdd bdd = from_truth(src_str);

  BddMgr mgr2;
  auto var1 = mgr2.variable(0);
  auto var2 = mgr2.variable(2);
  Bdd cube = ~var1 & var2;
  EXPECT_THROW({
      Bdd bdd1 = bdd / cube;
    }, std::invalid_argument );
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

TEST_F(BddTest, cofactor_int7_invalid1)
{
  const char* src_str = "10010111";
  Bdd bdd = from_truth(src_str);

  BddMgr mgr2;
  auto var1 = mgr2.variable(0);
  auto var2 = mgr2.variable(2);
  Bdd cube = ~var1 & var2;
  EXPECT_THROW({
      Bdd bdd1 = bdd /= cube;
    }, std::invalid_argument );
}

TEST_F(BddTest, support_cup1)
{
  Bdd lit1 = literal(0);
  Bdd lit2 = literal(1);
  Bdd lit3 = literal(2);
  Bdd lit4 = literal(3);

  Bdd bdd1 = lit1 & lit2 & lit3;
  Bdd bdd2 = lit2 & lit4;

  BddVarSet sup1 = bdd1.get_support();
  BddVarSet sup2 = bdd2.get_support();
  BddVarSet sup = sup1 + sup2;
  auto var_list = sup.to_varlist();
  EXPECT_EQ( 4, var_list.size() );
  EXPECT_EQ( 0, var_list[0].id() );
  EXPECT_EQ( 1, var_list[1].id() );
  EXPECT_EQ( 2, var_list[2].id() );
  EXPECT_EQ( 3, var_list[3].id() );
}

TEST_F(BddTest, support_cup1_invalid1)
{
  Bdd lit1 = literal(0);
  Bdd lit2 = literal(1);
  Bdd lit3 = literal(2);
  Bdd lit4 = literal(3);

  Bdd bdd1 = lit1 & lit2 & lit3;
  Bdd bdd2 = lit2 & lit4;

  BddVarSet sup1; // 不正値
  BddVarSet sup2 = bdd2.get_support();
  EXPECT_THROW({
      auto sup = sup1 + sup2;
    }, std::invalid_argument );
}

TEST_F(BddTest, support_cup1_invalid2)
{
  Bdd lit1 = literal(0);
  Bdd lit2 = literal(1);
  Bdd lit3 = literal(2);
  Bdd lit4 = literal(3);

  Bdd bdd1 = lit1 & lit2 & lit3;
  Bdd bdd2 = lit2 & lit4;

  BddVarSet sup1 = bdd1.get_support();
  BddVarSet sup2; // 不正値
  EXPECT_THROW({
      auto sup = sup1 + sup2;
    }, std::invalid_argument );
}

TEST_F(BddTest, support_cup_int1)
{
  auto lit1 = literal(0);
  auto lit2 = literal(1);
  auto lit3 = literal(2);
  auto lit4 = literal(3);

  auto bdd1 = lit1 & lit2 & lit3;
  auto bdd2 = lit2 & lit4;

  auto sup1 = bdd1.get_support();
  auto sup2 = bdd2.get_support();
  auto sup = sup1 += sup2;
  auto var_list = sup1.to_varlist();
  EXPECT_EQ( 4, var_list.size() );
  EXPECT_EQ( 0, var_list[0].id() );
  EXPECT_EQ( 1, var_list[1].id() );
  EXPECT_EQ( 2, var_list[2].id() );
  EXPECT_EQ( 3, var_list[3].id() );

  EXPECT_EQ( sup1, sup );
}

TEST_F(BddTest, support_cup_int1_invalid1)
{
  auto lit1 = literal(0);
  auto lit2 = literal(1);
  auto lit3 = literal(2);
  auto lit4 = literal(3);

  auto bdd1 = lit1 & lit2 & lit3;
  auto bdd2 = lit2 & lit4;

  BddVarSet sup1; // 不正値
  BddVarSet sup2 = bdd2.get_support();
  EXPECT_THROW({
      auto sup = sup1 += sup2;
    }, std::invalid_argument );
}

TEST_F(BddTest, support_cup_int1_invalid2)
{
  auto lit1 = literal(0);
  auto lit2 = literal(1);
  auto lit3 = literal(2);
  auto lit4 = literal(3);

  auto bdd1 = lit1 & lit2 & lit3;
  auto bdd2 = lit2 & lit4;

  BddVarSet sup1 = bdd1.get_support();
  BddVarSet sup2; // 不正値
  EXPECT_THROW({
      auto sup = sup1 += sup2;
    }, std::invalid_argument );
}

TEST_F(BddTest, support_cap1)
{
  auto lit1 = literal(0);
  auto lit2 = literal(1);
  auto lit3 = literal(2);
  auto lit4 = literal(3);

  auto bdd1 = lit1 & lit2 & lit3;
  auto bdd2 = lit2 & lit4;

  auto sup1 = bdd1.get_support();
  auto sup2 = bdd2.get_support();
  auto sup = sup1 & sup2;
  auto var_list = sup.to_varlist();

  EXPECT_EQ( 1, var_list.size() );
  EXPECT_EQ( 1, var_list[0].id() );
}

TEST_F(BddTest, support_cap1_invalid1)
{
  auto lit1 = literal(0);
  auto lit2 = literal(1);
  auto lit3 = literal(2);
  auto lit4 = literal(3);

  auto bdd1 = lit1 & lit2 & lit3;
  auto bdd2 = lit2 & lit4;

  BddVarSet sup1; // 不正値
  BddVarSet sup2 = bdd2.get_support();
  EXPECT_THROW({
      auto sup = sup1 & sup2;
    }, std::invalid_argument );
}

TEST_F(BddTest, support_cap1_invalid2)
{
  auto lit1 = literal(0);
  auto lit2 = literal(1);
  auto lit3 = literal(2);
  auto lit4 = literal(3);

  auto bdd1 = lit1 & lit2 & lit3;
  auto bdd2 = lit2 & lit4;

  BddVarSet sup1 = bdd1.get_support();
  BddVarSet sup2; // 不正値
  EXPECT_THROW({
      auto sup = sup1 & sup2;
    }, std::invalid_argument );
}

TEST_F(BddTest, support_cap_int1)
{
  auto lit1 = literal(0);
  auto lit2 = literal(1);
  auto lit3 = literal(2);
  auto lit4 = literal(3);

  auto bdd1 = lit1 & lit2 & lit3;
  auto bdd2 = lit2 & lit4;

  auto sup1 = bdd1.get_support();
  auto sup2 = bdd2.get_support();
  auto sup = sup1 &= sup2;
  auto var_list = sup1.to_varlist();

  EXPECT_EQ( 1, var_list.size() );
  EXPECT_EQ( 1, var_list[0].id() );

  EXPECT_EQ( sup1, sup );
}

TEST_F(BddTest, support_cap_int1_invalid1)
{
  auto lit1 = literal(0);
  auto lit2 = literal(1);
  auto lit3 = literal(2);
  auto lit4 = literal(3);

  auto bdd1 = lit1 & lit2 & lit3;
  auto bdd2 = lit2 & lit4;

  BddVarSet sup1; // 不正値
  BddVarSet sup2 = bdd2.get_support();
  EXPECT_THROW({
      auto sup = sup1 &= sup2;
    }, std::invalid_argument );
}

TEST_F(BddTest, support_cap_int1_invalid2)
{
  auto lit1 = literal(0);
  auto lit2 = literal(1);
  auto lit3 = literal(2);
  auto lit4 = literal(3);

  auto bdd1 = lit1 & lit2 & lit3;
  auto bdd2 = lit2 & lit4;

  BddVarSet sup1 = bdd1.get_support();
  BddVarSet sup2; // 不正値
  EXPECT_THROW({
      auto sup = sup1 &= sup2;
    }, std::invalid_argument );
}

TEST_F(BddTest, support_diff1)
{
  auto lit1 = literal(0);
  auto lit2 = literal(1);
  auto lit3 = literal(2);
  auto lit4 = literal(3);

  auto bdd1 = lit1 & lit2 & lit3;
  auto bdd2 = lit2 & lit4;

  auto sup1 = bdd1.get_support();
  auto sup2 = bdd2.get_support();
  auto sup = sup1 - sup2;
  auto var_list = sup.to_varlist();

  EXPECT_EQ( 2, var_list.size() );
  EXPECT_EQ( 0, var_list[0].id() );
  EXPECT_EQ( 2, var_list[1].id() );
}

TEST_F(BddTest, support_diff1_invalid1)
{
  auto lit1 = literal(0);
  auto lit2 = literal(1);
  auto lit3 = literal(2);
  auto lit4 = literal(3);

  auto bdd1 = lit1 & lit2 & lit3;
  auto bdd2 = lit2 & lit4;

  BddVarSet sup1; // 不正値
  BddVarSet sup2 = bdd2.get_support();
  EXPECT_THROW({
      auto sup = sup1 - sup2;
    }, std::invalid_argument );
}

TEST_F(BddTest, support_diff1_invalid2)
{
  auto lit1 = literal(0);
  auto lit2 = literal(1);
  auto lit3 = literal(2);
  auto lit4 = literal(3);

  auto bdd1 = lit1 & lit2 & lit3;
  auto bdd2 = lit2 & lit4;

  BddVarSet sup1 = bdd1.get_support();
  BddVarSet sup2; // 不正値
  EXPECT_THROW({
      auto sup = sup1 - sup2;
    }, std::invalid_argument );
}

TEST_F(BddTest, support_diff_int1)
{
  auto lit1 = literal(0);
  auto lit2 = literal(1);
  auto lit3 = literal(2);
  auto lit4 = literal(3);

  auto bdd1 = lit1 & lit2 & lit3;
  auto bdd2 = lit2 & lit4;

  auto sup1 = bdd1.get_support();
  auto sup2 = bdd2.get_support();
  auto sup = sup1 -= sup2;
  auto var_list = sup.to_varlist();

  EXPECT_EQ( 2, var_list.size() );
  EXPECT_EQ( 0, var_list[0].id() );
  EXPECT_EQ( 2, var_list[1].id() );

  EXPECT_EQ( sup1, sup );
}

TEST_F(BddTest, support_diff_int1_invalid1)
{
  auto lit1 = literal(0);
  auto lit2 = literal(1);
  auto lit3 = literal(2);
  auto lit4 = literal(3);

  auto bdd1 = lit1 & lit2 & lit3;
  auto bdd2 = lit2 & lit4;

  BddVarSet sup1; // 不正値
  BddVarSet sup2 = bdd2.get_support();
  EXPECT_THROW({
      auto sup = sup1 -= sup2;
    }, std::invalid_argument );
}

TEST_F(BddTest, support_diff_int1_invalid2)
{
  auto lit1 = literal(0);
  auto lit2 = literal(1);
  auto lit3 = literal(2);
  auto lit4 = literal(3);

  auto bdd1 = lit1 & lit2 & lit3;
  auto bdd2 = lit2 & lit4;

  BddVarSet sup1 = bdd1.get_support();
  BddVarSet sup2; // 不正値
  EXPECT_THROW({
      auto sup = sup1 -= sup2;
    }, std::invalid_argument );
}

TEST_F(BddTest, support_diff2)
{
  auto lit1 = literal(0);
  auto lit2 = literal(1);
  auto lit3 = literal(2);
  auto lit4 = literal(3);

  auto bdd1 = lit2 & lit3 & lit4;
  auto bdd2 = lit1 & lit2 & lit3 & lit4;

  auto sup1 = bdd1.get_support();
  auto sup2 = bdd2.get_support();
  auto sup = sup1 - sup2;
  auto var_list = sup.to_varlist();

  EXPECT_EQ( 0, var_list.size() );
}

TEST_F(BddTest, support_check_intersect1)
{
  auto lit1 = literal(0);
  auto lit2 = literal(1);
  auto lit3 = literal(2);
  auto lit4 = literal(3);

  auto bdd1 = lit2 & lit3 & lit4;
  auto bdd2 = lit1 & lit2 & lit3 & lit4;

  auto sup1 = bdd1.get_support();
  auto sup2 = bdd2.get_support();
  bool result = sup1 && sup2;

  EXPECT_TRUE( result );
}

TEST_F(BddTest, support_check_intersect1_invalid1)
{
  auto lit1 = literal(0);
  auto lit2 = literal(1);
  auto lit3 = literal(2);
  auto lit4 = literal(3);

  auto bdd1 = lit1 & lit2 & lit3;
  auto bdd2 = lit2 & lit4;

  BddVarSet sup1; // 不正値
  BddVarSet sup2 = bdd2.get_support();
  EXPECT_THROW({
      auto r = sup1 && sup2;
    }, std::invalid_argument );
}

TEST_F(BddTest, support_check_intersect1_invalid2)
{
  auto lit1 = literal(0);
  auto lit2 = literal(1);
  auto lit3 = literal(2);
  auto lit4 = literal(3);

  auto bdd1 = lit1 & lit2 & lit3;
  auto bdd2 = lit2 & lit4;

  BddVarSet sup1 = bdd1.get_support();
  BddVarSet sup2; // 不正値
  EXPECT_THROW({
      auto r = sup1 && sup2;
    }, std::invalid_argument );
}

TEST_F(BddTest, support_check_intersect2)
{
  auto lit1 = literal(0);
  auto lit2 = literal(1);
  auto lit3 = literal(2);
  auto lit4 = literal(3);

  auto bdd1 = lit1 & lit3;
  auto bdd2 = lit2 & lit4;

  auto sup1 = bdd1.get_support();
  auto sup2 = bdd2.get_support();
  auto result = sup1 && sup2;

  EXPECT_FALSE( result );
}

TEST_F(BddTest, is_cube1)
{
  auto lit1 = literal(0);
  auto lit2 = literal(1);
  auto lit3 = literal(2);

  auto bdd1 = lit1 & lit2 & lit3;

  EXPECT_TRUE( bdd1.is_cube() );
}

TEST_F(BddTest, is_cube2)
{
  auto lit1 = literal(0);
  auto lit2 = literal(1);
  auto lit3 = literal(2);

  auto bdd1 = lit1 & lit2 | lit3;

  EXPECT_FALSE( bdd1.is_cube() );
}

TEST_F(BddTest, is_posicube1)
{
  auto lit1 = literal(0);
  auto lit2 = literal(1);
  auto lit3 = literal(2);

  auto bdd1 = lit1 & lit2 & lit3;

  EXPECT_TRUE( bdd1.is_posicube() );
}

TEST_F(BddTest, is_posicube2)
{
  auto lit1 = literal(0);
  auto lit2 = literal(1);
  auto lit3 = literal(2);

  auto bdd1 = lit1 & lit2 & ~lit3;

  EXPECT_FALSE( bdd1.is_posicube() );
}

TEST_F(BddTest, is_posicube3)
{
  auto lit1 = literal(0);
  auto lit2 = literal(1);
  auto lit3 = literal(2);

  auto bdd1 = lit1 & lit2 | lit3;

  EXPECT_FALSE( bdd1.is_posicube() );
}

TEST_F(BddTest, check_sup1)
{
  auto bdd = from_truth("1100");

  test_check_sup(bdd, 0);
  test_check_sup(bdd, 1);
}

TEST_F(BddTest, check_sym1)
{
  auto bdd = from_truth("11101010");

  test_check_sym(bdd, 0, 1);
  test_check_sym(bdd, 0, 2);
  test_check_sym(bdd, 1, 2);
}

TEST_F(BddTest, check_sym2)
{
  auto bdd = from_truth("01001011");

  test_check_sym(bdd, 0, 1);
  test_check_sym(bdd, 0, 2);
  test_check_sym(bdd, 1, 2);
}

TEST_F(BddTest, check_sym3)
{
  auto bdd = from_truth("0110");

  test_check_sym(bdd, 0, 1);
}

TEST_F(BddTest, check_sym4)
{
  auto bdd = from_truth("01100101");

  test_check_sym(bdd, 0, 1);
  test_check_sym(bdd, 0, 2);
  test_check_sym(bdd, 1, 2);
}

TEST_F(BddTest, check_sym5)
{
  auto bdd = from_truth("0100");

  test_check_sym(bdd, 0, 1);
}

TEST_F(BddTest, get_support1)
{
  auto lit1 = literal(1);
  auto lit3 = literal(3);
  auto lit5 = literal(5);
  auto lit7 = literal(7);

  auto bdd = (lit1 & lit3) | (~lit1 & lit5) | (lit3 & ~lit7);

  auto sup = bdd.get_support();

  test_support(bdd, sup, 8);
}

TEST_F(BddTest, get_support_invalid1)
{
  Bdd bdd1; // 不正値

  EXPECT_THROW({
      auto sup1 = bdd1.get_support();
    }, std::invalid_argument );
}

TEST_F(BddTest, get_support2)
{
  auto lit1 = literal(1);
  auto lit2 = literal(2);
  auto lit3 = literal(3);

  auto bdd = (lit1 & lit2) | (~lit1 & lit2) | lit3;

  auto sup = bdd.get_support();

  test_support(bdd, sup, 4);
}

TEST_F(BddTest, get_onepath1)
{
  auto lit1 = literal(0);
  auto lit2 = literal(1);
  auto lit3 = literal(2);

  auto bdd = lit1 & ~lit2 & lit3;

  auto bdd1 = bdd.get_onepath();

  EXPECT_EQ( bdd, bdd1 );
}

TEST_F(BddTest, get_onepath_invalid1)
{
  Bdd bdd; // 不正値
  EXPECT_THROW({
      auto bdd1 = bdd.get_onepath();
    }, std::invalid_argument );
}

TEST_F(BddTest, get_onepath2)
{
  auto bdd = mMgr.zero();

  auto bdd1 = bdd.get_onepath();

  EXPECT_EQ( bdd, bdd1 );
}

TEST_F(BddTest, get_onepath3)
{
  auto lit1 = literal(0);
  auto lit2 = literal(1);
  auto lit3 = literal(2);

  auto bdd = lit1 & ~lit2 | lit3;

  auto bdd1 = bdd.get_onepath();

  test_onepath(bdd, bdd1);
}

TEST_F(BddTest, get_zeropath1)
{
  auto lit1 = literal(0);
  auto lit2 = literal(1);
  auto lit3 = literal(2);

  auto bdd = lit1 | ~lit2 | lit3;

  auto bdd1 = bdd.get_zeropath();

  auto exp_bdd = ~lit1 & lit2 & ~lit3;

  EXPECT_EQ( exp_bdd, bdd1 );
}

TEST_F(BddTest, get_zeropath_invalid1)
{
  Bdd bdd; // 不正値
  EXPECT_THROW({
      auto bdd1 = bdd.get_zeropath();
    }, std::invalid_argument );
}

TEST_F(BddTest, get_zeropath2)
{
  auto bdd = mMgr.one();

  auto bdd1 = bdd.get_zeropath();

  auto exp_bdd = mMgr.zero();

  EXPECT_EQ( exp_bdd, bdd1 );
}

TEST_F(BddTest, get_zeropath3)
{
  auto lit1 = literal(0);
  auto lit2 = literal(1);
  auto lit3 = literal(2);

  auto bdd = lit1 & ~lit2 | lit3;

  auto bdd1 = bdd.get_zeropath();

  test_zeropath(bdd, bdd1);
}

TEST_F(BddTest, root_decomp1)
{
  auto bdd = from_truth("1000");

  Bdd bdd0;
  Bdd bdd1;
  auto top = bdd.root_decomp(bdd0, bdd1);

  EXPECT_EQ( 0, top.id() );
  EXPECT_TRUE( bdd0.is_zero() );

  Bdd bdd00;
  Bdd bdd01;
  auto dummy = bdd0.root_decomp(bdd00, bdd01);
  EXPECT_TRUE( dummy.is_invalid() );
  EXPECT_EQ( bdd0, bdd00 );
  EXPECT_EQ( bdd0, bdd01 );

  Bdd bdd10;
  Bdd bdd11;
  auto second = bdd1.root_decomp(bdd10, bdd11);

  EXPECT_EQ( 1, second.id() );
  EXPECT_TRUE( bdd10.is_zero() );
  EXPECT_TRUE( bdd11.is_one() );
}

TEST_F(BddTest, root_decomp_invalid1)
{
  Bdd bdd; // 不正値

  Bdd bdd0;
  Bdd bdd1;
  EXPECT_THROW({
      auto top = bdd.root_decomp(bdd0, bdd1);
    }, std::invalid_argument );
}

TEST_F(BddTest, root_decomp2)
{
  auto bdd = from_truth("1000");

  auto top = bdd.root_var();
  auto bdd0 = bdd.root_cofactor0();
  auto bdd1 = bdd.root_cofactor1();

  EXPECT_EQ( 0, top.id() );
  EXPECT_TRUE( bdd0.is_zero() );

  auto dummy = bdd0.root_var();
  auto bdd00 = bdd0.root_cofactor0();
  auto bdd01 = bdd0.root_cofactor1();

  EXPECT_TRUE( dummy.is_invalid() );
  EXPECT_EQ( bdd0, bdd00 );
  EXPECT_EQ( bdd0, bdd01 );

  auto second = bdd1.root_var();
  auto bdd10 = bdd1.root_cofactor0();
  auto bdd11 = bdd1.root_cofactor1();

  EXPECT_EQ( 1, second.id() );
  EXPECT_TRUE( bdd10.is_zero() );
  EXPECT_TRUE( bdd11.is_one() );
}

TEST_F(BddTest, root_inv1)
{
  auto bdd = mMgr.zero();

  EXPECT_FALSE( bdd.root_inv() );
}

TEST_F(BddTest, root_inv2)
{
  auto bdd = mMgr.one();

  EXPECT_TRUE( bdd.root_inv() );
}

TEST_F(BddTest, root_inv3)
{
  auto bdd = from_truth("1110");

  EXPECT_FALSE( bdd.root_inv() );
}

TEST_F(BddTest, root_inv4)
{
  auto bdd = from_truth("0001");

  EXPECT_TRUE( bdd.root_inv() );
}

TEST_F(BddTest, root_inv_invalid1)
{
  Bdd bdd; // 不正値

  EXPECT_THROW({ auto r = bdd.root_inv(); }, std::invalid_argument );
}

TEST_F(BddTest, size1)
{
  auto bdd = mMgr.zero();

  EXPECT_EQ( 0, bdd.size() );
}

TEST_F(BddTest, size2)
{
  auto bdd = mMgr.one();

  EXPECT_EQ( 0, bdd.size() );
}

TEST_F(BddTest, size3)
{
  auto bdd = from_truth("1000");

  EXPECT_EQ( 2, bdd.size() );
}

TEST_F(BddTest, size4)
{
  auto bdd1 = from_truth("1100");
  auto bdd2 = from_truth("1010");
  auto bdd3 = from_truth("1000");

  EXPECT_EQ( 1, bdd1.size() );
  EXPECT_EQ( 1, bdd2.size() );
  EXPECT_EQ( 2, bdd3.size() );
  EXPECT_EQ( 3, Bdd::bdd_size({bdd1, bdd2, bdd3}) );
}

TEST_F(BddTest, size_invalid1)
{
  Bdd bdd; // 不正値

  EXPECT_EQ( 0, bdd.size() );
}

TEST_F(BddTest, is_identical1)
{
  Bdd bdd1 = from_truth("1011");
  Bdd bdd2 = from_truth("1011");

  EXPECT_TRUE( bdd1.is_identical(bdd2) );

  BddMgr mgr2;

  Bdd bdd3 = mgr2.from_truth("1011");
  EXPECT_TRUE( bdd1.is_identical(bdd3) );
}

TEST_F(BddTest, is_identical2)
{
  Bdd bdd1 = from_truth("1011");
  Bdd bdd2 = from_truth("1010");

  EXPECT_FALSE( bdd1.is_identical(bdd2) );

  BddMgr mgr2;
  Bdd bdd3 = mgr2.from_truth("1010");
  EXPECT_FALSE( bdd1.is_identical(bdd3) );
}

TEST_F(BddTest, is_identical_invalid1)
{
  Bdd bdd1; // 不正値
  Bdd bdd2 = from_truth("1011");

  EXPECT_FALSE( bdd1.is_identical(bdd2) );
}

TEST_F(BddTest, is_identical_invalid2)
{
  Bdd bdd1 = from_truth("1011");
  Bdd bdd2; // 不正値

  EXPECT_FALSE( bdd1.is_identical(bdd2) );
}

TEST_F(BddTest, dump_restore)
{
  auto bdd1 = from_truth("1100");
  auto bdd2 = from_truth("1010");
  auto bdd3 = from_truth("1000");

  ostringstream obuf;
  BinEnc enc{obuf};
  Bdd::dump(enc, {bdd1, bdd2, bdd3});
  string tmp = obuf.str();

  istringstream ibuf{tmp};
  BinDec dec{ibuf};
  auto bdd_list = mMgr.restore(dec);

  EXPECT_EQ( 3, bdd_list.size() );
  EXPECT_EQ( bdd1, bdd_list[0] );
  EXPECT_EQ( bdd2, bdd_list[1] );
  EXPECT_EQ( bdd3, bdd_list[2] );
}

TEST_F(BddTest, display1)
{
  auto bdd1 = from_truth("1000");

  ostringstream os;
  bdd1.display(os);

  static const char* exp_str =
    "     2 \n"
    "     2:    0   ZERO:      1 \n"
    "     1:    1   ZERO:     ONE\n";

  EXPECT_EQ( exp_str, os.str() );
}

END_NAMESPACE_YM
