
/// @file FuncAnalyzerTest.cc
/// @brief FuncAnalyzerTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016, 2023 Yusuke Matsunaga
/// All rights reserved.

#include "gtest/gtest.h"
#include "ym/Expr.h"


BEGIN_NAMESPACE_YM_LOGIC

class FuncAnalyzerTest :
  public ::testing::Test
{
public:

  /// @brief コンストラクタ
  FuncAnalyzerTest() { }

  /// @brief テストを行う．
  void
  do_test(
    const Expr& expr,    ///< [in] 論理式
    PrimType node_type ///< [in] 期待値
  );


protected:

};

void
FuncAnalyzerTest::do_test(
  const Expr& expr,
  PrimType node_type
)
{
  auto act_node_type = expr.analyze();
  EXPECT_EQ( act_node_type, node_type );
}

TEST_F(FuncAnalyzerTest, const0)
{
  auto expr = Expr::make_zero();
  do_test(expr, PrimType::C0);
}

TEST_F(FuncAnalyzerTest, const1)
{
  auto expr = Expr::make_one();
  do_test(expr, PrimType::C1);
}

TEST_F(FuncAnalyzerTest, plit)
{
  auto expr = Expr::make_posi_literal(0);
  do_test(expr, PrimType::Buff);
}

TEST_F(FuncAnalyzerTest, nlit)
{
  auto expr = Expr::make_nega_literal(0);
  do_test(expr, PrimType::Not);
}

TEST_F(FuncAnalyzerTest, and2)
{
  auto lit0 = Expr::make_posi_literal(0);
  auto lit1 = Expr::make_posi_literal(1);
  auto expr = lit0 & lit1;
  do_test(expr, PrimType::And);
}

TEST_F(FuncAnalyzerTest, and3)
{
  auto lit0 = Expr::make_posi_literal(0);
  auto lit1 = Expr::make_posi_literal(1);
  auto lit2 = Expr::make_posi_literal(2);
  auto expr = lit0 & lit1 & lit2;
  do_test(expr, PrimType::And);
}

TEST_F(FuncAnalyzerTest, and3_another)
{
  auto lit0 = Expr::make_posi_literal(0);
  auto lit1 = Expr::make_posi_literal(1);
  auto lit2 = Expr::make_posi_literal(2);
  auto expr1 = ~lit0 | ~lit1;
  auto expr = lit0 & ~expr1;
  do_test(expr, PrimType::And);
}

TEST_F(FuncAnalyzerTest, and4)
{
  auto lit0 = Expr::make_posi_literal(0);
  auto lit1 = Expr::make_posi_literal(1);
  auto lit2 = Expr::make_posi_literal(2);
  auto lit3 = Expr::make_posi_literal(3);
  auto expr = lit0 & lit1 & lit2 & lit3;
  do_test(expr, PrimType::And);
}

TEST_F(FuncAnalyzerTest, and5)
{
  auto lit0 = Expr::make_posi_literal(0);
  auto lit1 = Expr::make_posi_literal(1);
  auto lit2 = Expr::make_posi_literal(2);
  auto lit3 = Expr::make_posi_literal(3);
  auto lit4 = Expr::make_posi_literal(4);
  auto expr = lit0 & lit1 & lit2 & lit3 & lit4;
  do_test(expr, PrimType::And);
}

TEST_F(FuncAnalyzerTest, and6)
{
  auto lit0 = Expr::make_posi_literal(0);
  auto lit1 = Expr::make_posi_literal(1);
  auto lit2 = Expr::make_posi_literal(2);
  auto lit3 = Expr::make_posi_literal(3);
  auto lit4 = Expr::make_posi_literal(4);
  auto lit5 = Expr::make_posi_literal(5);
  auto expr = lit0 & lit1 & lit2 & lit3 & lit4 & lit5;
  do_test(expr, PrimType::And);
}

TEST_F(FuncAnalyzerTest, and7)
{
  auto lit0 = Expr::make_posi_literal(0);
  auto lit1 = Expr::make_posi_literal(1);
  auto lit2 = Expr::make_posi_literal(2);
  auto lit3 = Expr::make_posi_literal(3);
  auto lit4 = Expr::make_posi_literal(4);
  auto lit5 = Expr::make_posi_literal(5);
  auto lit6 = Expr::make_posi_literal(6);
  auto expr = lit0 & lit1 & lit2 & lit3 & lit4 & lit5 & lit6;
  do_test(expr, PrimType::And);
}

TEST_F(FuncAnalyzerTest, and8)
{
  auto lit0 = Expr::make_posi_literal(0);
  auto lit1 = Expr::make_posi_literal(1);
  auto lit2 = Expr::make_posi_literal(2);
  auto lit3 = Expr::make_posi_literal(3);
  auto lit4 = Expr::make_posi_literal(4);
  auto lit5 = Expr::make_posi_literal(5);
  auto lit6 = Expr::make_posi_literal(6);
  auto lit7 = Expr::make_posi_literal(7);
  auto expr = lit0 & lit1 & lit2 & lit3 & lit4 & lit5 & lit6 & lit7;
  do_test(expr, PrimType::And);
}

TEST_F(FuncAnalyzerTest, and9)
{
  auto lit0 = Expr::make_posi_literal(0);
  auto lit1 = Expr::make_posi_literal(1);
  auto lit2 = Expr::make_posi_literal(2);
  auto lit3 = Expr::make_posi_literal(3);
  auto lit4 = Expr::make_posi_literal(4);
  auto lit5 = Expr::make_posi_literal(5);
  auto lit6 = Expr::make_posi_literal(6);
  auto lit7 = Expr::make_posi_literal(7);
  auto lit8 = Expr::make_posi_literal(8);
  auto expr = lit0 & lit1 & lit2 & lit3 & lit4 & lit5 & lit6 & lit7 & lit8;
  do_test(expr, PrimType::And);
}

TEST_F(FuncAnalyzerTest, and10)
{
  auto lit0 = Expr::make_posi_literal(0);
  auto lit1 = Expr::make_posi_literal(1);
  auto lit2 = Expr::make_posi_literal(2);
  auto lit3 = Expr::make_posi_literal(3);
  auto lit4 = Expr::make_posi_literal(4);
  auto lit5 = Expr::make_posi_literal(5);
  auto lit6 = Expr::make_posi_literal(6);
  auto lit7 = Expr::make_posi_literal(7);
  auto lit8 = Expr::make_posi_literal(8);
  auto lit9 = Expr::make_posi_literal(9);
  auto expr = lit0 & lit1 & lit2 & lit3 & lit4 & lit5 & lit6 & lit7 & lit8 & lit9;
  do_test(expr, PrimType::And);
}

TEST_F(FuncAnalyzerTest, and11)
{
  auto lit0 = Expr::make_posi_literal(0);
  auto lit1 = Expr::make_posi_literal(1);
  auto lit2 = Expr::make_posi_literal(2);
  auto lit3 = Expr::make_posi_literal(3);
  auto lit4 = Expr::make_posi_literal(4);
  auto lit5 = Expr::make_posi_literal(5);
  auto lit6 = Expr::make_posi_literal(6);
  auto lit7 = Expr::make_posi_literal(7);
  auto lit8 = Expr::make_posi_literal(8);
  auto lit9 = Expr::make_posi_literal(9);
  auto lit10 = Expr::make_posi_literal(10);
  auto expr = lit0 & lit1 & lit2 & lit3 & lit4 & lit5 & lit6 & lit7 & lit8 & lit9 & lit10;
  do_test(expr, PrimType::And);
}

TEST_F(FuncAnalyzerTest, nand2)
{
  auto lit0 = Expr::make_posi_literal(0);
  auto lit1 = Expr::make_posi_literal(1);
  auto expr = ~(lit0 & lit1);
  do_test(expr, PrimType::Nand);
}

TEST_F(FuncAnalyzerTest, nand3)
{
  auto lit0 = Expr::make_posi_literal(0);
  auto lit1 = Expr::make_posi_literal(1);
  auto lit2 = Expr::make_posi_literal(2);
  auto expr = ~(lit0 & lit1 & lit2);
  do_test(expr, PrimType::Nand);
}

TEST_F(FuncAnalyzerTest, nand4)
{
  auto lit0 = Expr::make_posi_literal(0);
  auto lit1 = Expr::make_posi_literal(1);
  auto lit2 = Expr::make_posi_literal(2);
  auto lit3 = Expr::make_posi_literal(3);
  auto expr = ~(lit0 & lit1 & lit2 & lit3);
  do_test(expr, PrimType::Nand);
}

TEST_F(FuncAnalyzerTest, nand4_another)
{
  auto lit0 = Expr::make_posi_literal(0);
  auto lit1 = Expr::make_posi_literal(1);
  auto lit2 = Expr::make_posi_literal(2);
  auto lit3 = Expr::make_posi_literal(3);
  auto expr1 = ~lit0 | ~lit1;
  auto expr2 = ~lit2 | ~lit3;
  auto expr = expr1 | expr2;
  do_test(expr, PrimType::Nand);
}

TEST_F(FuncAnalyzerTest, nand5)
{
  auto lit0 = Expr::make_posi_literal(0);
  auto lit1 = Expr::make_posi_literal(1);
  auto lit2 = Expr::make_posi_literal(2);
  auto lit3 = Expr::make_posi_literal(3);
  auto lit4 = Expr::make_posi_literal(4);
  auto expr = ~(lit0 & lit1 & lit2 & lit3 & lit4);
  do_test(expr, PrimType::Nand);
}

TEST_F(FuncAnalyzerTest, nand6)
{
  auto lit0 = Expr::make_posi_literal(0);
  auto lit1 = Expr::make_posi_literal(1);
  auto lit2 = Expr::make_posi_literal(2);
  auto lit3 = Expr::make_posi_literal(3);
  auto lit4 = Expr::make_posi_literal(4);
  auto lit5 = Expr::make_posi_literal(5);
  auto expr = ~(lit0 & lit1 & lit2 & lit3 & lit4 & lit5);
  do_test(expr, PrimType::Nand);
}

TEST_F(FuncAnalyzerTest, nand7)
{
  auto lit0 = Expr::make_posi_literal(0);
  auto lit1 = Expr::make_posi_literal(1);
  auto lit2 = Expr::make_posi_literal(2);
  auto lit3 = Expr::make_posi_literal(3);
  auto lit4 = Expr::make_posi_literal(4);
  auto lit5 = Expr::make_posi_literal(5);
  auto lit6 = Expr::make_posi_literal(6);
  auto expr = ~(lit0 & lit1 & lit2 & lit3 & lit4 & lit5 & lit6);
  do_test(expr, PrimType::Nand);
}

TEST_F(FuncAnalyzerTest, nand8)
{
  auto lit0 = Expr::make_posi_literal(0);
  auto lit1 = Expr::make_posi_literal(1);
  auto lit2 = Expr::make_posi_literal(2);
  auto lit3 = Expr::make_posi_literal(3);
  auto lit4 = Expr::make_posi_literal(4);
  auto lit5 = Expr::make_posi_literal(5);
  auto lit6 = Expr::make_posi_literal(6);
  auto lit7 = Expr::make_posi_literal(7);
  auto expr = ~(lit0 & lit1 & lit2 & lit3 & lit4 & lit5 & lit6 & lit7);
  do_test(expr, PrimType::Nand);
}

TEST_F(FuncAnalyzerTest, nand9)
{
  auto lit0 = Expr::make_posi_literal(0);
  auto lit1 = Expr::make_posi_literal(1);
  auto lit2 = Expr::make_posi_literal(2);
  auto lit3 = Expr::make_posi_literal(3);
  auto lit4 = Expr::make_posi_literal(4);
  auto lit5 = Expr::make_posi_literal(5);
  auto lit6 = Expr::make_posi_literal(6);
  auto lit7 = Expr::make_posi_literal(7);
  auto lit8 = Expr::make_posi_literal(8);
  auto expr = ~(lit0 & lit1 & lit2 & lit3 & lit4 & lit5 & lit6 & lit7 & lit8);
  do_test(expr, PrimType::Nand);
}

TEST_F(FuncAnalyzerTest, nand10)
{
  auto lit0 = Expr::make_posi_literal(0);
  auto lit1 = Expr::make_posi_literal(1);
  auto lit2 = Expr::make_posi_literal(2);
  auto lit3 = Expr::make_posi_literal(3);
  auto lit4 = Expr::make_posi_literal(4);
  auto lit5 = Expr::make_posi_literal(5);
  auto lit6 = Expr::make_posi_literal(6);
  auto lit7 = Expr::make_posi_literal(7);
  auto lit8 = Expr::make_posi_literal(8);
  auto lit9 = Expr::make_posi_literal(9);
  auto expr = ~(lit0 & lit1 & lit2 & lit3 & lit4 & lit5 & lit6 & lit7 & lit8 & lit9);
  do_test(expr, PrimType::Nand);
}

TEST_F(FuncAnalyzerTest, nand11)
{
  auto lit0 = Expr::make_posi_literal(0);
  auto lit1 = Expr::make_posi_literal(1);
  auto lit2 = Expr::make_posi_literal(2);
  auto lit3 = Expr::make_posi_literal(3);
  auto lit4 = Expr::make_posi_literal(4);
  auto lit5 = Expr::make_posi_literal(5);
  auto lit6 = Expr::make_posi_literal(6);
  auto lit7 = Expr::make_posi_literal(7);
  auto lit8 = Expr::make_posi_literal(8);
  auto lit9 = Expr::make_posi_literal(9);
  auto lit10 = Expr::make_posi_literal(10);
  auto expr = ~(lit0 & lit1 & lit2 & lit3 & lit4 & lit5 & lit6 & lit7 & lit8 & lit9 & lit10);
  do_test(expr, PrimType::Nand);
}

TEST_F(FuncAnalyzerTest, or2)
{
  auto lit0 = Expr::make_posi_literal(0);
  auto lit1 = Expr::make_posi_literal(1);
  auto expr = lit0 | lit1;
  do_test(expr, PrimType::Or);
}

TEST_F(FuncAnalyzerTest, or3)
{
  auto lit0 = Expr::make_posi_literal(0);
  auto lit1 = Expr::make_posi_literal(1);
  auto lit2 = Expr::make_posi_literal(2);
  auto expr = lit0 | lit1 | lit2;
  do_test(expr, PrimType::Or);
}

TEST_F(FuncAnalyzerTest, or4)
{
  auto lit0 = Expr::make_posi_literal(0);
  auto lit1 = Expr::make_posi_literal(1);
  auto lit2 = Expr::make_posi_literal(2);
  auto lit3 = Expr::make_posi_literal(3);
  auto expr = lit0 | lit1 | lit2 | lit3;
  do_test(expr, PrimType::Or);
}

TEST_F(FuncAnalyzerTest, or4_another)
{
  auto lit0 = Expr::make_posi_literal(0);
  auto lit1 = Expr::make_posi_literal(1);
  auto lit2 = Expr::make_posi_literal(2);
  auto lit3 = Expr::make_posi_literal(3);
  auto expr1 = ~lit0 & ~lit1;
  auto expr2 = ~lit2 & ~lit3;
  auto expr = ~(expr1 & expr2);
  do_test(expr, PrimType::Or);
}

TEST_F(FuncAnalyzerTest, or5)
{
  auto lit0 = Expr::make_posi_literal(0);
  auto lit1 = Expr::make_posi_literal(1);
  auto lit2 = Expr::make_posi_literal(2);
  auto lit3 = Expr::make_posi_literal(3);
  auto lit4 = Expr::make_posi_literal(4);
  auto expr = lit0 | lit1 | lit2 | lit3 | lit4;
  do_test(expr, PrimType::Or);
}

TEST_F(FuncAnalyzerTest, or6)
{
  auto lit0 = Expr::make_posi_literal(0);
  auto lit1 = Expr::make_posi_literal(1);
  auto lit2 = Expr::make_posi_literal(2);
  auto lit3 = Expr::make_posi_literal(3);
  auto lit4 = Expr::make_posi_literal(4);
  auto lit5 = Expr::make_posi_literal(5);
  auto expr = lit0 | lit1 | lit2 | lit3 | lit4 | lit5;
  do_test(expr, PrimType::Or);
}

TEST_F(FuncAnalyzerTest, or7)
{
  auto lit0 = Expr::make_posi_literal(0);
  auto lit1 = Expr::make_posi_literal(1);
  auto lit2 = Expr::make_posi_literal(2);
  auto lit3 = Expr::make_posi_literal(3);
  auto lit4 = Expr::make_posi_literal(4);
  auto lit5 = Expr::make_posi_literal(5);
  auto lit6 = Expr::make_posi_literal(6);
  auto expr = lit0 | lit1 | lit2 | lit3 | lit4 | lit5 | lit6;
  do_test(expr, PrimType::Or);
}

TEST_F(FuncAnalyzerTest, or8)
{
  auto lit0 = Expr::make_posi_literal(0);
  auto lit1 = Expr::make_posi_literal(1);
  auto lit2 = Expr::make_posi_literal(2);
  auto lit3 = Expr::make_posi_literal(3);
  auto lit4 = Expr::make_posi_literal(4);
  auto lit5 = Expr::make_posi_literal(5);
  auto lit6 = Expr::make_posi_literal(6);
  auto lit7 = Expr::make_posi_literal(7);
  auto expr = lit0 | lit1 | lit2 | lit3 | lit4 | lit5 | lit6 | lit7;
  do_test(expr, PrimType::Or);
}

TEST_F(FuncAnalyzerTest, or9)
{
  auto lit0 = Expr::make_posi_literal(0);
  auto lit1 = Expr::make_posi_literal(1);
  auto lit2 = Expr::make_posi_literal(2);
  auto lit3 = Expr::make_posi_literal(3);
  auto lit4 = Expr::make_posi_literal(4);
  auto lit5 = Expr::make_posi_literal(5);
  auto lit6 = Expr::make_posi_literal(6);
  auto lit7 = Expr::make_posi_literal(7);
  auto lit8 = Expr::make_posi_literal(8);
  auto expr = lit0 | lit1 | lit2 | lit3 | lit4 | lit5 | lit6 | lit7 | lit8;
  do_test(expr, PrimType::Or);
}

TEST_F(FuncAnalyzerTest, or10)
{
  auto lit0 = Expr::make_posi_literal(0);
  auto lit1 = Expr::make_posi_literal(1);
  auto lit2 = Expr::make_posi_literal(2);
  auto lit3 = Expr::make_posi_literal(3);
  auto lit4 = Expr::make_posi_literal(4);
  auto lit5 = Expr::make_posi_literal(5);
  auto lit6 = Expr::make_posi_literal(6);
  auto lit7 = Expr::make_posi_literal(7);
  auto lit8 = Expr::make_posi_literal(8);
  auto lit9 = Expr::make_posi_literal(9);
  auto expr = lit0 | lit1 | lit2 | lit3 | lit4 | lit5 | lit6 | lit7 | lit8 | lit9;
  do_test(expr, PrimType::Or);
}

TEST_F(FuncAnalyzerTest, or11)
{
  auto lit0 = Expr::make_posi_literal(0);
  auto lit1 = Expr::make_posi_literal(1);
  auto lit2 = Expr::make_posi_literal(2);
  auto lit3 = Expr::make_posi_literal(3);
  auto lit4 = Expr::make_posi_literal(4);
  auto lit5 = Expr::make_posi_literal(5);
  auto lit6 = Expr::make_posi_literal(6);
  auto lit7 = Expr::make_posi_literal(7);
  auto lit8 = Expr::make_posi_literal(8);
  auto lit9 = Expr::make_posi_literal(9);
  auto lit10 = Expr::make_posi_literal(10);
  auto expr = lit0 | lit1 | lit2 | lit3 | lit4 | lit5 | lit6 | lit7 | lit8 | lit9 | lit10;
  do_test(expr, PrimType::Or);
}

TEST_F(FuncAnalyzerTest, nor2)
{
  auto lit0 = Expr::make_posi_literal(0);
  auto lit1 = Expr::make_posi_literal(1);
  auto expr = ~(lit0 | lit1);
  do_test(expr, PrimType::Nor);
}

TEST_F(FuncAnalyzerTest, nor3)
{
  auto lit0 = Expr::make_posi_literal(0);
  auto lit1 = Expr::make_posi_literal(1);
  auto lit2 = Expr::make_posi_literal(2);
  auto expr = ~(lit0 | lit1 | lit2);
  do_test(expr, PrimType::Nor);
}

TEST_F(FuncAnalyzerTest, nor4)
{
  auto lit0 = Expr::make_posi_literal(0);
  auto lit1 = Expr::make_posi_literal(1);
  auto lit2 = Expr::make_posi_literal(2);
  auto lit3 = Expr::make_posi_literal(3);
  auto expr = ~(lit0 | lit1 | lit2 | lit3);
  do_test(expr, PrimType::Nor);
}

TEST_F(FuncAnalyzerTest, nor4_another)
{
  auto lit0 = Expr::make_posi_literal(0);
  auto lit1 = Expr::make_posi_literal(1);
  auto lit2 = Expr::make_posi_literal(2);
  auto lit3 = Expr::make_posi_literal(3);
  auto expr1 = ~lit0 & ~lit1;
  auto expr2 = ~lit2 & ~lit3;
  auto expr = expr1 & expr2;
  do_test(expr, PrimType::Nor);
}

TEST_F(FuncAnalyzerTest, nor5)
{
  auto lit0 = Expr::make_posi_literal(0);
  auto lit1 = Expr::make_posi_literal(1);
  auto lit2 = Expr::make_posi_literal(2);
  auto lit3 = Expr::make_posi_literal(3);
  auto lit4 = Expr::make_posi_literal(4);
  auto expr = ~(lit0 | lit1 | lit2 | lit3 | lit4);
  do_test(expr, PrimType::Nor);
}

TEST_F(FuncAnalyzerTest, nor6)
{
  auto lit0 = Expr::make_posi_literal(0);
  auto lit1 = Expr::make_posi_literal(1);
  auto lit2 = Expr::make_posi_literal(2);
  auto lit3 = Expr::make_posi_literal(3);
  auto lit4 = Expr::make_posi_literal(4);
  auto lit5 = Expr::make_posi_literal(5);
  auto expr = ~(lit0 | lit1 | lit2 | lit3 | lit4 | lit5);
  do_test(expr, PrimType::Nor);
}

TEST_F(FuncAnalyzerTest, nor7)
{
  auto lit0 = Expr::make_posi_literal(0);
  auto lit1 = Expr::make_posi_literal(1);
  auto lit2 = Expr::make_posi_literal(2);
  auto lit3 = Expr::make_posi_literal(3);
  auto lit4 = Expr::make_posi_literal(4);
  auto lit5 = Expr::make_posi_literal(5);
  auto lit6 = Expr::make_posi_literal(6);
  auto expr = ~(lit0 | lit1 | lit2 | lit3 | lit4 | lit5 | lit6);
  do_test(expr, PrimType::Nor);
}

TEST_F(FuncAnalyzerTest, nor8)
{
  auto lit0 = Expr::make_posi_literal(0);
  auto lit1 = Expr::make_posi_literal(1);
  auto lit2 = Expr::make_posi_literal(2);
  auto lit3 = Expr::make_posi_literal(3);
  auto lit4 = Expr::make_posi_literal(4);
  auto lit5 = Expr::make_posi_literal(5);
  auto lit6 = Expr::make_posi_literal(6);
  auto lit7 = Expr::make_posi_literal(7);
  auto expr = ~(lit0 | lit1 | lit2 | lit3 | lit4 | lit5 | lit6 | lit7);
  do_test(expr, PrimType::Nor);
}

TEST_F(FuncAnalyzerTest, nor9)
{
  auto lit0 = Expr::make_posi_literal(0);
  auto lit1 = Expr::make_posi_literal(1);
  auto lit2 = Expr::make_posi_literal(2);
  auto lit3 = Expr::make_posi_literal(3);
  auto lit4 = Expr::make_posi_literal(4);
  auto lit5 = Expr::make_posi_literal(5);
  auto lit6 = Expr::make_posi_literal(6);
  auto lit7 = Expr::make_posi_literal(7);
  auto lit8 = Expr::make_posi_literal(8);
  auto expr = ~(lit0 | lit1 | lit2 | lit3 | lit4 | lit5 | lit6 | lit7 | lit8);
  do_test(expr, PrimType::Nor);
}

TEST_F(FuncAnalyzerTest, nor10)
{
  auto lit0 = Expr::make_posi_literal(0);
  auto lit1 = Expr::make_posi_literal(1);
  auto lit2 = Expr::make_posi_literal(2);
  auto lit3 = Expr::make_posi_literal(3);
  auto lit4 = Expr::make_posi_literal(4);
  auto lit5 = Expr::make_posi_literal(5);
  auto lit6 = Expr::make_posi_literal(6);
  auto lit7 = Expr::make_posi_literal(7);
  auto lit8 = Expr::make_posi_literal(8);
  auto lit9 = Expr::make_posi_literal(9);
  auto expr = ~(lit0 | lit1 | lit2 | lit3 | lit4 | lit5 | lit6 | lit7 | lit8 | lit9);
  do_test(expr, PrimType::Nor);
}

TEST_F(FuncAnalyzerTest, nor11)
{
  auto lit0 = Expr::make_posi_literal(0);
  auto lit1 = Expr::make_posi_literal(1);
  auto lit2 = Expr::make_posi_literal(2);
  auto lit3 = Expr::make_posi_literal(3);
  auto lit4 = Expr::make_posi_literal(4);
  auto lit5 = Expr::make_posi_literal(5);
  auto lit6 = Expr::make_posi_literal(6);
  auto lit7 = Expr::make_posi_literal(7);
  auto lit8 = Expr::make_posi_literal(8);
  auto lit9 = Expr::make_posi_literal(9);
  auto lit10 = Expr::make_posi_literal(10);
  auto expr = ~(lit0 | lit1 | lit2 | lit3 | lit4 | lit5 | lit6 | lit7 | lit8 | lit9 | lit10);
  do_test(expr, PrimType::Nor);
}

TEST_F(FuncAnalyzerTest, xor2)
{
  auto lit0 = Expr::make_posi_literal(0);
  auto lit1 = Expr::make_posi_literal(1);
  auto expr = lit0 ^ lit1;
  do_test(expr, PrimType::Xor);
}

TEST_F(FuncAnalyzerTest, xor2_another)
{
  auto lit0 = Expr::make_posi_literal(0);
  auto lit1 = Expr::make_posi_literal(1);
  auto expr = (lit0 & ~lit1) | (~lit0 & lit1);
  do_test(expr, PrimType::Xor);
}

TEST_F(FuncAnalyzerTest, xor3)
{
  auto lit0 = Expr::make_posi_literal(0);
  auto lit1 = Expr::make_posi_literal(1);
  auto lit2 = Expr::make_posi_literal(2);
  auto expr = lit0 ^ lit1 ^ lit2;
  do_test(expr, PrimType::Xor);
}

TEST_F(FuncAnalyzerTest, xor4)
{
  auto lit0 = Expr::make_posi_literal(0);
  auto lit1 = Expr::make_posi_literal(1);
  auto lit2 = Expr::make_posi_literal(2);
  auto lit3 = Expr::make_posi_literal(3);
  auto expr = lit0 ^ lit1 ^ lit2 ^ lit3;
  do_test(expr, PrimType::Xor);
}

TEST_F(FuncAnalyzerTest, xor5)
{
  auto lit0 = Expr::make_posi_literal(0);
  auto lit1 = Expr::make_posi_literal(1);
  auto lit2 = Expr::make_posi_literal(2);
  auto lit3 = Expr::make_posi_literal(3);
  auto lit4 = Expr::make_posi_literal(4);
  auto expr = lit0 ^ lit1 ^ lit2 ^ lit3 ^ lit4;
  do_test(expr, PrimType::Xor);
}

TEST_F(FuncAnalyzerTest, xor6)
{
  auto lit0 = Expr::make_posi_literal(0);
  auto lit1 = Expr::make_posi_literal(1);
  auto lit2 = Expr::make_posi_literal(2);
  auto lit3 = Expr::make_posi_literal(3);
  auto lit4 = Expr::make_posi_literal(4);
  auto lit5 = Expr::make_posi_literal(5);
  auto expr = lit0 ^ lit1 ^ lit2 ^ lit3 ^ lit4 ^ lit5;
  do_test(expr, PrimType::Xor);
}

TEST_F(FuncAnalyzerTest, xor7)
{
  auto lit0 = Expr::make_posi_literal(0);
  auto lit1 = Expr::make_posi_literal(1);
  auto lit2 = Expr::make_posi_literal(2);
  auto lit3 = Expr::make_posi_literal(3);
  auto lit4 = Expr::make_posi_literal(4);
  auto lit5 = Expr::make_posi_literal(5);
  auto lit6 = Expr::make_posi_literal(6);
  auto expr = lit0 ^ lit1 ^ lit2 ^ lit3 ^ lit4 ^ lit5 ^ lit6;
  do_test(expr, PrimType::Xor);
}

TEST_F(FuncAnalyzerTest, xor8)
{
  auto lit0 = Expr::make_posi_literal(0);
  auto lit1 = Expr::make_posi_literal(1);
  auto lit2 = Expr::make_posi_literal(2);
  auto lit3 = Expr::make_posi_literal(3);
  auto lit4 = Expr::make_posi_literal(4);
  auto lit5 = Expr::make_posi_literal(5);
  auto lit6 = Expr::make_posi_literal(6);
  auto lit7 = Expr::make_posi_literal(7);
  auto expr = lit0 ^ lit1 ^ lit2 ^ lit3 ^ lit4 ^ lit5 ^ lit6 ^ lit7;
  do_test(expr, PrimType::Xor);
}

TEST_F(FuncAnalyzerTest, xor9)
{
  auto lit0 = Expr::make_posi_literal(0);
  auto lit1 = Expr::make_posi_literal(1);
  auto lit2 = Expr::make_posi_literal(2);
  auto lit3 = Expr::make_posi_literal(3);
  auto lit4 = Expr::make_posi_literal(4);
  auto lit5 = Expr::make_posi_literal(5);
  auto lit6 = Expr::make_posi_literal(6);
  auto lit7 = Expr::make_posi_literal(7);
  auto lit8 = Expr::make_posi_literal(8);
  auto expr = lit0 ^ lit1 ^ lit2 ^ lit3 ^ lit4 ^ lit5 ^ lit6 ^ lit7 ^ lit8;
  do_test(expr, PrimType::Xor);
}

TEST_F(FuncAnalyzerTest, xor10)
{
  auto lit0 = Expr::make_posi_literal(0);
  auto lit1 = Expr::make_posi_literal(1);
  auto lit2 = Expr::make_posi_literal(2);
  auto lit3 = Expr::make_posi_literal(3);
  auto lit4 = Expr::make_posi_literal(4);
  auto lit5 = Expr::make_posi_literal(5);
  auto lit6 = Expr::make_posi_literal(6);
  auto lit7 = Expr::make_posi_literal(7);
  auto lit8 = Expr::make_posi_literal(8);
  auto lit9 = Expr::make_posi_literal(9);
  auto expr = lit0 ^ lit1 ^ lit2 ^ lit3 ^ lit4 ^ lit5 ^ lit6 ^ lit7 ^ lit8 ^ lit9;
  do_test(expr, PrimType::Xor);
}

TEST_F(FuncAnalyzerTest, xor11)
{
  auto lit0 = Expr::make_posi_literal(0);
  auto lit1 = Expr::make_posi_literal(1);
  auto lit2 = Expr::make_posi_literal(2);
  auto lit3 = Expr::make_posi_literal(3);
  auto lit4 = Expr::make_posi_literal(4);
  auto lit5 = Expr::make_posi_literal(5);
  auto lit6 = Expr::make_posi_literal(6);
  auto lit7 = Expr::make_posi_literal(7);
  auto lit8 = Expr::make_posi_literal(8);
  auto lit9 = Expr::make_posi_literal(9);
  auto lit10 = Expr::make_posi_literal(10);
  auto expr = lit0 ^ lit1 ^ lit2 ^ lit3 ^ lit4 ^ lit5 ^ lit6 ^ lit7 ^ lit8 ^ lit9 ^ lit10;
  do_test(expr, PrimType::Xor);
}

TEST_F(FuncAnalyzerTest, xnor2)
{
  auto lit0 = Expr::make_posi_literal(0);
  auto lit1 = Expr::make_posi_literal(1);
  auto expr = ~(lit0 ^ lit1);
  do_test(expr, PrimType::Xnor);
}

TEST_F(FuncAnalyzerTest, xnor2_another)
{
  auto lit0 = Expr::make_posi_literal(0);
  auto lit1 = Expr::make_posi_literal(1);
  auto expr = (lit0 & lit1) | (~lit0 & ~lit1);
  do_test(expr, PrimType::Xnor);
}

TEST_F(FuncAnalyzerTest, xnor3)
{
  auto lit0 = Expr::make_posi_literal(0);
  auto lit1 = Expr::make_posi_literal(1);
  auto lit2 = Expr::make_posi_literal(2);
  auto expr = ~(lit0 ^ lit1 ^ lit2);
  do_test(expr, PrimType::Xnor);
}

TEST_F(FuncAnalyzerTest, xnor4)
{
  auto lit0 = Expr::make_posi_literal(0);
  auto lit1 = Expr::make_posi_literal(1);
  auto lit2 = Expr::make_posi_literal(2);
  auto lit3 = Expr::make_posi_literal(3);
  auto expr = ~(lit0 ^ lit1 ^ lit2 ^ lit3);
  do_test(expr, PrimType::Xnor);
}

TEST_F(FuncAnalyzerTest, xnor5)
{
  auto lit0 = Expr::make_posi_literal(0);
  auto lit1 = Expr::make_posi_literal(1);
  auto lit2 = Expr::make_posi_literal(2);
  auto lit3 = Expr::make_posi_literal(3);
  auto lit4 = Expr::make_posi_literal(4);
  auto expr = ~(lit0 ^ lit1 ^ lit2 ^ lit3 ^ lit4);
  do_test(expr, PrimType::Xnor);
}

TEST_F(FuncAnalyzerTest, xnor6)
{
  auto lit0 = Expr::make_posi_literal(0);
  auto lit1 = Expr::make_posi_literal(1);
  auto lit2 = Expr::make_posi_literal(2);
  auto lit3 = Expr::make_posi_literal(3);
  auto lit4 = Expr::make_posi_literal(4);
  auto lit5 = Expr::make_posi_literal(5);
  auto expr = ~(lit0 ^ lit1 ^ lit2 ^ lit3 ^ lit4 ^ lit5);
  do_test(expr, PrimType::Xnor);
}

TEST_F(FuncAnalyzerTest, xnor7)
{
  auto lit0 = Expr::make_posi_literal(0);
  auto lit1 = Expr::make_posi_literal(1);
  auto lit2 = Expr::make_posi_literal(2);
  auto lit3 = Expr::make_posi_literal(3);
  auto lit4 = Expr::make_posi_literal(4);
  auto lit5 = Expr::make_posi_literal(5);
  auto lit6 = Expr::make_posi_literal(6);
  auto expr = ~(lit0 ^ lit1 ^ lit2 ^ lit3 ^ lit4 ^ lit5 ^ lit6);
  do_test(expr, PrimType::Xnor);
}

TEST_F(FuncAnalyzerTest, xnor8)
{
  auto lit0 = Expr::make_posi_literal(0);
  auto lit1 = Expr::make_posi_literal(1);
  auto lit2 = Expr::make_posi_literal(2);
  auto lit3 = Expr::make_posi_literal(3);
  auto lit4 = Expr::make_posi_literal(4);
  auto lit5 = Expr::make_posi_literal(5);
  auto lit6 = Expr::make_posi_literal(6);
  auto lit7 = Expr::make_posi_literal(7);
  auto expr = ~(lit0 ^ lit1 ^ lit2 ^ lit3 ^ lit4 ^ lit5 ^ lit6 ^ lit7);
  do_test(expr, PrimType::Xnor);
}

TEST_F(FuncAnalyzerTest, xnor9)
{
  auto lit0 = Expr::make_posi_literal(0);
  auto lit1 = Expr::make_posi_literal(1);
  auto lit2 = Expr::make_posi_literal(2);
  auto lit3 = Expr::make_posi_literal(3);
  auto lit4 = Expr::make_posi_literal(4);
  auto lit5 = Expr::make_posi_literal(5);
  auto lit6 = Expr::make_posi_literal(6);
  auto lit7 = Expr::make_posi_literal(7);
  auto lit8 = Expr::make_posi_literal(8);
  auto expr = ~(lit0 ^ lit1 ^ lit2 ^ lit3 ^ lit4 ^ lit5 ^ lit6 ^ lit7 ^ lit8);
  do_test(expr, PrimType::Xnor);
}

TEST_F(FuncAnalyzerTest, xnor10)
{
  auto lit0 = Expr::make_posi_literal(0);
  auto lit1 = Expr::make_posi_literal(1);
  auto lit2 = Expr::make_posi_literal(2);
  auto lit3 = Expr::make_posi_literal(3);
  auto lit4 = Expr::make_posi_literal(4);
  auto lit5 = Expr::make_posi_literal(5);
  auto lit6 = Expr::make_posi_literal(6);
  auto lit7 = Expr::make_posi_literal(7);
  auto lit8 = Expr::make_posi_literal(8);
  auto lit9 = Expr::make_posi_literal(9);
  auto expr = ~(lit0 ^ lit1 ^ lit2 ^ lit3 ^ lit4 ^ lit5 ^ lit6 ^ lit7 ^ lit8 ^ lit9);
  do_test(expr, PrimType::Xnor);
}

TEST_F(FuncAnalyzerTest, xnor11)
{
  auto lit0 = Expr::make_posi_literal(0);
  auto lit1 = Expr::make_posi_literal(1);
  auto lit2 = Expr::make_posi_literal(2);
  auto lit3 = Expr::make_posi_literal(3);
  auto lit4 = Expr::make_posi_literal(4);
  auto lit5 = Expr::make_posi_literal(5);
  auto lit6 = Expr::make_posi_literal(6);
  auto lit7 = Expr::make_posi_literal(7);
  auto lit8 = Expr::make_posi_literal(8);
  auto lit9 = Expr::make_posi_literal(9);
  auto lit10 = Expr::make_posi_literal(10);
  auto expr = ~(lit0 ^ lit1 ^ lit2 ^ lit3 ^ lit4 ^ lit5 ^ lit6 ^ lit7 ^ lit8 ^ lit9 ^ lit10);
  do_test(expr, PrimType::Xnor);
}

TEST_F(FuncAnalyzerTest, expr1)
{
  auto lit0 = Expr::make_posi_literal(0);
  auto lit1 = Expr::make_posi_literal(1);
  auto lit2 = Expr::make_posi_literal(2);
  auto expr = (~lit0 & ~lit1 & ~lit2) | (lit0 & lit1 & lit2);
  do_test(expr, PrimType::None);
}

TEST_F(FuncAnalyzerTest, expr2)
{
  auto lit0 = Expr::make_posi_literal(0);
  auto lit1 = Expr::make_posi_literal(1);
  auto lit2 = Expr::make_posi_literal(2);
  auto lit3 = Expr::make_posi_literal(3);
  auto expr = (lit0 & lit1) | (lit2 & lit3);
  do_test(expr, PrimType::None);
}

END_NAMESPACE_YM_LOGIC
