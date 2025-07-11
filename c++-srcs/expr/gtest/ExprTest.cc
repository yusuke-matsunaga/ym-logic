
/// @file ExprTet.cc
/// @brief ExprTet の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018, 2023 Yusuke Matsunaga
/// All rights reserved.

#include "gtest/gtest.h"
#include "ym/Expr.h"
#include "ym/TvFunc.h"


BEGIN_NAMESPACE_YM

class ExprTest :
public ::testing::Test
{
public:

  void
  SetUp() override;

  void
  TearDown() override;

  SizeType var0{0};
  SizeType var1{1};
  SizeType var2{2};
  SizeType var3{3};
  SizeType var4{4};
  SizeType var5{5};
  SizeType var6{6};
  SizeType var7{7};
  SizeType var8{8};
  SizeType var9{9};
};

void
ExprTest::SetUp()
{
}

void
ExprTest::TearDown()
{
}

TEST_F(ExprTest, empty_constr)
{
  // 空のコンストラクタ
  Expr expr{};

  EXPECT_FALSE( expr.is_valid() );
  EXPECT_FALSE( expr.is_zero() );
  EXPECT_FALSE( expr.is_one() );
  EXPECT_FALSE( expr.is_constant() );
  EXPECT_FALSE( expr.is_positive_literal() );
  EXPECT_FALSE( expr.is_negative_literal() );
  EXPECT_FALSE( expr.is_literal() );
  EXPECT_EQ( BAD_VARID, expr.varid() );
  EXPECT_FALSE( expr.is_and() );
  EXPECT_FALSE( expr.is_or() );
  EXPECT_FALSE( expr.is_xor() );
  EXPECT_FALSE( expr.is_op() );
  EXPECT_EQ( 0, expr.operand_num() );
  EXPECT_EQ( std::vector<Expr>{}, expr.operand_list() );
  EXPECT_FALSE( expr.is_simple() );
  EXPECT_FALSE( expr.is_simple_and() );
  EXPECT_FALSE( expr.is_simple_or() );
  EXPECT_FALSE( expr.is_simple_xor() );
  EXPECT_FALSE( expr.is_sop() );

  EXPECT_EQ( 0, expr.literal_num() );
  EXPECT_EQ( 0, expr.input_size() );
  EXPECT_EQ( 0, expr.sop_literal_num() );

  std::ostringstream obuf;
  BinEnc bos{obuf};
  bos << expr;

  std::istringstream ibuf{obuf.str()};
  BinDec bis{ibuf};
  Expr expr2;
  bis >> expr2;

  EXPECT_EQ( expr2, expr );
}

TEST_F(ExprTest, invalid)
{
  auto expr = Expr::invalid();

  EXPECT_FALSE( expr.is_valid() );
  EXPECT_FALSE( expr.is_zero() );
  EXPECT_FALSE( expr.is_one() );
  EXPECT_FALSE( expr.is_constant() );
  EXPECT_FALSE( expr.is_positive_literal() );
  EXPECT_FALSE( expr.is_negative_literal() );
  EXPECT_FALSE( expr.is_literal() );
  EXPECT_FALSE( expr.is_and() );
  EXPECT_FALSE( expr.is_or() );
  EXPECT_FALSE( expr.is_xor() );
  EXPECT_FALSE( expr.is_op() );
  EXPECT_EQ( 0, expr.operand_num() );
  EXPECT_EQ( std::vector<Expr>{}, expr.operand_list() );
  EXPECT_FALSE( expr.is_simple() );
  EXPECT_FALSE( expr.is_simple_and() );
  EXPECT_FALSE( expr.is_simple_or() );
  EXPECT_FALSE( expr.is_simple_xor() );
  EXPECT_FALSE( expr.is_sop() );

  EXPECT_EQ( 0, expr.literal_num() );
  EXPECT_EQ( 0, expr.input_size() );
  EXPECT_EQ( 0, expr.sop_literal_num() );

  std::ostringstream obuf;
  BinEnc bos{obuf};
  bos << expr;

  std::istringstream ibuf{obuf.str()};
  BinDec bis{ibuf};
  Expr expr2;
  bis >> expr2;

  EXPECT_EQ( expr2, expr );
}

TEST_F(ExprTest, zero)
{
  auto expr = Expr::zero();

  EXPECT_TRUE( expr.is_valid() );
  EXPECT_TRUE( expr.is_zero() );
  EXPECT_FALSE( expr.is_one() );
  EXPECT_TRUE( expr.is_constant() );
  EXPECT_FALSE( expr.is_positive_literal() );
  EXPECT_FALSE( expr.is_negative_literal() );
  EXPECT_FALSE( expr.is_literal() );
  EXPECT_EQ( BAD_VARID, expr.varid() );
  EXPECT_FALSE( expr.is_and() );
  EXPECT_FALSE( expr.is_or() );
  EXPECT_FALSE( expr.is_xor() );
  EXPECT_FALSE( expr.is_op() );
  EXPECT_EQ( 0, expr.operand_num() );
  EXPECT_EQ( std::vector<Expr>{}, expr.operand_list() );
  EXPECT_TRUE( expr.is_simple() );
  EXPECT_FALSE( expr.is_simple_and() );
  EXPECT_FALSE( expr.is_simple_or() );
  EXPECT_FALSE( expr.is_simple_xor() );
  EXPECT_TRUE( expr.is_sop() );

  EXPECT_EQ( 0, expr.literal_num() );
  EXPECT_EQ( 0, expr.input_size() );
  EXPECT_EQ( 0, expr.sop_literal_num() );

  std::ostringstream obuf;
  BinEnc bos{obuf};
  bos << expr;

  std::istringstream ibuf{obuf.str()};
  BinDec bis{ibuf};
  Expr expr2;
  bis >> expr2;

  EXPECT_EQ( expr2, expr );
}

TEST_F(ExprTest, one)
{
  auto expr = Expr::one();

  EXPECT_TRUE( expr.is_valid() );
  EXPECT_FALSE( expr.is_zero() );
  EXPECT_TRUE( expr.is_one() );
  EXPECT_TRUE( expr.is_constant() );
  EXPECT_FALSE( expr.is_positive_literal() );
  EXPECT_FALSE( expr.is_negative_literal() );
  EXPECT_FALSE( expr.is_literal() );
  EXPECT_EQ( BAD_VARID, expr.varid() );
  EXPECT_FALSE( expr.is_and() );
  EXPECT_FALSE( expr.is_or() );
  EXPECT_FALSE( expr.is_xor() );
  EXPECT_FALSE( expr.is_op() );
  EXPECT_EQ( 0, expr.operand_num() );
  EXPECT_EQ( std::vector<Expr>{}, expr.operand_list() );
  EXPECT_TRUE( expr.is_simple() );
  EXPECT_FALSE( expr.is_simple_and() );
  EXPECT_FALSE( expr.is_simple_or() );
  EXPECT_FALSE( expr.is_simple_xor() );
  EXPECT_TRUE( expr.is_sop() );

  EXPECT_EQ( 0, expr.literal_num() );
  EXPECT_EQ( 0, expr.input_size() );
  EXPECT_EQ( 0, expr.sop_literal_num() );

  std::ostringstream obuf;
  BinEnc bos{obuf};
  bos << expr;

  std::istringstream ibuf{obuf.str()};
  BinDec bis{ibuf};
  Expr expr2;
  bis >> expr2;

  EXPECT_EQ( expr2, expr );
}

TEST_F(ExprTest, literal1)
{
  auto expr = Expr::literal(var0, false);

  EXPECT_TRUE( expr.is_valid() );
  EXPECT_FALSE( expr.is_zero() );
  EXPECT_FALSE( expr.is_one() );
  EXPECT_FALSE( expr.is_constant() );
  EXPECT_TRUE( expr.is_positive_literal() );
  EXPECT_FALSE( expr.is_negative_literal() );
  EXPECT_TRUE( expr.is_literal() );
  EXPECT_EQ( var0, expr.varid() );
  EXPECT_FALSE( expr.is_and() );
  EXPECT_FALSE( expr.is_or() );
  EXPECT_FALSE( expr.is_xor() );
  EXPECT_FALSE( expr.is_op() );
  EXPECT_EQ( 0, expr.operand_num() );
  EXPECT_EQ( std::vector<Expr>{}, expr.operand_list() );
  EXPECT_TRUE( expr.is_simple() );
  EXPECT_FALSE( expr.is_simple_and() );
  EXPECT_FALSE( expr.is_simple_or() );
  EXPECT_FALSE( expr.is_simple_xor() );
  EXPECT_TRUE( expr.is_sop() );

  EXPECT_EQ( 1, expr.literal_num() );
  EXPECT_EQ( 1, expr.literal_num(var0) );
  EXPECT_EQ( 1, expr.literal_num(var0, false) );
  EXPECT_EQ( 0, expr.literal_num(var0, true) );
  EXPECT_EQ( 1, expr.input_size() );
  EXPECT_EQ( 1, expr.sop_literal_num() );

  std::ostringstream obuf;
  BinEnc bos{obuf};
  bos << expr;

  istringstream ibuf{obuf.str()};
  BinDec bis{ibuf};
  Expr expr2;
  bis >> expr2;

  EXPECT_EQ( expr2, expr );
}

TEST_F(ExprTest, literal2)
{
  auto expr = Expr::literal(var0, true);

  EXPECT_TRUE( expr.is_valid() );
  EXPECT_FALSE( expr.is_zero() );
  EXPECT_FALSE( expr.is_one() );
  EXPECT_FALSE( expr.is_constant() );
  EXPECT_FALSE( expr.is_positive_literal() );
  EXPECT_TRUE( expr.is_negative_literal() );
  EXPECT_TRUE( expr.is_literal() );
  EXPECT_EQ( var0, expr.varid() );
  EXPECT_FALSE( expr.is_and() );
  EXPECT_FALSE( expr.is_or() );
  EXPECT_FALSE( expr.is_xor() );
  EXPECT_FALSE( expr.is_op() );
  EXPECT_EQ( 0, expr.operand_num() );
  EXPECT_EQ( std::vector<Expr>{}, expr.operand_list() );
  EXPECT_TRUE( expr.is_simple() );
  EXPECT_FALSE( expr.is_simple_and() );
  EXPECT_FALSE( expr.is_simple_or() );
  EXPECT_FALSE( expr.is_simple_xor() );
  EXPECT_TRUE( expr.is_sop() );

  EXPECT_EQ( 1, expr.literal_num() );
  EXPECT_EQ( 1, expr.literal_num(var0) );
  EXPECT_EQ( 0, expr.literal_num(var0, false) );
  EXPECT_EQ( 1, expr.literal_num(var0, true) );
  EXPECT_EQ( 1, expr.input_size() );
  EXPECT_EQ( 1, expr.sop_literal_num() );

  // 式自体は上と同一なので dump/restore はテストは不要
}

TEST_F(ExprTest, posi_literal)
{
  auto expr = Expr::posi_literal(var0);

  EXPECT_TRUE( expr.is_valid() );
  EXPECT_FALSE( expr.is_zero() );
  EXPECT_FALSE( expr.is_one() );
  EXPECT_FALSE( expr.is_constant() );
  EXPECT_TRUE( expr.is_positive_literal() );
  EXPECT_FALSE( expr.is_negative_literal() );
  EXPECT_TRUE( expr.is_literal() );
  EXPECT_EQ( var0, expr.varid() );
  EXPECT_FALSE( expr.is_and() );
  EXPECT_FALSE( expr.is_or() );
  EXPECT_FALSE( expr.is_xor() );
  EXPECT_FALSE( expr.is_op() );
  EXPECT_EQ( 0, expr.operand_num() );
  EXPECT_EQ( std::vector<Expr>{}, expr.operand_list() );
  EXPECT_TRUE( expr.is_simple() );
  EXPECT_FALSE( expr.is_simple_and() );
  EXPECT_FALSE( expr.is_simple_or() );
  EXPECT_FALSE( expr.is_simple_xor() );
  EXPECT_TRUE( expr.is_sop() );

  EXPECT_EQ( 1, expr.literal_num() );
  EXPECT_EQ( 1, expr.literal_num(var0) );
  EXPECT_EQ( 1, expr.literal_num(var0, false) );
  EXPECT_EQ( 0, expr.literal_num(var0, true) );
  EXPECT_EQ( 1, expr.input_size() );
  EXPECT_EQ( 1, expr.sop_literal_num() );
}

TEST_F(ExprTest, nega_literal)
{
  auto var = var0;
  auto expr = Expr::nega_literal(var);

  EXPECT_TRUE( expr.is_valid() );
  EXPECT_FALSE( expr.is_zero() );
  EXPECT_FALSE( expr.is_one() );
  EXPECT_FALSE( expr.is_constant() );
  EXPECT_FALSE( expr.is_positive_literal() );
  EXPECT_TRUE( expr.is_negative_literal() );
  EXPECT_TRUE( expr.is_literal() );
  EXPECT_EQ( var, expr.varid() );
  EXPECT_FALSE( expr.is_and() );
  EXPECT_FALSE( expr.is_or() );
  EXPECT_FALSE( expr.is_xor() );
  EXPECT_FALSE( expr.is_op() );
  EXPECT_EQ( 0, expr.operand_num() );
  EXPECT_EQ( std::vector<Expr>{}, expr.operand_list() );
  EXPECT_TRUE( expr.is_simple() );
  EXPECT_FALSE( expr.is_simple_and() );
  EXPECT_FALSE( expr.is_simple_or() );
  EXPECT_FALSE( expr.is_simple_xor() );
  EXPECT_TRUE( expr.is_sop() );

  EXPECT_EQ( 1, expr.literal_num() );
  EXPECT_EQ( 1, expr.literal_num(var) );
  EXPECT_EQ( 0, expr.literal_num(var, false) );
  EXPECT_EQ( 1, expr.literal_num(var, true) );
  EXPECT_EQ( 1, expr.input_size() );
  EXPECT_EQ( 1, expr.sop_literal_num() );
}

TEST_F(ExprTest, and_op1)
{
  auto lit0p = Expr::posi_literal(var0);
  auto lit1n = Expr::nega_literal(var1);
  auto expr = Expr::and_op(lit0p, lit1n);

  EXPECT_TRUE( expr.is_valid() );
  EXPECT_FALSE( expr.is_zero() );
  EXPECT_FALSE( expr.is_one() );
  EXPECT_FALSE( expr.is_constant() );
  EXPECT_FALSE( expr.is_positive_literal() );
  EXPECT_FALSE( expr.is_negative_literal() );
  EXPECT_FALSE( expr.is_literal() );
  EXPECT_EQ( BAD_VARID, expr.varid() );
  EXPECT_TRUE( expr.is_and() );
  EXPECT_FALSE( expr.is_or() );
  EXPECT_FALSE( expr.is_xor() );
  EXPECT_TRUE( expr.is_op() );
  EXPECT_EQ( 2, expr.operand_num() );
  EXPECT_EQ( (std::vector<Expr>{lit0p, lit1n}), expr.operand_list() );

  auto opr0 = expr.operand(0);
  EXPECT_TRUE( opr0.is_positive_literal() );
  EXPECT_EQ( var0, opr0.varid() );

  auto opr1 = expr.operand(1);
  EXPECT_TRUE( opr1.is_negative_literal() );
  EXPECT_EQ( var1, opr1.varid() );

  EXPECT_TRUE( expr.is_simple() );
  EXPECT_TRUE( expr.is_simple_and() );
  EXPECT_FALSE( expr.is_simple_or() );
  EXPECT_FALSE( expr.is_simple_xor() );
  EXPECT_TRUE( expr.is_sop() );

  EXPECT_EQ( 2, expr.literal_num() );
  EXPECT_EQ( 1, expr.literal_num(var0) );
  EXPECT_EQ( 1, expr.literal_num(var0, false) );
  EXPECT_EQ( 0, expr.literal_num(var0, true) );
  EXPECT_EQ( 1, expr.literal_num(var1) );
  EXPECT_EQ( 0, expr.literal_num(var1, false) );
  EXPECT_EQ( 1, expr.literal_num(var1, true) );
  EXPECT_EQ( 2, expr.input_size() );
  EXPECT_EQ( 2, expr.sop_literal_num() );

  std::ostringstream obuf;
  BinEnc bos{obuf};
  bos << expr;

  std::istringstream ibuf{obuf.str()};
  BinDec bis{ibuf};
  Expr expr2;
  bis >> expr2;

  EXPECT_EQ( expr2, expr );

  auto rep_str = expr.rep_string();
  auto expr3 = Expr::from_rep_string(rep_str);
  EXPECT_EQ( expr, expr3 );
}

TEST_F(ExprTest, and_op2)
{
  auto lit0p = Expr::posi_literal(var0);
  auto lit1n = Expr::nega_literal(var1);
  auto expr = lit0p & lit1n;

  EXPECT_TRUE( expr.is_valid() );
  EXPECT_FALSE( expr.is_zero() );
  EXPECT_FALSE( expr.is_one() );
  EXPECT_FALSE( expr.is_constant() );
  EXPECT_FALSE( expr.is_positive_literal() );
  EXPECT_FALSE( expr.is_negative_literal() );
  EXPECT_FALSE( expr.is_literal() );
  EXPECT_EQ( BAD_VARID, expr.varid() );
  EXPECT_TRUE( expr.is_and() );
  EXPECT_FALSE( expr.is_or() );
  EXPECT_FALSE( expr.is_xor() );
  EXPECT_TRUE( expr.is_op() );
  EXPECT_EQ( 2, expr.operand_num() );
  EXPECT_EQ( (std::vector<Expr>{lit0p, lit1n}), expr.operand_list() );

  auto opr0 = expr.operand(0);
  EXPECT_TRUE( opr0.is_positive_literal() );
  EXPECT_EQ( var0, opr0.varid() );

  auto opr1 = expr.operand(1);
  EXPECT_TRUE( opr1.is_negative_literal() );
  EXPECT_EQ( var1, opr1.varid() );

  EXPECT_TRUE( expr.is_simple() );
  EXPECT_TRUE( expr.is_simple_and() );
  EXPECT_FALSE( expr.is_simple_or() );
  EXPECT_FALSE( expr.is_simple_xor() );
  EXPECT_TRUE( expr.is_sop() );

  EXPECT_EQ( 2, expr.literal_num() );
  EXPECT_EQ( 1, expr.literal_num(var0) );
  EXPECT_EQ( 1, expr.literal_num(var0, false) );
  EXPECT_EQ( 0, expr.literal_num(var0, true) );
  EXPECT_EQ( 1, expr.literal_num(var1) );
  EXPECT_EQ( 0, expr.literal_num(var1, false) );
  EXPECT_EQ( 1, expr.literal_num(var1, true) );
  EXPECT_EQ( 2, expr.input_size() );
  EXPECT_EQ( 2, expr.sop_literal_num() );

  std::ostringstream obuf;
  BinEnc bos{obuf};
  bos << expr;

  std::istringstream ibuf{obuf.str()};
  BinDec bis{ibuf};
  Expr expr2;
  bis >> expr2;

  EXPECT_EQ( expr2, expr );
}

TEST_F(ExprTest, and_op3)
{
  std::vector<Expr> lit_list{};
  auto expr = Expr::and_op(lit_list);

  EXPECT_TRUE( expr.is_valid() );
  EXPECT_FALSE( expr.is_zero() );
  EXPECT_TRUE( expr.is_one() );
  EXPECT_TRUE( expr.is_constant() );
  EXPECT_FALSE( expr.is_positive_literal() );
  EXPECT_FALSE( expr.is_negative_literal() );
  EXPECT_FALSE( expr.is_literal() );
  EXPECT_EQ( BAD_VARID, expr.varid() );
  EXPECT_FALSE( expr.is_and() );
  EXPECT_FALSE( expr.is_or() );
  EXPECT_FALSE( expr.is_xor() );
  EXPECT_FALSE( expr.is_op() );
  EXPECT_EQ( 0, expr.operand_num() );
  EXPECT_EQ( std::vector<Expr>{}, expr.operand_list() );
}

TEST_F(ExprTest, and_op4)
{
  auto lit0p = Expr::posi_literal(var0);
  auto lit1n = Expr::nega_literal(var1);
  std::vector<Expr> lit_list{lit0p, lit1n};
  auto expr = Expr::and_op(lit_list);

  EXPECT_TRUE( expr.is_valid() );
  EXPECT_FALSE( expr.is_zero() );
  EXPECT_FALSE( expr.is_one() );
  EXPECT_FALSE( expr.is_constant() );
  EXPECT_FALSE( expr.is_positive_literal() );
  EXPECT_FALSE( expr.is_negative_literal() );
  EXPECT_FALSE( expr.is_literal() );
  EXPECT_EQ( BAD_VARID, expr.varid() );
  EXPECT_TRUE( expr.is_and() );
  EXPECT_FALSE( expr.is_or() );
  EXPECT_FALSE( expr.is_xor() );
  EXPECT_TRUE( expr.is_op() );
  EXPECT_EQ( 2, expr.operand_num() );
  EXPECT_EQ( (std::vector<Expr>{lit0p, lit1n}), expr.operand_list() );

  auto opr0 = expr.operand(0);
  EXPECT_TRUE( opr0.is_positive_literal() );
  EXPECT_EQ( var0, opr0.varid() );

  auto opr1 = expr.operand(1);
  EXPECT_TRUE( opr1.is_negative_literal() );
  EXPECT_EQ( var1, opr1.varid() );

  EXPECT_TRUE( expr.is_simple() );
  EXPECT_TRUE( expr.is_simple_and() );
  EXPECT_FALSE( expr.is_simple_or() );
  EXPECT_FALSE( expr.is_simple_xor() );
  EXPECT_TRUE( expr.is_sop() );

  EXPECT_EQ( 2, expr.literal_num() );
  EXPECT_EQ( 1, expr.literal_num(var0) );
  EXPECT_EQ( 1, expr.literal_num(var0, false) );
  EXPECT_EQ( 0, expr.literal_num(var0, true) );
  EXPECT_EQ( 1, expr.literal_num(var1) );
  EXPECT_EQ( 0, expr.literal_num(var1, false) );
  EXPECT_EQ( 1, expr.literal_num(var1, true) );
  EXPECT_EQ( 2, expr.input_size() );
  EXPECT_EQ( 2, expr.sop_literal_num() );

  std::ostringstream obuf;
  BinEnc bos{obuf};
  bos << expr;

  std::istringstream ibuf{obuf.str()};
  BinDec bis{ibuf};
  Expr expr2;
  bis >> expr2;

  EXPECT_EQ( expr2, expr );
}

TEST_F(ExprTest, and_op5)
{
  auto lit0p = Expr::posi_literal(var0);
  auto lit1n = Expr::nega_literal(var1);
  std::vector<Expr> lit_list{lit0p, lit1n};
  auto expr = Expr::and_op(lit_list);

  EXPECT_TRUE( expr.is_valid() );
  EXPECT_FALSE( expr.is_zero() );
  EXPECT_FALSE( expr.is_one() );
  EXPECT_FALSE( expr.is_constant() );
  EXPECT_FALSE( expr.is_positive_literal() );
  EXPECT_FALSE( expr.is_negative_literal() );
  EXPECT_FALSE( expr.is_literal() );
  EXPECT_EQ( BAD_VARID, expr.varid() );
  EXPECT_TRUE( expr.is_and() );
  EXPECT_FALSE( expr.is_or() );
  EXPECT_FALSE( expr.is_xor() );
  EXPECT_TRUE( expr.is_op() );
  EXPECT_EQ( 2, expr.operand_num() );
  EXPECT_EQ( lit_list, expr.operand_list() );

  auto opr0 = expr.operand(0);
  EXPECT_TRUE( opr0.is_positive_literal() );
  EXPECT_EQ( var0, opr0.varid() );

  auto opr1 = expr.operand(1);
  EXPECT_TRUE( opr1.is_negative_literal() );
  EXPECT_EQ( var1, opr1.varid() );

  EXPECT_TRUE( expr.is_simple() );
  EXPECT_TRUE( expr.is_simple_and() );
  EXPECT_FALSE( expr.is_simple_or() );
  EXPECT_FALSE( expr.is_simple_xor() );
  EXPECT_TRUE( expr.is_sop() );

  EXPECT_EQ( 2, expr.literal_num() );
  EXPECT_EQ( 1, expr.literal_num(var0) );
  EXPECT_EQ( 1, expr.literal_num(var0, false) );
  EXPECT_EQ( 0, expr.literal_num(var0, true) );
  EXPECT_EQ( 1, expr.literal_num(var1) );
  EXPECT_EQ( 0, expr.literal_num(var1, false) );
  EXPECT_EQ( 1, expr.literal_num(var1, true) );
  EXPECT_EQ( 2, expr.input_size() );
  EXPECT_EQ( 2, expr.sop_literal_num() );

  std::ostringstream obuf;
  BinEnc bos{obuf};
  bos << expr;

  std::istringstream ibuf{obuf.str()};
  BinDec bis{ibuf};
  Expr expr2;
  bis >> expr2;

  EXPECT_EQ( expr2, expr );
}

TEST_F(ExprTest, and_int)
{
  auto lit0p = Expr::posi_literal(var0);
  auto lit1n = Expr::nega_literal(var1);
  auto expr = lit0p;
  expr &= lit1n;

  EXPECT_TRUE( expr.is_valid() );
  EXPECT_FALSE( expr.is_zero() );
  EXPECT_FALSE( expr.is_one() );
  EXPECT_FALSE( expr.is_constant() );
  EXPECT_FALSE( expr.is_positive_literal() );
  EXPECT_FALSE( expr.is_negative_literal() );
  EXPECT_FALSE( expr.is_literal() );
  EXPECT_EQ( BAD_VARID, expr.varid() );
  EXPECT_TRUE( expr.is_and() );
  EXPECT_FALSE( expr.is_or() );
  EXPECT_FALSE( expr.is_xor() );
  EXPECT_TRUE( expr.is_op() );
  EXPECT_EQ( 2, expr.operand_num() );
  EXPECT_EQ( (std::vector<Expr>{lit0p, lit1n}), expr.operand_list() );

  auto opr0 = expr.operand(0);
  EXPECT_TRUE( opr0.is_positive_literal() );
  EXPECT_EQ( var0, opr0.varid() );

  auto opr1 = expr.operand(1);
  EXPECT_TRUE( opr1.is_negative_literal() );
  EXPECT_EQ( var1, opr1.varid() );

  EXPECT_TRUE( expr.is_simple() );
  EXPECT_TRUE( expr.is_simple_and() );
  EXPECT_FALSE( expr.is_simple_or() );
  EXPECT_FALSE( expr.is_simple_xor() );
  EXPECT_TRUE( expr.is_sop() );

  EXPECT_EQ( 2, expr.literal_num() );
  EXPECT_EQ( 1, expr.literal_num(var0) );
  EXPECT_EQ( 1, expr.literal_num(var0, false) );
  EXPECT_EQ( 0, expr.literal_num(var0, true) );
  EXPECT_EQ( 1, expr.literal_num(var1) );
  EXPECT_EQ( 0, expr.literal_num(var1, false) );
  EXPECT_EQ( 1, expr.literal_num(var1, true) );
  EXPECT_EQ( 2, expr.input_size() );
  EXPECT_EQ( 2, expr.sop_literal_num() );

  std::ostringstream obuf;
  BinEnc bos{obuf};
  bos << expr;

  std::istringstream ibuf{obuf.str()};
  BinDec bis{ibuf};
  Expr expr2;
  bis >> expr2;

  EXPECT_EQ( expr2, expr );
}

TEST_F(ExprTest, or_op1)
{
  auto lit0p = Expr::posi_literal(var0);
  auto lit1n = Expr::nega_literal(var1);
  auto expr = Expr::or_op(lit0p, lit1n);

  EXPECT_TRUE( expr.is_valid() );
  EXPECT_FALSE( expr.is_zero() );
  EXPECT_FALSE( expr.is_one() );
  EXPECT_FALSE( expr.is_constant() );
  EXPECT_FALSE( expr.is_positive_literal() );
  EXPECT_FALSE( expr.is_negative_literal() );
  EXPECT_FALSE( expr.is_literal() );
  EXPECT_EQ( BAD_VARID, expr.varid() );
  EXPECT_FALSE( expr.is_and() );
  EXPECT_TRUE( expr.is_or() );
  EXPECT_FALSE( expr.is_xor() );
  EXPECT_TRUE( expr.is_op() );
  EXPECT_EQ( 2, expr.operand_num() );
  EXPECT_EQ( (std::vector<Expr>{lit0p, lit1n}), expr.operand_list() );

  auto opr0 = expr.operand(0);
  EXPECT_TRUE( opr0.is_positive_literal() );
  EXPECT_EQ( var0, opr0.varid() );

  auto opr1 = expr.operand(1);
  EXPECT_TRUE( opr1.is_negative_literal() );
  EXPECT_EQ( var1, opr1.varid() );

  EXPECT_TRUE( expr.is_simple() );
  EXPECT_FALSE( expr.is_simple_and() );
  EXPECT_TRUE( expr.is_simple_or() );
  EXPECT_FALSE( expr.is_simple_xor() );
  EXPECT_TRUE( expr.is_sop() );

  EXPECT_EQ( 2, expr.literal_num() );
  EXPECT_EQ( 1, expr.literal_num(var0) );
  EXPECT_EQ( 1, expr.literal_num(var0, false) );
  EXPECT_EQ( 0, expr.literal_num(var0, true) );
  EXPECT_EQ( 1, expr.literal_num(var1) );
  EXPECT_EQ( 0, expr.literal_num(var1, false) );
  EXPECT_EQ( 1, expr.literal_num(var1, true) );
  EXPECT_EQ( 2, expr.input_size() );
  EXPECT_EQ( 2, expr.sop_literal_num() );

  std::ostringstream obuf;
  BinEnc bos{obuf};
  bos << expr;

  std::istringstream ibuf{obuf.str()};
  BinDec bis{ibuf};
  Expr expr2;
  bis >> expr2;

  EXPECT_EQ( expr2, expr );
}

TEST_F(ExprTest, or_op2)
{
  auto lit0p = Expr::posi_literal(var0);
  auto lit1n = Expr::nega_literal(var1);
  auto expr = lit0p | lit1n;

  EXPECT_TRUE( expr.is_valid() );
  EXPECT_FALSE( expr.is_zero() );
  EXPECT_FALSE( expr.is_one() );
  EXPECT_FALSE( expr.is_constant() );
  EXPECT_FALSE( expr.is_positive_literal() );
  EXPECT_FALSE( expr.is_negative_literal() );
  EXPECT_FALSE( expr.is_literal() );
  EXPECT_EQ( BAD_VARID, expr.varid() );
  EXPECT_FALSE( expr.is_and() );
  EXPECT_TRUE( expr.is_or() );
  EXPECT_FALSE( expr.is_xor() );
  EXPECT_TRUE( expr.is_op() );
  EXPECT_EQ( 2, expr.operand_num() );
  EXPECT_EQ( (std::vector<Expr>{lit0p, lit1n}), expr.operand_list() );

  auto opr0 = expr.operand(0);
  EXPECT_TRUE( opr0.is_positive_literal() );
  EXPECT_EQ( var0, opr0.varid() );

  auto opr1 = expr.operand(1);
  EXPECT_TRUE( opr1.is_negative_literal() );
  EXPECT_EQ( var1, opr1.varid() );

  EXPECT_TRUE( expr.is_simple() );
  EXPECT_FALSE( expr.is_simple_and() );
  EXPECT_TRUE( expr.is_simple_or() );
  EXPECT_FALSE( expr.is_simple_xor() );
  EXPECT_TRUE( expr.is_sop() );

  EXPECT_EQ( 2, expr.literal_num() );
  EXPECT_EQ( 1, expr.literal_num(var0) );
  EXPECT_EQ( 1, expr.literal_num(var0, false) );
  EXPECT_EQ( 0, expr.literal_num(var0, true) );
  EXPECT_EQ( 1, expr.literal_num(var1) );
  EXPECT_EQ( 0, expr.literal_num(var1, false) );
  EXPECT_EQ( 1, expr.literal_num(var1, true) );
  EXPECT_EQ( 2, expr.input_size() );
  EXPECT_EQ( 2, expr.sop_literal_num() );

  std::ostringstream obuf;
  BinEnc bos{obuf};
  bos << expr;

  std::istringstream ibuf{obuf.str()};
  BinDec bis{ibuf};
  Expr expr2;
  bis >> expr2;

  EXPECT_EQ( expr2, expr );
}

TEST_F(ExprTest, or_op3)
{
  std::vector<Expr> lit_list{};
  auto expr = Expr::or_op(lit_list);

  EXPECT_TRUE( expr.is_valid() );
  EXPECT_TRUE( expr.is_zero() );
  EXPECT_FALSE( expr.is_one() );
  EXPECT_TRUE( expr.is_constant() );
  EXPECT_FALSE( expr.is_positive_literal() );
  EXPECT_FALSE( expr.is_negative_literal() );
  EXPECT_FALSE( expr.is_literal() );
  EXPECT_EQ( BAD_VARID, expr.varid() );
  EXPECT_FALSE( expr.is_and() );
  EXPECT_FALSE( expr.is_or() );
  EXPECT_FALSE( expr.is_xor() );
  EXPECT_FALSE( expr.is_op() );
  EXPECT_EQ( 0, expr.operand_num() );
  EXPECT_EQ( std::vector<Expr>{}, expr.operand_list() );
}

TEST_F(ExprTest, or_op4)
{
  auto lit0p = Expr::posi_literal(var0);
  auto lit1n = Expr::nega_literal(var1);
  std::vector<Expr> lit_list{lit0p, lit1n};
  auto expr = Expr::or_op(lit_list);

  EXPECT_TRUE( expr.is_valid() );
  EXPECT_FALSE( expr.is_zero() );
  EXPECT_FALSE( expr.is_one() );
  EXPECT_FALSE( expr.is_constant() );
  EXPECT_FALSE( expr.is_positive_literal() );
  EXPECT_FALSE( expr.is_negative_literal() );
  EXPECT_FALSE( expr.is_literal() );
  EXPECT_EQ( BAD_VARID, expr.varid() );
  EXPECT_FALSE( expr.is_and() );
  EXPECT_TRUE( expr.is_or() );
  EXPECT_FALSE( expr.is_xor() );
  EXPECT_TRUE( expr.is_op() );
  EXPECT_EQ( 2, expr.operand_num() );
  EXPECT_EQ( (std::vector<Expr>{lit0p, lit1n}), expr.operand_list() );

  auto opr0 = expr.operand(0);
  EXPECT_TRUE( opr0.is_positive_literal() );
  EXPECT_EQ( var0, opr0.varid() );

  auto opr1 = expr.operand(1);
  EXPECT_TRUE( opr1.is_negative_literal() );
  EXPECT_EQ( var1, opr1.varid() );

  EXPECT_TRUE( expr.is_simple() );
  EXPECT_FALSE( expr.is_simple_and() );
  EXPECT_TRUE( expr.is_simple_or() );
  EXPECT_FALSE( expr.is_simple_xor() );
  EXPECT_TRUE( expr.is_sop() );

  EXPECT_EQ( 2, expr.literal_num() );
  EXPECT_EQ( 1, expr.literal_num(var0) );
  EXPECT_EQ( 1, expr.literal_num(var0, false) );
  EXPECT_EQ( 0, expr.literal_num(var0, true) );
  EXPECT_EQ( 1, expr.literal_num(var1) );
  EXPECT_EQ( 0, expr.literal_num(var1, false) );
  EXPECT_EQ( 1, expr.literal_num(var1, true) );
  EXPECT_EQ( 2, expr.input_size() );
  EXPECT_EQ( 2, expr.sop_literal_num() );

  std::ostringstream obuf;
  BinEnc bos{obuf};
  bos << expr;

  std::istringstream ibuf{obuf.str()};
  BinDec bis{ibuf};
  Expr expr2;
  bis >> expr2;

  EXPECT_EQ( expr2, expr );
}

TEST_F(ExprTest, or_op5)
{
  auto lit0p = Expr::posi_literal(var0);
  auto lit1n = Expr::nega_literal(var1);
  std::vector<Expr> lit_list{lit0p, lit1n};
  auto expr = Expr::or_op(lit_list);

  EXPECT_TRUE( expr.is_valid() );
  EXPECT_FALSE( expr.is_zero() );
  EXPECT_FALSE( expr.is_one() );
  EXPECT_FALSE( expr.is_constant() );
  EXPECT_FALSE( expr.is_positive_literal() );
  EXPECT_FALSE( expr.is_negative_literal() );
  EXPECT_FALSE( expr.is_literal() );
  EXPECT_EQ( BAD_VARID, expr.varid() );
  EXPECT_FALSE( expr.is_and() );
  EXPECT_TRUE( expr.is_or() );
  EXPECT_FALSE( expr.is_xor() );
  EXPECT_TRUE( expr.is_op() );
  EXPECT_EQ( 2, expr.operand_num() );
  EXPECT_EQ( (std::vector<Expr>{lit0p, lit1n}), expr.operand_list() );

  auto opr0 = expr.operand(0);
  EXPECT_TRUE( opr0.is_positive_literal() );
  EXPECT_EQ( var0, opr0.varid() );

  auto opr1 = expr.operand(1);
  EXPECT_TRUE( opr1.is_negative_literal() );
  EXPECT_EQ( var1, opr1.varid() );

  EXPECT_TRUE( expr.is_simple() );
  EXPECT_FALSE( expr.is_simple_and() );
  EXPECT_TRUE( expr.is_simple_or() );
  EXPECT_FALSE( expr.is_simple_xor() );
  EXPECT_TRUE( expr.is_sop() );

  EXPECT_EQ( 2, expr.literal_num() );
  EXPECT_EQ( 1, expr.literal_num(var0) );
  EXPECT_EQ( 1, expr.literal_num(var0, false) );
  EXPECT_EQ( 0, expr.literal_num(var0, true) );
  EXPECT_EQ( 1, expr.literal_num(var1) );
  EXPECT_EQ( 0, expr.literal_num(var1, false) );
  EXPECT_EQ( 1, expr.literal_num(var1, true) );
  EXPECT_EQ( 2, expr.input_size() );
  EXPECT_EQ( 2, expr.sop_literal_num() );

  std::ostringstream obuf;
  BinEnc bos{obuf};
  bos << expr;

  std::istringstream ibuf{obuf.str()};
  BinDec bis{ibuf};
  Expr expr2;
  bis >> expr2;

  EXPECT_EQ( expr2, expr );
}

TEST_F(ExprTest, or_int)
{
  auto lit0p = Expr::posi_literal(var0);
  auto lit1n = Expr::nega_literal(var1);
  auto expr = lit0p;
  expr |= lit1n;

  EXPECT_TRUE( expr.is_valid() );
  EXPECT_FALSE( expr.is_zero() );
  EXPECT_FALSE( expr.is_one() );
  EXPECT_FALSE( expr.is_constant() );
  EXPECT_FALSE( expr.is_positive_literal() );
  EXPECT_FALSE( expr.is_negative_literal() );
  EXPECT_FALSE( expr.is_literal() );
  EXPECT_EQ( BAD_VARID, expr.varid() );
  EXPECT_FALSE( expr.is_and() );
  EXPECT_TRUE( expr.is_or() );
  EXPECT_FALSE( expr.is_xor() );
  EXPECT_TRUE( expr.is_op() );
  EXPECT_EQ( 2, expr.operand_num() );
  EXPECT_EQ( (std::vector<Expr>{lit0p, lit1n}), expr.operand_list() );

  auto opr0 = expr.operand(0);
  EXPECT_TRUE( opr0.is_positive_literal() );
  EXPECT_EQ( var0, opr0.varid() );

  auto opr1 = expr.operand(1);
  EXPECT_TRUE( opr1.is_negative_literal() );
  EXPECT_EQ( var1, opr1.varid() );

  EXPECT_TRUE( expr.is_simple() );
  EXPECT_FALSE( expr.is_simple_and() );
  EXPECT_TRUE( expr.is_simple_or() );
  EXPECT_FALSE( expr.is_simple_xor() );
  EXPECT_TRUE( expr.is_sop() );

  EXPECT_EQ( 2, expr.literal_num() );
  EXPECT_EQ( 1, expr.literal_num(var0) );
  EXPECT_EQ( 1, expr.literal_num(var0, false) );
  EXPECT_EQ( 0, expr.literal_num(var0, true) );
  EXPECT_EQ( 1, expr.literal_num(var1) );
  EXPECT_EQ( 0, expr.literal_num(var1, false) );
  EXPECT_EQ( 1, expr.literal_num(var1, true) );
  EXPECT_EQ( 2, expr.input_size() );
  EXPECT_EQ( 2, expr.sop_literal_num() );

  std::ostringstream obuf;
  BinEnc bos{obuf};
  bos << expr;

  std::istringstream ibuf{obuf.str()};
  BinDec bis{ibuf};
  Expr expr2;
  bis >> expr2;

  EXPECT_EQ( expr2, expr );
}

TEST_F(ExprTest, xor_op1)
{
  auto lit0p = Expr::posi_literal(var0);
  auto lit1n = Expr::nega_literal(var1);
  auto expr = Expr::xor_op(lit0p, lit1n);

  EXPECT_TRUE( expr.is_valid() );
  EXPECT_FALSE( expr.is_zero() );
  EXPECT_FALSE( expr.is_one() );
  EXPECT_FALSE( expr.is_constant() );
  EXPECT_FALSE( expr.is_positive_literal() );
  EXPECT_FALSE( expr.is_negative_literal() );
  EXPECT_FALSE( expr.is_literal() );
  EXPECT_EQ( BAD_VARID, expr.varid() );
  EXPECT_FALSE( expr.is_and() );
  EXPECT_FALSE( expr.is_or() );
  EXPECT_TRUE( expr.is_xor() );
  EXPECT_TRUE( expr.is_op() );
  EXPECT_EQ( 2, expr.operand_num() );
  EXPECT_EQ( (std::vector<Expr>{lit0p, lit1n}), expr.operand_list() );

  auto opr0 = expr.operand(0);
  EXPECT_TRUE( opr0.is_positive_literal() );
  EXPECT_EQ( var0, opr0.varid() );

  auto opr1 = expr.operand(1);
  EXPECT_TRUE( opr1.is_negative_literal() );
  EXPECT_EQ( var1, opr1.varid() );

  EXPECT_TRUE( expr.is_simple() );
  EXPECT_FALSE( expr.is_simple_and() );
  EXPECT_FALSE( expr.is_simple_or() );
  EXPECT_TRUE( expr.is_simple_xor() );
  EXPECT_FALSE( expr.is_sop() );

  EXPECT_EQ( 2, expr.literal_num() );
  EXPECT_EQ( 1, expr.literal_num(var0) );
  EXPECT_EQ( 1, expr.literal_num(var0, false) );
  EXPECT_EQ( 0, expr.literal_num(var0, true) );
  EXPECT_EQ( 1, expr.literal_num(var1) );
  EXPECT_EQ( 0, expr.literal_num(var1, false) );
  EXPECT_EQ( 1, expr.literal_num(var1, true) );
  EXPECT_EQ( 2, expr.input_size() );
  EXPECT_EQ( 4, expr.sop_literal_num() );

  std::ostringstream obuf;
  BinEnc bos{obuf};
  bos << expr;

  std::istringstream ibuf{obuf.str()};
  BinDec bis{ibuf};
  Expr expr2;
  bis >> expr2;

  EXPECT_EQ( expr2, expr );
}

TEST_F(ExprTest, xor_op2)
{
  auto lit0p = Expr::posi_literal(var0);
  auto lit1n = Expr::nega_literal(var1);
  auto expr = lit0p ^ lit1n;

  EXPECT_TRUE( expr.is_valid() );
  EXPECT_FALSE( expr.is_zero() );
  EXPECT_FALSE( expr.is_one() );
  EXPECT_FALSE( expr.is_constant() );
  EXPECT_FALSE( expr.is_positive_literal() );
  EXPECT_FALSE( expr.is_negative_literal() );
  EXPECT_FALSE( expr.is_literal() );
  EXPECT_EQ( BAD_VARID, expr.varid() );
  EXPECT_FALSE( expr.is_and() );
  EXPECT_FALSE( expr.is_or() );
  EXPECT_TRUE( expr.is_xor() );
  EXPECT_TRUE( expr.is_op() );
  EXPECT_EQ( 2, expr.operand_num() );
  EXPECT_EQ( (std::vector<Expr>{lit0p, lit1n}), expr.operand_list() );

  auto opr0 = expr.operand(0);
  EXPECT_TRUE( opr0.is_positive_literal() );
  EXPECT_EQ( var0, opr0.varid() );

  auto opr1 = expr.operand(1);
  EXPECT_TRUE( opr1.is_negative_literal() );
  EXPECT_EQ( var1, opr1.varid() );

  EXPECT_TRUE( expr.is_simple() );
  EXPECT_FALSE( expr.is_simple_and() );
  EXPECT_FALSE( expr.is_simple_or() );
  EXPECT_TRUE( expr.is_simple_xor() );
  EXPECT_FALSE( expr.is_sop() );

  EXPECT_EQ( 2, expr.literal_num() );
  EXPECT_EQ( 1, expr.literal_num(var0) );
  EXPECT_EQ( 1, expr.literal_num(var0, false) );
  EXPECT_EQ( 0, expr.literal_num(var0, true) );
  EXPECT_EQ( 1, expr.literal_num(var1) );
  EXPECT_EQ( 0, expr.literal_num(var1, false) );
  EXPECT_EQ( 1, expr.literal_num(var1, true) );
  EXPECT_EQ( 2, expr.input_size() );
  EXPECT_EQ( 4, expr.sop_literal_num() );

  std::ostringstream obuf;
  BinEnc bos{obuf};
  bos << expr;

  std::istringstream ibuf{obuf.str()};
  BinDec bis{ibuf};
  Expr expr2;
  bis >> expr2;

  EXPECT_EQ( expr2, expr );
}

TEST_F(ExprTest, xor_op3)
{
  auto lit0p = Expr::posi_literal(var0);
  auto lit1n = Expr::nega_literal(var1);
  std::vector<Expr> lit_list{lit0p, lit1n};
  auto expr = Expr::xor_op(lit_list);

  EXPECT_TRUE( expr.is_valid() );
  EXPECT_FALSE( expr.is_zero() );
  EXPECT_FALSE( expr.is_one() );
  EXPECT_FALSE( expr.is_constant() );
  EXPECT_FALSE( expr.is_positive_literal() );
  EXPECT_FALSE( expr.is_negative_literal() );
  EXPECT_FALSE( expr.is_literal() );
  EXPECT_EQ( BAD_VARID, expr.varid() );
  EXPECT_FALSE( expr.is_and() );
  EXPECT_FALSE( expr.is_or() );
  EXPECT_TRUE( expr.is_xor() );
  EXPECT_TRUE( expr.is_op() );
  EXPECT_EQ( 2, expr.operand_num() );
  EXPECT_EQ( (std::vector<Expr>{lit0p, lit1n}), expr.operand_list() );

  auto opr0 = expr.operand(0);
  EXPECT_TRUE( opr0.is_positive_literal() );
  EXPECT_EQ( var0, opr0.varid() );

  auto opr1 = expr.operand(1);
  EXPECT_TRUE( opr1.is_negative_literal() );
  EXPECT_EQ( var1, opr1.varid() );

  EXPECT_TRUE( expr.is_simple() );
  EXPECT_FALSE( expr.is_simple_and() );
  EXPECT_FALSE( expr.is_simple_or() );
  EXPECT_TRUE( expr.is_simple_xor() );
  EXPECT_FALSE( expr.is_sop() );

  EXPECT_EQ( 2, expr.literal_num() );
  EXPECT_EQ( 1, expr.literal_num(var0) );
  EXPECT_EQ( 1, expr.literal_num(var0, false) );
  EXPECT_EQ( 0, expr.literal_num(var0, true) );
  EXPECT_EQ( 1, expr.literal_num(var1) );
  EXPECT_EQ( 0, expr.literal_num(var1, false) );
  EXPECT_EQ( 1, expr.literal_num(var1, true) );
  EXPECT_EQ( 2, expr.input_size() );
  EXPECT_EQ( 4, expr.sop_literal_num() );

  std::ostringstream obuf;
  BinEnc bos{obuf};
  bos << expr;

  std::istringstream ibuf{obuf.str()};
  BinDec bis{ibuf};
  Expr expr2;
  bis >> expr2;

  EXPECT_EQ( expr2, expr );
}

TEST_F(ExprTest, xor_op4)
{
  auto lit0p = Expr::posi_literal(var0);
  auto lit1n = Expr::nega_literal(var1);
  std::vector<Expr> lit_list{lit0p, lit1n};
  auto expr = Expr::xor_op(lit_list);

  EXPECT_TRUE( expr.is_valid() );
  EXPECT_FALSE( expr.is_zero() );
  EXPECT_FALSE( expr.is_one() );
  EXPECT_FALSE( expr.is_constant() );
  EXPECT_FALSE( expr.is_positive_literal() );
  EXPECT_FALSE( expr.is_negative_literal() );
  EXPECT_FALSE( expr.is_literal() );
  EXPECT_EQ( BAD_VARID, expr.varid() );
  EXPECT_FALSE( expr.is_and() );
  EXPECT_FALSE( expr.is_or() );
  EXPECT_TRUE( expr.is_xor() );
  EXPECT_TRUE( expr.is_op() );
  EXPECT_EQ( 2, expr.operand_num() );
  EXPECT_EQ( (std::vector<Expr>{lit0p, lit1n}), expr.operand_list() );

  auto opr0 = expr.operand(0);
  EXPECT_TRUE( opr0.is_positive_literal() );
  EXPECT_EQ( var0, opr0.varid() );

  auto opr1 = expr.operand(1);
  EXPECT_TRUE( opr1.is_negative_literal() );
  EXPECT_EQ( var1, opr1.varid() );

  EXPECT_TRUE( expr.is_simple() );
  EXPECT_FALSE( expr.is_simple_and() );
  EXPECT_FALSE( expr.is_simple_or() );
  EXPECT_TRUE( expr.is_simple_xor() );
  EXPECT_FALSE( expr.is_sop() );

  EXPECT_EQ( 2, expr.literal_num() );
  EXPECT_EQ( 1, expr.literal_num(var0) );
  EXPECT_EQ( 1, expr.literal_num(var0, false) );
  EXPECT_EQ( 0, expr.literal_num(var0, true) );
  EXPECT_EQ( 1, expr.literal_num(var1) );
  EXPECT_EQ( 0, expr.literal_num(var1, false) );
  EXPECT_EQ( 1, expr.literal_num(var1, true) );
  EXPECT_EQ( 2, expr.input_size() );
  EXPECT_EQ( 4, expr.sop_literal_num() );

  std::ostringstream obuf;
  BinEnc bos{obuf};
  bos << expr;

  std::istringstream ibuf{obuf.str()};
  BinDec bis{ibuf};
  Expr expr2;
  bis >> expr2;

  EXPECT_EQ( expr2, expr );
}

TEST_F(ExprTest, xor_int)
{
  auto lit0p = Expr::posi_literal(var0);
  auto lit1n = Expr::nega_literal(var1);
  auto expr = lit0p;
  expr ^= lit1n;

  EXPECT_TRUE( expr.is_valid() );
  EXPECT_FALSE( expr.is_zero() );
  EXPECT_FALSE( expr.is_one() );
  EXPECT_FALSE( expr.is_constant() );
  EXPECT_FALSE( expr.is_positive_literal() );
  EXPECT_FALSE( expr.is_negative_literal() );
  EXPECT_FALSE( expr.is_literal() );
  EXPECT_EQ( BAD_VARID, expr.varid() );
  EXPECT_FALSE( expr.is_and() );
  EXPECT_FALSE( expr.is_or() );
  EXPECT_TRUE( expr.is_xor() );
  EXPECT_TRUE( expr.is_op() );
  EXPECT_EQ( 2, expr.operand_num() );
  EXPECT_EQ( (std::vector<Expr>{lit0p, lit1n}), expr.operand_list() );

  auto opr0 = expr.operand(0);
  EXPECT_TRUE( opr0.is_positive_literal() );
  EXPECT_EQ( var0, opr0.varid() );

  auto opr1 = expr.operand(1);
  EXPECT_TRUE( opr1.is_negative_literal() );
  EXPECT_EQ( var1, opr1.varid() );

  EXPECT_TRUE( expr.is_simple() );
  EXPECT_FALSE( expr.is_simple_and() );
  EXPECT_FALSE( expr.is_simple_or() );
  EXPECT_TRUE( expr.is_simple_xor() );
  EXPECT_FALSE( expr.is_sop() );

  EXPECT_EQ( 2, expr.literal_num() );
  EXPECT_EQ( 1, expr.literal_num(var0) );
  EXPECT_EQ( 1, expr.literal_num(var0, false) );
  EXPECT_EQ( 0, expr.literal_num(var0, true) );
  EXPECT_EQ( 1, expr.literal_num(var1) );
  EXPECT_EQ( 0, expr.literal_num(var1, false) );
  EXPECT_EQ( 1, expr.literal_num(var1, true) );
  EXPECT_EQ( 2, expr.input_size() );
  EXPECT_EQ( 4, expr.sop_literal_num() );

  std::ostringstream obuf;
  BinEnc bos{obuf};
  bos << expr;

  std::istringstream ibuf{obuf.str()};
  BinDec bis{ibuf};
  Expr expr2;
  bis >> expr2;

  EXPECT_EQ( expr2, expr );
}

TEST_F(ExprTest, inv1)
{
  auto lit0p = Expr::posi_literal(var0);
  auto lit1n = Expr::nega_literal(var1);

  auto lit0n = ~lit0p;
  {
    std::ostringstream oss;
    oss << lit0n;
    EXPECT_EQ( string("~0"), oss.str() );
  }
  auto lit1p = lit1n.invert();
  {
    std::ostringstream oss;
    oss << lit1p;
    EXPECT_EQ( string("1"), oss.str() );
  }

  auto prod = lit0p & lit1n;
  auto prod_n = ~prod;
  {
    std::ostringstream oss;
    oss << prod_n;
    EXPECT_EQ( string("( ~0 | 1 )"), oss.str() );
  }
}

TEST_F(ExprTest, compose1)
{
  auto lit0p = Expr::posi_literal(var0);
  auto lit1p = Expr::posi_literal(var1);
  auto lit1n = Expr::nega_literal(var1);
  auto lit2p = Expr::posi_literal(var2);
  auto lit3p = Expr::posi_literal(var3);

  auto expr0 = lit0p | (lit1n & lit2p);
  auto expr1 = lit1p & lit3p;
  auto expr = expr0.compose(var0, expr1);

  std::ostringstream oss;
  oss << expr;
  EXPECT_EQ( string("( ( 1 & 3 ) | ( ~1 & 2 ) )"), oss.str() );

  std::ostringstream obuf;
  BinEnc bos{obuf};
  bos << expr;

  std::istringstream ibuf{obuf.str()};
  BinDec bis{ibuf};
  Expr expr2;
  bis >> expr2;

  EXPECT_EQ( expr2, expr );
}

TEST_F(ExprTest, compose2)
{
  auto lit0p = Expr::posi_literal(var0);
  auto lit1p = Expr::posi_literal(var1);
  auto lit1n = Expr::nega_literal(var1);
  auto lit2p = Expr::posi_literal(var2);
  auto lit3p = Expr::posi_literal(var3);
  auto lit4n = Expr::nega_literal(var4);

  auto expr0 = lit0p | lit4n;
  auto opr1 = lit1p & lit3p;
  auto opr2 = ~(lit1n & lit2p);
  unordered_map<SizeType, Expr> comp_map;
  comp_map[var0] = opr1;
  comp_map[var4] = opr2;
  auto expr = expr0.compose(comp_map);

  std::ostringstream oss;
  oss << expr;
  EXPECT_EQ( string("( ( 1 & 3 ) | ( ~1 & 2 ) )"), oss.str() );

  std::ostringstream obuf;
  BinEnc bos{obuf};
  bos << expr;

  std::istringstream ibuf{obuf.str()};
  BinDec bis{ibuf};
  Expr expr2;
  bis >> expr2;

  EXPECT_EQ( expr2, expr );
}

TEST_F(ExprTest, compose3)
{
  auto lit0p = Expr::posi_literal(var0);
  auto lit1p = Expr::posi_literal(var1);
  auto lit1n = Expr::nega_literal(var1);
  auto lit2p = Expr::posi_literal(var2);
  auto lit3p = Expr::posi_literal(var3);
  auto lit4n = Expr::nega_literal(var4);

  auto expr0 = lit0p | lit4n;
  auto opr1 = lit1p & lit3p;
  auto opr2 = ~(lit1n & lit2p);
  std::vector<pair<SizeType, Expr>> comp_list;
  comp_list.push_back(make_pair(var0, opr1));
  comp_list.push_back(make_pair(var4, opr2));
  auto expr = expr0.compose(comp_list);

  std::ostringstream oss;
  oss << expr;
  EXPECT_EQ( string("( ( 1 & 3 ) | ( ~1 & 2 ) )"), oss.str() );

  std::ostringstream obuf;
  BinEnc bos{obuf};
  bos << expr;

  std::istringstream ibuf{obuf.str()};
  BinDec bis{ibuf};
  Expr expr2;
  bis >> expr2;

  EXPECT_EQ( expr2, expr );
}

TEST_F(ExprTest, remap_var1)
{
  auto lit0p = Expr::posi_literal(var0);
  auto lit1p = Expr::posi_literal(var1);
  auto lit1n = Expr::nega_literal(var1);
  auto lit2p = Expr::posi_literal(var2);
  auto lit3p = Expr::posi_literal(var3);

  auto expr0 = (lit0p & lit3p) | (lit1n & lit2p);
  unordered_map<SizeType, SizeType> varmap;
  varmap[var0] = var1;
  auto expr = expr0.remap_var(varmap);

  std::ostringstream oss;
  oss << expr;
  EXPECT_EQ( string("( ( 1 & 3 ) | ( ~1 & 2 ) )"), oss.str() );

  std::ostringstream obuf;
  BinEnc bos{obuf};
  bos << expr;

  std::istringstream ibuf{obuf.str()};
  BinDec bis{ibuf};
  Expr expr2;
  bis >> expr2;

  EXPECT_EQ( expr2, expr );
}

TEST_F(ExprTest, remap_var2)
{
  auto lit0p = Expr::posi_literal(var0);
  auto lit1p = Expr::posi_literal(var1);
  auto lit1n = Expr::nega_literal(var1);
  auto lit2p = Expr::posi_literal(var2);
  auto lit3p = Expr::posi_literal(var3);

  auto expr0 = (lit0p & lit3p) | (lit1n & lit2p);
  std::vector<pair<SizeType, SizeType>> varlist;
  varlist.push_back(make_pair(var0, var1));
  auto expr = expr0.remap_var(varlist);

  std::ostringstream oss;
  oss << expr;
  EXPECT_EQ( string("( ( 1 & 3 ) | ( ~1 & 2 ) )"), oss.str() );

  std::ostringstream obuf;
  BinEnc bos{obuf};
  bos << expr;

  std::istringstream ibuf{obuf.str()};
  BinDec bis{ibuf};
  Expr expr2;
  bis >> expr2;

  EXPECT_EQ( expr2, expr );
}

TEST_F(ExprTest, eval1)
{
  auto lit0p = Expr::posi_literal(var0);
  auto lit1p = Expr::posi_literal(var1);
  auto lit1n = Expr::nega_literal(var1);
  auto lit2p = Expr::posi_literal(var2);
  auto lit3p = Expr::posi_literal(var3);

  auto expr = (lit0p & lit3p) | (lit1n & lit2p);
  std::vector<Expr::BitVectType> vals(4, 0);
  Expr::BitVectType mask = 0;
  Expr::BitVectType exp_val = 0;
  for ( int p = 0; p < 16; ++ p ) {
    mask |= (1 << p);
    bool val[4];
    for ( int i: { 0, 1, 2, 3, } ) {
      if ( p & (1 << i) ) {
	vals[i] |= (1 << p);
	val[i] = 1;
      }
      else {
	val[i] = 0;
      }
    }
    if ( (val[0] && val[3]) || (!val[1] && val[2]) ) {
      exp_val |= (1 << p);
    }
  }

  Expr::BitVectType val = expr.eval(vals, mask);
  EXPECT_EQ( exp_val, val );
}

TEST_F(ExprTest, tvfunc1)
{
  auto lit0p = Expr::posi_literal(var0);
  auto lit1p = Expr::posi_literal(var1);
  auto lit1n = Expr::nega_literal(var1);
  auto lit2p = Expr::posi_literal(var2);
  auto lit3p = Expr::posi_literal(var3);

  auto expr = (lit0p & lit3p) | (lit1n & lit2p);
  auto f = expr.tvfunc();

  for ( int p = 0; p < 16; ++ p ) {
    bool val[4];
    for ( int i: { 0, 1, 2, 3, } ) {
      if ( p & (1 << i) ) {
	val[i] = 1;
      }
      else {
	val[i] = 0;
      }
    }
    int exp_val = 0;
    if ( (val[0] && val[3]) || (!val[1] && val[2]) ) {
      exp_val = 1;
    }
    int fval = f.value(p);
    EXPECT_EQ( exp_val, f.value(p) );
  }
}

TEST_F(ExprTest, from_string1)
{
  auto expr_str = std::string{"0 + (1 * ~2)"};
  auto expr = Expr::from_string(expr_str);

  EXPECT_TRUE( expr.is_valid() );

  std::ostringstream oss;
  oss << expr;
  EXPECT_EQ( string("( 0 | ( 1 & ~2 ) )"), oss.str() );

  std::ostringstream obuf;
  BinEnc bos{obuf};
  bos << expr;

  std::istringstream ibuf{obuf.str()};
  BinDec bis{ibuf};
  Expr expr2;
  bis >> expr2;

  EXPECT_EQ( expr2, expr );
}

TEST_F(ExprTest, from_string2)
{
  auto expr_str = std::string{"0 | (1 & !2)"};
  auto expr = Expr::from_string(expr_str);

  EXPECT_TRUE( expr.is_valid() );

  std::ostringstream oss;
  oss << expr;
  EXPECT_EQ( string("( 0 | ( 1 & ~2 ) )"), oss.str() );

  std::ostringstream obuf;
  BinEnc bos{obuf};
  bos << expr;

  std::istringstream ibuf{obuf.str()};
  BinDec bis{ibuf};
  Expr expr2;
  bis >> expr2;

  EXPECT_EQ( expr2, expr );
}

TEST_F(ExprTest, from_string3)
{
  auto expr_str = std::string{"0 + + (1 * ~2)"};
  EXPECT_THROW({
      auto _ = Expr::from_string(expr_str);
    }, std::invalid_argument);
}

TEST_F(ExprTest, dump_restore)
{
  auto expr_str = std::string{"0 | (1 & !2)"};
  auto expr = Expr::from_string(expr_str);

  ASSERT_TRUE( expr.is_valid() );

  std::ostringstream obuf;
  BinEnc enc{obuf};
  expr.dump(enc);

  std::istringstream ibuf{obuf.str()};
  BinDec dec{ibuf};
  auto expr2 = Expr::restore(dec);

  EXPECT_EQ( expr2, expr );
}

END_NAMESPACE_YM
