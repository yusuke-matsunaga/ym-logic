
/// @file ExprTet.cc
/// @brief ExprTet の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.


#include "gtest/gtest.h"
#include "ym/Expr.h"
#include "ym/TvFunc.h"


BEGIN_NAMESPACE_YM

TEST(ExprTest, empty_constr)
{
  Expr expr;

  EXPECT_FALSE( expr.is_valid() );
  EXPECT_FALSE( expr.is_zero() );
  EXPECT_FALSE( expr.is_one() );
  EXPECT_FALSE( expr.is_constant() );
  EXPECT_FALSE( expr.is_posi_literal() );
  EXPECT_FALSE( expr.is_nega_literal() );
  EXPECT_FALSE( expr.is_literal() );
  EXPECT_EQ( VarId(), expr.varid() );
  EXPECT_FALSE( expr.is_and() );
  EXPECT_FALSE( expr.is_or() );
  EXPECT_FALSE( expr.is_xor() );
  EXPECT_FALSE( expr.is_op() );
  EXPECT_EQ( 0, expr.child_num() );
  EXPECT_FALSE( expr.is_simple() );
  EXPECT_FALSE( expr.is_simple_and() );
  EXPECT_FALSE( expr.is_simple_or() );
  EXPECT_FALSE( expr.is_simple_xor() );
  EXPECT_FALSE( expr.is_sop() );

  EXPECT_EQ( 0, expr.literal_num() );
  EXPECT_EQ( 0, expr.input_size() );
  EXPECT_EQ( 0, expr.sop_literal_num() );

  ostringstream obuf;
  BinEnc bos{obuf};
  bos << expr;

  istringstream ibuf{obuf.str()};
  BinDec bis{ibuf};
  Expr expr2;
  bis >> expr2;

  EXPECT_TRUE( check_equiv(expr2, expr) );
}

TEST(ExprTest, make_invalid)
{
  Expr expr{Expr::make_invalid()};

  EXPECT_FALSE( expr.is_valid() );
  EXPECT_FALSE( expr.is_zero() );
  EXPECT_FALSE( expr.is_one() );
  EXPECT_FALSE( expr.is_constant() );
  EXPECT_FALSE( expr.is_posi_literal() );
  EXPECT_FALSE( expr.is_nega_literal() );
  EXPECT_FALSE( expr.is_literal() );
  EXPECT_FALSE( expr.is_and() );
  EXPECT_FALSE( expr.is_or() );
  EXPECT_FALSE( expr.is_xor() );
  EXPECT_FALSE( expr.is_op() );
  EXPECT_EQ( 0, expr.child_num() );
  EXPECT_FALSE( expr.is_simple() );
  EXPECT_FALSE( expr.is_simple_and() );
  EXPECT_FALSE( expr.is_simple_or() );
  EXPECT_FALSE( expr.is_simple_xor() );
  EXPECT_FALSE( expr.is_sop() );

  EXPECT_EQ( 0, expr.literal_num() );
  EXPECT_EQ( 0, expr.input_size() );
  EXPECT_EQ( 0, expr.sop_literal_num() );

  ostringstream obuf;
  BinEnc bos{obuf};
  bos << expr;

  istringstream ibuf{obuf.str()};
  BinDec bis{ibuf};
  Expr expr2;
  bis >> expr2;

  EXPECT_TRUE( check_equiv(expr2, expr) );
}

TEST(ExprTest, make_zero)
{
  Expr expr = Expr::make_zero();

  EXPECT_TRUE( expr.is_valid() );
  EXPECT_TRUE( expr.is_zero() );
  EXPECT_FALSE( expr.is_one() );
  EXPECT_TRUE( expr.is_constant() );
  EXPECT_FALSE( expr.is_posi_literal() );
  EXPECT_FALSE( expr.is_nega_literal() );
  EXPECT_FALSE( expr.is_literal() );
  EXPECT_EQ( VarId(), expr.varid() );
  EXPECT_FALSE( expr.is_and() );
  EXPECT_FALSE( expr.is_or() );
  EXPECT_FALSE( expr.is_xor() );
  EXPECT_FALSE( expr.is_op() );
  EXPECT_EQ( 0, expr.child_num() );
  EXPECT_TRUE( expr.is_simple() );
  EXPECT_FALSE( expr.is_simple_and() );
  EXPECT_FALSE( expr.is_simple_or() );
  EXPECT_FALSE( expr.is_simple_xor() );
  EXPECT_TRUE( expr.is_sop() );

  EXPECT_EQ( 0, expr.literal_num() );
  EXPECT_EQ( 0, expr.input_size() );
  EXPECT_EQ( 0, expr.sop_literal_num() );

  ostringstream obuf;
  BinEnc bos{obuf};
  bos << expr;

  istringstream ibuf{obuf.str()};
  BinDec bis{ibuf};
  Expr expr2;
  bis >> expr2;

  EXPECT_TRUE( check_equiv(expr2, expr) );
}

TEST(ExprTest, make_one)
{
  Expr expr = Expr::make_one();

  EXPECT_TRUE( expr.is_valid() );
  EXPECT_FALSE( expr.is_zero() );
  EXPECT_TRUE( expr.is_one() );
  EXPECT_TRUE( expr.is_constant() );
  EXPECT_FALSE( expr.is_posi_literal() );
  EXPECT_FALSE( expr.is_nega_literal() );
  EXPECT_FALSE( expr.is_literal() );
  EXPECT_EQ( VarId(), expr.varid() );
  EXPECT_FALSE( expr.is_and() );
  EXPECT_FALSE( expr.is_or() );
  EXPECT_FALSE( expr.is_xor() );
  EXPECT_FALSE( expr.is_op() );
  EXPECT_EQ( 0, expr.child_num() );
  EXPECT_TRUE( expr.is_simple() );
  EXPECT_FALSE( expr.is_simple_and() );
  EXPECT_FALSE( expr.is_simple_or() );
  EXPECT_FALSE( expr.is_simple_xor() );
  EXPECT_TRUE( expr.is_sop() );

  EXPECT_EQ( 0, expr.literal_num() );
  EXPECT_EQ( 0, expr.input_size() );
  EXPECT_EQ( 0, expr.sop_literal_num() );

  ostringstream obuf;
  BinEnc bos{obuf};
  bos << expr;

  istringstream ibuf{obuf.str()};
  BinDec bis{ibuf};
  Expr expr2;
  bis >> expr2;

  EXPECT_TRUE( check_equiv(expr2, expr) );
}

TEST(ExprTest, make_literal1)
{
  VarId var(0);
  Expr expr = Expr::make_literal(var, false);

  EXPECT_TRUE( expr.is_valid() );
  EXPECT_FALSE( expr.is_zero() );
  EXPECT_FALSE( expr.is_one() );
  EXPECT_FALSE( expr.is_constant() );
  EXPECT_TRUE( expr.is_posi_literal() );
  EXPECT_FALSE( expr.is_nega_literal() );
  EXPECT_TRUE( expr.is_literal() );
  EXPECT_EQ( VarId(0), expr.varid() );
  EXPECT_FALSE( expr.is_and() );
  EXPECT_FALSE( expr.is_or() );
  EXPECT_FALSE( expr.is_xor() );
  EXPECT_FALSE( expr.is_op() );
  EXPECT_EQ( 0, expr.child_num() );
  EXPECT_TRUE( expr.is_simple() );
  EXPECT_FALSE( expr.is_simple_and() );
  EXPECT_FALSE( expr.is_simple_or() );
  EXPECT_FALSE( expr.is_simple_xor() );
  EXPECT_TRUE( expr.is_sop() );

  EXPECT_EQ( 1, expr.literal_num() );
  EXPECT_EQ( 1, expr.literal_num(VarId(0)) );
  EXPECT_EQ( 1, expr.literal_num(VarId(0), false) );
  EXPECT_EQ( 0, expr.literal_num(VarId(0), true) );
  EXPECT_EQ( 1, expr.input_size() );
  EXPECT_EQ( 1, expr.sop_literal_num() );

  ostringstream obuf;
  BinEnc bos{obuf};
  bos << expr;

  istringstream ibuf{obuf.str()};
  BinDec bis{ibuf};
  Expr expr2;
  bis >> expr2;

  EXPECT_TRUE( check_equiv(expr2, expr) );
}

TEST(ExprTest, make_literal2)
{
  VarId var(0);
  Literal lit(var, true);
  Expr expr = Expr::make_literal(lit);

  EXPECT_TRUE( expr.is_valid() );
  EXPECT_FALSE( expr.is_zero() );
  EXPECT_FALSE( expr.is_one() );
  EXPECT_FALSE( expr.is_constant() );
  EXPECT_FALSE( expr.is_posi_literal() );
  EXPECT_TRUE( expr.is_nega_literal() );
  EXPECT_TRUE( expr.is_literal() );
  EXPECT_EQ( VarId(0), expr.varid() );
  EXPECT_FALSE( expr.is_and() );
  EXPECT_FALSE( expr.is_or() );
  EXPECT_FALSE( expr.is_xor() );
  EXPECT_FALSE( expr.is_op() );
  EXPECT_EQ( 0, expr.child_num() );
  EXPECT_TRUE( expr.is_simple() );
  EXPECT_FALSE( expr.is_simple_and() );
  EXPECT_FALSE( expr.is_simple_or() );
  EXPECT_FALSE( expr.is_simple_xor() );
  EXPECT_TRUE( expr.is_sop() );

  EXPECT_EQ( 1, expr.literal_num() );
  EXPECT_EQ( 1, expr.literal_num(VarId(0)) );
  EXPECT_EQ( 0, expr.literal_num(VarId(0), false) );
  EXPECT_EQ( 1, expr.literal_num(VarId(0), true) );
  EXPECT_EQ( 1, expr.input_size() );
  EXPECT_EQ( 1, expr.sop_literal_num() );

  // 式自体は上と同一なので dump/restore はテストは不要
}

TEST(ExprTest, make_posi_literal)
{
  VarId var(0);
  Expr expr = Expr::make_posi_literal(var);

  EXPECT_TRUE( expr.is_valid() );
  EXPECT_FALSE( expr.is_zero() );
  EXPECT_FALSE( expr.is_one() );
  EXPECT_FALSE( expr.is_constant() );
  EXPECT_TRUE( expr.is_posi_literal() );
  EXPECT_FALSE( expr.is_nega_literal() );
  EXPECT_TRUE( expr.is_literal() );
  EXPECT_EQ( VarId(0), expr.varid() );
  EXPECT_FALSE( expr.is_and() );
  EXPECT_FALSE( expr.is_or() );
  EXPECT_FALSE( expr.is_xor() );
  EXPECT_FALSE( expr.is_op() );
  EXPECT_EQ( 0, expr.child_num() );
  EXPECT_TRUE( expr.is_simple() );
  EXPECT_FALSE( expr.is_simple_and() );
  EXPECT_FALSE( expr.is_simple_or() );
  EXPECT_FALSE( expr.is_simple_xor() );
  EXPECT_TRUE( expr.is_sop() );

  EXPECT_EQ( 1, expr.literal_num() );
  EXPECT_EQ( 1, expr.literal_num(VarId(0)) );
  EXPECT_EQ( 1, expr.literal_num(VarId(0), false) );
  EXPECT_EQ( 0, expr.literal_num(VarId(0), true) );
  EXPECT_EQ( 1, expr.input_size() );
  EXPECT_EQ( 1, expr.sop_literal_num() );
}

TEST(ExprTest, make_nega_literal)
{
  VarId var(0);
  Expr expr = Expr::make_nega_literal(var);

  EXPECT_TRUE( expr.is_valid() );
  EXPECT_FALSE( expr.is_zero() );
  EXPECT_FALSE( expr.is_one() );
  EXPECT_FALSE( expr.is_constant() );
  EXPECT_FALSE( expr.is_posi_literal() );
  EXPECT_TRUE( expr.is_nega_literal() );
  EXPECT_TRUE( expr.is_literal() );
  EXPECT_EQ( VarId(0), expr.varid() );
  EXPECT_FALSE( expr.is_and() );
  EXPECT_FALSE( expr.is_or() );
  EXPECT_FALSE( expr.is_xor() );
  EXPECT_FALSE( expr.is_op() );
  EXPECT_EQ( 0, expr.child_num() );
  EXPECT_TRUE( expr.is_simple() );
  EXPECT_FALSE( expr.is_simple_and() );
  EXPECT_FALSE( expr.is_simple_or() );
  EXPECT_FALSE( expr.is_simple_xor() );
  EXPECT_TRUE( expr.is_sop() );

  EXPECT_EQ( 1, expr.literal_num() );
  EXPECT_EQ( 1, expr.literal_num(VarId(0)) );
  EXPECT_EQ( 0, expr.literal_num(VarId(0), false) );
  EXPECT_EQ( 1, expr.literal_num(VarId(0), true) );
  EXPECT_EQ( 1, expr.input_size() );
  EXPECT_EQ( 1, expr.sop_literal_num() );
}

TEST(ExprTest, and_op1)
{
  VarId var0(0);
  VarId var1(1);
  Expr lit0p = Expr::make_posi_literal(var0);
  Expr lit1n = Expr::make_nega_literal(var1);
  Expr expr = Expr::and_op(lit0p, lit1n);

  EXPECT_TRUE( expr.is_valid() );
  EXPECT_FALSE( expr.is_zero() );
  EXPECT_FALSE( expr.is_one() );
  EXPECT_FALSE( expr.is_constant() );
  EXPECT_FALSE( expr.is_posi_literal() );
  EXPECT_FALSE( expr.is_nega_literal() );
  EXPECT_FALSE( expr.is_literal() );
  EXPECT_EQ( VarId(), expr.varid() );
  EXPECT_TRUE( expr.is_and() );
  EXPECT_FALSE( expr.is_or() );
  EXPECT_FALSE( expr.is_xor() );
  EXPECT_TRUE( expr.is_op() );
  EXPECT_EQ( 2, expr.child_num() );

  Expr child0 = expr.child(0);
  EXPECT_TRUE( child0.is_posi_literal() );
  EXPECT_EQ( VarId(0), child0.varid() );

  Expr child1 = expr.child(1);
  EXPECT_TRUE( child1.is_nega_literal() );
  EXPECT_EQ( VarId(1), child1.varid() );

  EXPECT_TRUE( expr.is_simple() );
  EXPECT_TRUE( expr.is_simple_and() );
  EXPECT_FALSE( expr.is_simple_or() );
  EXPECT_FALSE( expr.is_simple_xor() );
  EXPECT_TRUE( expr.is_sop() );

  EXPECT_EQ( 2, expr.literal_num() );
  EXPECT_EQ( 1, expr.literal_num(VarId(0)) );
  EXPECT_EQ( 1, expr.literal_num(VarId(0), false) );
  EXPECT_EQ( 0, expr.literal_num(VarId(0), true) );
  EXPECT_EQ( 1, expr.literal_num(VarId(1)) );
  EXPECT_EQ( 0, expr.literal_num(VarId(1), false) );
  EXPECT_EQ( 1, expr.literal_num(VarId(1), true) );
  EXPECT_EQ( 2, expr.input_size() );
  EXPECT_EQ( 2, expr.sop_literal_num() );

  ostringstream obuf;
  BinEnc bos{obuf};
  bos << expr;

  istringstream ibuf{obuf.str()};
  BinDec bis{ibuf};
  Expr expr2;
  bis >> expr2;

  EXPECT_TRUE( check_equiv(expr2, expr) );
}

TEST(ExprTest, and_op2)
{
  VarId var0(0);
  VarId var1(1);
  Expr lit0p = Expr::make_posi_literal(var0);
  Expr lit1n = Expr::make_nega_literal(var1);
  Expr expr = lit0p & lit1n;

  EXPECT_TRUE( expr.is_valid() );
  EXPECT_FALSE( expr.is_zero() );
  EXPECT_FALSE( expr.is_one() );
  EXPECT_FALSE( expr.is_constant() );
  EXPECT_FALSE( expr.is_posi_literal() );
  EXPECT_FALSE( expr.is_nega_literal() );
  EXPECT_FALSE( expr.is_literal() );
  EXPECT_EQ( VarId(), expr.varid() );
  EXPECT_TRUE( expr.is_and() );
  EXPECT_FALSE( expr.is_or() );
  EXPECT_FALSE( expr.is_xor() );
  EXPECT_TRUE( expr.is_op() );
  EXPECT_EQ( 2, expr.child_num() );

  Expr child0 = expr.child(0);
  EXPECT_TRUE( child0.is_posi_literal() );
  EXPECT_EQ( VarId(0), child0.varid() );

  Expr child1 = expr.child(1);
  EXPECT_TRUE( child1.is_nega_literal() );
  EXPECT_EQ( VarId(1), child1.varid() );

  EXPECT_TRUE( expr.is_simple() );
  EXPECT_TRUE( expr.is_simple_and() );
  EXPECT_FALSE( expr.is_simple_or() );
  EXPECT_FALSE( expr.is_simple_xor() );
  EXPECT_TRUE( expr.is_sop() );

  EXPECT_EQ( 2, expr.literal_num() );
  EXPECT_EQ( 1, expr.literal_num(VarId(0)) );
  EXPECT_EQ( 1, expr.literal_num(VarId(0), false) );
  EXPECT_EQ( 0, expr.literal_num(VarId(0), true) );
  EXPECT_EQ( 1, expr.literal_num(VarId(1)) );
  EXPECT_EQ( 0, expr.literal_num(VarId(1), false) );
  EXPECT_EQ( 1, expr.literal_num(VarId(1), true) );
  EXPECT_EQ( 2, expr.input_size() );
  EXPECT_EQ( 2, expr.sop_literal_num() );

  ostringstream obuf;
  BinEnc bos{obuf};
  bos << expr;

  istringstream ibuf{obuf.str()};
  BinDec bis{ibuf};
  Expr expr2;
  bis >> expr2;

  EXPECT_TRUE( check_equiv(expr2, expr) );
}

TEST(ExprTest, make_and1)
{
  VarId var0(0);
  VarId var1(1);
  Expr lit0p = Expr::make_posi_literal(var0);
  Expr lit1n = Expr::make_nega_literal(var1);
  vector<Expr> lit_list{lit0p, lit1n};
  Expr expr = Expr::make_and(lit_list);

  EXPECT_TRUE( expr.is_valid() );
  EXPECT_FALSE( expr.is_zero() );
  EXPECT_FALSE( expr.is_one() );
  EXPECT_FALSE( expr.is_constant() );
  EXPECT_FALSE( expr.is_posi_literal() );
  EXPECT_FALSE( expr.is_nega_literal() );
  EXPECT_FALSE( expr.is_literal() );
  EXPECT_EQ( VarId(), expr.varid() );
  EXPECT_TRUE( expr.is_and() );
  EXPECT_FALSE( expr.is_or() );
  EXPECT_FALSE( expr.is_xor() );
  EXPECT_TRUE( expr.is_op() );
  EXPECT_EQ( 2, expr.child_num() );

  Expr child0 = expr.child(0);
  EXPECT_TRUE( child0.is_posi_literal() );
  EXPECT_EQ( VarId(0), child0.varid() );

  Expr child1 = expr.child(1);
  EXPECT_TRUE( child1.is_nega_literal() );
  EXPECT_EQ( VarId(1), child1.varid() );

  EXPECT_TRUE( expr.is_simple() );
  EXPECT_TRUE( expr.is_simple_and() );
  EXPECT_FALSE( expr.is_simple_or() );
  EXPECT_FALSE( expr.is_simple_xor() );
  EXPECT_TRUE( expr.is_sop() );

  EXPECT_EQ( 2, expr.literal_num() );
  EXPECT_EQ( 1, expr.literal_num(VarId(0)) );
  EXPECT_EQ( 1, expr.literal_num(VarId(0), false) );
  EXPECT_EQ( 0, expr.literal_num(VarId(0), true) );
  EXPECT_EQ( 1, expr.literal_num(VarId(1)) );
  EXPECT_EQ( 0, expr.literal_num(VarId(1), false) );
  EXPECT_EQ( 1, expr.literal_num(VarId(1), true) );
  EXPECT_EQ( 2, expr.input_size() );
  EXPECT_EQ( 2, expr.sop_literal_num() );

  ostringstream obuf;
  BinEnc bos{obuf};
  bos << expr;

  istringstream ibuf{obuf.str()};
  BinDec bis{ibuf};
  Expr expr2;
  bis >> expr2;

  EXPECT_TRUE( check_equiv(expr2, expr) );
}

TEST(ExprTest, make_and2)
{
  VarId var0(0);
  VarId var1(1);
  Expr lit0p = Expr::make_posi_literal(var0);
  Expr lit1n = Expr::make_nega_literal(var1);
  vector<Expr> lit_list{lit0p, lit1n};
  Expr expr = Expr::make_and(lit_list);

  EXPECT_TRUE( expr.is_valid() );
  EXPECT_FALSE( expr.is_zero() );
  EXPECT_FALSE( expr.is_one() );
  EXPECT_FALSE( expr.is_constant() );
  EXPECT_FALSE( expr.is_posi_literal() );
  EXPECT_FALSE( expr.is_nega_literal() );
  EXPECT_FALSE( expr.is_literal() );
  EXPECT_EQ( VarId(), expr.varid() );
  EXPECT_TRUE( expr.is_and() );
  EXPECT_FALSE( expr.is_or() );
  EXPECT_FALSE( expr.is_xor() );
  EXPECT_TRUE( expr.is_op() );
  EXPECT_EQ( 2, expr.child_num() );

  Expr child0 = expr.child(0);
  EXPECT_TRUE( child0.is_posi_literal() );
  EXPECT_EQ( VarId(0), child0.varid() );

  Expr child1 = expr.child(1);
  EXPECT_TRUE( child1.is_nega_literal() );
  EXPECT_EQ( VarId(1), child1.varid() );

  EXPECT_TRUE( expr.is_simple() );
  EXPECT_TRUE( expr.is_simple_and() );
  EXPECT_FALSE( expr.is_simple_or() );
  EXPECT_FALSE( expr.is_simple_xor() );
  EXPECT_TRUE( expr.is_sop() );

  EXPECT_EQ( 2, expr.literal_num() );
  EXPECT_EQ( 1, expr.literal_num(VarId(0)) );
  EXPECT_EQ( 1, expr.literal_num(VarId(0), false) );
  EXPECT_EQ( 0, expr.literal_num(VarId(0), true) );
  EXPECT_EQ( 1, expr.literal_num(VarId(1)) );
  EXPECT_EQ( 0, expr.literal_num(VarId(1), false) );
  EXPECT_EQ( 1, expr.literal_num(VarId(1), true) );
  EXPECT_EQ( 2, expr.input_size() );
  EXPECT_EQ( 2, expr.sop_literal_num() );

  ostringstream obuf;
  BinEnc bos{obuf};
  bos << expr;

  istringstream ibuf{obuf.str()};
  BinDec bis{ibuf};
  Expr expr2;
  bis >> expr2;

  EXPECT_TRUE( check_equiv(expr2, expr) );
}

TEST(ExprTest, and_int)
{
  VarId var0(0);
  VarId var1(1);
  Expr lit0p = Expr::make_posi_literal(var0);
  Expr lit1n = Expr::make_nega_literal(var1);
  Expr expr = lit0p;
  expr &= lit1n;

  EXPECT_TRUE( expr.is_valid() );
  EXPECT_FALSE( expr.is_zero() );
  EXPECT_FALSE( expr.is_one() );
  EXPECT_FALSE( expr.is_constant() );
  EXPECT_FALSE( expr.is_posi_literal() );
  EXPECT_FALSE( expr.is_nega_literal() );
  EXPECT_FALSE( expr.is_literal() );
  EXPECT_EQ( VarId(), expr.varid() );
  EXPECT_TRUE( expr.is_and() );
  EXPECT_FALSE( expr.is_or() );
  EXPECT_FALSE( expr.is_xor() );
  EXPECT_TRUE( expr.is_op() );
  EXPECT_EQ( 2, expr.child_num() );

  Expr child0 = expr.child(0);
  EXPECT_TRUE( child0.is_posi_literal() );
  EXPECT_EQ( VarId(0), child0.varid() );

  Expr child1 = expr.child(1);
  EXPECT_TRUE( child1.is_nega_literal() );
  EXPECT_EQ( VarId(1), child1.varid() );

  EXPECT_TRUE( expr.is_simple() );
  EXPECT_TRUE( expr.is_simple_and() );
  EXPECT_FALSE( expr.is_simple_or() );
  EXPECT_FALSE( expr.is_simple_xor() );
  EXPECT_TRUE( expr.is_sop() );

  EXPECT_EQ( 2, expr.literal_num() );
  EXPECT_EQ( 1, expr.literal_num(VarId(0)) );
  EXPECT_EQ( 1, expr.literal_num(VarId(0), false) );
  EXPECT_EQ( 0, expr.literal_num(VarId(0), true) );
  EXPECT_EQ( 1, expr.literal_num(VarId(1)) );
  EXPECT_EQ( 0, expr.literal_num(VarId(1), false) );
  EXPECT_EQ( 1, expr.literal_num(VarId(1), true) );
  EXPECT_EQ( 2, expr.input_size() );
  EXPECT_EQ( 2, expr.sop_literal_num() );

  ostringstream obuf;
  BinEnc bos{obuf};
  bos << expr;

  istringstream ibuf{obuf.str()};
  BinDec bis{ibuf};
  Expr expr2;
  bis >> expr2;

  EXPECT_TRUE( check_equiv(expr2, expr) );
}

TEST(ExprTest, or_op1)
{
  VarId var0(0);
  VarId var1(1);
  Expr lit0p = Expr::make_posi_literal(var0);
  Expr lit1n = Expr::make_nega_literal(var1);
  Expr expr = Expr::or_op(lit0p, lit1n);

  EXPECT_TRUE( expr.is_valid() );
  EXPECT_FALSE( expr.is_zero() );
  EXPECT_FALSE( expr.is_one() );
  EXPECT_FALSE( expr.is_constant() );
  EXPECT_FALSE( expr.is_posi_literal() );
  EXPECT_FALSE( expr.is_nega_literal() );
  EXPECT_FALSE( expr.is_literal() );
  EXPECT_EQ( VarId(), expr.varid() );
  EXPECT_FALSE( expr.is_and() );
  EXPECT_TRUE( expr.is_or() );
  EXPECT_FALSE( expr.is_xor() );
  EXPECT_TRUE( expr.is_op() );
  EXPECT_EQ( 2, expr.child_num() );

  Expr child0 = expr.child(0);
  EXPECT_TRUE( child0.is_posi_literal() );
  EXPECT_EQ( VarId(0), child0.varid() );

  Expr child1 = expr.child(1);
  EXPECT_TRUE( child1.is_nega_literal() );
  EXPECT_EQ( VarId(1), child1.varid() );

  EXPECT_TRUE( expr.is_simple() );
  EXPECT_FALSE( expr.is_simple_and() );
  EXPECT_TRUE( expr.is_simple_or() );
  EXPECT_FALSE( expr.is_simple_xor() );
  EXPECT_TRUE( expr.is_sop() );

  EXPECT_EQ( 2, expr.literal_num() );
  EXPECT_EQ( 1, expr.literal_num(VarId(0)) );
  EXPECT_EQ( 1, expr.literal_num(VarId(0), false) );
  EXPECT_EQ( 0, expr.literal_num(VarId(0), true) );
  EXPECT_EQ( 1, expr.literal_num(VarId(1)) );
  EXPECT_EQ( 0, expr.literal_num(VarId(1), false) );
  EXPECT_EQ( 1, expr.literal_num(VarId(1), true) );
  EXPECT_EQ( 2, expr.input_size() );
  EXPECT_EQ( 2, expr.sop_literal_num() );

  ostringstream obuf;
  BinEnc bos{obuf};
  bos << expr;

  istringstream ibuf{obuf.str()};
  BinDec bis{ibuf};
  Expr expr2;
  bis >> expr2;

  EXPECT_TRUE( check_equiv(expr2, expr) );
}

TEST(ExprTest, or_op2)
{
  VarId var0(0);
  VarId var1(1);
  Expr lit0p = Expr::make_posi_literal(var0);
  Expr lit1n = Expr::make_nega_literal(var1);
  Expr expr = lit0p | lit1n;

  EXPECT_TRUE( expr.is_valid() );
  EXPECT_FALSE( expr.is_zero() );
  EXPECT_FALSE( expr.is_one() );
  EXPECT_FALSE( expr.is_constant() );
  EXPECT_FALSE( expr.is_posi_literal() );
  EXPECT_FALSE( expr.is_nega_literal() );
  EXPECT_FALSE( expr.is_literal() );
  EXPECT_EQ( VarId(), expr.varid() );
  EXPECT_FALSE( expr.is_and() );
  EXPECT_TRUE( expr.is_or() );
  EXPECT_FALSE( expr.is_xor() );
  EXPECT_TRUE( expr.is_op() );
  EXPECT_EQ( 2, expr.child_num() );

  Expr child0 = expr.child(0);
  EXPECT_TRUE( child0.is_posi_literal() );
  EXPECT_EQ( VarId(0), child0.varid() );

  Expr child1 = expr.child(1);
  EXPECT_TRUE( child1.is_nega_literal() );
  EXPECT_EQ( VarId(1), child1.varid() );

  EXPECT_TRUE( expr.is_simple() );
  EXPECT_FALSE( expr.is_simple_and() );
  EXPECT_TRUE( expr.is_simple_or() );
  EXPECT_FALSE( expr.is_simple_xor() );
  EXPECT_TRUE( expr.is_sop() );

  EXPECT_EQ( 2, expr.literal_num() );
  EXPECT_EQ( 1, expr.literal_num(VarId(0)) );
  EXPECT_EQ( 1, expr.literal_num(VarId(0), false) );
  EXPECT_EQ( 0, expr.literal_num(VarId(0), true) );
  EXPECT_EQ( 1, expr.literal_num(VarId(1)) );
  EXPECT_EQ( 0, expr.literal_num(VarId(1), false) );
  EXPECT_EQ( 1, expr.literal_num(VarId(1), true) );
  EXPECT_EQ( 2, expr.input_size() );
  EXPECT_EQ( 2, expr.sop_literal_num() );

  ostringstream obuf;
  BinEnc bos{obuf};
  bos << expr;

  istringstream ibuf{obuf.str()};
  BinDec bis{ibuf};
  Expr expr2;
  bis >> expr2;

  EXPECT_TRUE( check_equiv(expr2, expr) );
}

TEST(ExprTest, make_or1)
{
  VarId var0(0);
  VarId var1(1);
  Expr lit0p = Expr::make_posi_literal(var0);
  Expr lit1n = Expr::make_nega_literal(var1);
  vector<Expr> lit_list{lit0p, lit1n};
  Expr expr = Expr::make_or(lit_list);

  EXPECT_TRUE( expr.is_valid() );
  EXPECT_FALSE( expr.is_zero() );
  EXPECT_FALSE( expr.is_one() );
  EXPECT_FALSE( expr.is_constant() );
  EXPECT_FALSE( expr.is_posi_literal() );
  EXPECT_FALSE( expr.is_nega_literal() );
  EXPECT_FALSE( expr.is_literal() );
  EXPECT_EQ( VarId(), expr.varid() );
  EXPECT_FALSE( expr.is_and() );
  EXPECT_TRUE( expr.is_or() );
  EXPECT_FALSE( expr.is_xor() );
  EXPECT_TRUE( expr.is_op() );
  EXPECT_EQ( 2, expr.child_num() );

  Expr child0 = expr.child(0);
  EXPECT_TRUE( child0.is_posi_literal() );
  EXPECT_EQ( VarId(0), child0.varid() );

  Expr child1 = expr.child(1);
  EXPECT_TRUE( child1.is_nega_literal() );
  EXPECT_EQ( VarId(1), child1.varid() );

  EXPECT_TRUE( expr.is_simple() );
  EXPECT_FALSE( expr.is_simple_and() );
  EXPECT_TRUE( expr.is_simple_or() );
  EXPECT_FALSE( expr.is_simple_xor() );
  EXPECT_TRUE( expr.is_sop() );

  EXPECT_EQ( 2, expr.literal_num() );
  EXPECT_EQ( 1, expr.literal_num(VarId(0)) );
  EXPECT_EQ( 1, expr.literal_num(VarId(0), false) );
  EXPECT_EQ( 0, expr.literal_num(VarId(0), true) );
  EXPECT_EQ( 1, expr.literal_num(VarId(1)) );
  EXPECT_EQ( 0, expr.literal_num(VarId(1), false) );
  EXPECT_EQ( 1, expr.literal_num(VarId(1), true) );
  EXPECT_EQ( 2, expr.input_size() );
  EXPECT_EQ( 2, expr.sop_literal_num() );

  ostringstream obuf;
  BinEnc bos{obuf};
  bos << expr;

  istringstream ibuf{obuf.str()};
  BinDec bis{ibuf};
  Expr expr2;
  bis >> expr2;

  EXPECT_TRUE( check_equiv(expr2, expr) );
}

TEST(ExprTest, make_or2)
{
  VarId var0(0);
  VarId var1(1);
  Expr lit0p = Expr::make_posi_literal(var0);
  Expr lit1n = Expr::make_nega_literal(var1);
  vector<Expr> lit_list{lit0p, lit1n};
  Expr expr = Expr::make_or(lit_list);

  EXPECT_TRUE( expr.is_valid() );
  EXPECT_FALSE( expr.is_zero() );
  EXPECT_FALSE( expr.is_one() );
  EXPECT_FALSE( expr.is_constant() );
  EXPECT_FALSE( expr.is_posi_literal() );
  EXPECT_FALSE( expr.is_nega_literal() );
  EXPECT_FALSE( expr.is_literal() );
  EXPECT_EQ( VarId(), expr.varid() );
  EXPECT_FALSE( expr.is_and() );
  EXPECT_TRUE( expr.is_or() );
  EXPECT_FALSE( expr.is_xor() );
  EXPECT_TRUE( expr.is_op() );
  EXPECT_EQ( 2, expr.child_num() );

  Expr child0 = expr.child(0);
  EXPECT_TRUE( child0.is_posi_literal() );
  EXPECT_EQ( VarId(0), child0.varid() );

  Expr child1 = expr.child(1);
  EXPECT_TRUE( child1.is_nega_literal() );
  EXPECT_EQ( VarId(1), child1.varid() );

  EXPECT_TRUE( expr.is_simple() );
  EXPECT_FALSE( expr.is_simple_and() );
  EXPECT_TRUE( expr.is_simple_or() );
  EXPECT_FALSE( expr.is_simple_xor() );
  EXPECT_TRUE( expr.is_sop() );

  EXPECT_EQ( 2, expr.literal_num() );
  EXPECT_EQ( 1, expr.literal_num(VarId(0)) );
  EXPECT_EQ( 1, expr.literal_num(VarId(0), false) );
  EXPECT_EQ( 0, expr.literal_num(VarId(0), true) );
  EXPECT_EQ( 1, expr.literal_num(VarId(1)) );
  EXPECT_EQ( 0, expr.literal_num(VarId(1), false) );
  EXPECT_EQ( 1, expr.literal_num(VarId(1), true) );
  EXPECT_EQ( 2, expr.input_size() );
  EXPECT_EQ( 2, expr.sop_literal_num() );

  ostringstream obuf;
  BinEnc bos{obuf};
  bos << expr;

  istringstream ibuf{obuf.str()};
  BinDec bis{ibuf};
  Expr expr2;
  bis >> expr2;

  EXPECT_TRUE( check_equiv(expr2, expr) );
}

TEST(ExprTest, or_int)
{
  VarId var0(0);
  VarId var1(1);
  Expr lit0p = Expr::make_posi_literal(var0);
  Expr lit1n = Expr::make_nega_literal(var1);
  Expr expr = lit0p;
  expr |= lit1n;

  EXPECT_TRUE( expr.is_valid() );
  EXPECT_FALSE( expr.is_zero() );
  EXPECT_FALSE( expr.is_one() );
  EXPECT_FALSE( expr.is_constant() );
  EXPECT_FALSE( expr.is_posi_literal() );
  EXPECT_FALSE( expr.is_nega_literal() );
  EXPECT_FALSE( expr.is_literal() );
  EXPECT_EQ( VarId(), expr.varid() );
  EXPECT_FALSE( expr.is_and() );
  EXPECT_TRUE( expr.is_or() );
  EXPECT_FALSE( expr.is_xor() );
  EXPECT_TRUE( expr.is_op() );
  EXPECT_EQ( 2, expr.child_num() );

  Expr child0 = expr.child(0);
  EXPECT_TRUE( child0.is_posi_literal() );
  EXPECT_EQ( VarId(0), child0.varid() );

  Expr child1 = expr.child(1);
  EXPECT_TRUE( child1.is_nega_literal() );
  EXPECT_EQ( VarId(1), child1.varid() );

  EXPECT_TRUE( expr.is_simple() );
  EXPECT_FALSE( expr.is_simple_and() );
  EXPECT_TRUE( expr.is_simple_or() );
  EXPECT_FALSE( expr.is_simple_xor() );
  EXPECT_TRUE( expr.is_sop() );

  EXPECT_EQ( 2, expr.literal_num() );
  EXPECT_EQ( 1, expr.literal_num(VarId(0)) );
  EXPECT_EQ( 1, expr.literal_num(VarId(0), false) );
  EXPECT_EQ( 0, expr.literal_num(VarId(0), true) );
  EXPECT_EQ( 1, expr.literal_num(VarId(1)) );
  EXPECT_EQ( 0, expr.literal_num(VarId(1), false) );
  EXPECT_EQ( 1, expr.literal_num(VarId(1), true) );
  EXPECT_EQ( 2, expr.input_size() );
  EXPECT_EQ( 2, expr.sop_literal_num() );

  ostringstream obuf;
  BinEnc bos{obuf};
  bos << expr;

  istringstream ibuf{obuf.str()};
  BinDec bis{ibuf};
  Expr expr2;
  bis >> expr2;

  EXPECT_TRUE( check_equiv(expr2, expr) );
}

TEST(ExprTest, xor_op1)
{
  VarId var0(0);
  VarId var1(1);
  Expr lit0p = Expr::make_posi_literal(var0);
  Expr lit1n = Expr::make_nega_literal(var1);
  Expr expr = Expr::xor_op(lit0p, lit1n);

  EXPECT_TRUE( expr.is_valid() );
  EXPECT_FALSE( expr.is_zero() );
  EXPECT_FALSE( expr.is_one() );
  EXPECT_FALSE( expr.is_constant() );
  EXPECT_FALSE( expr.is_posi_literal() );
  EXPECT_FALSE( expr.is_nega_literal() );
  EXPECT_FALSE( expr.is_literal() );
  EXPECT_EQ( VarId(), expr.varid() );
  EXPECT_FALSE( expr.is_and() );
  EXPECT_FALSE( expr.is_or() );
  EXPECT_TRUE( expr.is_xor() );
  EXPECT_TRUE( expr.is_op() );
  EXPECT_EQ( 2, expr.child_num() );

  Expr child0 = expr.child(0);
  EXPECT_TRUE( child0.is_posi_literal() );
  EXPECT_EQ( VarId(0), child0.varid() );

  Expr child1 = expr.child(1);
  EXPECT_TRUE( child1.is_nega_literal() );
  EXPECT_EQ( VarId(1), child1.varid() );

  EXPECT_TRUE( expr.is_simple() );
  EXPECT_FALSE( expr.is_simple_and() );
  EXPECT_FALSE( expr.is_simple_or() );
  EXPECT_TRUE( expr.is_simple_xor() );
  EXPECT_FALSE( expr.is_sop() );

  EXPECT_EQ( 2, expr.literal_num() );
  EXPECT_EQ( 1, expr.literal_num(VarId(0)) );
  EXPECT_EQ( 1, expr.literal_num(VarId(0), false) );
  EXPECT_EQ( 0, expr.literal_num(VarId(0), true) );
  EXPECT_EQ( 1, expr.literal_num(VarId(1)) );
  EXPECT_EQ( 0, expr.literal_num(VarId(1), false) );
  EXPECT_EQ( 1, expr.literal_num(VarId(1), true) );
  EXPECT_EQ( 2, expr.input_size() );
  EXPECT_EQ( 4, expr.sop_literal_num() );

  ostringstream obuf;
  BinEnc bos{obuf};
  bos << expr;

  istringstream ibuf{obuf.str()};
  BinDec bis{ibuf};
  Expr expr2;
  bis >> expr2;

  EXPECT_TRUE( check_equiv(expr2, expr) );
}

TEST(ExprTest, xor_op2)
{
  VarId var0(0);
  VarId var1(1);
  Expr lit0p = Expr::make_posi_literal(var0);
  Expr lit1n = Expr::make_nega_literal(var1);
  Expr expr = lit0p ^ lit1n;

  EXPECT_TRUE( expr.is_valid() );
  EXPECT_FALSE( expr.is_zero() );
  EXPECT_FALSE( expr.is_one() );
  EXPECT_FALSE( expr.is_constant() );
  EXPECT_FALSE( expr.is_posi_literal() );
  EXPECT_FALSE( expr.is_nega_literal() );
  EXPECT_FALSE( expr.is_literal() );
  EXPECT_EQ( VarId(), expr.varid() );
  EXPECT_FALSE( expr.is_and() );
  EXPECT_FALSE( expr.is_or() );
  EXPECT_TRUE( expr.is_xor() );
  EXPECT_TRUE( expr.is_op() );
  EXPECT_EQ( 2, expr.child_num() );

  Expr child0 = expr.child(0);
  EXPECT_TRUE( child0.is_posi_literal() );
  EXPECT_EQ( VarId(0), child0.varid() );

  Expr child1 = expr.child(1);
  EXPECT_TRUE( child1.is_nega_literal() );
  EXPECT_EQ( VarId(1), child1.varid() );

  EXPECT_TRUE( expr.is_simple() );
  EXPECT_FALSE( expr.is_simple_and() );
  EXPECT_FALSE( expr.is_simple_or() );
  EXPECT_TRUE( expr.is_simple_xor() );
  EXPECT_FALSE( expr.is_sop() );

  EXPECT_EQ( 2, expr.literal_num() );
  EXPECT_EQ( 1, expr.literal_num(VarId(0)) );
  EXPECT_EQ( 1, expr.literal_num(VarId(0), false) );
  EXPECT_EQ( 0, expr.literal_num(VarId(0), true) );
  EXPECT_EQ( 1, expr.literal_num(VarId(1)) );
  EXPECT_EQ( 0, expr.literal_num(VarId(1), false) );
  EXPECT_EQ( 1, expr.literal_num(VarId(1), true) );
  EXPECT_EQ( 2, expr.input_size() );
  EXPECT_EQ( 4, expr.sop_literal_num() );

  ostringstream obuf;
  BinEnc bos{obuf};
  bos << expr;

  istringstream ibuf{obuf.str()};
  BinDec bis{ibuf};
  Expr expr2;
  bis >> expr2;

  EXPECT_TRUE( check_equiv(expr2, expr) );
}

TEST(ExprTest, make_xor1)
{
  VarId var0(0);
  VarId var1(1);
  Expr lit0p = Expr::make_posi_literal(var0);
  Expr lit1n = Expr::make_nega_literal(var1);
  vector<Expr> lit_list{lit0p, lit1n};
  Expr expr = Expr::make_xor(lit_list);

  EXPECT_TRUE( expr.is_valid() );
  EXPECT_FALSE( expr.is_zero() );
  EXPECT_FALSE( expr.is_one() );
  EXPECT_FALSE( expr.is_constant() );
  EXPECT_FALSE( expr.is_posi_literal() );
  EXPECT_FALSE( expr.is_nega_literal() );
  EXPECT_FALSE( expr.is_literal() );
  EXPECT_EQ( VarId(), expr.varid() );
  EXPECT_FALSE( expr.is_and() );
  EXPECT_FALSE( expr.is_or() );
  EXPECT_TRUE( expr.is_xor() );
  EXPECT_TRUE( expr.is_op() );
  EXPECT_EQ( 2, expr.child_num() );

  Expr child0 = expr.child(0);
  EXPECT_TRUE( child0.is_posi_literal() );
  EXPECT_EQ( VarId(0), child0.varid() );

  Expr child1 = expr.child(1);
  EXPECT_TRUE( child1.is_nega_literal() );
  EXPECT_EQ( VarId(1), child1.varid() );

  EXPECT_TRUE( expr.is_simple() );
  EXPECT_FALSE( expr.is_simple_and() );
  EXPECT_FALSE( expr.is_simple_or() );
  EXPECT_TRUE( expr.is_simple_xor() );
  EXPECT_FALSE( expr.is_sop() );

  EXPECT_EQ( 2, expr.literal_num() );
  EXPECT_EQ( 1, expr.literal_num(VarId(0)) );
  EXPECT_EQ( 1, expr.literal_num(VarId(0), false) );
  EXPECT_EQ( 0, expr.literal_num(VarId(0), true) );
  EXPECT_EQ( 1, expr.literal_num(VarId(1)) );
  EXPECT_EQ( 0, expr.literal_num(VarId(1), false) );
  EXPECT_EQ( 1, expr.literal_num(VarId(1), true) );
  EXPECT_EQ( 2, expr.input_size() );
  EXPECT_EQ( 4, expr.sop_literal_num() );

  ostringstream obuf;
  BinEnc bos{obuf};
  bos << expr;

  istringstream ibuf{obuf.str()};
  BinDec bis{ibuf};
  Expr expr2;
  bis >> expr2;

  EXPECT_TRUE( check_equiv(expr2, expr) );
}

TEST(ExprTest, make_xor2)
{
  VarId var0(0);
  VarId var1(1);
  Expr lit0p = Expr::make_posi_literal(var0);
  Expr lit1n = Expr::make_nega_literal(var1);
  vector<Expr> lit_list{lit0p, lit1n};
  Expr expr = Expr::make_xor(lit_list);

  EXPECT_TRUE( expr.is_valid() );
  EXPECT_FALSE( expr.is_zero() );
  EXPECT_FALSE( expr.is_one() );
  EXPECT_FALSE( expr.is_constant() );
  EXPECT_FALSE( expr.is_posi_literal() );
  EXPECT_FALSE( expr.is_nega_literal() );
  EXPECT_FALSE( expr.is_literal() );
  EXPECT_EQ( VarId(), expr.varid() );
  EXPECT_FALSE( expr.is_and() );
  EXPECT_FALSE( expr.is_or() );
  EXPECT_TRUE( expr.is_xor() );
  EXPECT_TRUE( expr.is_op() );
  EXPECT_EQ( 2, expr.child_num() );

  Expr child0 = expr.child(0);
  EXPECT_TRUE( child0.is_posi_literal() );
  EXPECT_EQ( VarId(0), child0.varid() );

  Expr child1 = expr.child(1);
  EXPECT_TRUE( child1.is_nega_literal() );
  EXPECT_EQ( VarId(1), child1.varid() );

  EXPECT_TRUE( expr.is_simple() );
  EXPECT_FALSE( expr.is_simple_and() );
  EXPECT_FALSE( expr.is_simple_or() );
  EXPECT_TRUE( expr.is_simple_xor() );
  EXPECT_FALSE( expr.is_sop() );

  EXPECT_EQ( 2, expr.literal_num() );
  EXPECT_EQ( 1, expr.literal_num(VarId(0)) );
  EXPECT_EQ( 1, expr.literal_num(VarId(0), false) );
  EXPECT_EQ( 0, expr.literal_num(VarId(0), true) );
  EXPECT_EQ( 1, expr.literal_num(VarId(1)) );
  EXPECT_EQ( 0, expr.literal_num(VarId(1), false) );
  EXPECT_EQ( 1, expr.literal_num(VarId(1), true) );
  EXPECT_EQ( 2, expr.input_size() );
  EXPECT_EQ( 4, expr.sop_literal_num() );

  ostringstream obuf;
  BinEnc bos{obuf};
  bos << expr;

  istringstream ibuf{obuf.str()};
  BinDec bis{ibuf};
  Expr expr2;
  bis >> expr2;

  EXPECT_TRUE( check_equiv(expr2, expr) );
}

TEST(ExprTest, xor_int)
{
  VarId var0(0);
  VarId var1(1);
  Expr lit0p = Expr::make_posi_literal(var0);
  Expr lit1n = Expr::make_nega_literal(var1);
  Expr expr = lit0p;
  expr ^= lit1n;

  EXPECT_TRUE( expr.is_valid() );
  EXPECT_FALSE( expr.is_zero() );
  EXPECT_FALSE( expr.is_one() );
  EXPECT_FALSE( expr.is_constant() );
  EXPECT_FALSE( expr.is_posi_literal() );
  EXPECT_FALSE( expr.is_nega_literal() );
  EXPECT_FALSE( expr.is_literal() );
  EXPECT_EQ( VarId(), expr.varid() );
  EXPECT_FALSE( expr.is_and() );
  EXPECT_FALSE( expr.is_or() );
  EXPECT_TRUE( expr.is_xor() );
  EXPECT_TRUE( expr.is_op() );
  EXPECT_EQ( 2, expr.child_num() );

  Expr child0 = expr.child(0);
  EXPECT_TRUE( child0.is_posi_literal() );
  EXPECT_EQ( VarId(0), child0.varid() );

  Expr child1 = expr.child(1);
  EXPECT_TRUE( child1.is_nega_literal() );
  EXPECT_EQ( VarId(1), child1.varid() );

  EXPECT_TRUE( expr.is_simple() );
  EXPECT_FALSE( expr.is_simple_and() );
  EXPECT_FALSE( expr.is_simple_or() );
  EXPECT_TRUE( expr.is_simple_xor() );
  EXPECT_FALSE( expr.is_sop() );

  EXPECT_EQ( 2, expr.literal_num() );
  EXPECT_EQ( 1, expr.literal_num(VarId(0)) );
  EXPECT_EQ( 1, expr.literal_num(VarId(0), false) );
  EXPECT_EQ( 0, expr.literal_num(VarId(0), true) );
  EXPECT_EQ( 1, expr.literal_num(VarId(1)) );
  EXPECT_EQ( 0, expr.literal_num(VarId(1), false) );
  EXPECT_EQ( 1, expr.literal_num(VarId(1), true) );
  EXPECT_EQ( 2, expr.input_size() );
  EXPECT_EQ( 4, expr.sop_literal_num() );

  ostringstream obuf;
  BinEnc bos{obuf};
  bos << expr;

  istringstream ibuf{obuf.str()};
  BinDec bis{ibuf};
  Expr expr2;
  bis >> expr2;

  EXPECT_TRUE( check_equiv(expr2, expr) );
}

TEST(ExprTest, inv1)
{
  VarId var0(0);
  VarId var1(1);
  Expr lit0p = Expr::make_posi_literal(var0);
  Expr lit1n = Expr::make_nega_literal(var1);

  Expr lit0n = ~lit0p;
  {
    ostringstream oss;
    oss << lit0n;
    EXPECT_EQ( string("~V_0"), oss.str() );
  }
  Expr lit1p = lit1n.invert();
  {
    ostringstream oss;
    oss << lit1p;
    EXPECT_EQ( string("V_1"), oss.str() );
  }

  Expr lit = lit0p & lit1n;
  Expr lit_n = ~lit;
  {
    ostringstream oss;
    oss << lit_n;
    EXPECT_EQ( string("( ~V_0 | V_1 )"), oss.str() );
  }
}

TEST(ExprTest, compose1)
{
  VarId var0(0);
  VarId var1(1);
  VarId var2(2);
  VarId var3(3);
  Expr lit0p = Expr::make_posi_literal(var0);
  Expr lit1p = Expr::make_posi_literal(var1);
  Expr lit1n = Expr::make_nega_literal(var1);
  Expr lit2p = Expr::make_posi_literal(var2);
  Expr lit3p = Expr::make_posi_literal(var3);

  Expr expr0 = lit0p | (lit1n & lit2p);
  Expr expr1 = lit1p & lit3p;
  Expr expr = expr0.compose(var0, expr1);

  ostringstream oss;
  oss << expr;
  EXPECT_EQ( string("( ( V_1 & V_3 ) | ( ~V_1 & V_2 ) )"), oss.str() );

  ostringstream obuf;
  BinEnc bos{obuf};
  bos << expr;

  istringstream ibuf{obuf.str()};
  BinDec bis{ibuf};
  Expr expr2;
  bis >> expr2;

  EXPECT_TRUE( check_equiv(expr2, expr) );
}

TEST(ExprTest, compose2)
{
  VarId var0(0);
  VarId var1(1);
  VarId var2(2);
  VarId var3(3);
  VarId var4(4);
  Expr lit0p = Expr::make_posi_literal(var0);
  Expr lit1p = Expr::make_posi_literal(var1);
  Expr lit1n = Expr::make_nega_literal(var1);
  Expr lit2p = Expr::make_posi_literal(var2);
  Expr lit3p = Expr::make_posi_literal(var3);
  Expr lit4n = Expr::make_nega_literal(var4);

  Expr expr0 = lit0p | lit4n;
  Expr child1 = lit1p & lit3p;
  Expr child2 = ~(lit1n & lit2p);
  unordered_map<VarId, Expr> comp_map;
  comp_map[var0] = child1;
  comp_map[var4] = child2;
  Expr expr = expr0.compose(comp_map);

  ostringstream oss;
  oss << expr;
  EXPECT_EQ( string("( ( V_1 & V_3 ) | ( ~V_1 & V_2 ) )"), oss.str() );

  ostringstream obuf;
  BinEnc bos{obuf};
  bos << expr;

  istringstream ibuf{obuf.str()};
  BinDec bis{ibuf};
  Expr expr2;
  bis >> expr2;

  EXPECT_TRUE( check_equiv(expr2, expr) );
}

TEST(ExprTest, compose3)
{
  VarId var0(0);
  VarId var1(1);
  VarId var2(2);
  VarId var3(3);
  VarId var4(4);
  Expr lit0p = Expr::make_posi_literal(var0);
  Expr lit1p = Expr::make_posi_literal(var1);
  Expr lit1n = Expr::make_nega_literal(var1);
  Expr lit2p = Expr::make_posi_literal(var2);
  Expr lit3p = Expr::make_posi_literal(var3);
  Expr lit4n = Expr::make_nega_literal(var4);

  Expr expr0 = lit0p | lit4n;
  Expr child1 = lit1p & lit3p;
  Expr child2 = ~(lit1n & lit2p);
  vector<pair<VarId, Expr>> comp_list;
  comp_list.push_back(make_pair(var0, child1));
  comp_list.push_back(make_pair(var4, child2));
  Expr expr = expr0.compose(comp_list);

  ostringstream oss;
  oss << expr;
  EXPECT_EQ( string("( ( V_1 & V_3 ) | ( ~V_1 & V_2 ) )"), oss.str() );

  ostringstream obuf;
  BinEnc bos{obuf};
  bos << expr;

  istringstream ibuf{obuf.str()};
  BinDec bis{ibuf};
  Expr expr2;
  bis >> expr2;

  EXPECT_TRUE( check_equiv(expr2, expr) );
}

TEST(ExprTest, remap_var1)
{
  VarId var0(0);
  VarId var1(1);
  VarId var2(2);
  VarId var3(3);
  Expr lit0p = Expr::make_posi_literal(var0);
  Expr lit1p = Expr::make_posi_literal(var1);
  Expr lit1n = Expr::make_nega_literal(var1);
  Expr lit2p = Expr::make_posi_literal(var2);
  Expr lit3p = Expr::make_posi_literal(var3);

  Expr expr0 = (lit0p & lit3p) | (lit1n & lit2p);
  unordered_map<VarId, VarId> varmap;
  varmap[var0] = var1;
  Expr expr = expr0.remap_var(varmap);

  ostringstream oss;
  oss << expr;
  EXPECT_EQ( string("( ( V_1 & V_3 ) | ( ~V_1 & V_2 ) )"), oss.str() );

  ostringstream obuf;
  BinEnc bos{obuf};
  bos << expr;

  istringstream ibuf{obuf.str()};
  BinDec bis{ibuf};
  Expr expr2;
  bis >> expr2;

  EXPECT_TRUE( check_equiv(expr2, expr) );
}

TEST(ExprTest, remap_var2)
{
  VarId var0(0);
  VarId var1(1);
  VarId var2(2);
  VarId var3(3);
  Expr lit0p = Expr::make_posi_literal(var0);
  Expr lit1p = Expr::make_posi_literal(var1);
  Expr lit1n = Expr::make_nega_literal(var1);
  Expr lit2p = Expr::make_posi_literal(var2);
  Expr lit3p = Expr::make_posi_literal(var3);

  Expr expr0 = (lit0p & lit3p) | (lit1n & lit2p);
  vector<pair<VarId, VarId>> varlist;
  varlist.push_back(make_pair(var0, var1));
  Expr expr = expr0.remap_var(varlist);

  ostringstream oss;
  oss << expr;
  EXPECT_EQ( string("( ( V_1 & V_3 ) | ( ~V_1 & V_2 ) )"), oss.str() );

  ostringstream obuf;
  BinEnc bos{obuf};
  bos << expr;

  istringstream ibuf{obuf.str()};
  BinDec bis{ibuf};
  Expr expr2;
  bis >> expr2;

  EXPECT_TRUE( check_equiv(expr2, expr) );
}

TEST(ExprTest, eval1)
{
  VarId var0(0);
  VarId var1(1);
  VarId var2(2);
  VarId var3(3);
  Expr lit0p = Expr::make_posi_literal(var0);
  Expr lit1p = Expr::make_posi_literal(var1);
  Expr lit1n = Expr::make_nega_literal(var1);
  Expr lit2p = Expr::make_posi_literal(var2);
  Expr lit3p = Expr::make_posi_literal(var3);

  Expr expr = (lit0p & lit3p) | (lit1n & lit2p);
  vector<Expr::BitVectType> vals(4, 0);
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

TEST(ExprTest, make_tv1)
{
  VarId var0(0);
  VarId var1(1);
  VarId var2(2);
  VarId var3(3);
  Expr lit0p = Expr::make_posi_literal(var0);
  Expr lit1p = Expr::make_posi_literal(var1);
  Expr lit1n = Expr::make_nega_literal(var1);
  Expr lit2p = Expr::make_posi_literal(var2);
  Expr lit3p = Expr::make_posi_literal(var3);

  Expr expr = (lit0p & lit3p) | (lit1n & lit2p);
  TvFunc f = expr.make_tv();

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

TEST(ExprTest, from_string1)
{
  string expr_str("0 + (1 * ~2)");
  string err_msg;
  Expr expr = Expr::from_string(expr_str, err_msg);

  EXPECT_TRUE( expr.is_valid() );

  ostringstream oss;
  oss << expr;
  EXPECT_EQ( string("( V_0 | ( V_1 & ~V_2 ) )"), oss.str() );

  ostringstream obuf;
  BinEnc bos{obuf};
  bos << expr;

  istringstream ibuf{obuf.str()};
  BinDec bis{ibuf};
  Expr expr2;
  bis >> expr2;

  EXPECT_TRUE( check_equiv(expr2, expr) );
}

TEST(ExprTest, from_string2)
{
  string expr_str("0 | (1 & !2)");
  string err_msg;
  Expr expr = Expr::from_string(expr_str, err_msg);

  EXPECT_TRUE( expr.is_valid() );

  ostringstream oss;
  oss << expr;
  EXPECT_EQ( string("( V_0 | ( V_1 & ~V_2 ) )"), oss.str() );

  ostringstream obuf;
  BinEnc bos{obuf};
  bos << expr;

  istringstream ibuf{obuf.str()};
  BinDec bis{ibuf};
  Expr expr2;
  bis >> expr2;

  EXPECT_TRUE( check_equiv(expr2, expr) );
}

TEST(ExprTest, from_string3)
{
  string expr_str("0 + + (1 * ~2)");
  string err_msg;
  Expr expr = Expr::from_string(expr_str, err_msg);

  EXPECT_FALSE( expr.is_valid() );
  EXPECT_EQ( string("syntax error"), err_msg);
}

TEST(ExprTest, dump_restore)
{
  string expr_str("0 | (1 & !2)");
  string err_msg;
  Expr expr = Expr::from_string(expr_str, err_msg);

  ASSERT_TRUE( expr.is_valid() );

  ostringstream obuf;
  BinEnc enc{obuf};
  expr.dump(enc);

  istringstream ibuf{obuf.str()};
  BinDec dec{ibuf};
  auto expr2 = Expr::restore(dec);

  EXPECT_TRUE( check_equiv(expr, expr2) );
}

END_NAMESPACE_YM
