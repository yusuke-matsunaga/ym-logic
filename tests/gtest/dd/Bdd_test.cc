
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

TEST_F(BddTest, and_op_invalid1)
{
  Bdd var1; // 不正値
  Bdd var2 = literal(1);
  EXPECT_THROW( {
      Bdd bdd = var1.and_op(var2);
    }, std::invalid_argument );
}

TEST_F(BddTest, and_op_invalid2)
{
  Bdd var1 = literal(0);
  Bdd var2; // 不正値
  EXPECT_THROW( {
      Bdd bdd = var1.and_op(var2);
    }, std::invalid_argument );
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
  EXPECT_THROW( {
      Bdd bdd = var1.and_int(var2);
    }, std::invalid_argument );
}

TEST_F(BddTest, and_int_invalid2)
{
  Bdd var1 = literal(0);
  Bdd var2; // 不正値
  EXPECT_THROW( {
      Bdd bdd = var1.and_int(var2);
    }, std::invalid_argument );
}

TEST_F(BddTest, and_op2)
{
  Bdd var1 = literal(0);
  Bdd var2 = literal(1);
  Bdd bdd = var1 & var2;

  check(bdd, "1000");
}

TEST_F(BddTest, and_op2_invalid1)
{
  Bdd var1; // 不正値
  Bdd var2 = literal(1);
  EXPECT_THROW( {
      Bdd bdd = var1 & var2;
    }, std::invalid_argument );
}

TEST_F(BddTest, and_op2_invalid2)
{
  Bdd var1 = literal(0);
  Bdd var2; // 不正値
  EXPECT_THROW( {
      Bdd bdd = var1 & var2;
    }, std::invalid_argument );
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
  EXPECT_THROW( {
      Bdd bdd = var1 &= var2;
    }, std::invalid_argument );
}

TEST_F(BddTest, and_int2_invalid2)
{
  Bdd var1 = literal(0);
  Bdd var2; // 不正値
  EXPECT_THROW( {
      Bdd bdd = var1 &= var2;
    }, std::invalid_argument );
}

TEST_F(BddTest, and_op3)
{

  Bdd var1 = literal(0);
  BddMgr mgr2;
  Bdd var2 = mgr2.literal(1);
  Bdd bdd = var1.and_op(var2);

  check(bdd, "1000");
}

TEST_F(BddTest, and_int3)
{

  Bdd var1 = literal(0);
  BddMgr mgr2;
  Bdd var2 = mgr2.literal(1);
  Bdd bdd = var1.and_int(var2);

  check(var1, "1000");
  check(bdd, "1000");
}

TEST_F(BddTest, and_op4)
{

  Bdd var1 = literal(0);
  BddMgr mgr2;
  Bdd var2 = mgr2.literal(1);
  Bdd bdd = var1 & var2;

  check(bdd, "1000");
}

TEST_F(BddTest, and_int4)
{

  Bdd var1 = literal(0);
  BddMgr mgr2;
  Bdd var2 = mgr2.literal(1);
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

TEST_F(BddTest, or_op1_invalid1)
{
  Bdd var1; // 不正値
  Bdd var2 = literal(1);
  EXPECT_THROW({
      Bdd bdd = var1.or_op(var2);
    }, std::invalid_argument );
}

TEST_F(BddTest, or_op1_invalid2)
{
  Bdd var1 = literal(0);
  Bdd var2; // 不正値
  EXPECT_THROW({
      Bdd bdd = var1.or_op(var2);
    }, std::invalid_argument );
}

TEST_F(BddTest, or_int1)
{
  Bdd var1 = literal(0);
  Bdd var2 = literal(1);
  Bdd bdd = var1.or_int(var2);

  check(var1, "1110");
  check(bdd, "1110");
}

TEST_F(BddTest, or_int1_invalid1)
{
  Bdd var1; // 不正値
  Bdd var2 = literal(1);
  EXPECT_THROW({
      Bdd bdd = var1.or_int(var2);
    }, std::invalid_argument );
}

TEST_F(BddTest, or_int1_invalid2)
{
  Bdd var1 = literal(0);
  Bdd var2; // 不正値
  EXPECT_THROW({
      Bdd bdd = var1.or_int(var2);
    }, std::invalid_argument );
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
  EXPECT_THROW({
      Bdd bdd = var1 | var2;
    }, std::invalid_argument );
}

TEST_F(BddTest, or_op2_invalid2)
{
  Bdd var1 = literal(0);
  Bdd var2; // 不正値
  EXPECT_THROW({
      Bdd bdd = var1 | var2;
    }, std::invalid_argument );
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
  EXPECT_THROW({
      Bdd bdd = var1 |= var2;
    }, std::invalid_argument );
}

TEST_F(BddTest, or_int2_invalid2)
{
  Bdd var1 = literal(0);
  Bdd var2; // 不正値
  EXPECT_THROW({
      Bdd bdd = var1 |= var2;
    }, std::invalid_argument );
}

TEST_F(BddTest, or_op3)
{
  Bdd var1 = literal(0);
  BddMgr mgr2;
  Bdd var2 = mgr2.literal(1);
  Bdd bdd = var1.or_op(var2);

  check(bdd, "1110");
}

TEST_F(BddTest, or_int3)
{
  Bdd var1 = literal(0);
  BddMgr mgr2;
  Bdd var2 = mgr2.literal(1);
  Bdd bdd = var1.or_int(var2);

  check(var1, "1110");
  check(bdd, "1110");
}

TEST_F(BddTest, or_op4)
{
  Bdd var1 = literal(0);
  BddMgr mgr2;
  Bdd var2 = mgr2.literal(1);
  Bdd bdd = var1 | var2;

  check(bdd, "1110");
}

TEST_F(BddTest, or_int4)
{
  Bdd var1 = literal(0);
  BddMgr mgr2;
  Bdd var2 = mgr2.literal(1);
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

TEST_F(BddTest, xor_op1_invalid1)
{
  Bdd var1; // 不正値
  Bdd var2 = literal(1);
  EXPECT_THROW({
      Bdd bdd = var1.xor_op(var2);
    }, std::invalid_argument );
}

TEST_F(BddTest, xor_op1_invalid2)
{
  Bdd var1 = literal(0);
  Bdd var2; // 不正値
  EXPECT_THROW({
      Bdd bdd = var1.xor_op(var2);
    }, std::invalid_argument );
}

TEST_F(BddTest, xor_int1)
{
  Bdd var1 = literal(0);
  Bdd var2 = literal(1);
  Bdd bdd = var1.xor_int(var2);

  check(var1, "0110");
  check(bdd, "0110");
}

TEST_F(BddTest, xor_int1_invalid1)
{
  Bdd var1; // 不正値
  Bdd var2 = literal(1);
  EXPECT_THROW({
      Bdd bdd = var1.xor_int(var2);
    }, std::invalid_argument );
}

TEST_F(BddTest, xor_int1_invalid2)
{
  Bdd var1 = literal(0);
  Bdd var2; // 不正値
  EXPECT_THROW({
      Bdd bdd = var1.xor_int(var2);
    }, std::invalid_argument );
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
  EXPECT_THROW({
      Bdd bdd = var1 ^ var2;
    }, std::invalid_argument );
}

TEST_F(BddTest, xor_op2_invalid2)
{
  Bdd var1 = literal(0);
  Bdd var2; // 不正値
  EXPECT_THROW({
      Bdd bdd = var1 ^ var2;
    }, std::invalid_argument );
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
  EXPECT_THROW({
      Bdd bdd = var1 ^= var2;
    }, std::invalid_argument );
}

TEST_F(BddTest, xor_int2_invalid2)
{
  Bdd var1 = literal(0);
  Bdd var2; // 不正値
  EXPECT_THROW({
      Bdd bdd = var1 ^= var2;
    }, std::invalid_argument );
}

TEST_F(BddTest, xor_op3)
{
  Bdd var1 = literal(0);
  BddMgr mgr2;
  Bdd var2 = mgr2.literal(1);
  Bdd bdd = var1.xor_op(var2);

  check(bdd, "0110");
}

TEST_F(BddTest, xor_int3)
{
  Bdd var1 = literal(0);
  BddMgr mgr2;
  Bdd var2 = mgr2.literal(1);
  Bdd bdd = var1.xor_int(var2);

  check(var1, "0110");
  check(bdd, "0110");
}

TEST_F(BddTest, xor_op4)
{
  Bdd var1 = literal(0);
  BddMgr mgr2;
  Bdd var2 = mgr2.literal(1);
  Bdd bdd = var1 ^ var2;

  check(bdd, "0110");
}

TEST_F(BddTest, xor_int4)
{
  Bdd var1 = literal(0);
  BddMgr mgr2;
  Bdd var2 = mgr2.literal(1);
  Bdd bdd = var1 ^= var2;

  check(var1, "0110");
  check(bdd, "0110");
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

TEST_F(BddTest, complex_expr2)
{
  Bdd var1 = literal(0);
  Bdd var2 = literal(1);
  Bdd var3 = literal(2);
  BddMgr mgr2;
  Bdd var4 = mgr2.literal(1);
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

TEST_F(BddTest, ite_invalid1)
{
  Bdd bdd1; // 不正値
  Bdd bdd2 = from_truth("11001100");
  Bdd bdd3 = from_truth("11110000");

  EXPECT_THROW( {
      Bdd bdd = ite(bdd1, bdd2, bdd3);
    }, std::invalid_argument );
}

TEST_F(BddTest, ite_invalid2)
{
  Bdd bdd1 = from_truth("10101010");
  Bdd bdd2; // 不正値
  Bdd bdd3 = from_truth("11110000");

  EXPECT_THROW( {
      Bdd bdd = ite(bdd1, bdd2, bdd3);
    }, std::invalid_argument );
}

TEST_F(BddTest, ite_invalid3)
{
  Bdd bdd1 = from_truth("10101010");
  Bdd bdd2 = from_truth("11001100");
  Bdd bdd3; // 不正値

  EXPECT_THROW( {
      Bdd bdd = ite(bdd1, bdd2, bdd3);
    }, std::invalid_argument );
}

TEST_F(BddTest, cofactor1)
{
  const char* src_str = "10010111";
  Bdd bdd = from_truth(src_str);

  Bdd bdd1 = bdd.cofactor(0, false);

  const char* exp_str = "10011001";
  check(bdd1, exp_str);
}

TEST_F(BddTest, cofactor1_invalid1)
{
  Bdd bdd; // 不正値

  EXPECT_THROW( {
      Bdd bdd1 = bdd.cofactor(0, false);
    }, std::invalid_argument );
}

TEST_F(BddTest, cofactor_int1)
{
  const char* src_str = "10010111";
  Bdd bdd = from_truth(src_str);

  Bdd bdd1 = bdd.cofactor_int(0, false);

  const char* exp_str = "10011001";
  check(bdd, exp_str);
  check(bdd1, exp_str);
}

TEST_F(BddTest, cofactor_int1_invalid1)
{
  Bdd bdd; // 不正値

  EXPECT_THROW( {
      Bdd bdd1 = bdd.cofactor_int(0, false);
    }, std::invalid_argument );
}

TEST_F(BddTest, cofactor2)
{
  const char* src_str = "10010111";
  Bdd bdd = from_truth(src_str);

  Literal lit{0, true};
  Bdd bdd1 = bdd.cofactor(lit);

  const char* exp_str = "01110111";
  check(bdd1, exp_str);
}

TEST_F(BddTest, cofactor2_invalid)
{
  Bdd bdd; // 不正値

  Literal lit{0, true};
  EXPECT_THROW({
      Bdd bdd1 = bdd.cofactor(lit);
    }, std::invalid_argument );
}

TEST_F(BddTest, cofactor_int2)
{
  const char* src_str = "10010111";
  Bdd bdd = from_truth(src_str);

  Literal lit{0, true};
  Bdd bdd1 = bdd.cofactor_int(lit);

  const char* exp_str = "01110111";
  check(bdd, exp_str);
  check(bdd1, exp_str);
}

TEST_F(BddTest, cofactor_int2_invalid)
{
  Bdd bdd; // 不正値

  Literal lit{0, true};
  EXPECT_THROW({
      Bdd bdd1 = bdd.cofactor_int(lit);
    }, std::invalid_argument );
}

TEST_F(BddTest, cofactor3)
{
  const char* src_str = "10010111";
  Bdd bdd = from_truth(src_str);

  Literal lit{0, true};

  Bdd bdd1 = bdd / lit;

  const char* exp_str = "01110111";
  check(bdd1, exp_str);
}

TEST_F(BddTest, cofactor3_invalid)
{
  Bdd bdd; // 不正値

  Literal lit{0, true};
  EXPECT_THROW({
      Bdd bdd1 = bdd / lit;
    }, std::invalid_argument );
}

TEST_F(BddTest, cofactor_int3)
{
  const char* src_str = "10010111";
  Bdd bdd = from_truth(src_str);

  Literal lit{0, true};
  Bdd bdd1 = bdd /= lit;

  const char* exp_str = "01110111";
  check(bdd, exp_str);
  check(bdd1, exp_str);
}

TEST_F(BddTest, cofactor_int3_invalid)
{
  Bdd bdd; // 不正値

  Literal lit{0, true};
  EXPECT_THROW({
      Bdd bdd1 = bdd /= lit;
    }, std::invalid_argument );
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

  EXPECT_THROW({
      Bdd bdd1 = bdd / cube;
    }, std::invalid_argument );
}

TEST_F(BddTest, cofactor4_invalid2)
{
  const char* src_str = "10010111";
  Bdd bdd = from_truth(src_str);

  Bdd cube; // 不正値

  EXPECT_THROW({
      Bdd bdd1 = bdd / cube;
    }, std::invalid_argument );
}

TEST_F(BddTest, cofactor4_invalid3)
{
  const char* src_str = "10010111";
  Bdd bdd = from_truth(src_str);

  Bdd not_cube = literal(0) ^ literal(1); // キューブでない．

  EXPECT_THROW({
      Bdd bdd1 = bdd / not_cube;
    }, std::invalid_argument );
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

  EXPECT_THROW({
      Bdd bdd1 = bdd /= cube;
    }, std::invalid_argument );
}

TEST_F(BddTest, cofactor_int4_invalid2)
{
  const char* src_str = "10010111";
  Bdd bdd = from_truth(src_str);

  Bdd cube; // 不正値

  EXPECT_THROW({
      Bdd bdd1 = bdd /= cube;
    }, std::invalid_argument );
}

TEST_F(BddTest, cofactor_int4_invalid3)
{
  const char* src_str = "10010111";
  Bdd bdd = from_truth(src_str);

  Bdd not_cube = literal(0) ^ literal(1); // キューブでない．

  EXPECT_THROW({
      Bdd bdd1 = bdd /= not_cube;
    }, std::invalid_argument );
}

TEST_F(BddTest, cofactor5)
{
  const char* src_str = "10010111";
  Bdd bdd = from_truth(src_str);

  BddMgr mgr2;
  Bdd lit1 = mgr2.literal(0);
  Bdd lit2 = mgr2.literal(2);
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
  Bdd lit1 = mgr2.literal(0);
  Bdd lit2 = mgr2.literal(2);
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
  Bdd lit1 = mgr2.literal(0);
  Bdd lit2 = mgr2.literal(2);
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
  Bdd lit1 = mgr2.literal(0);
  Bdd lit2 = mgr2.literal(2);
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

  BddVarSet sup1 = bdd1.get_support();
  BddVarSet sup2 = bdd2.get_support();
  BddVarSet sup = sup1 + sup2;
  vector<SizeType> var_list = sup.to_varlist();
  EXPECT_EQ( 4, var_list.size() );
  EXPECT_EQ( 0, var_list[0] );
  EXPECT_EQ( 1, var_list[1] );
  EXPECT_EQ( 2, var_list[2] );
  EXPECT_EQ( 3, var_list[3] );
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
      BddVarSet sup = sup1 + sup2;
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
      BddVarSet sup = sup1 + sup2;
    }, std::invalid_argument );
}

TEST_F(BddTest, support_cup_int1)
{
  Bdd lit1 = literal(0);
  Bdd lit2 = literal(1);
  Bdd lit3 = literal(2);
  Bdd lit4 = literal(3);

  Bdd bdd1 = lit1 & lit2 & lit3;
  Bdd bdd2 = lit2 & lit4;

  BddVarSet sup1 = bdd1.get_support();
  BddVarSet sup2 = bdd2.get_support();
  BddVarSet sup = sup1 += sup2;
  vector<SizeType> var_list = sup1.to_varlist();
  EXPECT_EQ( 4, var_list.size() );
  EXPECT_EQ( 0, var_list[0] );
  EXPECT_EQ( 1, var_list[1] );
  EXPECT_EQ( 2, var_list[2] );
  EXPECT_EQ( 3, var_list[3] );

  EXPECT_EQ( sup1, sup );
}

TEST_F(BddTest, support_cup_int1_invalid1)
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
      BddVarSet sup = sup1 += sup2;
    }, std::invalid_argument );
}

TEST_F(BddTest, support_cup_int1_invalid2)
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
      BddVarSet sup = sup1 += sup2;
    }, std::invalid_argument );
}

TEST_F(BddTest, support_cap1)
{
  Bdd lit1 = literal(0);
  Bdd lit2 = literal(1);
  Bdd lit3 = literal(2);
  Bdd lit4 = literal(3);

  Bdd bdd1 = lit1 & lit2 & lit3;
  Bdd bdd2 = lit2 & lit4;

  BddVarSet sup1 = bdd1.get_support();
  BddVarSet sup2 = bdd2.get_support();
  BddVarSet sup = sup1 & sup2;
  vector<SizeType> var_list = sup.to_varlist();

  EXPECT_EQ( 1, var_list.size() );
  EXPECT_EQ( 1, var_list[0] );
}

TEST_F(BddTest, support_cap1_invalid1)
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
      BddVarSet sup = sup1 & sup2;
    }, std::invalid_argument );
}

TEST_F(BddTest, support_cap1_invalid2)
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
      BddVarSet sup = sup1 & sup2;
    }, std::invalid_argument );
}

TEST_F(BddTest, support_cap_int1)
{
  Bdd lit1 = literal(0);
  Bdd lit2 = literal(1);
  Bdd lit3 = literal(2);
  Bdd lit4 = literal(3);

  Bdd bdd1 = lit1 & lit2 & lit3;
  Bdd bdd2 = lit2 & lit4;

  BddVarSet sup1 = bdd1.get_support();
  BddVarSet sup2 = bdd2.get_support();
  BddVarSet sup = sup1 &= sup2;
  vector<SizeType> var_list = sup1.to_varlist();

  EXPECT_EQ( 1, var_list.size() );
  EXPECT_EQ( 1, var_list[0] );

  EXPECT_EQ( sup1, sup );
}

TEST_F(BddTest, support_cap_int1_invalid1)
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
      BddVarSet sup = sup1 &= sup2;
    }, std::invalid_argument );
}

TEST_F(BddTest, support_cap_int1_invalid2)
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
      BddVarSet sup = sup1 &= sup2;
    }, std::invalid_argument );
}

TEST_F(BddTest, support_diff1)
{
  Bdd lit1 = literal(0);
  Bdd lit2 = literal(1);
  Bdd lit3 = literal(2);
  Bdd lit4 = literal(3);

  Bdd bdd1 = lit1 & lit2 & lit3;
  Bdd bdd2 = lit2 & lit4;

  BddVarSet sup1 = bdd1.get_support();
  BddVarSet sup2 = bdd2.get_support();
  BddVarSet sup = sup1 - sup2;
  vector<SizeType> var_list = sup.to_varlist();

  EXPECT_EQ( 2, var_list.size() );
  EXPECT_EQ( 0, var_list[0] );
  EXPECT_EQ( 2, var_list[1] );
}

TEST_F(BddTest, support_diff1_invalid1)
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
      BddVarSet sup = sup1 - sup2;
    }, std::invalid_argument );
}

TEST_F(BddTest, support_diff1_invalid2)
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
      BddVarSet sup = sup1 - sup2;
    }, std::invalid_argument );
}

TEST_F(BddTest, support_diff_int1)
{
  Bdd lit1 = literal(0);
  Bdd lit2 = literal(1);
  Bdd lit3 = literal(2);
  Bdd lit4 = literal(3);

  Bdd bdd1 = lit1 & lit2 & lit3;
  Bdd bdd2 = lit2 & lit4;

  BddVarSet sup1 = bdd1.get_support();
  BddVarSet sup2 = bdd2.get_support();
  BddVarSet sup = sup1 -= sup2;
  vector<SizeType> var_list = sup.to_varlist();

  EXPECT_EQ( 2, var_list.size() );
  EXPECT_EQ( 0, var_list[0] );
  EXPECT_EQ( 2, var_list[1] );

  EXPECT_EQ( sup1, sup );
}

TEST_F(BddTest, support_diff_int1_invalid1)
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
      BddVarSet sup = sup1 -= sup2;
    }, std::invalid_argument );
}

TEST_F(BddTest, support_diff_int1_invalid2)
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
      BddVarSet sup = sup1 -= sup2;
    }, std::invalid_argument );
}

TEST_F(BddTest, support_diff2)
{
  Bdd lit1 = literal(0);
  Bdd lit2 = literal(1);
  Bdd lit3 = literal(2);
  Bdd lit4 = literal(3);

  Bdd bdd1 = lit2 & lit3 & lit4;
  Bdd bdd2 = lit1 & lit2 & lit3 & lit4;

  BddVarSet sup1 = bdd1.get_support();
  BddVarSet sup2 = bdd2.get_support();
  BddVarSet sup = sup1 - sup2;
  vector<SizeType> var_list = sup.to_varlist();

  EXPECT_EQ( 0, var_list.size() );
}

TEST_F(BddTest, support_check_intersect1)
{
  Bdd lit1 = literal(0);
  Bdd lit2 = literal(1);
  Bdd lit3 = literal(2);
  Bdd lit4 = literal(3);

  Bdd bdd1 = lit2 & lit3 & lit4;
  Bdd bdd2 = lit1 & lit2 & lit3 & lit4;

  BddVarSet sup1 = bdd1.get_support();
  BddVarSet sup2 = bdd2.get_support();
  bool result = sup1 && sup2;

  EXPECT_TRUE( result );
}

TEST_F(BddTest, support_check_intersect1_invalid1)
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
      auto r = sup1 && sup2;
    }, std::invalid_argument );
}

TEST_F(BddTest, support_check_intersect1_invalid2)
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
      auto r = sup1 && sup2;
    }, std::invalid_argument );
}

TEST_F(BddTest, support_check_intersect2)
{
  Bdd lit1 = literal(0);
  Bdd lit2 = literal(1);
  Bdd lit3 = literal(2);
  Bdd lit4 = literal(3);

  Bdd bdd1 = lit1 & lit3;
  Bdd bdd2 = lit2 & lit4;

  BddVarSet sup1 = bdd1.get_support();
  BddVarSet sup2 = bdd2.get_support();
  bool result = sup1 && sup2;

  EXPECT_FALSE( result );
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

  test_check_sup(bdd, 0);
  test_check_sup(bdd, 1);
}

TEST_F(BddTest, check_sym1)
{
  Bdd bdd = from_truth("11101010");

  test_check_sym(bdd, 0, 1);
  test_check_sym(bdd, 0, 2);
  test_check_sym(bdd, 1, 2);
}

TEST_F(BddTest, check_sym2)
{
  Bdd bdd = from_truth("01001011");

  test_check_sym(bdd, 0, 1);
  test_check_sym(bdd, 0, 2);
  test_check_sym(bdd, 1, 2);
}

TEST_F(BddTest, check_sym3)
{
  Bdd bdd = from_truth("0110");

  test_check_sym(bdd, 0, 1);
}

TEST_F(BddTest, check_sym4)
{
  Bdd bdd = from_truth("01100101");

  test_check_sym(bdd, 0, 1);
  test_check_sym(bdd, 0, 2);
  test_check_sym(bdd, 1, 2);
}

TEST_F(BddTest, check_sym5)
{
  Bdd bdd = from_truth("0100");

  test_check_sym(bdd, 0, 1);
}

TEST_F(BddTest, get_support1)
{
  Bdd lit1 = literal(1);
  Bdd lit3 = literal(3);
  Bdd lit5 = literal(5);
  Bdd lit7 = literal(7);

  Bdd bdd = (lit1 & lit3) | (~lit1 & lit5) | (lit3 & ~lit7);

  BddVarSet sup = bdd.get_support();

  test_support(bdd, sup, 8);
}

TEST_F(BddTest, get_support_invalid1)
{
  Bdd bdd1; // 不正値

  EXPECT_THROW({
      BddVarSet sup1 = bdd1.get_support();
    }, std::invalid_argument );
}

TEST_F(BddTest, get_support2)
{
  Bdd lit1 = literal(1);
  Bdd lit2 = literal(2);
  Bdd lit3 = literal(3);

  Bdd bdd = (lit1 & lit2) | (~lit1 & lit2) | lit3;

  BddVarSet sup = bdd.get_support();

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

TEST_F(BddTest, get_onepath_invalid1)
{
  Bdd bdd; // 不正値
  EXPECT_THROW({
      Bdd bdd1 = bdd.get_onepath();
    }, std::invalid_argument );
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

TEST_F(BddTest, get_zeropath_invalid1)
{
  Bdd bdd; // 不正値
  EXPECT_THROW({
      Bdd bdd1 = bdd.get_zeropath();
    }, std::invalid_argument );
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
  auto top = bdd.root_decomp(bdd0, bdd1);

  EXPECT_EQ( 0, top );
  EXPECT_TRUE( bdd0.is_zero() );

  Bdd bdd00;
  Bdd bdd01;
  auto dummy = bdd0.root_decomp(bdd00, bdd01);
  EXPECT_EQ( BAD_VARID, dummy );
  EXPECT_EQ( bdd0, bdd00 );
  EXPECT_EQ( bdd0, bdd01 );

  Bdd bdd10;
  Bdd bdd11;
  auto second = bdd1.root_decomp(bdd10, bdd11);

  EXPECT_EQ( 1, second );
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
  Bdd bdd = from_truth("1000");

  auto top = bdd.root_var();
  Bdd bdd0 = bdd.root_cofactor0();
  Bdd bdd1 = bdd.root_cofactor1();

  EXPECT_EQ( 0, top );
  EXPECT_TRUE( bdd0.is_zero() );

  auto dummy = bdd0.root_var();
  Bdd bdd00 = bdd0.root_cofactor0();
  Bdd bdd01 = bdd0.root_cofactor1();

  EXPECT_EQ( BAD_VARID, dummy );
  EXPECT_EQ( bdd0, bdd00 );
  EXPECT_EQ( bdd0, bdd01 );

  auto second = bdd1.root_var();
  Bdd bdd10 = bdd1.root_cofactor0();
  Bdd bdd11 = bdd1.root_cofactor1();

  EXPECT_EQ( 1, second );
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

TEST_F(BddTest, root_inv_invalid1)
{
  Bdd bdd; // 不正値

  EXPECT_THROW({ auto r = bdd.root_inv(); }, std::invalid_argument );
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
  EXPECT_EQ( 3, mMgr.bdd_size({bdd1, bdd2, bdd3}) );
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
  Bdd bdd1 = from_truth("1100");
  Bdd bdd2 = from_truth("1010");
  Bdd bdd3 = from_truth("1000");

  ostringstream obuf;
  BinEnc enc{obuf};
  mMgr.dump(enc, {bdd1, bdd2, bdd3});
  string tmp = obuf.str();

  istringstream ibuf{tmp};
  BinDec dec{ibuf};
  auto bdd_list = mMgr.restore(dec);

  EXPECT_EQ( 3, bdd_list.size() );
  EXPECT_EQ( bdd1, bdd_list[0] );
  EXPECT_EQ( bdd2, bdd_list[1] );
  EXPECT_EQ( bdd3, bdd_list[2] );
}

END_NAMESPACE_YM
