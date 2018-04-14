
/// @file TvFuncMove.cc
/// @brief TvFuncMove の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.


#include "gtest/gtest.h"
#include "ym/TvFunc.h"
#include "ym/TvFuncM.h"


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

TEST(TvFuncMove, copy_m)
{
  vector<TvFunc> f_list(2);
  f_list[0] = TvFunc::const_zero(20);
  f_list[1] = TvFunc::const_one(20);
  TvFuncM f(f_list);

  vector<TvFunc> g_list(2);
  g_list[0] = TvFunc::posi_literal(20, VarId(0));
  g_list[1] = TvFunc::nega_literal(20, VarId(1));
  TvFuncM g(g_list);

  for ( auto i = 0; i < 10000; ++ i ) {
    auto tmp = f;
    f = g;
    g = tmp;
  }

  EXPECT_EQ( TvFunc::const_zero(20), f.output(VarId(0)) );
  EXPECT_EQ( TvFunc::const_one(20), f.output(VarId(1)) );
  EXPECT_EQ( TvFunc::posi_literal(20, VarId(0)), g.output(VarId(0)) );
  EXPECT_EQ( TvFunc::nega_literal(20, VarId(1)), g.output(VarId(1)) );
}

TEST(TvFuncMove, move_m)
{
  vector<TvFunc> f_list(2);
  f_list[0] = TvFunc::const_zero(20);
  f_list[1] = TvFunc::const_one(20);
  TvFuncM f(f_list);

  vector<TvFunc> g_list(2);
  g_list[0] = TvFunc::posi_literal(20, VarId(0));
  g_list[1] = TvFunc::nega_literal(20, VarId(1));
  TvFuncM g(g_list);

  for ( auto i = 0; i < 10000; ++ i ) {
    auto tmp = std::move(f);
    f = std::move(g);
    g = std::move(tmp);
  }

  EXPECT_EQ( TvFunc::const_zero(20), f.output(VarId(0)) );
  EXPECT_EQ( TvFunc::const_one(20), f.output(VarId(1)) );
  EXPECT_EQ( TvFunc::posi_literal(20, VarId(0)), g.output(VarId(0)) );
  EXPECT_EQ( TvFunc::nega_literal(20, VarId(1)), g.output(VarId(1)) );
}

END_NAMESPACE_YM
