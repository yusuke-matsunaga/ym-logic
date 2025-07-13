
/// @file Tv2BddTest.cc
/// @brief Tv2Bdd のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/Tv2Bdd.h"


BEGIN_NAMESPACE_YM

class Tv2BddTest :
  public ::testing::Test
{
public:

  void
  check_func(
    const TvFunc& func
  );


public:

  BddMgr mMgr;

};

void
Tv2BddTest::check_func(
  const TvFunc& func
)
{
  auto bdd = Tv2Bdd::gen_bdd(func, mMgr);

  SizeType ni = func.input_num();
  SizeType ni_exp = 1UL << ni;
  for ( SizeType b = 0; b < ni_exp; ++ b ) {
    auto v1 = func.value(b);
    std::vector<bool> inputs(ni);
    for ( SizeType i = 0; i < ni; ++ i ) {
      if ( b & (1UL << i) ) {
	inputs[i] = true;
      }
      else {
	inputs[i] = false;
      }
    }
    auto v2 = bdd.eval(inputs);
    EXPECT_EQ( v1, v2 );
  }
}


TEST_F(Tv2BddTest, const0)
{
  auto func = TvFunc::zero(10);

  check_func(func);
}

TEST_F(Tv2BddTest, const1)
{
  auto func = TvFunc::one(10);

  check_func(func);
}

TEST_F(Tv2BddTest, literal1)
{
  auto func = TvFunc::literal(10, 5, false);

  check_func(func);
}

TEST_F(Tv2BddTest, literal2)
{
  auto func = TvFunc::literal(10, 3, true);

  check_func(func);
}

TEST_F(Tv2BddTest, func1)
{
  std::vector<int> val_list{0, 1, 1, 0, 1, 0, 0, 0};
  TvFunc func{3, val_list};

  check_func(func);
}

END_NAMESPACE_YM
