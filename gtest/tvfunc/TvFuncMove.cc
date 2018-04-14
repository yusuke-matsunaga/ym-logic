
/// @file TvFuncMove.cc
/// @brief TvFuncMove の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.


#include "gtest/gtest.h"
#include "ym/TvFunc.h"


BEGIN_NAMESPACE_YM

TEST(TvFuncMove, copy)
{
  auto f = TvFunc::const_zero(20);
  auto g = TvFunc::const_one(20);

  for ( auto i = 0; i < 10000; ++ i ) {
    auto tmp = f;
    f = g;
    g = tmp;
  }

  EXPECT_EQ( TvFunc::const_zero(20), f );
  EXPECT_EQ( TvFunc::const_one(20), g );
}

TEST(TvFuncMove, move)
{
  auto f = TvFunc::const_zero(20);
  auto g = TvFunc::const_one(20);

  for ( auto i = 0; i < 10000; ++ i ) {
    auto tmp = std::move(f);
    f = std::move(g);
    g = std::move(tmp);
  }

  EXPECT_EQ( TvFunc::const_zero(20), f );
  EXPECT_EQ( TvFunc::const_one(20), g );
}

END_NAMESPACE_YM
