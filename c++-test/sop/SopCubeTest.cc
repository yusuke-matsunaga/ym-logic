
/// @file SopCubeTest.cc
/// @brief SopCubeTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/SopCube.h"
#include "ym/VarId.h"
#include "ym/Literal.h"


BEGIN_NAMESPACE_YM

TEST(SopCubeTest, constructor1)
{
  const int nv = 10;

  SopCube cube1(nv);

  EXPECT_EQ( nv, cube1.variable_num() );
  EXPECT_EQ( 0,  cube1.literal_num() );

  auto lit_list = cube1.to_literal_list();
  EXPECT_TRUE( lit_list.empty() );
};

TEST(SopCubeTest, constructor2)
{
  const int nv = 10;

  VarId var0(0);
  VarId var1(1);
  VarId var2(2);
  VarId var3(3);
  VarId var4(4);
  VarId var5(5);
  VarId var6(6);
  VarId var7(7);
  VarId var8(8);
  VarId var9(9);

  Literal lit(var5, true);
  SopCube cube1(nv, lit);

  EXPECT_EQ( nv, cube1.variable_num() );
  EXPECT_EQ( 1,  cube1.literal_num() );

  auto lit_list = cube1.to_literal_list();
  EXPECT_EQ( 1, lit_list.size() );
  EXPECT_EQ( lit, lit_list[0] );

  EXPECT_FALSE( cube1.has_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var5, false)) );
  EXPECT_TRUE(  cube1.has_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var7, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var9, true)) );

};

TEST(SopCubeTest, constructor3)
{
  const int nv = 10;

  VarId var0(0);
  VarId var1(1);
  VarId var2(2);
  VarId var3(3);
  VarId var4(4);
  VarId var5(5);
  VarId var6(6);
  VarId var7(7);
  VarId var8(8);
  VarId var9(9);

  Literal lit1(var5, true);
  Literal lit2(var0, false);
  vector<Literal> lit_list{lit1, lit2};
  SopCube cube1(nv, lit_list);

  int nl = lit_list.size();
  EXPECT_EQ( nv, cube1.variable_num() );
  EXPECT_EQ( nl,  cube1.literal_num() );

  auto lit_list1 = cube1.to_literal_list();
  EXPECT_EQ( nl, lit_list1.size() );
  EXPECT_EQ( lit2, lit_list1[0] );
  EXPECT_EQ( lit1, lit_list1[1] );

  EXPECT_TRUE(  cube1.has_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var5, false)) );
  EXPECT_TRUE(  cube1.has_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var7, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var9, true)) );

};

TEST(SopCubeTest, constructor4)
{
  const int nv = 10;

  VarId var0(0);
  VarId var1(1);
  VarId var2(2);
  VarId var3(3);
  VarId var4(4);
  VarId var5(5);
  VarId var6(6);
  VarId var7(7);
  VarId var8(8);
  VarId var9(9);

  Literal lit1(var5, true);
  Literal lit2(var0, false);
  std::initializer_list<Literal> lit_list{lit1, lit2};
  SopCube cube1(nv, lit_list);

  int nl = lit_list.size();
  EXPECT_EQ( nv, cube1.variable_num() );
  EXPECT_EQ( nl,  cube1.literal_num() );

  auto lit_list1 = cube1.to_literal_list();
  EXPECT_EQ( nl, lit_list1.size() );
  EXPECT_EQ( lit2, lit_list1[0] );
  EXPECT_EQ( lit1, lit_list1[1] );

  EXPECT_TRUE(  cube1.has_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var5, false)) );
  EXPECT_TRUE(  cube1.has_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var7, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var9, true)) );

};

TEST(SopCubeTest, copy_constructor)
{
  const int nv = 10;

  VarId var0(0);
  VarId var1(1);
  VarId var2(2);
  VarId var3(3);
  VarId var4(4);
  VarId var5(5);
  VarId var6(6);
  VarId var7(7);
  VarId var8(8);
  VarId var9(9);

  Literal lit1(var5, true);
  Literal lit2(var0, false);
  std::initializer_list<Literal> lit_list{lit1, lit2};
  int nl = lit_list.size();
  SopCube cube1(nv, lit_list);

  SopCube cube2(cube1);
  EXPECT_EQ( nv, cube2.variable_num() );
  EXPECT_EQ( nl,  cube2.literal_num() );

  auto lit_list1 = cube2.to_literal_list();
  EXPECT_EQ( nl, lit_list1.size() );
  EXPECT_EQ( lit2, lit_list1[0] );
  EXPECT_EQ( lit1, lit_list1[1] );

  EXPECT_TRUE(  cube2.has_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var5, false)) );
  EXPECT_TRUE(  cube2.has_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var7, false)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var9, true)) );

};

TEST(SopCubeTest, copy_assignment)
{
  const int nv = 10;

  VarId var0(0);
  VarId var1(1);
  VarId var2(2);
  VarId var3(3);
  VarId var4(4);
  VarId var5(5);
  VarId var6(6);
  VarId var7(7);
  VarId var8(8);
  VarId var9(9);

  Literal lit1(var5, true);
  Literal lit2(var0, false);
  std::initializer_list<Literal> lit_list{lit1, lit2};
  int nl = lit_list.size();
  SopCube cube1(nv, lit_list);

  SopCube cube2(nv);
  EXPECT_EQ( 0, cube2.literal_num() );

  cube2 = cube1;
  EXPECT_EQ( nv, cube2.variable_num() );
  EXPECT_EQ( nl,  cube2.literal_num() );

  auto lit_list1 = cube2.to_literal_list();
  EXPECT_EQ( nl, lit_list1.size() );
  EXPECT_EQ( lit2, lit_list1[0] );
  EXPECT_EQ( lit1, lit_list1[1] );

  EXPECT_TRUE(  cube2.has_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var5, false)) );
  EXPECT_TRUE(  cube2.has_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var7, false)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var9, true)) );

};

TEST(SopCubeTest, move_constructor)
{
  const int nv = 10;

  VarId var0(0);
  VarId var1(1);
  VarId var2(2);
  VarId var3(3);
  VarId var4(4);
  VarId var5(5);
  VarId var6(6);
  VarId var7(7);
  VarId var8(8);
  VarId var9(9);

  Literal lit1(var5, true);
  Literal lit2(var0, false);
  std::initializer_list<Literal> lit_list{lit1, lit2};
  int nl = lit_list.size();
  SopCube cube1(nv, lit_list);

  SopCube cube2(std::move(cube1));

  EXPECT_EQ( 0, cube1.literal_num() );

  EXPECT_EQ( nv, cube2.variable_num() );
  EXPECT_EQ( nl, cube2.literal_num() );

  auto lit_list1 = cube2.to_literal_list();
  EXPECT_EQ( nl, lit_list1.size() );
  EXPECT_EQ( lit2, lit_list1[0] );
  EXPECT_EQ( lit1, lit_list1[1] );

  EXPECT_TRUE(  cube2.has_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var5, false)) );
  EXPECT_TRUE(  cube2.has_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var7, false)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var9, true)) );

};

TEST(SopCubeTest, move_assignment)
{
  const int nv = 10;

  VarId var0(0);
  VarId var1(1);
  VarId var2(2);
  VarId var3(3);
  VarId var4(4);
  VarId var5(5);
  VarId var6(6);
  VarId var7(7);
  VarId var8(8);
  VarId var9(9);

  Literal lit1(var5, true);
  Literal lit2(var0, false);
  std::initializer_list<Literal> lit_list{lit1, lit2};
  int nl = lit_list.size();
  SopCube cube1(nv, lit_list);

  SopCube cube2(nv);
  EXPECT_EQ( 0, cube2.literal_num() );

  cube2 = std::move(cube1);

  EXPECT_EQ( 0, cube1.literal_num() );

  EXPECT_EQ( nv, cube2.variable_num() );
  EXPECT_EQ( nl,  cube2.literal_num() );

  auto lit_list1 = cube2.to_literal_list();
  EXPECT_EQ( nl, lit_list1.size() );
  EXPECT_EQ( lit2, lit_list1[0] );
  EXPECT_EQ( lit1, lit_list1[1] );

  EXPECT_TRUE(  cube2.has_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var5, false)) );
  EXPECT_TRUE(  cube2.has_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var7, false)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube2.has_literal(Literal(var9, true)) );

};

TEST(SopCubeTest, check_containment)
{
  const int nv = 10;

  VarId var0(0);
  VarId var1(1);
  VarId var2(2);
  VarId var3(3);
  VarId var4(4);
  VarId var5(5);
  VarId var6(6);
  VarId var7(7);
  VarId var8(8);
  VarId var9(9);

  Literal lit1(var5, true);
  Literal lit2(var0, false);
  std::initializer_list<Literal> lit_list{lit1, lit2};
  SopCube cube1(nv, lit_list);

  SopCube cube2(nv, {lit1});

  EXPECT_TRUE(  cube1.check_containment(cube2) );
  EXPECT_FALSE( cube2.check_containment(cube1) );

  SopCube cube3(nv, {lit2});
  EXPECT_FALSE( cube2.check_containment(cube3) );
  EXPECT_FALSE( cube3.check_containment(cube2) );
};

TEST(SopCubeTest, check_intersect)
{
  const int nv = 10;

  VarId var0(0);
  VarId var1(1);
  VarId var2(2);
  VarId var3(3);
  VarId var4(4);
  VarId var5(5);
  VarId var6(6);
  VarId var7(7);
  VarId var8(8);
  VarId var9(9);

  Literal lit0(var0, false);
  Literal lit1(var5, true);
  Literal lit2(var7, true);
  SopCube cube1(nv, {lit1, lit2});

  SopCube cube2(nv, {lit0, lit1});

  EXPECT_TRUE(  cube1.check_intersect(cube2) );
  EXPECT_TRUE(  cube2.check_intersect(cube1) );

  Literal lit3(var0, true);
  SopCube cube3(nv, {lit3});
  EXPECT_FALSE( cube1.check_containment(cube3) );
  EXPECT_FALSE( cube2.check_containment(cube3) );
  EXPECT_FALSE( cube3.check_containment(cube1) );
  EXPECT_FALSE( cube3.check_containment(cube2) );
};

TEST(SopCubeTest, cube_cube_product1)
{
  const int nv = 10;

  VarId var0(0);
  VarId var1(1);
  VarId var2(2);
  VarId var3(3);
  VarId var4(4);
  VarId var5(5);
  VarId var6(6);
  VarId var7(7);
  VarId var8(8);
  VarId var9(9);

  Literal lit0(var0, false);
  Literal lit1(var5, true);
  Literal lit2(var7, true);
  SopCube cube1(nv, {lit1, lit2});
  SopCube cube2(nv, {lit0, lit1});

  SopCube cube3 = cube1 * cube2;

  EXPECT_TRUE(  cube3.has_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var5, false)) );
  EXPECT_TRUE(  cube3.has_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var7, false)) );
  EXPECT_TRUE(  cube3.has_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var9, true)) );
};

TEST(SopCubeTest, cube_cube_product2)
{
  const int nv = 10;

  VarId var0(0);
  VarId var1(1);
  VarId var2(2);
  VarId var3(3);
  VarId var4(4);
  VarId var5(5);
  VarId var6(6);
  VarId var7(7);
  VarId var8(8);
  VarId var9(9);

  Literal lit0(var0, false);
  Literal lit1(var5, true);
  Literal lit2(var7, true);
  Literal lit3(var5, false);
  SopCube cube1(nv, {lit1, lit2});
  SopCube cube2(nv, {lit0, lit3});

  SopCube cube3 = cube1 * cube2;

  EXPECT_FALSE( cube3.has_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var5, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var7, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var9, true)) );
};

TEST(SopCubeTest, Rcube_cube_product1)
{
  const int nv = 10;

  VarId var0(0);
  VarId var1(1);
  VarId var2(2);
  VarId var3(3);
  VarId var4(4);
  VarId var5(5);
  VarId var6(6);
  VarId var7(7);
  VarId var8(8);
  VarId var9(9);

  Literal lit0(var0, false);
  Literal lit1(var5, true);
  Literal lit2(var7, true);
  SopCube cube1(nv, {lit1, lit2});
  SopCube cube2(nv, {lit0, lit1});

  SopCube cube3 = std::move(cube1) * cube2;

  EXPECT_EQ( 0, cube1.literal_num() );

  EXPECT_TRUE(  cube3.has_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var5, false)) );
  EXPECT_TRUE(  cube3.has_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var7, false)) );
  EXPECT_TRUE(  cube3.has_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var9, true)) );
};

TEST(SopCubeTest, cube_Rcube_product1)
{
  const int nv = 10;

  VarId var0(0);
  VarId var1(1);
  VarId var2(2);
  VarId var3(3);
  VarId var4(4);
  VarId var5(5);
  VarId var6(6);
  VarId var7(7);
  VarId var8(8);
  VarId var9(9);

  Literal lit0(var0, false);
  Literal lit1(var5, true);
  Literal lit2(var7, true);
  SopCube cube1(nv, {lit1, lit2});
  SopCube cube2(nv, {lit0, lit1});

  SopCube cube3 = cube1 * std::move(cube2);

  EXPECT_EQ( 0, cube2.literal_num() );

  EXPECT_TRUE(  cube3.has_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var5, false)) );
  EXPECT_TRUE(  cube3.has_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var7, false)) );
  EXPECT_TRUE(  cube3.has_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var9, true)) );
};

TEST(SopCubeTest, Rcube_Rcube_product1)
{
  const int nv = 10;

  VarId var0(0);
  VarId var1(1);
  VarId var2(2);
  VarId var3(3);
  VarId var4(4);
  VarId var5(5);
  VarId var6(6);
  VarId var7(7);
  VarId var8(8);
  VarId var9(9);

  Literal lit0(var0, false);
  Literal lit1(var5, true);
  Literal lit2(var7, true);
  SopCube cube1(nv, {lit1, lit2});
  SopCube cube2(nv, {lit0, lit1});

  SopCube cube3 = std::move(cube1) * std::move(cube2);

  EXPECT_EQ( 0, cube1.literal_num() );

  EXPECT_TRUE(  cube3.has_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var5, false)) );
  EXPECT_TRUE(  cube3.has_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var7, false)) );
  EXPECT_TRUE(  cube3.has_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var9, true)) );
};

TEST(SopCubeTest, lit_cube_product1)
{
  const int nv = 10;

  VarId var0(0);
  VarId var1(1);
  VarId var2(2);
  VarId var3(3);
  VarId var4(4);
  VarId var5(5);
  VarId var6(6);
  VarId var7(7);
  VarId var8(8);
  VarId var9(9);

  Literal lit0(var0, false);
  Literal lit1(var5, true);
  Literal lit2(var7, true);
  SopCube cube1(nv, {lit1, lit2});

  SopCube cube3 = lit0 * cube1;

  EXPECT_TRUE(  cube3.has_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var5, false)) );
  EXPECT_TRUE(  cube3.has_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var7, false)) );
  EXPECT_TRUE(  cube3.has_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var9, true)) );
};

TEST(SopCubeTest, lit_Rcube_product1)
{
  const int nv = 10;

  VarId var0(0);
  VarId var1(1);
  VarId var2(2);
  VarId var3(3);
  VarId var4(4);
  VarId var5(5);
  VarId var6(6);
  VarId var7(7);
  VarId var8(8);
  VarId var9(9);

  Literal lit0(var0, false);
  Literal lit1(var5, true);
  Literal lit2(var7, true);
  SopCube cube1(nv, {lit1, lit2});

  SopCube cube3 = lit0 * std::move(cube1);

  EXPECT_EQ( 0, cube1.literal_num() );

  EXPECT_TRUE(  cube3.has_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var5, false)) );
  EXPECT_TRUE(  cube3.has_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var7, false)) );
  EXPECT_TRUE(  cube3.has_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var9, true)) );
};

TEST(SopCubeTest, lit_cube_product2)
{
  const int nv = 10;

  VarId var0(0);
  VarId var1(1);
  VarId var2(2);
  VarId var3(3);
  VarId var4(4);
  VarId var5(5);
  VarId var6(6);
  VarId var7(7);
  VarId var8(8);
  VarId var9(9);

  Literal lit0(var5, false);
  Literal lit1(var5, true);
  Literal lit2(var7, true);
  SopCube cube1(nv, {lit1, lit2});

  SopCube cube3 = lit0 * cube1;

  EXPECT_FALSE( cube3.has_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var5, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var7, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var9, true)) );
};

TEST(SopCubeTest, cube_lit_product1)
{
  const int nv = 10;

  VarId var0(0);
  VarId var1(1);
  VarId var2(2);
  VarId var3(3);
  VarId var4(4);
  VarId var5(5);
  VarId var6(6);
  VarId var7(7);
  VarId var8(8);
  VarId var9(9);

  Literal lit0(var0, false);
  Literal lit1(var5, true);
  Literal lit2(var7, true);
  SopCube cube1(nv, {lit1, lit2});

  SopCube cube3 = cube1 * lit0;

  EXPECT_TRUE(  cube3.has_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var5, false)) );
  EXPECT_TRUE(  cube3.has_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var7, false)) );
  EXPECT_TRUE(  cube3.has_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var9, true)) );
};

TEST(SopCubeTest, Rcube_lit_product1)
{
  const int nv = 10;

  VarId var0(0);
  VarId var1(1);
  VarId var2(2);
  VarId var3(3);
  VarId var4(4);
  VarId var5(5);
  VarId var6(6);
  VarId var7(7);
  VarId var8(8);
  VarId var9(9);

  Literal lit0(var0, false);
  Literal lit1(var5, true);
  Literal lit2(var7, true);
  SopCube cube1(nv, {lit1, lit2});

  SopCube cube3 = std::move(cube1) * lit0;

  EXPECT_EQ( 0, cube1.literal_num() );

  EXPECT_TRUE(  cube3.has_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var5, false)) );
  EXPECT_TRUE(  cube3.has_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var7, false)) );
  EXPECT_TRUE(  cube3.has_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var9, true)) );
};

TEST(SopCubeTest, cube_lit_product2)
{
  const int nv = 10;

  VarId var0(0);
  VarId var1(1);
  VarId var2(2);
  VarId var3(3);
  VarId var4(4);
  VarId var5(5);
  VarId var6(6);
  VarId var7(7);
  VarId var8(8);
  VarId var9(9);

  Literal lit0(var5, false);
  Literal lit1(var5, true);
  Literal lit2(var7, true);
  SopCube cube1(nv, {lit1, lit2});

  SopCube cube3 = cube1 * lit0;

  EXPECT_FALSE( cube3.has_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var5, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var7, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var9, true)) );
};

TEST(SopCubeTest, Scube_cube_product1)
{
  const int nv = 10;

  VarId var0(0);
  VarId var1(1);
  VarId var2(2);
  VarId var3(3);
  VarId var4(4);
  VarId var5(5);
  VarId var6(6);
  VarId var7(7);
  VarId var8(8);
  VarId var9(9);

  Literal lit0(var0, false);
  Literal lit1(var5, true);
  Literal lit2(var7, true);
  SopCube cube1(nv, {lit1, lit2});
  SopCube cube2(nv, {lit0, lit1});

  cube1 = cube1 * cube2;

  EXPECT_TRUE(  cube1.has_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var5, false)) );
  EXPECT_TRUE(  cube1.has_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var7, false)) );
  EXPECT_TRUE(  cube1.has_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var9, true)) );
};

TEST(SopCubeTest, Scube_cube_product2)
{
  const int nv = 10;

  VarId var0(0);
  VarId var1(1);
  VarId var2(2);
  VarId var3(3);
  VarId var4(4);
  VarId var5(5);
  VarId var6(6);
  VarId var7(7);
  VarId var8(8);
  VarId var9(9);

  Literal lit0(var0, false);
  Literal lit1(var5, true);
  Literal lit2(var7, true);
  Literal lit3(var5, false);
  SopCube cube1(nv, {lit1, lit2});
  SopCube cube2(nv, {lit0, lit3});

  cube1 = cube1 * cube2;

  EXPECT_FALSE( cube1.has_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var5, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var7, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var9, true)) );
};

TEST(SopCubeTest, Scube_literal_product1)
{
  const int nv = 10;

  VarId var0(0);
  VarId var1(1);
  VarId var2(2);
  VarId var3(3);
  VarId var4(4);
  VarId var5(5);
  VarId var6(6);
  VarId var7(7);
  VarId var8(8);
  VarId var9(9);

  Literal lit0(var0, false);
  Literal lit1(var5, true);
  Literal lit2(var7, true);
  SopCube cube1(nv, {lit1, lit2});

  cube1 = cube1 * lit0;

  EXPECT_TRUE(  cube1.has_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var5, false)) );
  EXPECT_TRUE(  cube1.has_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var7, false)) );
  EXPECT_TRUE(  cube1.has_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var9, true)) );
};

TEST(SopCubeTest, Scube_literal_product2)
{
  const int nv = 10;

  VarId var0(0);
  VarId var1(1);
  VarId var2(2);
  VarId var3(3);
  VarId var4(4);
  VarId var5(5);
  VarId var6(6);
  VarId var7(7);
  VarId var8(8);
  VarId var9(9);

  Literal lit0(var5, false);
  Literal lit1(var5, true);
  Literal lit2(var7, true);
  SopCube cube1(nv, {lit1, lit2});

  cube1 = cube1 * lit0;

  EXPECT_FALSE( cube1.has_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var5, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var7, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var9, true)) );
};

TEST(SopCubeTest, literal_Scube_product1)
{
  const int nv = 10;

  VarId var0(0);
  VarId var1(1);
  VarId var2(2);
  VarId var3(3);
  VarId var4(4);
  VarId var5(5);
  VarId var6(6);
  VarId var7(7);
  VarId var8(8);
  VarId var9(9);

  Literal lit0(var0, false);
  Literal lit1(var5, true);
  Literal lit2(var7, true);
  SopCube cube1(nv, {lit1, lit2});

  cube1 = lit0 * cube1;

  EXPECT_TRUE(  cube1.has_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var5, false)) );
  EXPECT_TRUE(  cube1.has_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var7, false)) );
  EXPECT_TRUE(  cube1.has_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var9, true)) );
};

TEST(SopCubeTest, literal_Scube_product2)
{
  const int nv = 10;

  VarId var0(0);
  VarId var1(1);
  VarId var2(2);
  VarId var3(3);
  VarId var4(4);
  VarId var5(5);
  VarId var6(6);
  VarId var7(7);
  VarId var8(8);
  VarId var9(9);

  Literal lit0(var5, false);
  Literal lit1(var5, true);
  Literal lit2(var7, true);
  SopCube cube1(nv, {lit1, lit2});

  cube1 = lit0 * cube1;

  EXPECT_FALSE( cube1.has_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var5, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var7, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var9, true)) );
};

TEST(SopCubeTest, Icube_cube_product1)
{
  const int nv = 10;

  VarId var0(0);
  VarId var1(1);
  VarId var2(2);
  VarId var3(3);
  VarId var4(4);
  VarId var5(5);
  VarId var6(6);
  VarId var7(7);
  VarId var8(8);
  VarId var9(9);

  Literal lit0(var0, false);
  Literal lit1(var5, true);
  Literal lit2(var7, true);
  SopCube cube1(nv, {lit1, lit2});
  SopCube cube2(nv, {lit0, lit1});

  cube1 *= cube2;

  EXPECT_TRUE(  cube1.has_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var5, false)) );
  EXPECT_TRUE(  cube1.has_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var7, false)) );
  EXPECT_TRUE(  cube1.has_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var9, true)) );
};

TEST(SopCubeTest, Icube_cube_product2)
{
  const int nv = 10;

  VarId var0(0);
  VarId var1(1);
  VarId var2(2);
  VarId var3(3);
  VarId var4(4);
  VarId var5(5);
  VarId var6(6);
  VarId var7(7);
  VarId var8(8);
  VarId var9(9);

  Literal lit0(var0, false);
  Literal lit1(var5, true);
  Literal lit2(var7, true);
  Literal lit3(var5, false);
  SopCube cube1(nv, {lit1, lit2});
  SopCube cube2(nv, {lit0, lit3});

  cube1 *= cube2;

  EXPECT_FALSE( cube1.has_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var5, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var7, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var9, true)) );
};

TEST(SopCubeTest, Icube_literal_product1)
{
  const int nv = 10;

  VarId var0(0);
  VarId var1(1);
  VarId var2(2);
  VarId var3(3);
  VarId var4(4);
  VarId var5(5);
  VarId var6(6);
  VarId var7(7);
  VarId var8(8);
  VarId var9(9);

  Literal lit0(var0, false);
  Literal lit1(var5, true);
  Literal lit2(var7, true);
  SopCube cube1(nv, {lit1, lit2});

  cube1 *= lit0;

  EXPECT_TRUE(  cube1.has_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var5, false)) );
  EXPECT_TRUE(  cube1.has_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var7, false)) );
  EXPECT_TRUE(  cube1.has_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var9, true)) );
};

TEST(SopCubeTest, Icube_literal_product2)
{
  const int nv = 10;

  VarId var0(0);
  VarId var1(1);
  VarId var2(2);
  VarId var3(3);
  VarId var4(4);
  VarId var5(5);
  VarId var6(6);
  VarId var7(7);
  VarId var8(8);
  VarId var9(9);

  Literal lit0(var5, false);
  Literal lit1(var5, true);
  Literal lit2(var7, true);
  SopCube cube1(nv, {lit1, lit2});

  cube1 *= lit0;

  EXPECT_FALSE( cube1.has_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var5, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var7, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var9, true)) );
};

TEST(SopCubeTest, cube_cube_quotient1)
{
  const int nv = 10;

  VarId var0(0);
  VarId var1(1);
  VarId var2(2);
  VarId var3(3);
  VarId var4(4);
  VarId var5(5);
  VarId var6(6);
  VarId var7(7);
  VarId var8(8);
  VarId var9(9);

  Literal lit0(var0, false);
  Literal lit1(var5, true);
  Literal lit2(var7, true);
  SopCube cube1(nv, {lit0, lit1, lit2});
  SopCube cube2(nv, {lit0, lit2});

  SopCube cube3 = cube1 / cube2;

  EXPECT_FALSE( cube3.has_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var5, false)) );
  EXPECT_TRUE(  cube3.has_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var7, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var9, true)) );
};

TEST(SopCubeTest, Rcube_cube_quotient1)
{
  const int nv = 10;

  VarId var0(0);
  VarId var1(1);
  VarId var2(2);
  VarId var3(3);
  VarId var4(4);
  VarId var5(5);
  VarId var6(6);
  VarId var7(7);
  VarId var8(8);
  VarId var9(9);

  Literal lit0(var0, false);
  Literal lit1(var5, true);
  Literal lit2(var7, true);
  SopCube cube1(nv, {lit0, lit1, lit2});
  SopCube cube2(nv, {lit0, lit2});

  SopCube cube3 = std::move(cube1) / cube2;

  EXPECT_EQ( 0, cube1.literal_num() );

  EXPECT_FALSE( cube3.has_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var5, false)) );
  EXPECT_TRUE(  cube3.has_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var7, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var9, true)) );
};

TEST(SopCubeTest, cube_cube_quotient2)
{
  const int nv = 10;

  VarId var0(0);
  VarId var1(1);
  VarId var2(2);
  VarId var3(3);
  VarId var4(4);
  VarId var5(5);
  VarId var6(6);
  VarId var7(7);
  VarId var8(8);
  VarId var9(9);

  Literal lit0(var0, false);
  Literal lit1(var5, true);
  Literal lit2(var7, true);
  SopCube cube1(nv, {lit0, lit1});
  SopCube cube2(nv, {lit2});

  SopCube cube3 = cube1 / cube2;

  EXPECT_FALSE( cube3.has_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var5, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var7, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var9, true)) );
};

TEST(SopCubeTest, cube_lit_quotient1)
{
  const int nv = 10;

  VarId var0(0);
  VarId var1(1);
  VarId var2(2);
  VarId var3(3);
  VarId var4(4);
  VarId var5(5);
  VarId var6(6);
  VarId var7(7);
  VarId var8(8);
  VarId var9(9);

  Literal lit0(var0, false);
  Literal lit1(var5, true);
  Literal lit2(var7, true);
  SopCube cube1(nv, {lit0, lit1, lit2});

  SopCube cube3 = cube1 / lit0;

  EXPECT_FALSE( cube3.has_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var5, false)) );
  EXPECT_TRUE(  cube3.has_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var7, false)) );
  EXPECT_TRUE(  cube3.has_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var9, true)) );
};

TEST(SopCubeTest, Rcube_lit_quotient1)
{
  const int nv = 10;

  VarId var0(0);
  VarId var1(1);
  VarId var2(2);
  VarId var3(3);
  VarId var4(4);
  VarId var5(5);
  VarId var6(6);
  VarId var7(7);
  VarId var8(8);
  VarId var9(9);

  Literal lit0(var0, false);
  Literal lit1(var5, true);
  Literal lit2(var7, true);
  SopCube cube1(nv, {lit0, lit1, lit2});

  SopCube cube3 = std::move(cube1) / lit0;

  EXPECT_EQ( 0, cube1.literal_num() );

  EXPECT_FALSE( cube3.has_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var5, false)) );
  EXPECT_TRUE(  cube3.has_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var7, false)) );
  EXPECT_TRUE(  cube3.has_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var9, true)) );
};

TEST(SopCubeTest, cube_lit_quotient2)
{
  const int nv = 10;

  VarId var0(0);
  VarId var1(1);
  VarId var2(2);
  VarId var3(3);
  VarId var4(4);
  VarId var5(5);
  VarId var6(6);
  VarId var7(7);
  VarId var8(8);
  VarId var9(9);

  Literal lit0(var5, false);
  Literal lit1(var5, true);
  Literal lit2(var7, true);
  SopCube cube1(nv, {lit0, lit1, lit2});

  SopCube cube3 = cube1 / lit0;

  EXPECT_FALSE( cube3.has_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var5, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var7, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube3.has_literal(Literal(var9, true)) );
};

TEST(SopCubeTest, Scube_cube_quotient1)
{
  const int nv = 10;

  VarId var0(0);
  VarId var1(1);
  VarId var2(2);
  VarId var3(3);
  VarId var4(4);
  VarId var5(5);
  VarId var6(6);
  VarId var7(7);
  VarId var8(8);
  VarId var9(9);

  Literal lit0(var0, false);
  Literal lit1(var5, true);
  Literal lit2(var7, true);
  SopCube cube1(nv, {lit0, lit1, lit2});
  SopCube cube2(nv, {lit0, lit2});

  cube1 = cube1 / cube2;

  EXPECT_FALSE( cube1.has_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var5, false)) );
  EXPECT_TRUE(  cube1.has_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var7, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var9, true)) );
};

TEST(SopCubeTest, Scube_cube_quotient2)
{
  const int nv = 10;

  VarId var0(0);
  VarId var1(1);
  VarId var2(2);
  VarId var3(3);
  VarId var4(4);
  VarId var5(5);
  VarId var6(6);
  VarId var7(7);
  VarId var8(8);
  VarId var9(9);

  Literal lit0(var0, false);
  Literal lit1(var5, true);
  Literal lit2(var7, true);
  SopCube cube1(nv, {lit0, lit1});
  SopCube cube2(nv, {lit2});

  cube1 = cube1 / cube2;

  EXPECT_FALSE( cube1.has_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var5, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var7, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var9, true)) );
};

TEST(SopCubeTest, Scube_literal_quotient1)
{
  const int nv = 10;

  VarId var0(0);
  VarId var1(1);
  VarId var2(2);
  VarId var3(3);
  VarId var4(4);
  VarId var5(5);
  VarId var6(6);
  VarId var7(7);
  VarId var8(8);
  VarId var9(9);

  Literal lit0(var0, false);
  Literal lit1(var5, true);
  Literal lit2(var7, true);
  SopCube cube1(nv, {lit0, lit1, lit2});

  cube1 = cube1 / lit0;

  EXPECT_FALSE( cube1.has_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var5, false)) );
  EXPECT_TRUE(  cube1.has_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var7, false)) );
  EXPECT_TRUE(  cube1.has_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var9, true)) );
};

TEST(SopCubeTest, Scube_literal_quotient2)
{
  const int nv = 10;

  VarId var0(0);
  VarId var1(1);
  VarId var2(2);
  VarId var3(3);
  VarId var4(4);
  VarId var5(5);
  VarId var6(6);
  VarId var7(7);
  VarId var8(8);
  VarId var9(9);

  Literal lit0(var5, false);
  Literal lit1(var5, true);
  Literal lit2(var7, true);
  SopCube cube1(nv, {lit0, lit1, lit2});

  cube1 = cube1 / lit0;

  EXPECT_FALSE( cube1.has_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var5, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var7, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var9, true)) );
};

TEST(SopCubeTest, Icube_cube_quotient1)
{
  const int nv = 10;

  VarId var0(0);
  VarId var1(1);
  VarId var2(2);
  VarId var3(3);
  VarId var4(4);
  VarId var5(5);
  VarId var6(6);
  VarId var7(7);
  VarId var8(8);
  VarId var9(9);

  Literal lit0(var0, false);
  Literal lit1(var5, true);
  Literal lit2(var7, true);
  SopCube cube1(nv, {lit0, lit1, lit2});
  SopCube cube2(nv, {lit0, lit2});

  cube1 /= cube2;

  EXPECT_FALSE( cube1.has_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var5, false)) );
  EXPECT_TRUE(  cube1.has_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var7, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var9, true)) );
};

TEST(SopCubeTest, Icube_cube_quotient2)
{
  const int nv = 10;

  VarId var0(0);
  VarId var1(1);
  VarId var2(2);
  VarId var3(3);
  VarId var4(4);
  VarId var5(5);
  VarId var6(6);
  VarId var7(7);
  VarId var8(8);
  VarId var9(9);

  Literal lit0(var0, false);
  Literal lit1(var5, true);
  Literal lit2(var7, true);
  SopCube cube1(nv, {lit0, lit1});
  SopCube cube2(nv, {lit2});

  cube1 /= cube2;

  EXPECT_FALSE( cube1.has_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var5, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var7, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var9, true)) );
};

TEST(SopCubeTest, Icube_literal_quotient1)
{
  const int nv = 10;

  VarId var0(0);
  VarId var1(1);
  VarId var2(2);
  VarId var3(3);
  VarId var4(4);
  VarId var5(5);
  VarId var6(6);
  VarId var7(7);
  VarId var8(8);
  VarId var9(9);

  Literal lit0(var0, false);
  Literal lit1(var5, true);
  Literal lit2(var7, true);
  SopCube cube1(nv, {lit0, lit1, lit2});

  cube1 /= lit0;

  EXPECT_FALSE( cube1.has_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var5, false)) );
  EXPECT_TRUE(  cube1.has_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var7, false)) );
  EXPECT_TRUE(  cube1.has_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var9, true)) );
};

TEST(SopCubeTest, Icube_literal_quotient2)
{
  const int nv = 10;

  VarId var0(0);
  VarId var1(1);
  VarId var2(2);
  VarId var3(3);
  VarId var4(4);
  VarId var5(5);
  VarId var6(6);
  VarId var7(7);
  VarId var8(8);
  VarId var9(9);

  Literal lit0(var5, false);
  Literal lit1(var5, true);
  Literal lit2(var7, true);
  SopCube cube1(nv, {lit0, lit1, lit2});

  cube1 /= lit0;

  EXPECT_FALSE( cube1.has_literal(Literal(var0, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var0, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var1, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var1, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var2, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var2, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var3, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var3, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var4, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var4, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var5, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var5, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var6, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var6, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var7, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var7, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var8, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var8, true)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var9, false)) );
  EXPECT_FALSE( cube1.has_literal(Literal(var9, true)) );
};

TEST(SopCubeTest, compare1)
{
  const int nv = 100;

  VarId var0(0);
  VarId var80(80);


  Literal lit0(var0, false);
  Literal lit1(var80, false);

  SopCube cube1(nv, { lit0,  lit1});
  SopCube cube2(nv, { lit0, ~lit1});
  SopCube cube3(nv, {~lit0,  lit1});

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

TEST(SopCubeTest, compare2)
{
  const int nv = 100;

  VarId var0(0);
  VarId var80(80);

  Literal lit0(var0, false);
  Literal lit1(var80, false);

  SopCube cube1(nv, { lit0,  lit1});
  SopCube cube2(nv, { lit0, ~lit1});
  SopCube cube3(nv, {~lit0,  lit1});

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

END_NAMESPACE_YM
