
/// @file LitSetTest.cc
/// @brief LitSetTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/SopCube.h"
#include "LitSet.h"


BEGIN_NAMESPACE_YM_SOP

TEST(LitSetTest, empty)
{
  SizeType nv = 10;
  auto litset = LitSet(nv);

  EXPECT_EQ( nv, litset.variable_num() );
  for ( SizeType i = 0; i < nv; ++ i ) {
    for ( auto inv: {false, true} ) {
      auto lit = Literal(i, inv);
      EXPECT_FALSE( litset.is_in(lit) );
    }
  }
}

TEST(LitSetTest, single)
{
  SizeType nv = 10;
  auto lit0 = Literal(5, true);
  auto litset = LitSet(nv, lit0);
  EXPECT_EQ( nv, litset.variable_num() );
  for ( SizeType i = 0; i < nv; ++ i ) {
    for ( auto inv: {false, true} ) {
      auto lit = Literal(i, inv);
      auto exp = lit == lit0;
      ostringstream buf;
      buf << lit;
      EXPECT_EQ( exp, litset.is_in(lit) ) << buf.str();
    }
  }
}

TEST(LitSetTest, multi2)
{
  SizeType nv = 10;
  auto lit0 = Literal(5, true);
  auto lit1 = Literal(2, false);
  auto lits = vector<Literal>{lit0, lit1};
  auto litset = LitSet(nv, lits);
  EXPECT_EQ( nv, litset.variable_num() );
  for ( SizeType i = 0; i < nv; ++ i ) {
    for ( auto inv: {false, true} ) {
      auto lit = Literal(i, inv);
      auto exp = lit == lit0 || lit == lit1;
      ostringstream buf;
      buf << lit;
      EXPECT_EQ( exp, litset.is_in(lit) ) << buf.str();
    }
  }
}

TEST(LitSetTest, multi1)
{
  SizeType nv = 10;
  auto lit0 = Literal(5, true);
  auto lit1 = Literal(2, false);
  auto litset = LitSet(nv, {lit0, lit1});
  EXPECT_EQ( nv, litset.variable_num() );
  for ( SizeType i = 0; i < nv; ++ i ) {
    for ( auto inv: {false, true} ) {
      auto lit = Literal(i, inv);
      auto exp = lit == lit0 || lit == lit1;
      ostringstream buf;
      buf << lit;
      EXPECT_EQ( exp, litset.is_in(lit) ) << buf.str();
    }
  }
}

TEST(LitSetTest, check_intersect)
{
  SizeType nv = 10;
  auto lit2 = Literal(2, false);
  auto lit5 = Literal(5, false);
  auto lit7 = Literal(7, false);
  auto litset = LitSet(nv, {lit2, lit5, lit7});
  auto cube1 = SopCube(nv, {lit2, lit5, ~lit7});
  ostringstream buf1;
  buf1 << "litset = " << litset << endl
       << "cube1 = " << cube1;
  EXPECT_TRUE( litset.check_intersect(cube1) ) << buf1.str();
  auto cube2 = SopCube(nv, {~lit2});
  ostringstream buf2;
  buf2 << "litset = " << litset << endl
       << "cube2 = " << cube2;
  EXPECT_FALSE( litset.check_intersect(cube2) ) << buf2.str();
  auto cube3 = SopCube(nv);
  EXPECT_FALSE( litset.check_intersect(cube3) );
}

END_NAMESPACE_YM_SOP
