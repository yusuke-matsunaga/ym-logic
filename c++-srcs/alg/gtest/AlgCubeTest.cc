
/// @file AlgCubeTest.cc
/// @brief AlgCubeTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/AlgCube.h"
#include "ym/Literal.h"


BEGIN_NAMESPACE_YM

class AlgCubeTest :
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

TEST_F(AlgCubeTest, constructor1)
{
  AlgCube cube1{nv};

  EXPECT_EQ( nv, cube1.variable_num() );
  EXPECT_EQ( 0,  cube1.literal_num() );

  auto lit_list = cube1.literal_list();
  EXPECT_TRUE( lit_list.empty() );
};

TEST_F(AlgCubeTest, constructor2)
{
  Literal lit{var5, true};
  AlgCube cube1{nv, lit};

  EXPECT_EQ( nv, cube1.variable_num() );
  EXPECT_EQ( 1,  cube1.literal_num() );

  auto lit_list = cube1.literal_list();
  EXPECT_EQ( 1, lit_list.size() );
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

TEST_F(AlgCubeTest, constructor3)
{
  Literal lit1{var5, true};
  Literal lit2{var0, false};
  vector<Literal> lit_list{lit1, lit2};
  AlgCube cube1{nv, lit_list};

  auto nl = lit_list.size();
  EXPECT_EQ( nv, cube1.variable_num() );
  EXPECT_EQ( nl,  cube1.literal_num() );

  auto lit_list1 = cube1.literal_list();
  EXPECT_EQ( nl, lit_list1.size() );
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

TEST_F(AlgCubeTest, constructor4)
{
  Literal lit1{var5, true};
  Literal lit2{var0, false};
  std::initializer_list<Literal> lit_list{lit1, lit2};
  AlgCube cube1{nv, lit_list};

  auto nl = lit_list.size();
  EXPECT_EQ( nv, cube1.variable_num() );
  EXPECT_EQ( nl,  cube1.literal_num() );

  auto lit_list1 = cube1.literal_list();
  EXPECT_EQ( nl, lit_list1.size() );
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

TEST_F(AlgCubeTest, copy_constructor)
{
  Literal lit1{var5, true};
  Literal lit2{var0, false};
  std::initializer_list<Literal> lit_list{lit1, lit2};
  auto nl = lit_list.size();
  AlgCube cube1{nv, lit_list};

  AlgCube cube2{cube1};
  EXPECT_EQ( nv, cube2.variable_num() );
  EXPECT_EQ( nl,  cube2.literal_num() );

  auto lit_list1 = cube2.literal_list();
  EXPECT_EQ( nl, lit_list1.size() );
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

TEST_F(AlgCubeTest, copy_assignment)
{
  Literal lit1{var5, true};
  Literal lit2{var0, false};
  std::initializer_list<Literal> lit_list{lit1, lit2};
  auto nl = lit_list.size();
  AlgCube cube1{nv, lit_list};

  AlgCube cube2{nv};
  EXPECT_EQ( 0, cube2.literal_num() );

  cube2 = cube1;
  EXPECT_EQ( nv, cube2.variable_num() );
  EXPECT_EQ( nl, cube2.literal_num() );

  auto lit_list1 = cube2.literal_list();
  EXPECT_EQ( nl, lit_list1.size() );
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

TEST_F(AlgCubeTest, move_constructor)
{
  Literal lit1{var5, true};
  Literal lit2{var0, false};
  std::initializer_list<Literal> lit_list{lit1, lit2};
  auto nl = lit_list.size();
  AlgCube cube1{nv, lit_list};

  AlgCube cube2{std::move(cube1)};

  EXPECT_EQ( 0, cube1.literal_num() );

  EXPECT_EQ( nv, cube2.variable_num() );
  EXPECT_EQ( nl, cube2.literal_num() );

  auto lit_list1 = cube2.literal_list();
  EXPECT_EQ( nl, lit_list1.size() );
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

TEST_F(AlgCubeTest, move_assignment)
{
  Literal lit1{var5, true};
  Literal lit2{var0, false};
  std::initializer_list<Literal> lit_list{lit1, lit2};
  auto nl = lit_list.size();
  AlgCube cube1{nv, lit_list};

  AlgCube cube2{nv};
  EXPECT_EQ( 0, cube2.literal_num() );

  cube2 = std::move(cube1);

  EXPECT_EQ( 0, cube1.literal_num() );

  EXPECT_EQ( nv, cube2.variable_num() );
  EXPECT_EQ( nl,  cube2.literal_num() );

  auto lit_list1 = cube2.literal_list();
  EXPECT_EQ( nl, lit_list1.size() );
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

TEST_F(AlgCubeTest, check_containment)
{
  Literal lit1{var5, true};
  Literal lit2{var0, false};
  std::initializer_list<Literal> lit_list{lit1, lit2};
  AlgCube cube1{nv, lit_list};

  AlgCube cube2{nv, {lit1}};

  EXPECT_TRUE(  cube1.check_containment(cube2) );
  EXPECT_FALSE( cube2.check_containment(cube1) );

  AlgCube cube3{nv, {lit2}};
  EXPECT_FALSE( cube2.check_containment(cube3) );
  EXPECT_FALSE( cube3.check_containment(cube2) );
};

TEST_F(AlgCubeTest, check_containment_bad)
{
  Literal lit1{var5, true};
  Literal lit2{var0, false};
  std::initializer_list<Literal> lit_list{lit1, lit2};
  AlgCube cube1{nv, lit_list};

  AlgCube cube2{nv + 1, {lit1}};

  ASSERT_THROW( cube1.check_containment(cube2), std::invalid_argument );
  ASSERT_THROW( cube2.check_containment(cube1), std::invalid_argument );
};

TEST_F(AlgCubeTest, check_intersect)
{
  Literal lit0{var0, false};
  Literal lit1{var5, true};
  Literal lit2{var7, true};
  AlgCube cube1{nv, {lit1, lit2}};

  AlgCube cube2{nv, {lit0, lit1}};

  EXPECT_TRUE(  cube1.check_intersect(cube2) );
  EXPECT_TRUE(  cube2.check_intersect(cube1) );

  Literal lit3{var0, true};
  AlgCube cube3{nv, {lit3}};
  EXPECT_FALSE( cube1.check_containment(cube3) );
  EXPECT_FALSE( cube2.check_containment(cube3) );
  EXPECT_FALSE( cube3.check_containment(cube1) );
  EXPECT_FALSE( cube3.check_containment(cube2) );
};

TEST_F(AlgCubeTest, check_intersect_bad)
{
  Literal lit0{var0, false};
  Literal lit1{var5, true};
  Literal lit2{var7, true};
  AlgCube cube1{nv, {lit1, lit2}};

  AlgCube cube2{nv + 1, {lit0, lit1}};

  ASSERT_THROW( cube1.check_intersect(cube2), std::invalid_argument );
  ASSERT_THROW( cube2.check_intersect(cube1), std::invalid_argument );
};

TEST_F(AlgCubeTest, cube_cube_product1)
{
  Literal lit0{var0, false};
  Literal lit1{var5, true};
  Literal lit2{var7, true};
  AlgCube cube1{nv, {lit1, lit2}};
  AlgCube cube2{nv, {lit0, lit1}};

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

TEST_F(AlgCubeTest, cube_cube_product2)
{
  Literal lit0{var0, false};
  Literal lit1{var5, true};
  Literal lit2{var7, true};
  Literal lit3{var5, false};
  AlgCube cube1{nv, {lit1, lit2}};
  AlgCube cube2{nv, {lit0, lit3}};

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

TEST_F(AlgCubeTest, cube_cube_product_bad)
{
  Literal lit0{var0, false};
  Literal lit1{var5, true};
  Literal lit2{var7, true};

  AlgCube cube1{nv, {lit1, lit2}};
  AlgCube cube2{nv + 1, {lit0, lit1}};

  ASSERT_THROW( cube1 * cube2, std::invalid_argument );
}

TEST_F(AlgCubeTest, Rcube_cube_product1)
{
  Literal lit0{var0, false};
  Literal lit1{var5, true};
  Literal lit2{var7, true};
  AlgCube cube1{nv, {lit1, lit2}};
  AlgCube cube2{nv, {lit0, lit1}};

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

TEST_F(AlgCubeTest, cube_Rcube_product1)
{
  Literal lit0{var0, false};
  Literal lit1{var5, true};
  Literal lit2{var7, true};
  AlgCube cube1{nv, {lit1, lit2}};
  AlgCube cube2{nv, {lit0, lit1}};

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

TEST_F(AlgCubeTest, Rcube_Rcube_product1)
{
  Literal lit0{var0, false};
  Literal lit1{var5, true};
  Literal lit2{var7, true};
  AlgCube cube1{nv, {lit1, lit2}};
  AlgCube cube2{nv, {lit0, lit1}};

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

TEST_F(AlgCubeTest, lit_cube_product1)
{
  Literal lit0{var0, false};
  Literal lit1{var5, true};
  Literal lit2{var7, true};
  AlgCube cube1{nv, {lit1, lit2}};

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

TEST_F(AlgCubeTest, lit_Rcube_product1)
{
  Literal lit0{var0, false};
  Literal lit1{var5, true};
  Literal lit2{var7, true};
  AlgCube cube1{nv, {lit1, lit2}};

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

TEST_F(AlgCubeTest, lit_cube_product2)
{
  Literal lit0{var5, false};
  Literal lit1{var5, true};
  Literal lit2{var7, true};
  AlgCube cube1{nv, {lit1, lit2}};

  auto cube3 = lit0 * cube1;

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

TEST_F(AlgCubeTest, cube_lit_product1)
{
  Literal lit0{var0, false};
  Literal lit1{var5, true};
  Literal lit2{var7, true};
  AlgCube cube1{nv, {lit1, lit2}};

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

TEST_F(AlgCubeTest, Rcube_lit_product1)
{
  Literal lit0{var0, false};
  Literal lit1{var5, true};
  Literal lit2{var7, true};
  AlgCube cube1{nv, {lit1, lit2}};

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

TEST_F(AlgCubeTest, cube_lit_product2)
{
  Literal lit0{var5, false};
  Literal lit1{var5, true};
  Literal lit2{var7, true};
  AlgCube cube1{nv, {lit1, lit2}};

  auto cube3 = cube1 * lit0;

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

TEST_F(AlgCubeTest, Scube_cube_product1)
{
  Literal lit0{var0, false};
  Literal lit1{var5, true};
  Literal lit2{var7, true};
  AlgCube cube1{nv, {lit1, lit2}};
  AlgCube cube2{nv, {lit0, lit1}};

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

TEST_F(AlgCubeTest, Scube_cube_product2)
{
  Literal lit0{var0, false};
  Literal lit1{var5, true};
  Literal lit2{var7, true};
  Literal lit3{var5, false};
  AlgCube cube1{nv, {lit1, lit2}};
  AlgCube cube2{nv, {lit0, lit3}};

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

TEST_F(AlgCubeTest, Scube_literal_product1)
{
  Literal lit0{var0, false};
  Literal lit1{var5, true};
  Literal lit2{var7, true};
  AlgCube cube1{nv, {lit1, lit2}};

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

TEST_F(AlgCubeTest, Scube_literal_product2)
{
  Literal lit0{var5, false};
  Literal lit1{var5, true};
  Literal lit2{var7, true};
  AlgCube cube1{nv, {lit1, lit2}};

  cube1 = cube1 * lit0;

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

TEST_F(AlgCubeTest, literal_Scube_product1)
{
  Literal lit0{var0, false};
  Literal lit1{var5, true};
  Literal lit2{var7, true};
  AlgCube cube1{nv, {lit1, lit2}};

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

TEST_F(AlgCubeTest, literal_Scube_product2)
{
  Literal lit0{var5, false};
  Literal lit1{var5, true};
  Literal lit2{var7, true};
  AlgCube cube1{nv, {lit1, lit2}};

  cube1 = lit0 * cube1;

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

TEST_F(AlgCubeTest, Icube_cube_product1)
{
  Literal lit0{var0, false};
  Literal lit1{var5, true};
  Literal lit2{var7, true};
  AlgCube cube1{nv, {lit1, lit2}};
  AlgCube cube2{nv, {lit0, lit1}};

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

TEST_F(AlgCubeTest, Icube_cube_product2)
{
  Literal lit0{var0, false};
  Literal lit1{var5, true};
  Literal lit2{var7, true};
  Literal lit3{var5, false};
  AlgCube cube1{nv, {lit1, lit2}};
  AlgCube cube2{nv, {lit0, lit3}};

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

TEST_F(AlgCubeTest, Icube_literal_product1)
{
  Literal lit0{var0, false};
  Literal lit1{var5, true};
  Literal lit2{var7, true};
  AlgCube cube1{nv, {lit1, lit2}};

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

TEST_F(AlgCubeTest, Icube_literal_product2)
{
  Literal lit0{var5, false};
  Literal lit1{var5, true};
  Literal lit2{var7, true};
  AlgCube cube1{nv, {lit1, lit2}};

  cube1 *= lit0;

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

TEST_F(AlgCubeTest, cube_cube_quotient1)
{
  Literal lit0{var0, false};
  Literal lit1{var5, true};
  Literal lit2{var7, true};
  AlgCube cube1{nv, {lit0, lit1, lit2}};
  AlgCube cube2{nv, {lit0, lit2}};

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

TEST_F(AlgCubeTest, Rcube_cube_quotient1)
{
  Literal lit0{var0, false};
  Literal lit1{var5, true};
  Literal lit2{var7, true};
  AlgCube cube1{nv, {lit0, lit1, lit2}};
  AlgCube cube2{nv, {lit0, lit2}};

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

TEST_F(AlgCubeTest, cube_cube_quotient2)
{
  Literal lit0{var0, false};
  Literal lit1{var5, true};
  Literal lit2{var7, true};
  AlgCube cube1{nv, {lit0, lit1}};
  AlgCube cube2{nv, {lit2}};

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

TEST_F(AlgCubeTest, cube_cube_quotient_bad)
{
  Literal lit0{var0, false};
  Literal lit1{var5, true};
  Literal lit2{var7, true};
  AlgCube cube1{nv, {lit0, lit1, lit2}};
  AlgCube cube2{nv + 1, {lit0, lit2}};

  ASSERT_THROW( cube1 / cube2, std::invalid_argument );
}

TEST_F(AlgCubeTest, cube_lit_quotient1)
{
  Literal lit0{var0, false};
  Literal lit1{var5, true};
  Literal lit2{var7, true};
  AlgCube cube1{nv, {lit0, lit1, lit2}};

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

TEST_F(AlgCubeTest, Rcube_lit_quotient1)
{
  Literal lit0{var0, false};
  Literal lit1{var5, true};
  Literal lit2{var7, true};
  AlgCube cube1{nv, {lit0, lit1, lit2}};

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

TEST_F(AlgCubeTest, cube_lit_quotient2)
{
  Literal lit0{var5, false};
  Literal lit1{var5, true};
  Literal lit2{var7, true};
  AlgCube cube1{nv, {lit0, lit1, lit2}};

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

TEST_F(AlgCubeTest, Scube_cube_quotient1)
{
  Literal lit0{var0, false};
  Literal lit1{var5, true};
  Literal lit2{var7, true};
  AlgCube cube1{nv, {lit0, lit1, lit2}};
  AlgCube cube2{nv, {lit0, lit2}};

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

TEST_F(AlgCubeTest, Scube_cube_quotient2)
{
  Literal lit0{var0, false};
  Literal lit1{var5, true};
  Literal lit2{var7, true};
  AlgCube cube1{nv, {lit0, lit1}};
  AlgCube cube2{nv, {lit2}};

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

TEST_F(AlgCubeTest, Scube_literal_quotient1)
{
  Literal lit0{var0, false};
  Literal lit1{var5, true};
  Literal lit2{var7, true};
  AlgCube cube1{nv, {lit0, lit1, lit2}};

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

TEST_F(AlgCubeTest, Scube_literal_quotient2)
{
  Literal lit0{var5, false};
  Literal lit1{var5, true};
  Literal lit2{var7, true};
  AlgCube cube1{nv, {lit0, lit1, lit2}};

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

TEST_F(AlgCubeTest, Icube_cube_quotient1)
{
  Literal lit0{var0, false};
  Literal lit1{var5, true};
  Literal lit2{var7, true};
  AlgCube cube1{nv, {lit0, lit1, lit2}};
  AlgCube cube2{nv, {lit0, lit2}};

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

TEST_F(AlgCubeTest, Icube_cube_quotient2)
{
  Literal lit0{var0, false};
  Literal lit1{var5, true};
  Literal lit2{var7, true};
  AlgCube cube1{nv, {lit0, lit1}};
  AlgCube cube2{nv, {lit2}};

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

TEST_F(AlgCubeTest, Icube_literal_quotient1)
{
  Literal lit0{var0, false};
  Literal lit1{var5, true};
  Literal lit2{var7, true};
  AlgCube cube1{nv, {lit0, lit1, lit2}};

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

TEST_F(AlgCubeTest, Icube_literal_quotient2)
{
  Literal lit0{var5, false};
  Literal lit1{var5, true};
  Literal lit2{var7, true};
  AlgCube cube1{nv, {lit0, lit1, lit2}};

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

TEST_F(AlgCubeTest, compare1)
{
  const int nv1 = 100;
  SizeType var80{80};

  Literal lit0{var0, false};
  Literal lit1{var80, false};

  AlgCube cube1{nv1, { lit0,  lit1}};
  AlgCube cube2{nv1, { lit0, ~lit1}};
  AlgCube cube3{nv1, {~lit0,  lit1}};

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

TEST_F(AlgCubeTest, compare2)
{
  const int nv1 = 100;
  SizeType var80{80};

  Literal lit0{var0, false};
  Literal lit1{var80, false};

  AlgCube cube1{nv1, { lit0,  lit1}};
  AlgCube cube2{nv1, { lit0, ~lit1}};
  AlgCube cube3{nv1, {~lit0,  lit1}};

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

TEST_F(AlgCubeTest, compare_bad)
{
  const int nv1 = 100;
  SizeType var80{80};

  Literal lit0{var0, false};
  Literal lit1{var80, false};

  AlgCube cube1{nv1, { lit0,  lit1}};
  AlgCube cube2{nv, { lit0, ~lit1}};

  ASSERT_THROW( auto r = (cube1 == cube2), std::invalid_argument );
  ASSERT_THROW( auto r = (cube1 < cube2), std::invalid_argument );
  ASSERT_THROW( auto r = (cube1 > cube2), std::invalid_argument );
  ASSERT_THROW( auto r = (cube1 <= cube2), std::invalid_argument );
  ASSERT_THROW( auto r = (cube1 >= cube2), std::invalid_argument );
}

TEST_F(AlgCubeTest, to_expr1)
{
  Literal lit0{var0, false};
  Literal lit1{var1, false};
  Literal lit2{var2, false};

  AlgCube cube1{3, {lit0, lit1, ~lit2}};

  auto expr = cube1.expr();
  auto expr_str = expr.to_string();
  EXPECT_EQ( "( 0 & 1 & ~2 )", expr_str );
}

END_NAMESPACE_YM
