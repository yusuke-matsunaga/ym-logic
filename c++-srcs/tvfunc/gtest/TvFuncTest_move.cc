
/// @file TvFuncTest_move.cc
/// @brief TvFunc のムーブコンストラクタ系のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "gtest/gtest.h"
#include "ym/TvFunc.h"
#include "ym/TvFuncM.h"


BEGIN_NAMESPACE_YM

TEST(TvFuncMove, copy)
{
  auto f = TvFunc::zero(20);
  auto g = TvFunc::one(20);

  for ( auto i = 0; i < 10000; ++ i ) {
    auto tmp = f;
    f = g;
    g = tmp;
  }

  EXPECT_EQ( TvFunc::zero(20), f );
  EXPECT_EQ( TvFunc::one(20), g );
}

TEST(TvFuncMove, move)
{
  auto f = TvFunc::zero(20);
  auto g = TvFunc::one(20);

  for ( auto i = 0; i < 10000; ++ i ) {
    auto tmp = std::move(f);
    f = std::move(g);
    g = std::move(tmp);
  }

  EXPECT_EQ( TvFunc::zero(20), f );
  EXPECT_EQ( TvFunc::one(20), g );
}

TEST(TvFuncMove, copy_m)
{
  std::vector<TvFunc> f_list(2);
  f_list[0] = TvFunc::zero(20);
  f_list[1] = TvFunc::one(20);
  TvFuncM f(f_list);

  std::vector<TvFunc> g_list(2);
  g_list[0] = TvFunc::posi_literal(20, 0);
  g_list[1] = TvFunc::nega_literal(20, 1);
  TvFuncM g(g_list);

  for ( auto i = 0; i < 10000; ++ i ) {
    auto tmp = f;
    f = g;
    g = tmp;
  }

  EXPECT_EQ( TvFunc::zero(20), f.slice(0) );
  EXPECT_EQ( TvFunc::one(20), f.slice(1) );
  EXPECT_EQ( TvFunc::posi_literal(20, 0), g.slice(0) );
  EXPECT_EQ( TvFunc::nega_literal(20, 1), g.slice(1) );
}

TEST(TvFuncMove, move_m)
{
  std::vector<TvFunc> f_list(2);
  f_list[0] = TvFunc::zero(20);
  f_list[1] = TvFunc::one(20);
  TvFuncM f(f_list);

  std::vector<TvFunc> g_list(2);
  g_list[0] = TvFunc::posi_literal(20, 0);
  g_list[1] = TvFunc::nega_literal(20, 1);
  TvFuncM g(g_list);

  for ( auto i = 0; i < 10000; ++ i ) {
    auto tmp = std::move(f);
    f = std::move(g);
    g = std::move(tmp);
  }

  EXPECT_EQ( TvFunc::zero(20), f.slice(0) );
  EXPECT_EQ( TvFunc::one(20), f.slice(1) );
  EXPECT_EQ( TvFunc::posi_literal(20, 0), g.slice(0) );
  EXPECT_EQ( TvFunc::nega_literal(20, 1), g.slice(1) );
}

END_NAMESPACE_YM
