
/// @file TvFuncTest_analyze.cc
/// @brief TvFunc::analyze() のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "gtest/gtest.h"
#include "ym/TvFunc.h"


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
    const TvFunc& func,   ///< [in] 論理関数
    PrimType node_type ///< [in] 期待値
  );


protected:

};

void
FuncAnalyzerTest::do_test(
  const TvFunc& func,
  PrimType node_type
)
{
  auto act_node_type = func.analyze();
  EXPECT_EQ( act_node_type, node_type );
}

TEST_F(FuncAnalyzerTest, const0)
{
  auto func = TvFunc::zero(10);
  do_test(func, PrimType::C0);
}

TEST_F(FuncAnalyzerTest, const1)
{
  auto func = TvFunc::one(10);
  do_test(func, PrimType::C1);
}

TEST_F(FuncAnalyzerTest, plit)
{
  auto func = TvFunc::posi_literal(10, 0);
  do_test(func, PrimType::Buff);
}

TEST_F(FuncAnalyzerTest, nlit)
{
  auto func = TvFunc::nega_literal(10, 0);
  do_test(func, PrimType::Not);
}

TEST_F(FuncAnalyzerTest, and2)
{
  auto lit0 = TvFunc::posi_literal(10, 0);
  auto lit1 = TvFunc::posi_literal(10, 1);
  auto func = lit0 & lit1;
  do_test(func, PrimType::And);
}

TEST_F(FuncAnalyzerTest, and3)
{
  auto lit0 = TvFunc::posi_literal(10, 0);
  auto lit1 = TvFunc::posi_literal(10, 1);
  auto lit2 = TvFunc::posi_literal(10, 2);
  auto func = lit0 & lit1 & lit2;
  do_test(func, PrimType::And);
}

TEST_F(FuncAnalyzerTest, and4)
{
  auto lit0 = TvFunc::posi_literal(10, 0);
  auto lit1 = TvFunc::posi_literal(10, 1);
  auto lit2 = TvFunc::posi_literal(10, 2);
  auto lit3 = TvFunc::posi_literal(10, 3);
  auto func = lit0 & lit1 & lit2 & lit3;
  do_test(func, PrimType::And);
}

TEST_F(FuncAnalyzerTest, and5)
{
  auto lit0 = TvFunc::posi_literal(10, 0);
  auto lit1 = TvFunc::posi_literal(10, 1);
  auto lit2 = TvFunc::posi_literal(10, 2);
  auto lit3 = TvFunc::posi_literal(10, 3);
  auto lit4 = TvFunc::posi_literal(10, 4);
  auto func = lit0 & lit1 & lit2 & lit3 & lit4;
  do_test(func, PrimType::And);
}

TEST_F(FuncAnalyzerTest, and6)
{
  auto lit0 = TvFunc::posi_literal(10, 0);
  auto lit1 = TvFunc::posi_literal(10, 1);
  auto lit2 = TvFunc::posi_literal(10, 2);
  auto lit3 = TvFunc::posi_literal(10, 3);
  auto lit4 = TvFunc::posi_literal(10, 4);
  auto lit5 = TvFunc::posi_literal(10, 5);
  auto func = lit0 & lit1 & lit2 & lit3 & lit4 & lit5;
  do_test(func, PrimType::And);
}

TEST_F(FuncAnalyzerTest, and7)
{
  auto lit0 = TvFunc::posi_literal(10, 0);
  auto lit1 = TvFunc::posi_literal(10, 1);
  auto lit2 = TvFunc::posi_literal(10, 2);
  auto lit3 = TvFunc::posi_literal(10, 3);
  auto lit4 = TvFunc::posi_literal(10, 4);
  auto lit5 = TvFunc::posi_literal(10, 5);
  auto lit6 = TvFunc::posi_literal(10, 6);
  auto func = lit0 & lit1 & lit2 & lit3 & lit4 & lit5 & lit6;
  do_test(func, PrimType::And);
}

TEST_F(FuncAnalyzerTest, and8)
{
  auto lit0 = TvFunc::posi_literal(10, 0);
  auto lit1 = TvFunc::posi_literal(10, 1);
  auto lit2 = TvFunc::posi_literal(10, 2);
  auto lit3 = TvFunc::posi_literal(10, 3);
  auto lit4 = TvFunc::posi_literal(10, 4);
  auto lit5 = TvFunc::posi_literal(10, 5);
  auto lit6 = TvFunc::posi_literal(10, 6);
  auto lit7 = TvFunc::posi_literal(10, 7);
  auto func = lit0 & lit1 & lit2 & lit3 & lit4 & lit5 & lit6 & lit7;
  do_test(func, PrimType::And);
}

TEST_F(FuncAnalyzerTest, and9)
{
  auto lit0 = TvFunc::posi_literal(10, 0);
  auto lit1 = TvFunc::posi_literal(10, 1);
  auto lit2 = TvFunc::posi_literal(10, 2);
  auto lit3 = TvFunc::posi_literal(10, 3);
  auto lit4 = TvFunc::posi_literal(10, 4);
  auto lit5 = TvFunc::posi_literal(10, 5);
  auto lit6 = TvFunc::posi_literal(10, 6);
  auto lit7 = TvFunc::posi_literal(10, 7);
  auto lit8 = TvFunc::posi_literal(10, 8);
  auto func = lit0 & lit1 & lit2 & lit3 & lit4 & lit5 & lit6 & lit7 & lit8;
  do_test(func, PrimType::And);
}

TEST_F(FuncAnalyzerTest, and10)
{
  auto lit0 = TvFunc::posi_literal(10, 0);
  auto lit1 = TvFunc::posi_literal(10, 1);
  auto lit2 = TvFunc::posi_literal(10, 2);
  auto lit3 = TvFunc::posi_literal(10, 3);
  auto lit4 = TvFunc::posi_literal(10, 4);
  auto lit5 = TvFunc::posi_literal(10, 5);
  auto lit6 = TvFunc::posi_literal(10, 6);
  auto lit7 = TvFunc::posi_literal(10, 7);
  auto lit8 = TvFunc::posi_literal(10, 8);
  auto lit9 = TvFunc::posi_literal(10, 9);
  auto func = lit0 & lit1 & lit2 & lit3 & lit4 & lit5 & lit6 & lit7 & lit8 & lit9;
  do_test(func, PrimType::And);
}

TEST_F(FuncAnalyzerTest, nand2)
{
  auto lit0 = TvFunc::posi_literal(10, 0);
  auto lit1 = TvFunc::posi_literal(10, 1);
  auto func = ~(lit0 & lit1);
  do_test(func, PrimType::Nand);
}

TEST_F(FuncAnalyzerTest, nand3)
{
  auto lit0 = TvFunc::posi_literal(10, 0);
  auto lit1 = TvFunc::posi_literal(10, 1);
  auto lit2 = TvFunc::posi_literal(10, 2);
  auto func = ~(lit0 & lit1 & lit2);
  do_test(func, PrimType::Nand);
}

TEST_F(FuncAnalyzerTest, nand4)
{
  auto lit0 = TvFunc::posi_literal(10, 0);
  auto lit1 = TvFunc::posi_literal(10, 1);
  auto lit2 = TvFunc::posi_literal(10, 2);
  auto lit3 = TvFunc::posi_literal(10, 3);
  auto func = ~(lit0 & lit1 & lit2 & lit3);
  do_test(func, PrimType::Nand);
}

TEST_F(FuncAnalyzerTest, nand5)
{
  auto lit0 = TvFunc::posi_literal(10, 0);
  auto lit1 = TvFunc::posi_literal(10, 1);
  auto lit2 = TvFunc::posi_literal(10, 2);
  auto lit3 = TvFunc::posi_literal(10, 3);
  auto lit4 = TvFunc::posi_literal(10, 4);
  auto func = ~(lit0 & lit1 & lit2 & lit3 & lit4);
  do_test(func, PrimType::Nand);
}

TEST_F(FuncAnalyzerTest, nand6)
{
  auto lit0 = TvFunc::posi_literal(10, 0);
  auto lit1 = TvFunc::posi_literal(10, 1);
  auto lit2 = TvFunc::posi_literal(10, 2);
  auto lit3 = TvFunc::posi_literal(10, 3);
  auto lit4 = TvFunc::posi_literal(10, 4);
  auto lit5 = TvFunc::posi_literal(10, 5);
  auto func = ~(lit0 & lit1 & lit2 & lit3 & lit4 & lit5);
  do_test(func, PrimType::Nand);
}

TEST_F(FuncAnalyzerTest, nand7)
{
  auto lit0 = TvFunc::posi_literal(10, 0);
  auto lit1 = TvFunc::posi_literal(10, 1);
  auto lit2 = TvFunc::posi_literal(10, 2);
  auto lit3 = TvFunc::posi_literal(10, 3);
  auto lit4 = TvFunc::posi_literal(10, 4);
  auto lit5 = TvFunc::posi_literal(10, 5);
  auto lit6 = TvFunc::posi_literal(10, 6);
  auto func = ~(lit0 & lit1 & lit2 & lit3 & lit4 & lit5 & lit6);
  do_test(func, PrimType::Nand);
}

TEST_F(FuncAnalyzerTest, nand8)
{
  auto lit0 = TvFunc::posi_literal(10, 0);
  auto lit1 = TvFunc::posi_literal(10, 1);
  auto lit2 = TvFunc::posi_literal(10, 2);
  auto lit3 = TvFunc::posi_literal(10, 3);
  auto lit4 = TvFunc::posi_literal(10, 4);
  auto lit5 = TvFunc::posi_literal(10, 5);
  auto lit6 = TvFunc::posi_literal(10, 6);
  auto lit7 = TvFunc::posi_literal(10, 7);
  auto func = ~(lit0 & lit1 & lit2 & lit3 & lit4 & lit5 & lit6 & lit7);
  do_test(func, PrimType::Nand);
}

TEST_F(FuncAnalyzerTest, nand9)
{
  auto lit0 = TvFunc::posi_literal(10, 0);
  auto lit1 = TvFunc::posi_literal(10, 1);
  auto lit2 = TvFunc::posi_literal(10, 2);
  auto lit3 = TvFunc::posi_literal(10, 3);
  auto lit4 = TvFunc::posi_literal(10, 4);
  auto lit5 = TvFunc::posi_literal(10, 5);
  auto lit6 = TvFunc::posi_literal(10, 6);
  auto lit7 = TvFunc::posi_literal(10, 7);
  auto lit8 = TvFunc::posi_literal(10, 8);
  auto func = ~(lit0 & lit1 & lit2 & lit3 & lit4 & lit5 & lit6 & lit7 & lit8);
  do_test(func, PrimType::Nand);
}

TEST_F(FuncAnalyzerTest, nand10)
{
  auto lit0 = TvFunc::posi_literal(10, 0);
  auto lit1 = TvFunc::posi_literal(10, 1);
  auto lit2 = TvFunc::posi_literal(10, 2);
  auto lit3 = TvFunc::posi_literal(10, 3);
  auto lit4 = TvFunc::posi_literal(10, 4);
  auto lit5 = TvFunc::posi_literal(10, 5);
  auto lit6 = TvFunc::posi_literal(10, 6);
  auto lit7 = TvFunc::posi_literal(10, 7);
  auto lit8 = TvFunc::posi_literal(10, 8);
  auto lit9 = TvFunc::posi_literal(10, 9);
  auto func = ~(lit0 & lit1 & lit2 & lit3 & lit4 & lit5 & lit6 & lit7 & lit8 & lit9);
  do_test(func, PrimType::Nand);
}

TEST_F(FuncAnalyzerTest, or2)
{
  auto lit0 = TvFunc::posi_literal(10, 0);
  auto lit1 = TvFunc::posi_literal(10, 1);
  auto func = lit0 | lit1;
  do_test(func, PrimType::Or);
}

TEST_F(FuncAnalyzerTest, or3)
{
  auto lit0 = TvFunc::posi_literal(10, 0);
  auto lit1 = TvFunc::posi_literal(10, 1);
  auto lit2 = TvFunc::posi_literal(10, 2);
  auto func = lit0 | lit1 | lit2;
  do_test(func, PrimType::Or);
}

TEST_F(FuncAnalyzerTest, or4)
{
  auto lit0 = TvFunc::posi_literal(10, 0);
  auto lit1 = TvFunc::posi_literal(10, 1);
  auto lit2 = TvFunc::posi_literal(10, 2);
  auto lit3 = TvFunc::posi_literal(10, 3);
  auto func = lit0 | lit1 | lit2 | lit3;
  do_test(func, PrimType::Or);
}

TEST_F(FuncAnalyzerTest, or5)
{
  auto lit0 = TvFunc::posi_literal(10, 0);
  auto lit1 = TvFunc::posi_literal(10, 1);
  auto lit2 = TvFunc::posi_literal(10, 2);
  auto lit3 = TvFunc::posi_literal(10, 3);
  auto lit4 = TvFunc::posi_literal(10, 4);
  auto func = lit0 | lit1 | lit2 | lit3 | lit4;
  do_test(func, PrimType::Or);
}

TEST_F(FuncAnalyzerTest, or6)
{
  auto lit0 = TvFunc::posi_literal(10, 0);
  auto lit1 = TvFunc::posi_literal(10, 1);
  auto lit2 = TvFunc::posi_literal(10, 2);
  auto lit3 = TvFunc::posi_literal(10, 3);
  auto lit4 = TvFunc::posi_literal(10, 4);
  auto lit5 = TvFunc::posi_literal(10, 5);
  auto func = lit0 | lit1 | lit2 | lit3 | lit4 | lit5;
  do_test(func, PrimType::Or);
}

TEST_F(FuncAnalyzerTest, or7)
{
  auto lit0 = TvFunc::posi_literal(10, 0);
  auto lit1 = TvFunc::posi_literal(10, 1);
  auto lit2 = TvFunc::posi_literal(10, 2);
  auto lit3 = TvFunc::posi_literal(10, 3);
  auto lit4 = TvFunc::posi_literal(10, 4);
  auto lit5 = TvFunc::posi_literal(10, 5);
  auto lit6 = TvFunc::posi_literal(10, 6);
  auto func = lit0 | lit1 | lit2 | lit3 | lit4 | lit5 | lit6;
  do_test(func, PrimType::Or);
}

TEST_F(FuncAnalyzerTest, or8)
{
  auto lit0 = TvFunc::posi_literal(10, 0);
  auto lit1 = TvFunc::posi_literal(10, 1);
  auto lit2 = TvFunc::posi_literal(10, 2);
  auto lit3 = TvFunc::posi_literal(10, 3);
  auto lit4 = TvFunc::posi_literal(10, 4);
  auto lit5 = TvFunc::posi_literal(10, 5);
  auto lit6 = TvFunc::posi_literal(10, 6);
  auto lit7 = TvFunc::posi_literal(10, 7);
  auto func = lit0 | lit1 | lit2 | lit3 | lit4 | lit5 | lit6 | lit7;
  do_test(func, PrimType::Or);
}

TEST_F(FuncAnalyzerTest, or9)
{
  auto lit0 = TvFunc::posi_literal(10, 0);
  auto lit1 = TvFunc::posi_literal(10, 1);
  auto lit2 = TvFunc::posi_literal(10, 2);
  auto lit3 = TvFunc::posi_literal(10, 3);
  auto lit4 = TvFunc::posi_literal(10, 4);
  auto lit5 = TvFunc::posi_literal(10, 5);
  auto lit6 = TvFunc::posi_literal(10, 6);
  auto lit7 = TvFunc::posi_literal(10, 7);
  auto lit8 = TvFunc::posi_literal(10, 8);
  auto func = lit0 | lit1 | lit2 | lit3 | lit4 | lit5 | lit6 | lit7 | lit8;
  do_test(func, PrimType::Or);
}

TEST_F(FuncAnalyzerTest, or10)
{
  auto lit0 = TvFunc::posi_literal(10, 0);
  auto lit1 = TvFunc::posi_literal(10, 1);
  auto lit2 = TvFunc::posi_literal(10, 2);
  auto lit3 = TvFunc::posi_literal(10, 3);
  auto lit4 = TvFunc::posi_literal(10, 4);
  auto lit5 = TvFunc::posi_literal(10, 5);
  auto lit6 = TvFunc::posi_literal(10, 6);
  auto lit7 = TvFunc::posi_literal(10, 7);
  auto lit8 = TvFunc::posi_literal(10, 8);
  auto lit9 = TvFunc::posi_literal(10, 9);
  auto func = lit0 | lit1 | lit2 | lit3 | lit4 | lit5 | lit6 | lit7 | lit8 | lit9;
  do_test(func, PrimType::Or);
}

TEST_F(FuncAnalyzerTest, nor2)
{
  auto lit0 = TvFunc::posi_literal(10, 0);
  auto lit1 = TvFunc::posi_literal(10, 1);
  auto func = ~(lit0 | lit1);
  do_test(func, PrimType::Nor);
}

TEST_F(FuncAnalyzerTest, nor3)
{
  auto lit0 = TvFunc::posi_literal(10, 0);
  auto lit1 = TvFunc::posi_literal(10, 1);
  auto lit2 = TvFunc::posi_literal(10, 2);
  auto func = ~(lit0 | lit1 | lit2);
  do_test(func, PrimType::Nor);
}

TEST_F(FuncAnalyzerTest, nor4)
{
  auto lit0 = TvFunc::posi_literal(10, 0);
  auto lit1 = TvFunc::posi_literal(10, 1);
  auto lit2 = TvFunc::posi_literal(10, 2);
  auto lit3 = TvFunc::posi_literal(10, 3);
  auto func = ~(lit0 | lit1 | lit2 | lit3);
  do_test(func, PrimType::Nor);
}

TEST_F(FuncAnalyzerTest, nor5)
{
  auto lit0 = TvFunc::posi_literal(10, 0);
  auto lit1 = TvFunc::posi_literal(10, 1);
  auto lit2 = TvFunc::posi_literal(10, 2);
  auto lit3 = TvFunc::posi_literal(10, 3);
  auto lit4 = TvFunc::posi_literal(10, 4);
  auto func = ~(lit0 | lit1 | lit2 | lit3 | lit4);
  do_test(func, PrimType::Nor);
}

TEST_F(FuncAnalyzerTest, nor6)
{
  auto lit0 = TvFunc::posi_literal(10, 0);
  auto lit1 = TvFunc::posi_literal(10, 1);
  auto lit2 = TvFunc::posi_literal(10, 2);
  auto lit3 = TvFunc::posi_literal(10, 3);
  auto lit4 = TvFunc::posi_literal(10, 4);
  auto lit5 = TvFunc::posi_literal(10, 5);
  auto func = ~(lit0 | lit1 | lit2 | lit3 | lit4 | lit5);
  do_test(func, PrimType::Nor);
}

TEST_F(FuncAnalyzerTest, nor7)
{
  auto lit0 = TvFunc::posi_literal(10, 0);
  auto lit1 = TvFunc::posi_literal(10, 1);
  auto lit2 = TvFunc::posi_literal(10, 2);
  auto lit3 = TvFunc::posi_literal(10, 3);
  auto lit4 = TvFunc::posi_literal(10, 4);
  auto lit5 = TvFunc::posi_literal(10, 5);
  auto lit6 = TvFunc::posi_literal(10, 6);
  auto func = ~(lit0 | lit1 | lit2 | lit3 | lit4 | lit5 | lit6);
  do_test(func, PrimType::Nor);
}

TEST_F(FuncAnalyzerTest, nor8)
{
  auto lit0 = TvFunc::posi_literal(10, 0);
  auto lit1 = TvFunc::posi_literal(10, 1);
  auto lit2 = TvFunc::posi_literal(10, 2);
  auto lit3 = TvFunc::posi_literal(10, 3);
  auto lit4 = TvFunc::posi_literal(10, 4);
  auto lit5 = TvFunc::posi_literal(10, 5);
  auto lit6 = TvFunc::posi_literal(10, 6);
  auto lit7 = TvFunc::posi_literal(10, 7);
  auto func = ~(lit0 | lit1 | lit2 | lit3 | lit4 | lit5 | lit6 | lit7);
  do_test(func, PrimType::Nor);
}

TEST_F(FuncAnalyzerTest, nor9)
{
  auto lit0 = TvFunc::posi_literal(10, 0);
  auto lit1 = TvFunc::posi_literal(10, 1);
  auto lit2 = TvFunc::posi_literal(10, 2);
  auto lit3 = TvFunc::posi_literal(10, 3);
  auto lit4 = TvFunc::posi_literal(10, 4);
  auto lit5 = TvFunc::posi_literal(10, 5);
  auto lit6 = TvFunc::posi_literal(10, 6);
  auto lit7 = TvFunc::posi_literal(10, 7);
  auto lit8 = TvFunc::posi_literal(10, 8);
  auto func = ~(lit0 | lit1 | lit2 | lit3 | lit4 | lit5 | lit6 | lit7 | lit8);
  do_test(func, PrimType::Nor);
}

TEST_F(FuncAnalyzerTest, nor10)
{
  auto lit0 = TvFunc::posi_literal(10, 0);
  auto lit1 = TvFunc::posi_literal(10, 1);
  auto lit2 = TvFunc::posi_literal(10, 2);
  auto lit3 = TvFunc::posi_literal(10, 3);
  auto lit4 = TvFunc::posi_literal(10, 4);
  auto lit5 = TvFunc::posi_literal(10, 5);
  auto lit6 = TvFunc::posi_literal(10, 6);
  auto lit7 = TvFunc::posi_literal(10, 7);
  auto lit8 = TvFunc::posi_literal(10, 8);
  auto lit9 = TvFunc::posi_literal(10, 9);
  auto func = ~(lit0 | lit1 | lit2 | lit3 | lit4 | lit5 | lit6 | lit7 | lit8 | lit9);
  do_test(func, PrimType::Nor);
}

TEST_F(FuncAnalyzerTest, xor2)
{
  auto lit0 = TvFunc::posi_literal(10, 0);
  auto lit1 = TvFunc::posi_literal(10, 1);
  auto func = lit0 ^ lit1;
  do_test(func, PrimType::Xor);
}

TEST_F(FuncAnalyzerTest, xor3)
{
  auto lit0 = TvFunc::posi_literal(10, 0);
  auto lit1 = TvFunc::posi_literal(10, 1);
  auto lit2 = TvFunc::posi_literal(10, 2);
  auto func = lit0 ^ lit1 ^ lit2;
  do_test(func, PrimType::Xor);
}

TEST_F(FuncAnalyzerTest, xor4)
{
  auto lit0 = TvFunc::posi_literal(10, 0);
  auto lit1 = TvFunc::posi_literal(10, 1);
  auto lit2 = TvFunc::posi_literal(10, 2);
  auto lit3 = TvFunc::posi_literal(10, 3);
  auto func = lit0 ^ lit1 ^ lit2 ^ lit3;
  do_test(func, PrimType::Xor);
}

TEST_F(FuncAnalyzerTest, xor5)
{
  auto lit0 = TvFunc::posi_literal(10, 0);
  auto lit1 = TvFunc::posi_literal(10, 1);
  auto lit2 = TvFunc::posi_literal(10, 2);
  auto lit3 = TvFunc::posi_literal(10, 3);
  auto lit4 = TvFunc::posi_literal(10, 4);
  auto func = lit0 ^ lit1 ^ lit2 ^ lit3 ^ lit4;
  do_test(func, PrimType::Xor);
}

TEST_F(FuncAnalyzerTest, xor6)
{
  auto lit0 = TvFunc::posi_literal(10, 0);
  auto lit1 = TvFunc::posi_literal(10, 1);
  auto lit2 = TvFunc::posi_literal(10, 2);
  auto lit3 = TvFunc::posi_literal(10, 3);
  auto lit4 = TvFunc::posi_literal(10, 4);
  auto lit5 = TvFunc::posi_literal(10, 5);
  auto func = lit0 ^ lit1 ^ lit2 ^ lit3 ^ lit4 ^ lit5;
  do_test(func, PrimType::Xor);
}

TEST_F(FuncAnalyzerTest, xor7)
{
  auto lit0 = TvFunc::posi_literal(10, 0);
  auto lit1 = TvFunc::posi_literal(10, 1);
  auto lit2 = TvFunc::posi_literal(10, 2);
  auto lit3 = TvFunc::posi_literal(10, 3);
  auto lit4 = TvFunc::posi_literal(10, 4);
  auto lit5 = TvFunc::posi_literal(10, 5);
  auto lit6 = TvFunc::posi_literal(10, 6);
  auto func = lit0 ^ lit1 ^ lit2 ^ lit3 ^ lit4 ^ lit5 ^ lit6;
  do_test(func, PrimType::Xor);
}

TEST_F(FuncAnalyzerTest, xor8)
{
  auto lit0 = TvFunc::posi_literal(10, 0);
  auto lit1 = TvFunc::posi_literal(10, 1);
  auto lit2 = TvFunc::posi_literal(10, 2);
  auto lit3 = TvFunc::posi_literal(10, 3);
  auto lit4 = TvFunc::posi_literal(10, 4);
  auto lit5 = TvFunc::posi_literal(10, 5);
  auto lit6 = TvFunc::posi_literal(10, 6);
  auto lit7 = TvFunc::posi_literal(10, 7);
  auto func = lit0 ^ lit1 ^ lit2 ^ lit3 ^ lit4 ^ lit5 ^ lit6 ^ lit7;
  do_test(func, PrimType::Xor);
}

TEST_F(FuncAnalyzerTest, xor9)
{
  auto lit0 = TvFunc::posi_literal(10, 0);
  auto lit1 = TvFunc::posi_literal(10, 1);
  auto lit2 = TvFunc::posi_literal(10, 2);
  auto lit3 = TvFunc::posi_literal(10, 3);
  auto lit4 = TvFunc::posi_literal(10, 4);
  auto lit5 = TvFunc::posi_literal(10, 5);
  auto lit6 = TvFunc::posi_literal(10, 6);
  auto lit7 = TvFunc::posi_literal(10, 7);
  auto lit8 = TvFunc::posi_literal(10, 8);
  auto func = lit0 ^ lit1 ^ lit2 ^ lit3 ^ lit4 ^ lit5 ^ lit6 ^ lit7 ^ lit8;
  do_test(func, PrimType::Xor);
}

TEST_F(FuncAnalyzerTest, xor10)
{
  auto lit0 = TvFunc::posi_literal(10, 0);
  auto lit1 = TvFunc::posi_literal(10, 1);
  auto lit2 = TvFunc::posi_literal(10, 2);
  auto lit3 = TvFunc::posi_literal(10, 3);
  auto lit4 = TvFunc::posi_literal(10, 4);
  auto lit5 = TvFunc::posi_literal(10, 5);
  auto lit6 = TvFunc::posi_literal(10, 6);
  auto lit7 = TvFunc::posi_literal(10, 7);
  auto lit8 = TvFunc::posi_literal(10, 8);
  auto lit9 = TvFunc::posi_literal(10, 9);
  auto func = lit0 ^ lit1 ^ lit2 ^ lit3 ^ lit4 ^ lit5 ^ lit6 ^ lit7 ^ lit8 ^ lit9;
  do_test(func, PrimType::Xor);
}

TEST_F(FuncAnalyzerTest, xnor2)
{
  auto lit0 = TvFunc::posi_literal(10, 0);
  auto lit1 = TvFunc::posi_literal(10, 1);
  auto func = ~(lit0 ^ lit1);
  do_test(func, PrimType::Xnor);
}

TEST_F(FuncAnalyzerTest, xnor3)
{
  auto lit0 = TvFunc::posi_literal(10, 0);
  auto lit1 = TvFunc::posi_literal(10, 1);
  auto lit2 = TvFunc::posi_literal(10, 2);
  auto func = ~(lit0 ^ lit1 ^ lit2);
  do_test(func, PrimType::Xnor);
}

TEST_F(FuncAnalyzerTest, xnor4)
{
  auto lit0 = TvFunc::posi_literal(10, 0);
  auto lit1 = TvFunc::posi_literal(10, 1);
  auto lit2 = TvFunc::posi_literal(10, 2);
  auto lit3 = TvFunc::posi_literal(10, 3);
  auto func = ~(lit0 ^ lit1 ^ lit2 ^ lit3);
  do_test(func, PrimType::Xnor);
}

TEST_F(FuncAnalyzerTest, xnor5)
{
  auto lit0 = TvFunc::posi_literal(10, 0);
  auto lit1 = TvFunc::posi_literal(10, 1);
  auto lit2 = TvFunc::posi_literal(10, 2);
  auto lit3 = TvFunc::posi_literal(10, 3);
  auto lit4 = TvFunc::posi_literal(10, 4);
  auto func = ~(lit0 ^ lit1 ^ lit2 ^ lit3 ^ lit4);
  do_test(func, PrimType::Xnor);
}

TEST_F(FuncAnalyzerTest, xnor6)
{
  auto lit0 = TvFunc::posi_literal(10, 0);
  auto lit1 = TvFunc::posi_literal(10, 1);
  auto lit2 = TvFunc::posi_literal(10, 2);
  auto lit3 = TvFunc::posi_literal(10, 3);
  auto lit4 = TvFunc::posi_literal(10, 4);
  auto lit5 = TvFunc::posi_literal(10, 5);
  auto func = ~(lit0 ^ lit1 ^ lit2 ^ lit3 ^ lit4 ^ lit5);
  do_test(func, PrimType::Xnor);
}

TEST_F(FuncAnalyzerTest, xnor7)
{
  auto lit0 = TvFunc::posi_literal(10, 0);
  auto lit1 = TvFunc::posi_literal(10, 1);
  auto lit2 = TvFunc::posi_literal(10, 2);
  auto lit3 = TvFunc::posi_literal(10, 3);
  auto lit4 = TvFunc::posi_literal(10, 4);
  auto lit5 = TvFunc::posi_literal(10, 5);
  auto lit6 = TvFunc::posi_literal(10, 6);
  auto func = ~(lit0 ^ lit1 ^ lit2 ^ lit3 ^ lit4 ^ lit5 ^ lit6);
  do_test(func, PrimType::Xnor);
}

TEST_F(FuncAnalyzerTest, xnor8)
{
  auto lit0 = TvFunc::posi_literal(10, 0);
  auto lit1 = TvFunc::posi_literal(10, 1);
  auto lit2 = TvFunc::posi_literal(10, 2);
  auto lit3 = TvFunc::posi_literal(10, 3);
  auto lit4 = TvFunc::posi_literal(10, 4);
  auto lit5 = TvFunc::posi_literal(10, 5);
  auto lit6 = TvFunc::posi_literal(10, 6);
  auto lit7 = TvFunc::posi_literal(10, 7);
  auto func = ~(lit0 ^ lit1 ^ lit2 ^ lit3 ^ lit4 ^ lit5 ^ lit6 ^ lit7);
  do_test(func, PrimType::Xnor);
}

TEST_F(FuncAnalyzerTest, xnor9)
{
  auto lit0 = TvFunc::posi_literal(10, 0);
  auto lit1 = TvFunc::posi_literal(10, 1);
  auto lit2 = TvFunc::posi_literal(10, 2);
  auto lit3 = TvFunc::posi_literal(10, 3);
  auto lit4 = TvFunc::posi_literal(10, 4);
  auto lit5 = TvFunc::posi_literal(10, 5);
  auto lit6 = TvFunc::posi_literal(10, 6);
  auto lit7 = TvFunc::posi_literal(10, 7);
  auto lit8 = TvFunc::posi_literal(10, 8);
  auto func = ~(lit0 ^ lit1 ^ lit2 ^ lit3 ^ lit4 ^ lit5 ^ lit6 ^ lit7 ^ lit8);
  do_test(func, PrimType::Xnor);
}

TEST_F(FuncAnalyzerTest, xnor10)
{
  auto lit0 = TvFunc::posi_literal(10, 0);
  auto lit1 = TvFunc::posi_literal(10, 1);
  auto lit2 = TvFunc::posi_literal(10, 2);
  auto lit3 = TvFunc::posi_literal(10, 3);
  auto lit4 = TvFunc::posi_literal(10, 4);
  auto lit5 = TvFunc::posi_literal(10, 5);
  auto lit6 = TvFunc::posi_literal(10, 6);
  auto lit7 = TvFunc::posi_literal(10, 7);
  auto lit8 = TvFunc::posi_literal(10, 8);
  auto lit9 = TvFunc::posi_literal(10, 9);
  auto func = ~(lit0 ^ lit1 ^ lit2 ^ lit3 ^ lit4 ^ lit5 ^ lit6 ^ lit7 ^ lit8 ^ lit9);
  do_test(func, PrimType::Xnor);
}

TEST_F(FuncAnalyzerTest, func1)
{
  auto lit0 = TvFunc::posi_literal(10, 0);
  auto lit1 = TvFunc::posi_literal(10, 1);
  auto lit2 = TvFunc::posi_literal(10, 2);
  auto func = (~lit0 & ~lit1 & ~lit2) | (lit0 & lit1 & lit2);
  do_test(func, PrimType::None);
}

TEST_F(FuncAnalyzerTest, func2)
{
  auto lit0 = TvFunc::posi_literal(10, 0);
  auto lit1 = TvFunc::posi_literal(10, 1);
  auto lit2 = TvFunc::posi_literal(10, 2);
  auto lit3 = TvFunc::posi_literal(10, 3);
  auto func = (lit0 & lit1) | (lit2 & lit3);
  do_test(func, PrimType::None);
}

END_NAMESPACE_YM_LOGIC
