
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

  EXPECT_EQ( kVarIdIllegal, var );
  EXPECT_EQ( -1, var.val() );
}

TEST(VarIdTest, int_constr)
{
  ymuint val = 2;
  VarId var(val);

  EXPECT_NE( kVarIdIllegal, var );
  EXPECT_EQ( val, var.val() );
}

END_NAMESPACE_YM
