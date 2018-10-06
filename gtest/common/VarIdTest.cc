
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

TEST(VarIdTest, compare)
{
  int val_list[] = {3, 5, 9};
  VarId var_list[] = { VarId(val_list[0]), VarId(val_list[1]), VarId(val_list[2]) };

  for ( int i1: { 0, 1, 2} ) {
    VarId var1 = var_list[i1];
    for ( int i2: { 0, 1, 2} ) {
      VarId var2 = var_list[i2];
      EXPECT_EQ( val_list[i1] == val_list[i2], var1 == var2 );
      EXPECT_EQ( val_list[i1] != val_list[i2], var1 != var2 );
      EXPECT_EQ( val_list[i1] < val_list[i2],  var1 < var2 );
      EXPECT_EQ( val_list[i1] > val_list[i2],  var1 > var2 );
      EXPECT_EQ( val_list[i1] <= val_list[i2], var1 <= var2 );
      EXPECT_EQ( val_list[i1] >= val_list[i2], var1 >= var2 );
    }
  }
}

END_NAMESPACE_YM
