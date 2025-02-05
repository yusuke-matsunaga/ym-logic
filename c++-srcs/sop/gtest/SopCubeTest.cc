
/// @file SopCubeTest.cc
/// @brief SopCubeTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/SopCube.h"
#include "ym/Literal.h"


BEGIN_NAMESPACE_YM

class SopCubeTest :
public ::testing::Test
{
public:

  SizeType nv{10};
  SizeType var0{0};
  SizeType var1{1};
  SizeType var2{2};
  SizeType var3{3};
  SizeType var4{4};
  SizeType var5{5};
  SizeType var6{6};
  SizeType var7{7};
  SizeType var8{8};
  SizeType var9{9};

};

TEST_F(SopCubeTest, constructor1)
{
  auto cube1 = SopCube{nv};

  EXPECT_EQ( nv, cube1.variable_num() );
  EXPECT_EQ( 0,  cube1.literal_num() );

  auto lit_list = cube1.literal_list();
  EXPECT_TRUE( lit_list.empty() );
};

TEST_F(SopCubeTest, constructor2)
{
  auto lit = Literal{var5, true};
  auto cube1 = SopCube{nv, lit};

  EXPECT_EQ( nv, cube1.variable_num() );
  EXPECT_EQ( 1,  cube1.literal_num() );

  auto lit_list = cube1.literal_list();
  ASSERT_EQ( 1, lit_list.size() );
  EXPECT_EQ( lit, lit_list[0] );

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

TEST_F(SopCubeTest, constructor3)
{
  auto lit1 = Literal{var5, true};
  auto lit2 = Literal{var0, false};
  auto lit_list = vector<Literal>{lit1, lit2};
  auto cube1 = SopCube{nv, lit_list};

  auto nl = lit_list.size();
  EXPECT_EQ( nv, cube1.variable_num() );
  EXPECT_EQ( nl,  cube1.literal_num() );

  auto lit_list1 = cube1.literal_list();
  ASSERT_EQ( nl, lit_list1.size() );
  EXPECT_EQ( lit2, lit_list1[0] );
  EXPECT_EQ( lit1, lit_list1[1] );

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

TEST_F(SopCubeTest, constructor4)
{
  auto lit1 = Literal{var5, true};
  auto lit2 = Literal{var0, false};
  auto cube1 = SopCube{nv, {lit1, lit2}};

  auto nl = 2;
  EXPECT_EQ( nv, cube1.variable_num() );
  EXPECT_EQ( nl,  cube1.literal_num() );

  auto lit_list1 = cube1.literal_list();
  ASSERT_EQ( nl, lit_list1.size() );
  EXPECT_EQ( lit2, lit_list1[0] );
  EXPECT_EQ( lit1, lit_list1[1] );

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

TEST_F(SopCubeTest, copy_constructor)
{
  auto lit1 = Literal{var5, true};
  auto lit2 = Literal{var0, false};
  auto nl = 2;
  auto cube1 = SopCube{nv, {lit1, lit2}};

  auto cube2 = SopCube{cube1};
  EXPECT_EQ( nv, cube2.variable_num() );
  EXPECT_EQ( nl,  cube2.literal_num() );

  auto lit_list1 = cube2.literal_list();
  ASSERT_EQ( nl, lit_list1.size() );
  EXPECT_EQ( lit2, lit_list1[0] );
  EXPECT_EQ( lit1, lit_list1[1] );

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

TEST_F(SopCubeTest, copy_assignment)
{
  auto lit1 = Literal{var5, true};
  auto lit2 = Literal{var0, false};
  auto nl = 2;
  auto cube1 = SopCube{nv, {lit1, lit2}};

  auto cube2 = SopCube{nv};
  EXPECT_EQ( 0, cube2.literal_num() );

  cube2 = cube1;
  EXPECT_EQ( nv, cube2.variable_num() );
  EXPECT_EQ( nl, cube2.literal_num() );

  auto lit_list1 = cube2.literal_list();
  ASSERT_EQ( nl, lit_list1.size() );
  EXPECT_EQ( lit2, lit_list1[0] );
  EXPECT_EQ( lit1, lit_list1[1] );

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

TEST_F(SopCubeTest, move_constructor)
{
  auto lit1 = Literal{var5, true};
  auto lit2 = Literal{var0, false};
  auto nl = 2;
  auto cube1 = SopCube{nv, {lit1, lit2}};

  auto cube2 = SopCube{std::move(cube1)};

  EXPECT_EQ( 0, cube1.literal_num() );

  EXPECT_EQ( nv, cube2.variable_num() );
  EXPECT_EQ( nl, cube2.literal_num() );

  auto lit_list1 = cube2.literal_list();
  ASSERT_EQ( nl, lit_list1.size() );
  EXPECT_EQ( lit2, lit_list1[0] );
  EXPECT_EQ( lit1, lit_list1[1] );

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

TEST_F(SopCubeTest, move_assignment)
{
  auto lit1 = Literal{var5, true};
  auto lit2 = Literal{var0, false};
  auto nl = 2;
  auto cube1 = SopCube{nv, {lit1, lit2}};

  auto cube2 = SopCube{nv};
  EXPECT_EQ( 0, cube2.literal_num() );

  cube2 = std::move(cube1);

  EXPECT_EQ( 0, cube1.literal_num() );

  EXPECT_EQ( nv, cube2.variable_num() );
  EXPECT_EQ( nl,  cube2.literal_num() );

  auto lit_list1 = cube2.literal_list();
  ASSERT_EQ( nl, lit_list1.size() );
  EXPECT_EQ( lit2, lit_list1[0] );
  EXPECT_EQ( lit1, lit_list1[1] );

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

TEST_F(SopCubeTest, check_containment)
{
  auto lit1 = Literal{var5, true};
  auto lit2 = Literal{var0, false};
  auto cube1 = SopCube{nv, {lit1, lit2}};
  auto cube2 = SopCube{nv, {lit1}};

  EXPECT_TRUE(  cube1.check_containment(cube2) );
  EXPECT_FALSE( cube2.check_containment(cube1) );

  auto cube3 = SopCube{nv, {lit2}};
  EXPECT_FALSE( cube2.check_containment(cube3) );
  EXPECT_FALSE( cube3.check_containment(cube2) );
};

TEST_F(SopCubeTest, check_containment_bad)
{
  auto lit1 = Literal{var5, true};
  auto lit2 = Literal{var0, false};
  auto cube1 = SopCube{nv, {lit1, lit2}};
  auto cube2 = SopCube{nv + 1, {lit1}};

  ASSERT_THROW( cube1.check_containment(cube2), std::invalid_argument );
  ASSERT_THROW( cube2.check_containment(cube1), std::invalid_argument );
};

#if 0
TEST_F(SopCubeTest, check_intersect)
{
  auto lit0 = Literal{var0, false};
  auto lit1 = Literal{var5, true};
  auto lit2 = Literal{var7, true};
  auto cube1 = SopCube{nv, {lit1, lit2}};
  auto cube2 = SopCube{nv, {lit0, lit1}};

  EXPECT_TRUE(  cube1.check_intersect(cube2) );
  EXPECT_TRUE(  cube2.check_intersect(cube1) );

  auto lit3 = Literal{var0, true};
  auto cube3 = SopCube{nv, {lit3}};
  EXPECT_FALSE( cube1.check_containment(cube3) );
  EXPECT_FALSE( cube2.check_containment(cube3) );
  EXPECT_FALSE( cube3.check_containment(cube1) );
  EXPECT_FALSE( cube3.check_containment(cube2) );
};

TEST_F(SopCubeTest, check_intersect_bad)
{
  auto lit0 = Literal{var0, false};
  auto lit1 = Literal{var5, true};
  auto lit2 = Literal{var7, true};
  auto cube1 = SopCube{nv, {lit1, lit2}};
  auto cube2 = SopCube{nv + 1, {lit0, lit1}};

  ASSERT_THROW( cube1.check_intersect(cube2), std::invalid_argument );
  ASSERT_THROW( cube2.check_intersect(cube1), std::invalid_argument );
};
#endif

TEST_F(SopCubeTest, cube_cube_product1)
{
  auto lit0 = Literal{var0, false};
  auto lit1 = Literal{var5, true};
  auto lit2 = Literal{var7, true};
  auto cube1 = SopCube{nv, {lit1, lit2}};
  auto cube2 = SopCube{nv, {lit0, lit1}};
  auto cube3 = cube1 & cube2;

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

TEST_F(SopCubeTest, cube_cube_product2)
{
  auto lit0 = Literal{var0, false};
  auto lit1 = Literal{var5, true};
  auto lit2 = Literal{var7, true};
  auto lit3 = Literal{var5, false};
  auto cube1 = SopCube{nv, {lit1, lit2}};
  auto cube2 = SopCube{nv, {lit0, lit3}};
  auto cube3 = cube1 & cube2;

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

TEST_F(SopCubeTest, cube_cube_product_bad)
{
  auto lit0 = Literal{var0, false};
  auto lit1 = Literal{var5, true};
  auto lit2 = Literal{var7, true};

  auto cube1 = SopCube{nv, {lit1, lit2}};
  auto cube2 = SopCube{nv + 1, {lit0, lit1}};

  ASSERT_THROW( cube1 & cube2, std::invalid_argument );
}

TEST_F(SopCubeTest, Rcube_cube_product1)
{
  auto lit0 = Literal{var0, false};
  auto lit1 = Literal{var5, true};
  auto lit2 = Literal{var7, true};
  auto cube1 = SopCube{nv, {lit1, lit2}};
  auto cube2 = SopCube{nv, {lit0, lit1}};
  auto cube3 = std::move(cube1) & cube2;

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

TEST_F(SopCubeTest, cube_Rcube_product1)
{
  auto lit0 = Literal{var0, false};
  auto lit1 = Literal{var5, true};
  auto lit2 = Literal{var7, true};
  auto cube1 = SopCube{nv, {lit1, lit2}};
  auto cube2 = SopCube{nv, {lit0, lit1}};
  auto cube3 = cube1 & std::move(cube2);

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

TEST_F(SopCubeTest, Rcube_Rcube_product1)
{
  auto lit0 = Literal{var0, false};
  auto lit1 = Literal{var5, true};
  auto lit2 = Literal{var7, true};
  auto cube1 = SopCube{nv, {lit1, lit2}};
  auto cube2 = SopCube{nv, {lit0, lit1}};
  auto cube3 = std::move(cube1) & std::move(cube2);

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

TEST_F(SopCubeTest, lit_cube_product1)
{
  auto lit0 = Literal{var0, false};
  auto lit1 = Literal{var5, true};
  auto lit2 = Literal{var7, true};
  auto cube1 = SopCube{nv, {lit1, lit2}};
  auto cube3 = lit0 & cube1;

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

TEST_F(SopCubeTest, lit_Rcube_product1)
{
  auto lit0 = Literal{var0, false};
  auto lit1 = Literal{var5, true};
  auto lit2 = Literal{var7, true};
  auto cube1 = SopCube{nv, {lit1, lit2}};
  auto cube3 = lit0 & std::move(cube1);

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

TEST_F(SopCubeTest, lit_cube_product2)
{
  auto lit0 = Literal{var5, false};
  auto lit1 = Literal{var5, true};
  auto lit2 = Literal{var7, true};
  auto cube1 = SopCube{nv, {lit1, lit2}};
  auto cube3 = lit0 & cube1;

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

TEST_F(SopCubeTest, cube_lit_product1)
{
  auto lit0 = Literal{var0, false};
  auto lit1 = Literal{var5, true};
  auto lit2 = Literal{var7, true};
  auto cube1 = SopCube{nv, {lit1, lit2}};
  auto cube3 = cube1 & lit0;

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

TEST_F(SopCubeTest, Rcube_lit_product1)
{
  auto lit0 = Literal{var0, false};
  auto lit1 = Literal{var5, true};
  auto lit2 = Literal{var7, true};
  auto cube1 = SopCube{nv, {lit1, lit2}};
  auto cube3 = std::move(cube1) & lit0;

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

TEST_F(SopCubeTest, cube_lit_product2)
{
  auto lit0 = Literal{var5, false};
  auto lit1 = Literal{var5, true};
  auto lit2 = Literal{var7, true};
  auto cube1 = SopCube{nv, {lit1, lit2}};
  auto cube3 = cube1 & lit0;

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

TEST_F(SopCubeTest, Scube_cube_product1)
{
  auto lit0 = Literal{var0, false};
  auto lit1 = Literal{var5, true};
  auto lit2 = Literal{var7, true};
  auto cube1 = SopCube{nv, {lit1, lit2}};
  auto cube2 = SopCube{nv, {lit0, lit1}};
  cube1 = cube1 & cube2;

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

TEST_F(SopCubeTest, Scube_cube_product2)
{
  auto lit0 = Literal{var0, false};
  auto lit1 = Literal{var5, true};
  auto lit2 = Literal{var7, true};
  auto lit3 = Literal{var5, false};
  auto cube1 = SopCube{nv, {lit1, lit2}};
  auto cube2 = SopCube{nv, {lit0, lit3}};
  cube1 = cube1 & cube2;

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

TEST_F(SopCubeTest, Scube_literal_product1)
{
  auto lit0 = Literal{var0, false};
  auto lit1 = Literal{var5, true};
  auto lit2 = Literal{var7, true};
  auto cube1 = SopCube{nv, {lit1, lit2}};
  cube1 = cube1 & lit0;

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

TEST_F(SopCubeTest, Scube_literal_product2)
{
  auto lit0 = Literal{var5, false};
  auto lit1 = Literal{var5, true};
  auto lit2 = Literal{var7, true};
  auto cube1 = SopCube{nv, {lit1, lit2}};
  cube1 = cube1 & lit0;

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

TEST_F(SopCubeTest, literal_Scube_product1)
{
  auto lit0 = Literal{var0, false};
  auto lit1 = Literal{var5, true};
  auto lit2 = Literal{var7, true};
  auto cube1 = SopCube{nv, {lit1, lit2}};
  cube1 = lit0 & cube1;

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

TEST_F(SopCubeTest, literal_Scube_product2)
{
  auto lit0 = Literal{var5, false};
  auto lit1 = Literal{var5, true};
  auto lit2 = Literal{var7, true};
  auto cube1 = SopCube{nv, {lit1, lit2}};

  cube1 = lit0 & cube1;

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

TEST_F(SopCubeTest, Icube_cube_product1)
{
  auto lit0 = Literal{var0, false};
  auto lit1 = Literal{var5, true};
  auto lit2 = Literal{var7, true};
  auto cube1 = SopCube{nv, {lit1, lit2}};
  auto cube2 = SopCube{nv, {lit0, lit1}};

  cube1 &= cube2;

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

TEST_F(SopCubeTest, Icube_cube_product2)
{
  auto lit0 = Literal{var0, false};
  auto lit1 = Literal{var5, true};
  auto lit2 = Literal{var7, true};
  auto lit3 = Literal{var5, false};
  auto cube1 = SopCube{nv, {lit1, lit2}};
  auto cube2 = SopCube{nv, {lit0, lit3}};

  cube1 &= cube2;

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

TEST_F(SopCubeTest, Icube_literal_product1)
{
  auto lit0 = Literal{var0, false};
  auto lit1 = Literal{var5, true};
  auto lit2 = Literal{var7, true};
  auto cube1 = SopCube{nv, {lit1, lit2}};

  cube1 &= lit0;

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

TEST_F(SopCubeTest, Icube_literal_product2)
{
  auto lit0 = Literal{var5, false};
  auto lit1 = Literal{var5, true};
  auto lit2 = Literal{var7, true};
  auto cube1 = SopCube{nv, {lit1, lit2}};

  cube1 &= lit0;

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

TEST_F(SopCubeTest, cube_cube_cofactor1)
{
  auto lit0 = Literal{var0, false};
  auto lit1 = Literal{var5, true};
  auto lit2 = Literal{var7, true};
  auto cube1 = SopCube{nv, {lit0, lit1, lit2}};
  auto cube2 = SopCube{nv, {lit0, lit2}};

  auto cube3 = cube1.cofactor(cube2);

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

TEST_F(SopCubeTest, cube_cube_cofactor2)
{
  auto lit0 = Literal{var0, false};
  auto lit1 = Literal{var5, true};
  auto lit2 = Literal{var7, true};
  auto cube1 = SopCube{nv, {lit0, lit1}};
  auto cube2 = SopCube{nv, {lit2}};

  auto cube3 = cube1.cofactor(cube2);

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

TEST_F(SopCubeTest, cube_cube_cofactor_bad)
{
  auto lit0 = Literal{var0, false};
  auto lit1 = Literal{var5, true};
  auto lit2 = Literal{var7, true};
  auto cube1 = SopCube{nv, {lit0, lit1, lit2}};
  auto cube2 = SopCube{nv + 1, {lit0, lit2}};

  ASSERT_THROW( cube1.cofactor(cube2), std::invalid_argument );
}

TEST_F(SopCubeTest, cube_lit_cofactor1)
{
  auto lit0 = Literal{var0, false};
  auto lit1 = Literal{var5, true};
  auto lit2 = Literal{var7, true};
  auto cube1 = SopCube{nv, {lit0, lit1, lit2}};

  auto cube3 = cube1.cofactor(lit0);

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

TEST_F(SopCubeTest, cube_lit_cofactor2)
{
  auto lit0 = Literal{var5, false};
  auto lit1 = Literal{var5, true};
  auto lit2 = Literal{var7, true};
  auto cube1 = SopCube{nv, {lit0, lit1, lit2}};

  auto cube3 = cube1.cofactor(lit0);

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

TEST_F(SopCubeTest, Scube_cube_cofactor1)
{
  auto lit0 = Literal{var0, false};
  auto lit1 = Literal{var5, true};
  auto lit2 = Literal{var7, true};
  auto cube1 = SopCube{nv, {lit0, lit1, lit2}};
  auto cube2 = SopCube{nv, {lit0, lit2}};

  cube1 = cube1.cofactor(cube2);

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

TEST_F(SopCubeTest, Scube_cube_cofactor2)
{
  auto lit0 = Literal{var0, false};
  auto lit1 = Literal{var5, true};
  auto lit2 = Literal{var7, true};
  auto cube1 = SopCube{nv, {lit0, lit1}};
  auto cube2 = SopCube{nv, {lit2}};

  cube1 = cube1.cofactor(cube2);

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

TEST_F(SopCubeTest, Scube_literal_cofactor1)
{
  auto lit0 = Literal{var0, false};
  auto lit1 = Literal{var5, true};
  auto lit2 = Literal{var7, true};
  auto cube1 = SopCube{nv, {lit0, lit1, lit2}};

  cube1 = cube1.cofactor(lit0);

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

TEST_F(SopCubeTest, Scube_literal_cofactor2)
{
  auto lit0 = Literal{var5, false};
  auto lit1 = Literal{var5, true};
  auto lit2 = Literal{var7, true};
  auto cube1 = SopCube{nv, {lit0, lit1, lit2}};

  cube1 = cube1.cofactor(lit0);

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

TEST_F(SopCubeTest, Icube_cube_cofactor1)
{
  auto lit0 = Literal{var0, false};
  auto lit1 = Literal{var5, true};
  auto lit2 = Literal{var7, true};
  auto cube1 = SopCube{nv, {lit0, lit1, lit2}};
  auto cube2 = SopCube{nv, {lit0, lit2}};

  cube1.cofactor_int(cube2);

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

TEST_F(SopCubeTest, Icube_cube_cofactor2)
{
  auto lit0 = Literal{var0, false};
  auto lit1 = Literal{var5, true};
  auto lit2 = Literal{var7, true};
  auto cube1 = SopCube{nv, {lit0, lit1}};
  auto cube2 = SopCube{nv, {lit2}};

  cube1.cofactor_int(cube2);

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

TEST_F(SopCubeTest, Icube_literal_cofactor1)
{
  auto lit0 = Literal{var0, false};
  auto lit1 = Literal{var5, true};
  auto lit2 = Literal{var7, true};
  auto cube1 = SopCube{nv, {lit0, lit1, lit2}};

  cube1.cofactor_int(lit0);

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

TEST_F(SopCubeTest, Icube_literal_cofactor2)
{
  auto lit0 = Literal{var5, false};
  auto lit1 = Literal{var5, true};
  auto lit2 = Literal{var7, true};
  auto cube1 = SopCube{nv, {lit0, lit1, lit2}};

  cube1.cofactor_int(lit0);

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

TEST_F(SopCubeTest, compare1)
{
  const int nv1 = 100;
  SizeType var80{80};

  auto lit0 = Literal{var0, false};
  auto lit1 = Literal{var80, false};

  auto cube1 = SopCube{nv1, { lit0,  lit1}};
  auto cube2 = SopCube{nv1, { lit0, ~lit1}};
  auto cube3 = SopCube{nv1, {~lit0,  lit1}};

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

TEST_F(SopCubeTest, compare2)
{
  const int nv1 = 100;
  SizeType var80{80};

  auto lit0 = Literal{var0, false};
  auto lit1 = Literal{var80, false};

  auto cube1 = SopCube{nv1, { lit0,  lit1}};
  auto cube2 = SopCube{nv1, { lit0, ~lit1}};
  auto cube3 = SopCube{nv1, {~lit0,  lit1}};

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

TEST_F(SopCubeTest, compare_bad)
{
  const int nv1 = 100;
  SizeType var80{80};

  auto lit0 = Literal{var0, false};
  auto lit1 = Literal{var80, false};

  auto cube1 = SopCube{nv1, { lit0,  lit1}};
  auto cube2 = SopCube{nv, { lit0, ~lit1}};

  ASSERT_THROW( auto r = (cube1 == cube2), std::invalid_argument );
  ASSERT_THROW( auto r = (cube1 < cube2), std::invalid_argument );
  ASSERT_THROW( auto r = (cube1 > cube2), std::invalid_argument );
  ASSERT_THROW( auto r = (cube1 <= cube2), std::invalid_argument );
  ASSERT_THROW( auto r = (cube1 >= cube2), std::invalid_argument );
}

TEST_F(SopCubeTest, to_expr1)
{
  auto lit0 = Literal{var0, false};
  auto lit1 = Literal{var1, false};
  auto lit2 = Literal{var2, false};

  auto cube1 = SopCube{3, {lit0, lit1, ~lit2}};

  auto expr = cube1.expr();
  auto expr_str = expr.to_string();
  EXPECT_EQ( "( 0 & 1 & ~2 )", expr_str );
}

END_NAMESPACE_YM
