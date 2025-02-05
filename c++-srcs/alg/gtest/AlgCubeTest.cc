
/// @file AlgCubeTest.cc
/// @brief AlgCubeTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "AlgTest.h"
#include "ym/AlgCube.h"
#include "ym/Literal.h"


BEGIN_NAMESPACE_YM

TEST_F(AlgTest, constructor1)
{
  auto cube1 = AlgCube{nv};

  EXPECT_EQ( nv, cube1.variable_num() );
  EXPECT_EQ( 0,  cube1.literal_num() );

  auto lit_list = cube1.literal_list();
  EXPECT_TRUE( lit_list.empty() );
};

TEST_F(AlgTest, constructor2)
{
  auto cube1 = AlgCube{nv, ~lit5};

  EXPECT_EQ( nv, cube1.variable_num() );
  EXPECT_EQ( 1,  cube1.literal_num() );

  auto lit_list = cube1.literal_list();
  EXPECT_EQ( 1, lit_list.size() );
  EXPECT_EQ( ~lit5, lit_list[0] );

  EXPECT_FALSE( cube1.check_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var5, false)) );
  EXPECT_TRUE(  cube1.check_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var7, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var9, true)) );

};

TEST_F(AlgTest, constructor3)
{
  vector<Literal> lit_list{~lit5, lit0};
  auto cube1 = AlgCube{nv, lit_list};

  auto nl = lit_list.size();
  EXPECT_EQ( nv, cube1.variable_num() );
  EXPECT_EQ( nl,  cube1.literal_num() );

  auto lit_list1 = cube1.literal_list();
  EXPECT_EQ( nl, lit_list1.size() );
  EXPECT_EQ( lit0, lit_list1[0] );
  EXPECT_EQ( ~lit5, lit_list1[1] );

  EXPECT_TRUE(  cube1.check_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var5, false)) );
  EXPECT_TRUE(  cube1.check_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var7, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var9, true)) );

};

TEST_F(AlgTest, constructor4)
{
  auto cube1 = AlgCube{nv, {~lit5, lit0}};

  auto nl = 2;
  EXPECT_EQ( nv, cube1.variable_num() );
  EXPECT_EQ( nl,  cube1.literal_num() );

  auto lit_list1 = cube1.literal_list();
  EXPECT_EQ( nl, lit_list1.size() );
  EXPECT_EQ( lit0, lit_list1[0] );
  EXPECT_EQ( ~lit5, lit_list1[1] );

  EXPECT_TRUE(  cube1.check_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var5, false)) );
  EXPECT_TRUE(  cube1.check_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var7, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var9, true)) );

};

TEST_F(AlgTest, copy_constructor)
{
  auto nl = 2;
  auto cube1 = AlgCube{nv, {~lit5, lit0}};

  auto cube2 = AlgCube{cube1};
  EXPECT_EQ( nv, cube2.variable_num() );
  EXPECT_EQ( nl,  cube2.literal_num() );

  auto lit_list1 = cube2.literal_list();
  EXPECT_EQ( nl, lit_list1.size() );
  EXPECT_EQ( lit0, lit_list1[0] );
  EXPECT_EQ( ~lit5, lit_list1[1] );

  EXPECT_TRUE(  cube2.check_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var5, false)) );
  EXPECT_TRUE(  cube2.check_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var7, false)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var9, true)) );

};

TEST_F(AlgTest, copy_assignment)
{
  auto nl = 2;
  auto cube1 = AlgCube{nv, {~lit5, lit0}};
  auto cube2 = AlgCube{nv};

  EXPECT_EQ( 0, cube2.literal_num() );

  cube2 = cube1;
  EXPECT_EQ( nv, cube2.variable_num() );
  EXPECT_EQ( nl, cube2.literal_num() );

  auto lit_list1 = cube2.literal_list();
  EXPECT_EQ( nl, lit_list1.size() );
  EXPECT_EQ( lit0, lit_list1[0] );
  EXPECT_EQ( ~lit5, lit_list1[1] );

  EXPECT_TRUE(  cube2.check_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var5, false)) );
  EXPECT_TRUE(  cube2.check_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var7, false)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var9, true)) );

};

TEST_F(AlgTest, move_constructor)
{
  auto nl = 2;
  auto cube1 = AlgCube{nv, {~lit5, lit0}};
  auto cube2 = AlgCube{std::move(cube1)};

  EXPECT_EQ( 0, cube1.literal_num() );

  EXPECT_EQ( nv, cube2.variable_num() );
  EXPECT_EQ( nl, cube2.literal_num() );

  auto lit_list1 = cube2.literal_list();
  EXPECT_EQ( nl, lit_list1.size() );
  EXPECT_EQ( lit0, lit_list1[0] );
  EXPECT_EQ( ~lit5, lit_list1[1] );

  EXPECT_TRUE(  cube2.check_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var5, false)) );
  EXPECT_TRUE(  cube2.check_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var7, false)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var9, true)) );

};

TEST_F(AlgTest, move_assignment)
{
  auto nl = 2;
  auto cube1 = AlgCube{nv, {~lit5, lit0}};
  auto cube2 = AlgCube{nv};
  EXPECT_EQ( 0, cube2.literal_num() );

  cube2 = std::move(cube1);

  EXPECT_EQ( 0, cube1.literal_num() );

  EXPECT_EQ( nv, cube2.variable_num() );
  EXPECT_EQ( nl,  cube2.literal_num() );

  auto lit_list1 = cube2.literal_list();
  EXPECT_EQ( nl, lit_list1.size() );
  EXPECT_EQ( lit0, lit_list1[0] );
  EXPECT_EQ( ~lit5, lit_list1[1] );

  EXPECT_TRUE(  cube2.check_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var5, false)) );
  EXPECT_TRUE(  cube2.check_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var7, false)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube2.check_literal(Literal(var9, true)) );

};

TEST_F(AlgTest, check_containment)
{
  auto cube1 = AlgCube{nv, {~lit5, lit0}};
  auto cube2 = AlgCube{nv, {~lit5}};

  EXPECT_TRUE(  cube1.check_containment(cube2) );
  EXPECT_FALSE( cube2.check_containment(cube1) );

  auto cube3 = AlgCube{nv, {lit0}};
  EXPECT_FALSE( cube2.check_containment(cube3) );
  EXPECT_FALSE( cube3.check_containment(cube2) );
};

TEST_F(AlgTest, check_containment_bad)
{
  auto cube1 = AlgCube{nv, {~lit5, lit0}};
  auto cube2 = AlgCube{nv + 1, {~lit5}};

  ASSERT_THROW( cube1.check_containment(cube2), std::invalid_argument );
  ASSERT_THROW( cube2.check_containment(cube1), std::invalid_argument );
};

TEST_F(AlgTest, check_intersect)
{
  auto cube1 = AlgCube{nv, {~lit5, ~lit7}};
  auto cube2 = AlgCube{nv, {lit0, ~lit5}};

  EXPECT_TRUE(  cube1.check_intersect(cube2) );
  EXPECT_TRUE(  cube2.check_intersect(cube1) );

  auto cube3 = AlgCube{nv, {~lit0}};
  EXPECT_FALSE( cube1.check_containment(cube3) );
  EXPECT_FALSE( cube2.check_containment(cube3) );
  EXPECT_FALSE( cube3.check_containment(cube1) );
  EXPECT_FALSE( cube3.check_containment(cube2) );
};

TEST_F(AlgTest, check_intersect_bad)
{
  auto cube1 = AlgCube{nv, {~lit5, ~lit7}};
  auto cube2 = AlgCube{nv + 1, {lit0, ~lit5}};

  ASSERT_THROW( cube1.check_intersect(cube2), std::invalid_argument );
  ASSERT_THROW( cube2.check_intersect(cube1), std::invalid_argument );
};

TEST_F(AlgTest, cube_cube_product1)
{
  auto cube1 = AlgCube{nv, {~lit5, ~lit7}};
  auto cube2 = AlgCube{nv, {lit0, ~lit5}};

  auto cube3 = cube1 * cube2;

  EXPECT_TRUE(  cube3.check_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var5, false)) );
  EXPECT_TRUE(  cube3.check_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var7, false)) );
  EXPECT_TRUE(  cube3.check_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var9, true)) );
};

TEST_F(AlgTest, cube_cube_product2)
{
  auto cube1 = AlgCube{nv, {~lit5, ~lit7}};
  auto cube2 = AlgCube{nv, {lit0, lit5}};

  auto cube3 = cube1 * cube2;

  EXPECT_FALSE( cube3.check_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var5, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var7, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var9, true)) );
};

TEST_F(AlgTest, cube_cube_product_bad)
{
  auto cube1 = AlgCube{nv, {~lit5, ~lit7}};
  auto cube2 = AlgCube{nv + 1, {lit0, ~lit5}};

  ASSERT_THROW( cube1 * cube2, std::invalid_argument );
}

TEST_F(AlgTest, Rcube_cube_product1)
{
  auto cube1 = AlgCube{nv, {~lit5, ~lit7}};
  auto cube2 = AlgCube{nv, {lit0, ~lit5}};
  auto cube3 = std::move(cube1) * cube2;

  EXPECT_EQ( 0, cube1.literal_num() );

  EXPECT_TRUE(  cube3.check_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var5, false)) );
  EXPECT_TRUE(  cube3.check_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var7, false)) );
  EXPECT_TRUE(  cube3.check_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var9, true)) );
};

TEST_F(AlgTest, cube_Rcube_product1)
{
  auto cube1 = AlgCube{nv, {~lit5, ~lit7}};
  auto cube2 = AlgCube{nv, {lit0, ~lit5}};
  auto cube3 = cube1 * std::move(cube2);

  EXPECT_EQ( 0, cube2.literal_num() );

  EXPECT_TRUE(  cube3.check_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var5, false)) );
  EXPECT_TRUE(  cube3.check_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var7, false)) );
  EXPECT_TRUE(  cube3.check_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var9, true)) );
};

TEST_F(AlgTest, Rcube_Rcube_product1)
{
  auto cube1 = AlgCube{nv, {~lit5, ~lit7}};
  auto cube2 = AlgCube{nv, {lit0, ~lit5}};
  auto cube3 = std::move(cube1) * std::move(cube2);

  EXPECT_EQ( 0, cube1.literal_num() );

  EXPECT_TRUE(  cube3.check_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var5, false)) );
  EXPECT_TRUE(  cube3.check_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var7, false)) );
  EXPECT_TRUE(  cube3.check_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var9, true)) );
};

TEST_F(AlgTest, lit_cube_product1)
{
  auto cube1 = AlgCube{nv, {~lit5, ~lit7}};
  auto cube3 = lit0 * cube1;

  EXPECT_TRUE(  cube3.check_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var5, false)) );
  EXPECT_TRUE(  cube3.check_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var7, false)) );
  EXPECT_TRUE(  cube3.check_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var9, true)) );
};

TEST_F(AlgTest, lit_Rcube_product1)
{
  auto cube1 = AlgCube{nv, {~lit5, ~lit7}};
  auto cube3 = lit0 * std::move(cube1);

  EXPECT_EQ( 0, cube1.literal_num() );

  EXPECT_TRUE(  cube3.check_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var5, false)) );
  EXPECT_TRUE(  cube3.check_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var7, false)) );
  EXPECT_TRUE(  cube3.check_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var9, true)) );
};

TEST_F(AlgTest, lit_cube_product2)
{
  auto cube1 = AlgCube{nv, {~lit5, ~lit7}};
  auto cube3 = lit5 * cube1;

  EXPECT_FALSE( cube3.check_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var5, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var7, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var9, true)) );
};

TEST_F(AlgTest, cube_lit_product1)
{
  auto cube1 = AlgCube{nv, {~lit5, ~lit7}};
  auto cube3 = cube1 * lit0;

  EXPECT_TRUE(  cube3.check_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var5, false)) );
  EXPECT_TRUE(  cube3.check_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var7, false)) );
  EXPECT_TRUE(  cube3.check_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var9, true)) );
};

TEST_F(AlgTest, Rcube_lit_product1)
{
  auto cube1 = AlgCube{nv, {~lit5, ~lit7}};
  auto cube3 = std::move(cube1) * lit0;

  EXPECT_EQ( 0, cube1.literal_num() );

  EXPECT_TRUE(  cube3.check_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var5, false)) );
  EXPECT_TRUE(  cube3.check_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var7, false)) );
  EXPECT_TRUE(  cube3.check_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var9, true)) );
};

TEST_F(AlgTest, cube_lit_product2)
{
  auto cube1 = AlgCube{nv, {~lit5, ~lit5}};
  auto cube3 = cube1 * lit5;

  EXPECT_FALSE( cube3.check_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var5, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var7, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var9, true)) );
};

TEST_F(AlgTest, Scube_cube_product1)
{
  auto cube1 = AlgCube{nv, {~lit5, ~lit7}};
  auto cube2 = AlgCube{nv, {lit0, ~lit5}};
  cube1 = cube1 * cube2;

  EXPECT_TRUE(  cube1.check_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var5, false)) );
  EXPECT_TRUE(  cube1.check_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var7, false)) );
  EXPECT_TRUE(  cube1.check_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var9, true)) );
};

TEST_F(AlgTest, Scube_cube_product2)
{
  auto cube1 = AlgCube{nv, {~lit5, ~lit7}};
  auto cube2 = AlgCube{nv, {lit0, lit5}};
  cube1 = cube1 * cube2;

  EXPECT_FALSE( cube1.check_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var5, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var7, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var9, true)) );
};

TEST_F(AlgTest, Scube_literal_product1)
{
  auto cube1 = AlgCube{nv, {~lit5, ~lit7}};
  cube1 = cube1 * lit0;

  EXPECT_TRUE(  cube1.check_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var5, false)) );
  EXPECT_TRUE(  cube1.check_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var7, false)) );
  EXPECT_TRUE(  cube1.check_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var9, true)) );
};

TEST_F(AlgTest, Scube_literal_product2)
{
  auto cube1 = AlgCube{nv, {~lit5, ~lit7}};
  cube1 = cube1 * lit5;

  EXPECT_FALSE( cube1.check_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var5, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var7, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var9, true)) );
};

TEST_F(AlgTest, literal_Scube_product1)
{
  auto cube1 = AlgCube{nv, {~lit5, ~lit7}};
  cube1 = lit0 * cube1;

  EXPECT_TRUE(  cube1.check_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var5, false)) );
  EXPECT_TRUE(  cube1.check_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var7, false)) );
  EXPECT_TRUE(  cube1.check_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var9, true)) );
};

TEST_F(AlgTest, literal_Scube_product2)
{
  auto cube1 = AlgCube{nv, {~lit5, ~lit7}};
  cube1 = lit5 * cube1;

  EXPECT_FALSE( cube1.check_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var5, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var7, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var9, true)) );
};

TEST_F(AlgTest, Icube_cube_product1)
{
  auto cube1 = AlgCube{nv, {~lit5, ~lit7}};
  auto cube2 = AlgCube{nv, {lit0, ~lit5}};
  cube1 *= cube2;

  EXPECT_TRUE(  cube1.check_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var5, false)) );
  EXPECT_TRUE(  cube1.check_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var7, false)) );
  EXPECT_TRUE(  cube1.check_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var9, true)) );
};

TEST_F(AlgTest, Icube_cube_product2)
{
  auto cube1 = AlgCube{nv, {~lit5, ~lit7}};
  auto cube2 = AlgCube{nv, {lit0, lit5}};
  cube1 *= cube2;

  EXPECT_FALSE( cube1.check_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var5, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var7, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var9, true)) );
};

TEST_F(AlgTest, Icube_literal_product1)
{
  auto cube1 = AlgCube{nv, {~lit5, ~lit7}};
  cube1 *= lit0;

  EXPECT_TRUE(  cube1.check_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var5, false)) );
  EXPECT_TRUE(  cube1.check_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var7, false)) );
  EXPECT_TRUE(  cube1.check_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var9, true)) );
};

TEST_F(AlgTest, Icube_literal_product2)
{
  auto cube1 = AlgCube{nv, {~lit5, ~lit7}};
  cube1 *= lit5;

  EXPECT_FALSE( cube1.check_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var5, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var7, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var9, true)) );
};

TEST_F(AlgTest, cube_cube_quotient1)
{
  auto cube1 = AlgCube{nv, {lit0, ~lit5, ~lit7}};
  auto cube2 = AlgCube{nv, {lit0, ~lit7}};
  auto cube3 = cube1 / cube2;

  EXPECT_FALSE( cube3.check_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var5, false)) );
  EXPECT_TRUE(  cube3.check_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var7, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var9, true)) );
};

TEST_F(AlgTest, Rcube_cube_quotient1)
{
  auto cube1 = AlgCube{nv, {lit0, ~lit5, ~lit7}};
  auto cube2 = AlgCube{nv, {lit0, ~lit7}};
  auto cube3 = std::move(cube1) / cube2;

  EXPECT_EQ( 0, cube1.literal_num() );

  EXPECT_FALSE( cube3.check_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var5, false)) );
  EXPECT_TRUE(  cube3.check_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var7, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var9, true)) );
};

TEST_F(AlgTest, cube_cube_quotient2)
{
  auto cube1 = AlgCube{nv, {lit0, ~lit5}};
  auto cube2 = AlgCube{nv, {~lit7}};
  auto cube3 = cube1 / cube2;

  EXPECT_FALSE( cube3.check_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var5, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var7, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var9, true)) );
};

TEST_F(AlgTest, cube_cube_quotient_bad)
{
  auto cube1 = AlgCube{nv, {lit0, ~lit5, ~lit7}};
  auto cube2 = AlgCube{nv + 1, {lit0, ~lit7}};

  ASSERT_THROW( cube1 / cube2, std::invalid_argument );
}

TEST_F(AlgTest, cube_lit_quotient1)
{
  auto cube1 = AlgCube{nv, {lit0, ~lit5, ~lit7}};
  auto cube3 = cube1 / lit0;

  EXPECT_FALSE( cube3.check_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var5, false)) );
  EXPECT_TRUE(  cube3.check_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var7, false)) );
  EXPECT_TRUE(  cube3.check_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var9, true)) );
};

TEST_F(AlgTest, Rcube_lit_quotient1)
{
  auto cube1 = AlgCube{nv, {lit0, ~lit5, ~lit7}};
  auto cube3 = std::move(cube1) / lit0;

  EXPECT_EQ( 0, cube1.literal_num() );

  EXPECT_FALSE( cube3.check_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var5, false)) );
  EXPECT_TRUE(  cube3.check_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var7, false)) );
  EXPECT_TRUE(  cube3.check_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var9, true)) );
};

TEST_F(AlgTest, cube_lit_quotient2)
{
  auto cube1 = AlgCube{nv, {lit0, ~lit5, ~lit7}};
  auto cube3 = cube1 / lit5;

  EXPECT_FALSE( cube3.check_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var5, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var7, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube3.check_literal(Literal(var9, true)) );
};

TEST_F(AlgTest, Scube_cube_quotient1)
{
  auto cube1 = AlgCube{nv, {lit0, ~lit5, ~lit7}};
  auto cube2 = AlgCube{nv, {lit0, ~lit7}};
  cube1 = cube1 / cube2;

  EXPECT_FALSE( cube1.check_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var5, false)) );
  EXPECT_TRUE(  cube1.check_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var7, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var9, true)) );
};

TEST_F(AlgTest, Scube_cube_quotient2)
{
  auto cube1 = AlgCube{nv, {lit0, ~lit5}};
  auto cube2 = AlgCube{nv, {~lit7}};
  cube1 = cube1 / cube2;

  EXPECT_FALSE( cube1.check_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var5, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var7, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var9, true)) );
};

TEST_F(AlgTest, Scube_literal_quotient1)
{
  auto cube1 = AlgCube{nv, {lit0, ~lit5, ~lit7}};
  cube1 = cube1 / lit0;

  EXPECT_FALSE( cube1.check_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var5, false)) );
  EXPECT_TRUE(  cube1.check_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var7, false)) );
  EXPECT_TRUE(  cube1.check_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var9, true)) );
};

TEST_F(AlgTest, Scube_literal_quotient2)
{
  auto cube1 = AlgCube{nv, {lit4, ~lit5, ~lit7}};
  cube1 = cube1 / lit5;

  EXPECT_FALSE( cube1.check_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var5, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var7, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var9, true)) );
};

TEST_F(AlgTest, Icube_cube_quotient1)
{
  auto cube1 = AlgCube{nv, {lit0, ~lit5, ~lit7}};
  auto cube2 = AlgCube{nv, {lit0, ~lit7}};
  cube1 /= cube2;

  EXPECT_FALSE( cube1.check_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var5, false)) );
  EXPECT_TRUE(  cube1.check_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var7, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var9, true)) );
};

TEST_F(AlgTest, Icube_cube_quotient2)
{
  auto cube1 = AlgCube{nv, {lit0, ~lit5}};
  auto cube2 = AlgCube{nv, {~lit7}};
  cube1 /= cube2;

  EXPECT_FALSE( cube1.check_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var5, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var7, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var9, true)) );
};

TEST_F(AlgTest, Icube_literal_quotient1)
{
  auto cube1 = AlgCube{nv, {lit0, ~lit5, ~lit7}};
  cube1 /= lit0;

  EXPECT_FALSE( cube1.check_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var5, false)) );
  EXPECT_TRUE(  cube1.check_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var7, false)) );
  EXPECT_TRUE(  cube1.check_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var9, true)) );
};

TEST_F(AlgTest, Icube_literal_quotient2)
{
  auto cube1 = AlgCube{nv, {~lit5, lit1, lit2}};

  cube1 /= lit5;

  EXPECT_FALSE( cube1.check_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var5, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var7, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube1.check_literal(Literal(var9, true)) );
};

TEST_F(AlgTest, compare1)
{
  const int nv1 = 100;
  SizeType var80{80};

  auto lit0 = Literal{var0, false};
  auto lit1 = Literal{var80, false};

  auto cube1 = AlgCube{nv1, { lit0,  lit1}};
  auto cube2 = AlgCube{nv1, { lit0, ~lit1}};
  auto cube3 = AlgCube{nv1, {~lit0,  lit1}};

  EXPECT_EQ(  0, compare(cube1, cube1) );
  EXPECT_EQ( -1, compare(cube1, cube2) );
  EXPECT_EQ( -1, compare(cube1, cube3) );
  EXPECT_EQ(  1, compare(cube2, cube1) );
  EXPECT_EQ(  0, compare(cube2, cube2) );
  EXPECT_EQ( -1, compare(cube2, cube3) );
  EXPECT_EQ(  1, compare(cube3, cube1) );
  EXPECT_EQ(  1, compare(cube3, cube2) );
  EXPECT_EQ(  0, compare(cube3, cube3) );
};

TEST_F(AlgTest, compare2)
{
  const int nv1 = 100;
  SizeType var80{80};

  auto lit0 = Literal{var0, false};
  auto lit1 = Literal{var80, false};

  auto cube1 = AlgCube{nv1, { lit0,  lit1}};
  auto cube2 = AlgCube{nv1, { lit0, ~lit1}};
  auto cube3 = AlgCube{nv1, {~lit0,  lit1}};

  EXPECT_TRUE(  cube1 == cube1 );
  EXPECT_FALSE( cube1 <  cube1 );
  EXPECT_FALSE( cube1 >  cube1 );
  EXPECT_TRUE(  cube1 <= cube1 );
  EXPECT_TRUE(  cube1 >= cube1 );

  EXPECT_FALSE( cube1 == cube2 );
  EXPECT_TRUE(  cube1 <  cube2 );
  EXPECT_FALSE( cube1 >  cube2 );
  EXPECT_TRUE(  cube1 <= cube2 );
  EXPECT_FALSE( cube1 >= cube2 );

  EXPECT_FALSE( cube1 == cube3 );
  EXPECT_TRUE(  cube1 <  cube3 );
  EXPECT_FALSE( cube1 >  cube3 );
  EXPECT_TRUE(  cube1 <= cube3 );
  EXPECT_FALSE( cube1 >= cube3 );

  EXPECT_FALSE( cube2 == cube1 );
  EXPECT_FALSE( cube2 <  cube1 );
  EXPECT_TRUE(  cube2 >  cube1 );
  EXPECT_FALSE( cube2 <= cube1 );
  EXPECT_TRUE(  cube2 >= cube1 );

  EXPECT_TRUE(  cube2 == cube2 );
  EXPECT_FALSE( cube2 <  cube2 );
  EXPECT_FALSE( cube2 >  cube2 );
  EXPECT_TRUE(  cube2 <= cube2 );
  EXPECT_TRUE(  cube2 >= cube2 );

  EXPECT_FALSE( cube2 == cube3 );
  EXPECT_TRUE(  cube2 <  cube3 );
  EXPECT_FALSE( cube2 >  cube3 );
  EXPECT_TRUE(  cube2 <= cube3 );
  EXPECT_FALSE( cube2 >= cube3 );

  EXPECT_FALSE( cube3 == cube1 );
  EXPECT_FALSE( cube3 <  cube1 );
  EXPECT_TRUE(  cube3 >  cube1 );
  EXPECT_FALSE( cube3 <= cube1 );
  EXPECT_TRUE(  cube3 >= cube1 );

  EXPECT_FALSE( cube3 == cube2 );
  EXPECT_FALSE( cube3 <  cube2 );
  EXPECT_TRUE(  cube3 >  cube2 );
  EXPECT_FALSE( cube3 <= cube2 );
  EXPECT_TRUE(  cube3 >= cube2 );

  EXPECT_TRUE(  cube3 == cube3 );
  EXPECT_FALSE( cube3 <  cube3 );
  EXPECT_FALSE( cube3 >  cube3 );
  EXPECT_TRUE(  cube3 <= cube3 );
  EXPECT_TRUE(  cube3 >= cube3 );

};

TEST_F(AlgTest, compare_bad)
{
  const int nv1 = 100;
  SizeType var80{80};

  auto lit0 = Literal{var0, false};
  auto lit1 = Literal{var80, false};

  auto cube1 = AlgCube{nv1, { lit0,  lit1}};
  auto cube2 = AlgCube{nv, { lit0, ~lit1}};

  ASSERT_THROW( auto r = (cube1 == cube2), std::invalid_argument );
  ASSERT_THROW( auto r = (cube1 < cube2), std::invalid_argument );
  ASSERT_THROW( auto r = (cube1 > cube2), std::invalid_argument );
  ASSERT_THROW( auto r = (cube1 <= cube2), std::invalid_argument );
  ASSERT_THROW( auto r = (cube1 >= cube2), std::invalid_argument );
}

TEST_F(AlgTest, to_expr1)
{
  auto cube1 = AlgCube{3, {lit0, lit1, ~lit2}};

  auto expr = cube1.expr();
  auto expr_str = expr.to_string();
  EXPECT_EQ( "( 0 & 1 & ~2 )", expr_str );
}

END_NAMESPACE_YM
