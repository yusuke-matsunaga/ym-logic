
/// @file VarIdTest.cc
/// @brief VarIdTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.


#include "gtest/gtest.h"
#include "ym/VarId.h"


BEGIN_NAMESPACE_YM

TEST(VarIdTest, empty_constr)
{
  VarId var;

  EXPECT_FALSE( var.is_valid() );
  EXPECT_EQ( -1, var.val() );

  ostringstream oss;
  oss << var;
  EXPECT_EQ( string("---"), oss.str() );
}

TEST(VarIdTest, int_constr)
{
  int val = 2;
  VarId var(val);

  EXPECT_TRUE( var.is_valid() );
  EXPECT_EQ( val, var.val() );

  ostringstream oss;
  oss << var;
  EXPECT_EQ( string("V_2"), oss.str() );
}

END_NAMESPACE_YM
