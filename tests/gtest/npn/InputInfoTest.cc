
/// @file InputInfoTest.cc
/// @brief InputInfoTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.


#include "gtest/gtest.h"
#include "InputInfo.h"


BEGIN_NAMESPACE_YM_LOGIC

TEST(InputInfoTest, constr)
{
  InputInfo iinfo;

  EXPECT_EQ( 0, iinfo.group_num() );
  EXPECT_EQ( 0, iinfo.polundet_num() );
}

TEST(InputInfoTest, base1)
{
  InputInfo iinfo;

  iinfo.new_group(0, 1);
  iinfo.new_group(1, 2);
  iinfo.add_elem(0, 2);

  EXPECT_EQ( 2, iinfo.group_num() );
  EXPECT_EQ( 0, iinfo.polundet_num() );

  // 等価グループ#0
  EXPECT_EQ( 1, iinfo.w1(0) );
  EXPECT_EQ( 2, iinfo.elem_num(0) );
  EXPECT_EQ( 0, iinfo.elem(0, 0) );
  EXPECT_EQ( 2, iinfo.elem(0, 1) );

  // 等価グループ#1
  EXPECT_EQ( 2, iinfo.w1(1) );
  EXPECT_EQ( 1, iinfo.elem_num(1) );
  EXPECT_EQ( 1, iinfo.elem(1, 0) );
}

TEST(InputInfoTest, base2)
{
  InputInfo iinfo;

  iinfo.new_group(0, 1);
  iinfo.new_group(1, 2);
  iinfo.add_elem(0, 2);
  iinfo.new_group(3, 0);
  iinfo.set_bisym(2);
  iinfo.add_elem(2, 4);
  iinfo.new_group(5, 0);
  iinfo.add_elem(3, 6);

  EXPECT_EQ( 4, iinfo.group_num() );
  EXPECT_EQ( 2, iinfo.polundet_num() );

  // 等価グループ#0
  EXPECT_EQ( 1, iinfo.w1(0) );
  EXPECT_EQ( 2, iinfo.elem_num(0) );
  EXPECT_EQ( 0, iinfo.elem(0, 0) );
  EXPECT_EQ( 2, iinfo.elem(0, 1) );
  EXPECT_FALSE( iinfo.bisym(0) );

  // 等価グループ#1
  EXPECT_EQ( 2, iinfo.w1(1) );
  EXPECT_EQ( 1, iinfo.elem_num(1) );
  EXPECT_EQ( 1, iinfo.elem(1, 0) );
  EXPECT_FALSE( iinfo.bisym(1) );

  // 等価グループ#2
  EXPECT_EQ( 0, iinfo.w1(2) );
  EXPECT_EQ( 2, iinfo.elem_num(2) );
  EXPECT_EQ( 3, iinfo.elem(2, 0) );
  EXPECT_EQ( 4, iinfo.elem(2, 1) );
  EXPECT_TRUE( iinfo.bisym(2) );
  EXPECT_EQ( 0x08U, iinfo.inv_bits(2) );

  // 等価グループ#3
  EXPECT_EQ( 0, iinfo.w1(3) );
  EXPECT_EQ( 2, iinfo.elem_num(3) );
  EXPECT_EQ( 5, iinfo.elem(3, 0) );
  EXPECT_EQ( 6, iinfo.elem(3, 1) );
  EXPECT_FALSE( iinfo.bisym(3) );
  EXPECT_EQ( 0x60U, iinfo.inv_bits(3) );

  EXPECT_EQ( 2, iinfo.polundet_num() );
  EXPECT_EQ( 2, iinfo.polundet_gid(0) );
  EXPECT_EQ( 3, iinfo.polundet_gid(1) );

  EXPECT_FALSE( iinfo.w1gt(0, 0) );
  EXPECT_FALSE( iinfo.w1gt(0, 1) );
  EXPECT_TRUE( iinfo.w1gt(0, 2) );
  EXPECT_TRUE( iinfo.w1gt(0, 3) );

  EXPECT_TRUE( iinfo.w1gt(1, 0) );
  EXPECT_FALSE( iinfo.w1gt(1, 1) );
  EXPECT_TRUE( iinfo.w1gt(1, 2) );
  EXPECT_TRUE( iinfo.w1gt(1, 3) );

  EXPECT_FALSE( iinfo.w1gt(2, 0) );
  EXPECT_FALSE( iinfo.w1gt(2, 1) );
  EXPECT_FALSE( iinfo.w1gt(2, 2) );
  EXPECT_TRUE( iinfo.w1gt(2, 3) );

  EXPECT_FALSE( iinfo.w1gt(3, 0) );
  EXPECT_FALSE( iinfo.w1gt(3, 1) );
  EXPECT_FALSE( iinfo.w1gt(3, 2) );
  EXPECT_FALSE( iinfo.w1gt(3, 3) );

  EXPECT_TRUE( iinfo.w1eq(0, 0) );
  EXPECT_FALSE( iinfo.w1eq(0, 1) );
  EXPECT_FALSE( iinfo.w1eq(0, 2) );
  EXPECT_FALSE( iinfo.w1eq(0, 3) );

  EXPECT_FALSE( iinfo.w1eq(1, 0) );
  EXPECT_TRUE( iinfo.w1eq(1, 1) );
  EXPECT_FALSE( iinfo.w1eq(1, 2) );
  EXPECT_FALSE( iinfo.w1eq(1, 3) );

  EXPECT_FALSE( iinfo.w1eq(2, 0) );
  EXPECT_FALSE( iinfo.w1eq(2, 1) );
  EXPECT_TRUE( iinfo.w1eq(2, 2) );
  EXPECT_FALSE( iinfo.w1eq(2, 3) );

  EXPECT_FALSE( iinfo.w1eq(3, 0) );
  EXPECT_FALSE( iinfo.w1eq(3, 1) );
  EXPECT_FALSE( iinfo.w1eq(3, 2) );
  EXPECT_TRUE( iinfo.w1eq(3, 3) );
}

END_NAMESPACE_YM_LOGIC
