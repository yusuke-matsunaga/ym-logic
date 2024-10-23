
/// @file SopCoverTest.cc
/// @brief SopCoverTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018, 2023 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "SopTest.h"
#include "ym/SopCover.h"
#include "ym/SopCube.h"


BEGIN_NAMESPACE_YM

TEST_F(SopTest, constructor1)
{
  Literal lit0(var0, false);
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  SopCover cover1{nv};

  EXPECT_EQ( nv, cover1.variable_num() );
  EXPECT_EQ(  0, cover1.cube_num() );
  EXPECT_EQ(  0, cover1.literal_num() );
  EXPECT_EQ(  0, cover1.literal_num( lit0) );
  EXPECT_EQ(  0, cover1.literal_num(~lit0) );
  EXPECT_EQ(  0, cover1.literal_num( lit1) );
  EXPECT_EQ(  0, cover1.literal_num(~lit1) );
  EXPECT_EQ(  0, cover1.literal_num( lit2) );
  EXPECT_EQ(  0, cover1.literal_num(~lit2) );
  EXPECT_EQ(  0, cover1.literal_num( lit3) );
  EXPECT_EQ(  0, cover1.literal_num(~lit3) );
  EXPECT_EQ(  0, cover1.literal_num( lit4) );
  EXPECT_EQ(  0, cover1.literal_num(~lit4) );
  EXPECT_EQ(  0, cover1.literal_num( lit5) );
  EXPECT_EQ(  0, cover1.literal_num(~lit5) );
  EXPECT_EQ(  0, cover1.literal_num( lit6) );
  EXPECT_EQ(  0, cover1.literal_num(~lit6) );
  EXPECT_EQ(  0, cover1.literal_num( lit7) );
  EXPECT_EQ(  0, cover1.literal_num(~lit7) );
  EXPECT_EQ(  0, cover1.literal_num( lit8) );
  EXPECT_EQ(  0, cover1.literal_num(~lit8) );
  EXPECT_EQ(  0, cover1.literal_num( lit9) );
  EXPECT_EQ(  0, cover1.literal_num(~lit9) );
};

TEST_F(SopTest, constructor2)
{
  Literal lit0(var0, false);
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  SopCube cube1{nv, {lit0, lit1}};
  SopCube cube2{nv, {lit2, lit3}};

  vector<SopCube> cube_list{cube1, cube2};

  SopCover cover1{nv, cube_list};

  EXPECT_EQ( nv, cover1.variable_num() );
  EXPECT_EQ(  2, cover1.cube_num() );
  EXPECT_EQ(  4, cover1.literal_num() );
  EXPECT_EQ(  1, cover1.literal_num( lit0) );
  EXPECT_EQ(  0, cover1.literal_num(~lit0) );
  EXPECT_EQ(  1, cover1.literal_num( lit1) );
  EXPECT_EQ(  0, cover1.literal_num(~lit1) );
  EXPECT_EQ(  1, cover1.literal_num( lit2) );
  EXPECT_EQ(  0, cover1.literal_num(~lit2) );
  EXPECT_EQ(  1, cover1.literal_num( lit3) );
  EXPECT_EQ(  0, cover1.literal_num(~lit3) );
  EXPECT_EQ(  0, cover1.literal_num( lit4) );
  EXPECT_EQ(  0, cover1.literal_num(~lit4) );
  EXPECT_EQ(  0, cover1.literal_num( lit5) );
  EXPECT_EQ(  0, cover1.literal_num(~lit5) );
  EXPECT_EQ(  0, cover1.literal_num( lit6) );
  EXPECT_EQ(  0, cover1.literal_num(~lit6) );
  EXPECT_EQ(  0, cover1.literal_num( lit7) );
  EXPECT_EQ(  0, cover1.literal_num(~lit7) );
  EXPECT_EQ(  0, cover1.literal_num( lit8) );
  EXPECT_EQ(  0, cover1.literal_num(~lit8) );
  EXPECT_EQ(  0, cover1.literal_num( lit9) );
  EXPECT_EQ(  0, cover1.literal_num(~lit9) );

  auto cube_list1 = cover1.literal_list();
  EXPECT_EQ( 2, cube_list1.size() );
  EXPECT_EQ( 2, cube_list1[0].size() );
  EXPECT_EQ( 2, cube_list1[1].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit1, cube_list1[0][1] );
  EXPECT_EQ( lit2, cube_list1[1][0] );
  EXPECT_EQ( lit3, cube_list1[1][1] );

};

TEST_F(SopTest, constructor3)
{
  Literal lit0(var0, false);
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  vector<vector<Literal>> cube_list{ {lit0, lit1}, {lit2, lit3} };
  SopCover cover1{nv, cube_list};

  EXPECT_EQ( nv, cover1.variable_num() );
  EXPECT_EQ(  2, cover1.cube_num() );
  EXPECT_EQ(  4, cover1.literal_num() );
  EXPECT_EQ(  1, cover1.literal_num( lit0) );
  EXPECT_EQ(  0, cover1.literal_num(~lit0) );
  EXPECT_EQ(  1, cover1.literal_num( lit1) );
  EXPECT_EQ(  0, cover1.literal_num(~lit1) );
  EXPECT_EQ(  1, cover1.literal_num( lit2) );
  EXPECT_EQ(  0, cover1.literal_num(~lit2) );
  EXPECT_EQ(  1, cover1.literal_num( lit3) );
  EXPECT_EQ(  0, cover1.literal_num(~lit3) );
  EXPECT_EQ(  0, cover1.literal_num( lit4) );
  EXPECT_EQ(  0, cover1.literal_num(~lit4) );
  EXPECT_EQ(  0, cover1.literal_num( lit5) );
  EXPECT_EQ(  0, cover1.literal_num(~lit5) );
  EXPECT_EQ(  0, cover1.literal_num( lit6) );
  EXPECT_EQ(  0, cover1.literal_num(~lit6) );
  EXPECT_EQ(  0, cover1.literal_num( lit7) );
  EXPECT_EQ(  0, cover1.literal_num(~lit7) );
  EXPECT_EQ(  0, cover1.literal_num( lit8) );
  EXPECT_EQ(  0, cover1.literal_num(~lit8) );
  EXPECT_EQ(  0, cover1.literal_num( lit9) );
  EXPECT_EQ(  0, cover1.literal_num(~lit9) );

  auto cube_list1 = cover1.literal_list();
  EXPECT_EQ( 2, cube_list1.size() );
  EXPECT_EQ( 2, cube_list1[0].size() );
  EXPECT_EQ( 2, cube_list1[1].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit1, cube_list1[0][1] );
  EXPECT_EQ( lit2, cube_list1[1][0] );
  EXPECT_EQ( lit3, cube_list1[1][1] );

};

TEST_F(SopTest, constructor4)
{
  Literal lit0(var0, false);
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  std::initializer_list<std::initializer_list<Literal>> cube_list{ {lit0, lit1}, {lit2, lit3} };
  SopCover cover1{nv, cube_list};

  EXPECT_EQ( nv, cover1.variable_num() );
  EXPECT_EQ(  2, cover1.cube_num() );
  EXPECT_EQ(  4, cover1.literal_num() );
  EXPECT_EQ(  1, cover1.literal_num( lit0) );
  EXPECT_EQ(  0, cover1.literal_num(~lit0) );
  EXPECT_EQ(  1, cover1.literal_num( lit1) );
  EXPECT_EQ(  0, cover1.literal_num(~lit1) );
  EXPECT_EQ(  1, cover1.literal_num( lit2) );
  EXPECT_EQ(  0, cover1.literal_num(~lit2) );
  EXPECT_EQ(  1, cover1.literal_num( lit3) );
  EXPECT_EQ(  0, cover1.literal_num(~lit3) );
  EXPECT_EQ(  0, cover1.literal_num( lit4) );
  EXPECT_EQ(  0, cover1.literal_num(~lit4) );
  EXPECT_EQ(  0, cover1.literal_num( lit5) );
  EXPECT_EQ(  0, cover1.literal_num(~lit5) );
  EXPECT_EQ(  0, cover1.literal_num( lit6) );
  EXPECT_EQ(  0, cover1.literal_num(~lit6) );
  EXPECT_EQ(  0, cover1.literal_num( lit7) );
  EXPECT_EQ(  0, cover1.literal_num(~lit7) );
  EXPECT_EQ(  0, cover1.literal_num( lit8) );
  EXPECT_EQ(  0, cover1.literal_num(~lit8) );
  EXPECT_EQ(  0, cover1.literal_num( lit9) );
  EXPECT_EQ(  0, cover1.literal_num(~lit9) );

  auto cube_list1 = cover1.literal_list();
  EXPECT_EQ( 2, cube_list1.size() );
  EXPECT_EQ( 2, cube_list1[0].size() );
  EXPECT_EQ( 2, cube_list1[1].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit1, cube_list1[0][1] );
  EXPECT_EQ( lit2, cube_list1[1][0] );
  EXPECT_EQ( lit3, cube_list1[1][1] );

};

TEST_F(SopTest, copy_constructor)
{
  Literal lit0(var0, false);
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  std::initializer_list<std::initializer_list<Literal>> cube_list{ {lit0, lit1}, {lit2, lit3} };
  SopCover src_cover{nv, cube_list};

  SopCover cover1{src_cover};

  EXPECT_EQ( nv, cover1.variable_num() );
  EXPECT_EQ(  2, cover1.cube_num() );
  EXPECT_EQ(  4, cover1.literal_num() );
  EXPECT_EQ(  1, cover1.literal_num( lit0) );
  EXPECT_EQ(  0, cover1.literal_num(~lit0) );
  EXPECT_EQ(  1, cover1.literal_num( lit1) );
  EXPECT_EQ(  0, cover1.literal_num(~lit1) );
  EXPECT_EQ(  1, cover1.literal_num( lit2) );
  EXPECT_EQ(  0, cover1.literal_num(~lit2) );
  EXPECT_EQ(  1, cover1.literal_num( lit3) );
  EXPECT_EQ(  0, cover1.literal_num(~lit3) );
  EXPECT_EQ(  0, cover1.literal_num( lit4) );
  EXPECT_EQ(  0, cover1.literal_num(~lit4) );
  EXPECT_EQ(  0, cover1.literal_num( lit5) );
  EXPECT_EQ(  0, cover1.literal_num(~lit5) );
  EXPECT_EQ(  0, cover1.literal_num( lit6) );
  EXPECT_EQ(  0, cover1.literal_num(~lit6) );
  EXPECT_EQ(  0, cover1.literal_num( lit7) );
  EXPECT_EQ(  0, cover1.literal_num(~lit7) );
  EXPECT_EQ(  0, cover1.literal_num( lit8) );
  EXPECT_EQ(  0, cover1.literal_num(~lit8) );
  EXPECT_EQ(  0, cover1.literal_num( lit9) );
  EXPECT_EQ(  0, cover1.literal_num(~lit9) );

  auto cube_list1 = cover1.literal_list();
  EXPECT_EQ( 2, cube_list1.size() );
  EXPECT_EQ( 2, cube_list1[0].size() );
  EXPECT_EQ( 2, cube_list1[1].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit1, cube_list1[0][1] );
  EXPECT_EQ( lit2, cube_list1[1][0] );
  EXPECT_EQ( lit3, cube_list1[1][1] );

};

TEST_F(SopTest, copy_assignment)
{
  Literal lit0(var0, false);
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  std::initializer_list<std::initializer_list<Literal>> cube_list{ {lit0, lit1}, {lit2, lit3} };
  SopCover src_cover{nv, cube_list};

  SopCover cover1 = src_cover;

  EXPECT_EQ( nv, cover1.variable_num() );
  EXPECT_EQ(  2, cover1.cube_num() );
  EXPECT_EQ(  4, cover1.literal_num() );
  EXPECT_EQ(  1, cover1.literal_num( lit0) );
  EXPECT_EQ(  0, cover1.literal_num(~lit0) );
  EXPECT_EQ(  1, cover1.literal_num( lit1) );
  EXPECT_EQ(  0, cover1.literal_num(~lit1) );
  EXPECT_EQ(  1, cover1.literal_num( lit2) );
  EXPECT_EQ(  0, cover1.literal_num(~lit2) );
  EXPECT_EQ(  1, cover1.literal_num( lit3) );
  EXPECT_EQ(  0, cover1.literal_num(~lit3) );
  EXPECT_EQ(  0, cover1.literal_num( lit4) );
  EXPECT_EQ(  0, cover1.literal_num(~lit4) );
  EXPECT_EQ(  0, cover1.literal_num( lit5) );
  EXPECT_EQ(  0, cover1.literal_num(~lit5) );
  EXPECT_EQ(  0, cover1.literal_num( lit6) );
  EXPECT_EQ(  0, cover1.literal_num(~lit6) );
  EXPECT_EQ(  0, cover1.literal_num( lit7) );
  EXPECT_EQ(  0, cover1.literal_num(~lit7) );
  EXPECT_EQ(  0, cover1.literal_num( lit8) );
  EXPECT_EQ(  0, cover1.literal_num(~lit8) );
  EXPECT_EQ(  0, cover1.literal_num( lit9) );
  EXPECT_EQ(  0, cover1.literal_num(~lit9) );

  auto cube_list1 = cover1.literal_list();
  EXPECT_EQ( 2, cube_list1.size() );
  EXPECT_EQ( 2, cube_list1[0].size() );
  EXPECT_EQ( 2, cube_list1[1].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit1, cube_list1[0][1] );
  EXPECT_EQ( lit2, cube_list1[1][0] );
  EXPECT_EQ( lit3, cube_list1[1][1] );

};

TEST_F(SopTest, move_constructor)
{
  Literal lit0(var0, false);
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  std::initializer_list<std::initializer_list<Literal>> cube_list{ {lit0, lit1}, {lit2, lit3} };
  SopCover src_cover{nv, cube_list};

  SopCover cover1{std::move(src_cover)};

  EXPECT_EQ(  0, src_cover.cube_num() );

  EXPECT_EQ( nv, cover1.variable_num() );
  EXPECT_EQ(  2, cover1.cube_num() );
  EXPECT_EQ(  4, cover1.literal_num() );
  EXPECT_EQ(  1, cover1.literal_num( lit0) );
  EXPECT_EQ(  0, cover1.literal_num(~lit0) );
  EXPECT_EQ(  1, cover1.literal_num( lit1) );
  EXPECT_EQ(  0, cover1.literal_num(~lit1) );
  EXPECT_EQ(  1, cover1.literal_num( lit2) );
  EXPECT_EQ(  0, cover1.literal_num(~lit2) );
  EXPECT_EQ(  1, cover1.literal_num( lit3) );
  EXPECT_EQ(  0, cover1.literal_num(~lit3) );
  EXPECT_EQ(  0, cover1.literal_num( lit4) );
  EXPECT_EQ(  0, cover1.literal_num(~lit4) );
  EXPECT_EQ(  0, cover1.literal_num( lit5) );
  EXPECT_EQ(  0, cover1.literal_num(~lit5) );
  EXPECT_EQ(  0, cover1.literal_num( lit6) );
  EXPECT_EQ(  0, cover1.literal_num(~lit6) );
  EXPECT_EQ(  0, cover1.literal_num( lit7) );
  EXPECT_EQ(  0, cover1.literal_num(~lit7) );
  EXPECT_EQ(  0, cover1.literal_num( lit8) );
  EXPECT_EQ(  0, cover1.literal_num(~lit8) );
  EXPECT_EQ(  0, cover1.literal_num( lit9) );
  EXPECT_EQ(  0, cover1.literal_num(~lit9) );

  auto cube_list1 = cover1.literal_list();
  EXPECT_EQ( 2, cube_list1.size() );
  EXPECT_EQ( 2, cube_list1[0].size() );
  EXPECT_EQ( 2, cube_list1[1].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit1, cube_list1[0][1] );
  EXPECT_EQ( lit2, cube_list1[1][0] );
  EXPECT_EQ( lit3, cube_list1[1][1] );

};

TEST_F(SopTest, move_assignment)
{
  Literal lit0(var0, false);
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  std::initializer_list<std::initializer_list<Literal>> cube_list{ {lit0, lit1}, {lit2, lit3} };
  SopCover src_cover{nv, cube_list};

  SopCover cover1 = std::move(src_cover);

  EXPECT_EQ(  0, src_cover.cube_num() );

  EXPECT_EQ( nv, cover1.variable_num() );
  EXPECT_EQ(  2, cover1.cube_num() );
  EXPECT_EQ(  4, cover1.literal_num() );
  EXPECT_EQ(  1, cover1.literal_num( lit0) );
  EXPECT_EQ(  0, cover1.literal_num(~lit0) );
  EXPECT_EQ(  1, cover1.literal_num( lit1) );
  EXPECT_EQ(  0, cover1.literal_num(~lit1) );
  EXPECT_EQ(  1, cover1.literal_num( lit2) );
  EXPECT_EQ(  0, cover1.literal_num(~lit2) );
  EXPECT_EQ(  1, cover1.literal_num( lit3) );
  EXPECT_EQ(  0, cover1.literal_num(~lit3) );
  EXPECT_EQ(  0, cover1.literal_num( lit4) );
  EXPECT_EQ(  0, cover1.literal_num(~lit4) );
  EXPECT_EQ(  0, cover1.literal_num( lit5) );
  EXPECT_EQ(  0, cover1.literal_num(~lit5) );
  EXPECT_EQ(  0, cover1.literal_num( lit6) );
  EXPECT_EQ(  0, cover1.literal_num(~lit6) );
  EXPECT_EQ(  0, cover1.literal_num( lit7) );
  EXPECT_EQ(  0, cover1.literal_num(~lit7) );
  EXPECT_EQ(  0, cover1.literal_num( lit8) );
  EXPECT_EQ(  0, cover1.literal_num(~lit8) );
  EXPECT_EQ(  0, cover1.literal_num( lit9) );
  EXPECT_EQ(  0, cover1.literal_num(~lit9) );

  auto cube_list1 = cover1.literal_list();
  EXPECT_EQ( 2, cube_list1.size() );
  EXPECT_EQ( 2, cube_list1[0].size() );
  EXPECT_EQ( 2, cube_list1[1].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit1, cube_list1[0][1] );
  EXPECT_EQ( lit2, cube_list1[1][0] );
  EXPECT_EQ( lit3, cube_list1[1][1] );

};

TEST_F(SopTest, cube_constructor)
{
  Literal lit0(var0, false);
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  SopCube src_cube{nv, {lit0, lit1}};

  SopCover cover1{src_cube};

  EXPECT_EQ( nv, cover1.variable_num() );
  EXPECT_EQ(  1, cover1.cube_num() );
  EXPECT_EQ(  2, cover1.literal_num() );
  EXPECT_EQ(  1, cover1.literal_num( lit0) );
  EXPECT_EQ(  0, cover1.literal_num(~lit0) );
  EXPECT_EQ(  1, cover1.literal_num( lit1) );
  EXPECT_EQ(  0, cover1.literal_num(~lit1) );
  EXPECT_EQ(  0, cover1.literal_num( lit2) );
  EXPECT_EQ(  0, cover1.literal_num(~lit2) );
  EXPECT_EQ(  0, cover1.literal_num( lit3) );
  EXPECT_EQ(  0, cover1.literal_num(~lit3) );
  EXPECT_EQ(  0, cover1.literal_num( lit4) );
  EXPECT_EQ(  0, cover1.literal_num(~lit4) );
  EXPECT_EQ(  0, cover1.literal_num( lit5) );
  EXPECT_EQ(  0, cover1.literal_num(~lit5) );
  EXPECT_EQ(  0, cover1.literal_num( lit6) );
  EXPECT_EQ(  0, cover1.literal_num(~lit6) );
  EXPECT_EQ(  0, cover1.literal_num( lit7) );
  EXPECT_EQ(  0, cover1.literal_num(~lit7) );
  EXPECT_EQ(  0, cover1.literal_num( lit8) );
  EXPECT_EQ(  0, cover1.literal_num(~lit8) );
  EXPECT_EQ(  0, cover1.literal_num( lit9) );
  EXPECT_EQ(  0, cover1.literal_num(~lit9) );

  auto cube_list1 = cover1.literal_list();
  EXPECT_EQ( 1, cube_list1.size() );
  EXPECT_EQ( 2, cube_list1[0].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit1, cube_list1[0][1] );

};

TEST_F(SopTest, cube_move_constructor)
{
  Literal lit0(var0, false);
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  SopCube src_cube{nv, {lit0, lit1}};

  SopCover cover1{std::move(src_cube)};

  EXPECT_EQ( 0, src_cube.literal_num() );

  EXPECT_EQ( nv, cover1.variable_num() );
  EXPECT_EQ(  1, cover1.cube_num() );
  EXPECT_EQ(  2, cover1.literal_num() );
  EXPECT_EQ(  1, cover1.literal_num( lit0) );
  EXPECT_EQ(  0, cover1.literal_num(~lit0) );
  EXPECT_EQ(  1, cover1.literal_num( lit1) );
  EXPECT_EQ(  0, cover1.literal_num(~lit1) );
  EXPECT_EQ(  0, cover1.literal_num( lit2) );
  EXPECT_EQ(  0, cover1.literal_num(~lit2) );
  EXPECT_EQ(  0, cover1.literal_num( lit3) );
  EXPECT_EQ(  0, cover1.literal_num(~lit3) );
  EXPECT_EQ(  0, cover1.literal_num( lit4) );
  EXPECT_EQ(  0, cover1.literal_num(~lit4) );
  EXPECT_EQ(  0, cover1.literal_num( lit5) );
  EXPECT_EQ(  0, cover1.literal_num(~lit5) );
  EXPECT_EQ(  0, cover1.literal_num( lit6) );
  EXPECT_EQ(  0, cover1.literal_num(~lit6) );
  EXPECT_EQ(  0, cover1.literal_num( lit7) );
  EXPECT_EQ(  0, cover1.literal_num(~lit7) );
  EXPECT_EQ(  0, cover1.literal_num( lit8) );
  EXPECT_EQ(  0, cover1.literal_num(~lit8) );
  EXPECT_EQ(  0, cover1.literal_num( lit9) );
  EXPECT_EQ(  0, cover1.literal_num(~lit9) );

  auto cube_list1 = cover1.literal_list();
  EXPECT_EQ( 1, cube_list1.size() );
  EXPECT_EQ( 2, cube_list1[0].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit1, cube_list1[0][1] );

};

TEST_F(SopTest, cover_cover_sum)
{

  SopCover src1{nv, { {lit0, lit1} } };
  SopCover src2{nv, { {lit2, lit3} } };

  SopCover cover1 = src1 + src2;

  EXPECT_EQ( nv, cover1.variable_num() );
  EXPECT_EQ(  2, cover1.cube_num() );
  EXPECT_EQ(  4, cover1.literal_num() );
  EXPECT_EQ(  1, cover1.literal_num( lit0) );
  EXPECT_EQ(  0, cover1.literal_num(~lit0) );
  EXPECT_EQ(  1, cover1.literal_num( lit1) );
  EXPECT_EQ(  0, cover1.literal_num(~lit1) );
  EXPECT_EQ(  1, cover1.literal_num( lit2) );
  EXPECT_EQ(  0, cover1.literal_num(~lit2) );
  EXPECT_EQ(  1, cover1.literal_num( lit3) );
  EXPECT_EQ(  0, cover1.literal_num(~lit3) );
  EXPECT_EQ(  0, cover1.literal_num( lit4) );
  EXPECT_EQ(  0, cover1.literal_num(~lit4) );
  EXPECT_EQ(  0, cover1.literal_num( lit5) );
  EXPECT_EQ(  0, cover1.literal_num(~lit5) );
  EXPECT_EQ(  0, cover1.literal_num( lit6) );
  EXPECT_EQ(  0, cover1.literal_num(~lit6) );
  EXPECT_EQ(  0, cover1.literal_num( lit7) );
  EXPECT_EQ(  0, cover1.literal_num(~lit7) );
  EXPECT_EQ(  0, cover1.literal_num( lit8) );
  EXPECT_EQ(  0, cover1.literal_num(~lit8) );
  EXPECT_EQ(  0, cover1.literal_num( lit9) );
  EXPECT_EQ(  0, cover1.literal_num(~lit9) );

  auto tmp_list = cover1.literal_list();
  EXPECT_EQ( 2, tmp_list.size() );
  EXPECT_EQ( 2, tmp_list[0].size() );
  EXPECT_EQ( 2, tmp_list[1].size() );
  EXPECT_EQ( lit0, tmp_list[0][0] );
  EXPECT_EQ( lit1, tmp_list[0][1] );
  EXPECT_EQ( lit2, tmp_list[1][0] );
  EXPECT_EQ( lit3, tmp_list[1][1] );

};

TEST_F(SopTest, cover_cube_sum)
{
  Literal lit0(var0, false);
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  SopCover src1{nv, { {lit0, lit1} } };
  SopCube src2{nv, {lit2, lit3} };

  SopCover cover1 = src1 + src2;

  EXPECT_EQ( nv, cover1.variable_num() );
  EXPECT_EQ(  2, cover1.cube_num() );
  EXPECT_EQ(  4, cover1.literal_num() );
  EXPECT_EQ(  1, cover1.literal_num( lit0) );
  EXPECT_EQ(  0, cover1.literal_num(~lit0) );
  EXPECT_EQ(  1, cover1.literal_num( lit1) );
  EXPECT_EQ(  0, cover1.literal_num(~lit1) );
  EXPECT_EQ(  1, cover1.literal_num( lit2) );
  EXPECT_EQ(  0, cover1.literal_num(~lit2) );
  EXPECT_EQ(  1, cover1.literal_num( lit3) );
  EXPECT_EQ(  0, cover1.literal_num(~lit3) );
  EXPECT_EQ(  0, cover1.literal_num( lit4) );
  EXPECT_EQ(  0, cover1.literal_num(~lit4) );
  EXPECT_EQ(  0, cover1.literal_num( lit5) );
  EXPECT_EQ(  0, cover1.literal_num(~lit5) );
  EXPECT_EQ(  0, cover1.literal_num( lit6) );
  EXPECT_EQ(  0, cover1.literal_num(~lit6) );
  EXPECT_EQ(  0, cover1.literal_num( lit7) );
  EXPECT_EQ(  0, cover1.literal_num(~lit7) );
  EXPECT_EQ(  0, cover1.literal_num( lit8) );
  EXPECT_EQ(  0, cover1.literal_num(~lit8) );
  EXPECT_EQ(  0, cover1.literal_num( lit9) );
  EXPECT_EQ(  0, cover1.literal_num(~lit9) );

  auto tmp_list = cover1.literal_list();
  EXPECT_EQ( 2, tmp_list.size() );
  EXPECT_EQ( 2, tmp_list[0].size() );
  EXPECT_EQ( 2, tmp_list[1].size() );
  EXPECT_EQ( lit0, tmp_list[0][0] );
  EXPECT_EQ( lit1, tmp_list[0][1] );
  EXPECT_EQ( lit2, tmp_list[1][0] );
  EXPECT_EQ( lit3, tmp_list[1][1] );

};

TEST_F(SopTest, cube_cover_sum)
{
  Literal lit0(var0, false);
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  SopCube src1{nv, {lit0, lit1} };
  SopCover src2{nv, { {lit2, lit3} } };

  SopCover cover1 = src1 + src2;

  EXPECT_EQ( nv, cover1.variable_num() );
  EXPECT_EQ(  2, cover1.cube_num() );
  EXPECT_EQ(  4, cover1.literal_num() );
  EXPECT_EQ(  1, cover1.literal_num( lit0) );
  EXPECT_EQ(  0, cover1.literal_num(~lit0) );
  EXPECT_EQ(  1, cover1.literal_num( lit1) );
  EXPECT_EQ(  0, cover1.literal_num(~lit1) );
  EXPECT_EQ(  1, cover1.literal_num( lit2) );
  EXPECT_EQ(  0, cover1.literal_num(~lit2) );
  EXPECT_EQ(  1, cover1.literal_num( lit3) );
  EXPECT_EQ(  0, cover1.literal_num(~lit3) );
  EXPECT_EQ(  0, cover1.literal_num( lit4) );
  EXPECT_EQ(  0, cover1.literal_num(~lit4) );
  EXPECT_EQ(  0, cover1.literal_num( lit5) );
  EXPECT_EQ(  0, cover1.literal_num(~lit5) );
  EXPECT_EQ(  0, cover1.literal_num( lit6) );
  EXPECT_EQ(  0, cover1.literal_num(~lit6) );
  EXPECT_EQ(  0, cover1.literal_num( lit7) );
  EXPECT_EQ(  0, cover1.literal_num(~lit7) );
  EXPECT_EQ(  0, cover1.literal_num( lit8) );
  EXPECT_EQ(  0, cover1.literal_num(~lit8) );
  EXPECT_EQ(  0, cover1.literal_num( lit9) );
  EXPECT_EQ(  0, cover1.literal_num(~lit9) );

  auto tmp_list = cover1.literal_list();
  EXPECT_EQ( 2, tmp_list.size() );
  EXPECT_EQ( 2, tmp_list[0].size() );
  EXPECT_EQ( 2, tmp_list[1].size() );
  EXPECT_EQ( lit0, tmp_list[0][0] );
  EXPECT_EQ( lit1, tmp_list[0][1] );
  EXPECT_EQ( lit2, tmp_list[1][0] );
  EXPECT_EQ( lit3, tmp_list[1][1] );

};

TEST_F(SopTest, Scover_cover_sum)
{
  Literal lit0(var0, false);
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  SopCover cover1{nv, { {lit0, lit1} } };
  SopCover src2{nv, { {lit2, lit3} } };

  cover1 = cover1 + src2;

  EXPECT_EQ( nv, cover1.variable_num() );
  EXPECT_EQ(  2, cover1.cube_num() );
  EXPECT_EQ(  4, cover1.literal_num() );
  EXPECT_EQ(  1, cover1.literal_num( lit0) );
  EXPECT_EQ(  0, cover1.literal_num(~lit0) );
  EXPECT_EQ(  1, cover1.literal_num( lit1) );
  EXPECT_EQ(  0, cover1.literal_num(~lit1) );
  EXPECT_EQ(  1, cover1.literal_num( lit2) );
  EXPECT_EQ(  0, cover1.literal_num(~lit2) );
  EXPECT_EQ(  1, cover1.literal_num( lit3) );
  EXPECT_EQ(  0, cover1.literal_num(~lit3) );
  EXPECT_EQ(  0, cover1.literal_num( lit4) );
  EXPECT_EQ(  0, cover1.literal_num(~lit4) );
  EXPECT_EQ(  0, cover1.literal_num( lit5) );
  EXPECT_EQ(  0, cover1.literal_num(~lit5) );
  EXPECT_EQ(  0, cover1.literal_num( lit6) );
  EXPECT_EQ(  0, cover1.literal_num(~lit6) );
  EXPECT_EQ(  0, cover1.literal_num( lit7) );
  EXPECT_EQ(  0, cover1.literal_num(~lit7) );
  EXPECT_EQ(  0, cover1.literal_num( lit8) );
  EXPECT_EQ(  0, cover1.literal_num(~lit8) );
  EXPECT_EQ(  0, cover1.literal_num( lit9) );
  EXPECT_EQ(  0, cover1.literal_num(~lit9) );

  auto tmp_list = cover1.literal_list();
  EXPECT_EQ( 2, tmp_list.size() );
  EXPECT_EQ( 2, tmp_list[0].size() );
  EXPECT_EQ( 2, tmp_list[1].size() );
  EXPECT_EQ( lit0, tmp_list[0][0] );
  EXPECT_EQ( lit1, tmp_list[0][1] );
  EXPECT_EQ( lit2, tmp_list[1][0] );
  EXPECT_EQ( lit3, tmp_list[1][1] );

};

TEST_F(SopTest, Scover_cube_sum1)
{
  Literal lit0(var0, false);
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  SopCover cover1{nv, { {lit0, lit1} }};
  SopCube src2{nv, {lit2, lit3}};

  cover1 = cover1 + src2;

  EXPECT_EQ( nv, cover1.variable_num() );
  EXPECT_EQ(  2, cover1.cube_num() );
  EXPECT_EQ(  4, cover1.literal_num() );
  EXPECT_EQ(  1, cover1.literal_num( lit0) );
  EXPECT_EQ(  0, cover1.literal_num(~lit0) );
  EXPECT_EQ(  1, cover1.literal_num( lit1) );
  EXPECT_EQ(  0, cover1.literal_num(~lit1) );
  EXPECT_EQ(  1, cover1.literal_num( lit2) );
  EXPECT_EQ(  0, cover1.literal_num(~lit2) );
  EXPECT_EQ(  1, cover1.literal_num( lit3) );
  EXPECT_EQ(  0, cover1.literal_num(~lit3) );
  EXPECT_EQ(  0, cover1.literal_num( lit4) );
  EXPECT_EQ(  0, cover1.literal_num(~lit4) );
  EXPECT_EQ(  0, cover1.literal_num( lit5) );
  EXPECT_EQ(  0, cover1.literal_num(~lit5) );
  EXPECT_EQ(  0, cover1.literal_num( lit6) );
  EXPECT_EQ(  0, cover1.literal_num(~lit6) );
  EXPECT_EQ(  0, cover1.literal_num( lit7) );
  EXPECT_EQ(  0, cover1.literal_num(~lit7) );
  EXPECT_EQ(  0, cover1.literal_num( lit8) );
  EXPECT_EQ(  0, cover1.literal_num(~lit8) );
  EXPECT_EQ(  0, cover1.literal_num( lit9) );
  EXPECT_EQ(  0, cover1.literal_num(~lit9) );

  auto tmp_list = cover1.literal_list();
  EXPECT_EQ( 2, tmp_list.size() );
  EXPECT_EQ( 2, tmp_list[0].size() );
  EXPECT_EQ( 2, tmp_list[1].size() );
  EXPECT_EQ( lit0, tmp_list[0][0] );
  EXPECT_EQ( lit1, tmp_list[0][1] );
  EXPECT_EQ( lit2, tmp_list[1][0] );
  EXPECT_EQ( lit3, tmp_list[1][1] );

};

TEST_F(SopTest, cube_Scover_sum)
{
  Literal lit0(var0, false);
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  SopCube src1{nv, {lit0, lit1} };
  SopCover cover1{nv, { {lit2, lit3} } };

  cover1 = src1 + cover1;

  EXPECT_EQ( nv, cover1.variable_num() );
  EXPECT_EQ(  2, cover1.cube_num() );
  EXPECT_EQ(  4, cover1.literal_num() );
  EXPECT_EQ(  1, cover1.literal_num( lit0) );
  EXPECT_EQ(  0, cover1.literal_num(~lit0) );
  EXPECT_EQ(  1, cover1.literal_num( lit1) );
  EXPECT_EQ(  0, cover1.literal_num(~lit1) );
  EXPECT_EQ(  1, cover1.literal_num( lit2) );
  EXPECT_EQ(  0, cover1.literal_num(~lit2) );
  EXPECT_EQ(  1, cover1.literal_num( lit3) );
  EXPECT_EQ(  0, cover1.literal_num(~lit3) );
  EXPECT_EQ(  0, cover1.literal_num( lit4) );
  EXPECT_EQ(  0, cover1.literal_num(~lit4) );
  EXPECT_EQ(  0, cover1.literal_num( lit5) );
  EXPECT_EQ(  0, cover1.literal_num(~lit5) );
  EXPECT_EQ(  0, cover1.literal_num( lit6) );
  EXPECT_EQ(  0, cover1.literal_num(~lit6) );
  EXPECT_EQ(  0, cover1.literal_num( lit7) );
  EXPECT_EQ(  0, cover1.literal_num(~lit7) );
  EXPECT_EQ(  0, cover1.literal_num( lit8) );
  EXPECT_EQ(  0, cover1.literal_num(~lit8) );
  EXPECT_EQ(  0, cover1.literal_num( lit9) );
  EXPECT_EQ(  0, cover1.literal_num(~lit9) );

  auto tmp_list = cover1.literal_list();
  EXPECT_EQ( 2, tmp_list.size() );
  EXPECT_EQ( 2, tmp_list[0].size() );
  EXPECT_EQ( 2, tmp_list[1].size() );
  EXPECT_EQ( lit0, tmp_list[0][0] );
  EXPECT_EQ( lit1, tmp_list[0][1] );
  EXPECT_EQ( lit2, tmp_list[1][0] );
  EXPECT_EQ( lit3, tmp_list[1][1] );

};

TEST_F(SopTest, Icover_cover_sum)
{
  Literal lit0(var0, false);
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  SopCover cover1{nv, { {lit0, lit1} } };
  SopCover src2{nv, { {lit2, lit3} } };

  cover1 += src2;

  EXPECT_EQ( nv, cover1.variable_num() );
  EXPECT_EQ(  2, cover1.cube_num() );
  EXPECT_EQ(  4, cover1.literal_num() );
  EXPECT_EQ(  1, cover1.literal_num( lit0) );
  EXPECT_EQ(  0, cover1.literal_num(~lit0) );
  EXPECT_EQ(  1, cover1.literal_num( lit1) );
  EXPECT_EQ(  0, cover1.literal_num(~lit1) );
  EXPECT_EQ(  1, cover1.literal_num( lit2) );
  EXPECT_EQ(  0, cover1.literal_num(~lit2) );
  EXPECT_EQ(  1, cover1.literal_num( lit3) );
  EXPECT_EQ(  0, cover1.literal_num(~lit3) );
  EXPECT_EQ(  0, cover1.literal_num( lit4) );
  EXPECT_EQ(  0, cover1.literal_num(~lit4) );
  EXPECT_EQ(  0, cover1.literal_num( lit5) );
  EXPECT_EQ(  0, cover1.literal_num(~lit5) );
  EXPECT_EQ(  0, cover1.literal_num( lit6) );
  EXPECT_EQ(  0, cover1.literal_num(~lit6) );
  EXPECT_EQ(  0, cover1.literal_num( lit7) );
  EXPECT_EQ(  0, cover1.literal_num(~lit7) );
  EXPECT_EQ(  0, cover1.literal_num( lit8) );
  EXPECT_EQ(  0, cover1.literal_num(~lit8) );
  EXPECT_EQ(  0, cover1.literal_num( lit9) );
  EXPECT_EQ(  0, cover1.literal_num(~lit9) );

  auto tmp_list = cover1.literal_list();
  EXPECT_EQ( 2, tmp_list.size() );
  EXPECT_EQ( 2, tmp_list[0].size() );
  EXPECT_EQ( 2, tmp_list[1].size() );
  EXPECT_EQ( lit0, tmp_list[0][0] );
  EXPECT_EQ( lit1, tmp_list[0][1] );
  EXPECT_EQ( lit2, tmp_list[1][0] );
  EXPECT_EQ( lit3, tmp_list[1][1] );

};

TEST_F(SopTest, Icover_cube_sum)
{
  Literal lit0(var0, false);
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  SopCover cover1{nv, { {lit0, lit1} }};
  SopCube src2{nv, {lit2, lit3}};

  cover1 += src2;

  EXPECT_EQ( nv, cover1.variable_num() );
  EXPECT_EQ(  2, cover1.cube_num() );
  EXPECT_EQ(  4, cover1.literal_num() );
  EXPECT_EQ(  1, cover1.literal_num( lit0) );
  EXPECT_EQ(  0, cover1.literal_num(~lit0) );
  EXPECT_EQ(  1, cover1.literal_num( lit1) );
  EXPECT_EQ(  0, cover1.literal_num(~lit1) );
  EXPECT_EQ(  1, cover1.literal_num( lit2) );
  EXPECT_EQ(  0, cover1.literal_num(~lit2) );
  EXPECT_EQ(  1, cover1.literal_num( lit3) );
  EXPECT_EQ(  0, cover1.literal_num(~lit3) );
  EXPECT_EQ(  0, cover1.literal_num( lit4) );
  EXPECT_EQ(  0, cover1.literal_num(~lit4) );
  EXPECT_EQ(  0, cover1.literal_num( lit5) );
  EXPECT_EQ(  0, cover1.literal_num(~lit5) );
  EXPECT_EQ(  0, cover1.literal_num( lit6) );
  EXPECT_EQ(  0, cover1.literal_num(~lit6) );
  EXPECT_EQ(  0, cover1.literal_num( lit7) );
  EXPECT_EQ(  0, cover1.literal_num(~lit7) );
  EXPECT_EQ(  0, cover1.literal_num( lit8) );
  EXPECT_EQ(  0, cover1.literal_num(~lit8) );
  EXPECT_EQ(  0, cover1.literal_num( lit9) );
  EXPECT_EQ(  0, cover1.literal_num(~lit9) );

  auto tmp_list = cover1.literal_list();
  EXPECT_EQ( 2, tmp_list.size() );
  EXPECT_EQ( 2, tmp_list[0].size() );
  EXPECT_EQ( 2, tmp_list[1].size() );
  EXPECT_EQ( lit0, tmp_list[0][0] );
  EXPECT_EQ( lit1, tmp_list[0][1] );
  EXPECT_EQ( lit2, tmp_list[1][0] );
  EXPECT_EQ( lit3, tmp_list[1][1] );

};

TEST_F(SopTest, cover_cover_diff)
{
  Literal lit0(var0, false);
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  SopCover src1{nv, { {lit0, lit1}, {lit2, lit3} } };
  SopCover src2{nv, { {lit3, lit2}, {lit0, ~lit1} } };

  SopCover cover1 = src1 - src2;

  EXPECT_EQ( nv, cover1.variable_num() );
  EXPECT_EQ(  1, cover1.cube_num() );
  EXPECT_EQ(  2, cover1.literal_num() );
  EXPECT_EQ(  1, cover1.literal_num( lit0) );
  EXPECT_EQ(  0, cover1.literal_num(~lit0) );
  EXPECT_EQ(  1, cover1.literal_num( lit1) );
  EXPECT_EQ(  0, cover1.literal_num(~lit1) );
  EXPECT_EQ(  0, cover1.literal_num( lit2) );
  EXPECT_EQ(  0, cover1.literal_num(~lit2) );
  EXPECT_EQ(  0, cover1.literal_num( lit3) );
  EXPECT_EQ(  0, cover1.literal_num(~lit3) );
  EXPECT_EQ(  0, cover1.literal_num( lit4) );
  EXPECT_EQ(  0, cover1.literal_num(~lit4) );
  EXPECT_EQ(  0, cover1.literal_num( lit5) );
  EXPECT_EQ(  0, cover1.literal_num(~lit5) );
  EXPECT_EQ(  0, cover1.literal_num( lit6) );
  EXPECT_EQ(  0, cover1.literal_num(~lit6) );
  EXPECT_EQ(  0, cover1.literal_num( lit7) );
  EXPECT_EQ(  0, cover1.literal_num(~lit7) );
  EXPECT_EQ(  0, cover1.literal_num( lit8) );
  EXPECT_EQ(  0, cover1.literal_num(~lit8) );
  EXPECT_EQ(  0, cover1.literal_num( lit9) );
  EXPECT_EQ(  0, cover1.literal_num(~lit9) );

  auto cube_list1 = cover1.literal_list();
  EXPECT_EQ( 1, cube_list1.size() );
  EXPECT_EQ( 2, cube_list1[0].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit1, cube_list1[0][1] );

};

TEST_F(SopTest, Rcover_cover_diff)
{
  Literal lit0(var0, false);
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  SopCover src1{nv, { {lit0, lit1}, {lit2, lit3} } };
  SopCover src2{nv, { {lit3, lit2}, {lit0, ~lit1} } };

  SopCover cover1 = std::move(src1) - src2;

  EXPECT_EQ( 0, src1.cube_num() );

  EXPECT_EQ( nv, cover1.variable_num() );
  EXPECT_EQ(  1, cover1.cube_num() );
  EXPECT_EQ(  2, cover1.literal_num() );
  EXPECT_EQ(  1, cover1.literal_num( lit0) );
  EXPECT_EQ(  0, cover1.literal_num(~lit0) );
  EXPECT_EQ(  1, cover1.literal_num( lit1) );
  EXPECT_EQ(  0, cover1.literal_num(~lit1) );
  EXPECT_EQ(  0, cover1.literal_num( lit2) );
  EXPECT_EQ(  0, cover1.literal_num(~lit2) );
  EXPECT_EQ(  0, cover1.literal_num( lit3) );
  EXPECT_EQ(  0, cover1.literal_num(~lit3) );
  EXPECT_EQ(  0, cover1.literal_num( lit4) );
  EXPECT_EQ(  0, cover1.literal_num(~lit4) );
  EXPECT_EQ(  0, cover1.literal_num( lit5) );
  EXPECT_EQ(  0, cover1.literal_num(~lit5) );
  EXPECT_EQ(  0, cover1.literal_num( lit6) );
  EXPECT_EQ(  0, cover1.literal_num(~lit6) );
  EXPECT_EQ(  0, cover1.literal_num( lit7) );
  EXPECT_EQ(  0, cover1.literal_num(~lit7) );
  EXPECT_EQ(  0, cover1.literal_num( lit8) );
  EXPECT_EQ(  0, cover1.literal_num(~lit8) );
  EXPECT_EQ(  0, cover1.literal_num( lit9) );
  EXPECT_EQ(  0, cover1.literal_num(~lit9) );

  auto cube_list1 = cover1.literal_list();
  EXPECT_EQ( 1, cube_list1.size() );
  EXPECT_EQ( 2, cube_list1[0].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit1, cube_list1[0][1] );

};

TEST_F(SopTest, cover_cube_diff)
{
  Literal lit0(var0, false);
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  SopCover src1{nv, { {lit0, lit1}, {lit2, lit3} } };
  SopCube src2{nv, {lit3, lit2} };

  SopCover cover1 = src1 - src2;

  EXPECT_EQ( nv, cover1.variable_num() );
  EXPECT_EQ(  1, cover1.cube_num() );
  EXPECT_EQ(  2, cover1.literal_num() );
  EXPECT_EQ(  1, cover1.literal_num( lit0) );
  EXPECT_EQ(  0, cover1.literal_num(~lit0) );
  EXPECT_EQ(  1, cover1.literal_num( lit1) );
  EXPECT_EQ(  0, cover1.literal_num(~lit1) );
  EXPECT_EQ(  0, cover1.literal_num( lit2) );
  EXPECT_EQ(  0, cover1.literal_num(~lit2) );
  EXPECT_EQ(  0, cover1.literal_num( lit3) );
  EXPECT_EQ(  0, cover1.literal_num(~lit3) );
  EXPECT_EQ(  0, cover1.literal_num( lit4) );
  EXPECT_EQ(  0, cover1.literal_num(~lit4) );
  EXPECT_EQ(  0, cover1.literal_num( lit5) );
  EXPECT_EQ(  0, cover1.literal_num(~lit5) );
  EXPECT_EQ(  0, cover1.literal_num( lit6) );
  EXPECT_EQ(  0, cover1.literal_num(~lit6) );
  EXPECT_EQ(  0, cover1.literal_num( lit7) );
  EXPECT_EQ(  0, cover1.literal_num(~lit7) );
  EXPECT_EQ(  0, cover1.literal_num( lit8) );
  EXPECT_EQ(  0, cover1.literal_num(~lit8) );
  EXPECT_EQ(  0, cover1.literal_num( lit9) );
  EXPECT_EQ(  0, cover1.literal_num(~lit9) );

  auto cube_list1 = cover1.literal_list();
  EXPECT_EQ( 1, cube_list1.size() );
  EXPECT_EQ( 2, cube_list1[0].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit1, cube_list1[0][1] );

};

TEST_F(SopTest, Scover_cover_diff)
{
  Literal lit0(var0, false);
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  SopCover cover1{nv, { {lit0, lit1}, {lit2, lit3} } };
  SopCover src2{nv, { {lit3, lit2}, {lit0, ~lit1} } };

  cover1 = cover1 - src2;

  EXPECT_EQ( nv, cover1.variable_num() );
  EXPECT_EQ(  1, cover1.cube_num() );
  EXPECT_EQ(  2, cover1.literal_num() );
  EXPECT_EQ(  1, cover1.literal_num( lit0) );
  EXPECT_EQ(  0, cover1.literal_num(~lit0) );
  EXPECT_EQ(  1, cover1.literal_num( lit1) );
  EXPECT_EQ(  0, cover1.literal_num(~lit1) );
  EXPECT_EQ(  0, cover1.literal_num( lit2) );
  EXPECT_EQ(  0, cover1.literal_num(~lit2) );
  EXPECT_EQ(  0, cover1.literal_num( lit3) );
  EXPECT_EQ(  0, cover1.literal_num(~lit3) );
  EXPECT_EQ(  0, cover1.literal_num( lit4) );
  EXPECT_EQ(  0, cover1.literal_num(~lit4) );
  EXPECT_EQ(  0, cover1.literal_num( lit5) );
  EXPECT_EQ(  0, cover1.literal_num(~lit5) );
  EXPECT_EQ(  0, cover1.literal_num( lit6) );
  EXPECT_EQ(  0, cover1.literal_num(~lit6) );
  EXPECT_EQ(  0, cover1.literal_num( lit7) );
  EXPECT_EQ(  0, cover1.literal_num(~lit7) );
  EXPECT_EQ(  0, cover1.literal_num( lit8) );
  EXPECT_EQ(  0, cover1.literal_num(~lit8) );
  EXPECT_EQ(  0, cover1.literal_num( lit9) );
  EXPECT_EQ(  0, cover1.literal_num(~lit9) );

  auto cube_list1 = cover1.literal_list();
  EXPECT_EQ( 1, cube_list1.size() );
  EXPECT_EQ( 2, cube_list1[0].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit1, cube_list1[0][1] );

};

TEST_F(SopTest, Scover_cube_diff)
{
  Literal lit0(var0, false);
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  SopCover cover1{nv, { {lit0, lit1}, {lit2, lit3} } };
  SopCube src2{nv, {lit3, lit2} };

  cover1 = cover1 - src2;

  EXPECT_EQ( nv, cover1.variable_num() );
  EXPECT_EQ(  1, cover1.cube_num() );
  EXPECT_EQ(  2, cover1.literal_num() );
  EXPECT_EQ(  1, cover1.literal_num( lit0) );
  EXPECT_EQ(  0, cover1.literal_num(~lit0) );
  EXPECT_EQ(  1, cover1.literal_num( lit1) );
  EXPECT_EQ(  0, cover1.literal_num(~lit1) );
  EXPECT_EQ(  0, cover1.literal_num( lit2) );
  EXPECT_EQ(  0, cover1.literal_num(~lit2) );
  EXPECT_EQ(  0, cover1.literal_num( lit3) );
  EXPECT_EQ(  0, cover1.literal_num(~lit3) );
  EXPECT_EQ(  0, cover1.literal_num( lit4) );
  EXPECT_EQ(  0, cover1.literal_num(~lit4) );
  EXPECT_EQ(  0, cover1.literal_num( lit5) );
  EXPECT_EQ(  0, cover1.literal_num(~lit5) );
  EXPECT_EQ(  0, cover1.literal_num( lit6) );
  EXPECT_EQ(  0, cover1.literal_num(~lit6) );
  EXPECT_EQ(  0, cover1.literal_num( lit7) );
  EXPECT_EQ(  0, cover1.literal_num(~lit7) );
  EXPECT_EQ(  0, cover1.literal_num( lit8) );
  EXPECT_EQ(  0, cover1.literal_num(~lit8) );
  EXPECT_EQ(  0, cover1.literal_num( lit9) );
  EXPECT_EQ(  0, cover1.literal_num(~lit9) );

  auto cube_list1 = cover1.literal_list();
  EXPECT_EQ( 1, cube_list1.size() );
  EXPECT_EQ( 2, cube_list1[0].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit1, cube_list1[0][1] );

};

TEST_F(SopTest, Icover_cover_diff)
{
  Literal lit0(var0, false);
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  SopCover cover1{nv, { {lit0, lit1}, {lit2, lit3} } };
  SopCover src2{nv, { {lit3, lit2}, {lit0, ~lit1} } };

  cover1 -= src2;

  EXPECT_EQ( nv, cover1.variable_num() );
  EXPECT_EQ(  1, cover1.cube_num() );
  EXPECT_EQ(  2, cover1.literal_num() );
  EXPECT_EQ(  1, cover1.literal_num( lit0) );
  EXPECT_EQ(  0, cover1.literal_num(~lit0) );
  EXPECT_EQ(  1, cover1.literal_num( lit1) );
  EXPECT_EQ(  0, cover1.literal_num(~lit1) );
  EXPECT_EQ(  0, cover1.literal_num( lit2) );
  EXPECT_EQ(  0, cover1.literal_num(~lit2) );
  EXPECT_EQ(  0, cover1.literal_num( lit3) );
  EXPECT_EQ(  0, cover1.literal_num(~lit3) );
  EXPECT_EQ(  0, cover1.literal_num( lit4) );
  EXPECT_EQ(  0, cover1.literal_num(~lit4) );
  EXPECT_EQ(  0, cover1.literal_num( lit5) );
  EXPECT_EQ(  0, cover1.literal_num(~lit5) );
  EXPECT_EQ(  0, cover1.literal_num( lit6) );
  EXPECT_EQ(  0, cover1.literal_num(~lit6) );
  EXPECT_EQ(  0, cover1.literal_num( lit7) );
  EXPECT_EQ(  0, cover1.literal_num(~lit7) );
  EXPECT_EQ(  0, cover1.literal_num( lit8) );
  EXPECT_EQ(  0, cover1.literal_num(~lit8) );
  EXPECT_EQ(  0, cover1.literal_num( lit9) );
  EXPECT_EQ(  0, cover1.literal_num(~lit9) );

  auto cube_list1 = cover1.literal_list();
  EXPECT_EQ( 1, cube_list1.size() );
  EXPECT_EQ( 2, cube_list1[0].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit1, cube_list1[0][1] );

};

TEST_F(SopTest, Icover_cube_diff)
{
  Literal lit0(var0, false);
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  SopCover cover1{nv, { {lit0, lit1}, {lit2, lit3} } };
  SopCube src2{nv, {lit3, lit2} };

  cover1 -= src2;

  EXPECT_EQ( nv, cover1.variable_num() );
  EXPECT_EQ(  1, cover1.cube_num() );
  EXPECT_EQ(  2, cover1.literal_num() );
  EXPECT_EQ(  1, cover1.literal_num( lit0) );
  EXPECT_EQ(  0, cover1.literal_num(~lit0) );
  EXPECT_EQ(  1, cover1.literal_num( lit1) );
  EXPECT_EQ(  0, cover1.literal_num(~lit1) );
  EXPECT_EQ(  0, cover1.literal_num( lit2) );
  EXPECT_EQ(  0, cover1.literal_num(~lit2) );
  EXPECT_EQ(  0, cover1.literal_num( lit3) );
  EXPECT_EQ(  0, cover1.literal_num(~lit3) );
  EXPECT_EQ(  0, cover1.literal_num( lit4) );
  EXPECT_EQ(  0, cover1.literal_num(~lit4) );
  EXPECT_EQ(  0, cover1.literal_num( lit5) );
  EXPECT_EQ(  0, cover1.literal_num(~lit5) );
  EXPECT_EQ(  0, cover1.literal_num( lit6) );
  EXPECT_EQ(  0, cover1.literal_num(~lit6) );
  EXPECT_EQ(  0, cover1.literal_num( lit7) );
  EXPECT_EQ(  0, cover1.literal_num(~lit7) );
  EXPECT_EQ(  0, cover1.literal_num( lit8) );
  EXPECT_EQ(  0, cover1.literal_num(~lit8) );
  EXPECT_EQ(  0, cover1.literal_num( lit9) );
  EXPECT_EQ(  0, cover1.literal_num(~lit9) );

  auto cube_list1 = cover1.literal_list();
  EXPECT_EQ( 1, cube_list1.size() );
  EXPECT_EQ( 2, cube_list1[0].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit1, cube_list1[0][1] );

};

TEST_F(SopTest, cover_cover_product)
{
  Literal lit0(var0, false);
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  SopCover src1{nv, { {lit0}, {lit1} } };
  SopCover src2{nv, { {lit2}, {lit3} } };

  SopCover cover1 = src1 * src2;

  EXPECT_EQ( nv, cover1.variable_num() );
  EXPECT_EQ(  4, cover1.cube_num() );
  EXPECT_EQ(  8, cover1.literal_num() );
  EXPECT_EQ(  2, cover1.literal_num( lit0) );
  EXPECT_EQ(  0, cover1.literal_num(~lit0) );
  EXPECT_EQ(  2, cover1.literal_num( lit1) );
  EXPECT_EQ(  0, cover1.literal_num(~lit1) );
  EXPECT_EQ(  2, cover1.literal_num( lit2) );
  EXPECT_EQ(  0, cover1.literal_num(~lit2) );
  EXPECT_EQ(  2, cover1.literal_num( lit3) );
  EXPECT_EQ(  0, cover1.literal_num(~lit3) );
  EXPECT_EQ(  0, cover1.literal_num( lit4) );
  EXPECT_EQ(  0, cover1.literal_num(~lit4) );
  EXPECT_EQ(  0, cover1.literal_num( lit5) );
  EXPECT_EQ(  0, cover1.literal_num(~lit5) );
  EXPECT_EQ(  0, cover1.literal_num( lit6) );
  EXPECT_EQ(  0, cover1.literal_num(~lit6) );
  EXPECT_EQ(  0, cover1.literal_num( lit7) );
  EXPECT_EQ(  0, cover1.literal_num(~lit7) );
  EXPECT_EQ(  0, cover1.literal_num( lit8) );
  EXPECT_EQ(  0, cover1.literal_num(~lit8) );
  EXPECT_EQ(  0, cover1.literal_num( lit9) );
  EXPECT_EQ(  0, cover1.literal_num(~lit9) );

  auto cube_list1 = cover1.literal_list();
  EXPECT_EQ( 4, cube_list1.size() );
  EXPECT_EQ( 2, cube_list1[0].size() );
  EXPECT_EQ( 2, cube_list1[1].size() );
  EXPECT_EQ( 2, cube_list1[2].size() );
  EXPECT_EQ( 2, cube_list1[3].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit2, cube_list1[0][1] );
  EXPECT_EQ( lit0, cube_list1[1][0] );
  EXPECT_EQ( lit3, cube_list1[1][1] );
  EXPECT_EQ( lit1, cube_list1[2][0] );
  EXPECT_EQ( lit2, cube_list1[2][1] );
  EXPECT_EQ( lit1, cube_list1[3][0] );
  EXPECT_EQ( lit3, cube_list1[3][1] );

};

TEST_F(SopTest, cover_cube_product)
{
  Literal lit0(var0, false);
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  SopCover src1{nv, { {lit0}, {lit1} } };
  SopCube src2{nv,  {lit2} };

  SopCover cover1 = src1 * src2;

  EXPECT_EQ( nv, cover1.variable_num() );
  EXPECT_EQ(  2, cover1.cube_num() );
  EXPECT_EQ(  4, cover1.literal_num() );
  EXPECT_EQ(  1, cover1.literal_num( lit0) );
  EXPECT_EQ(  0, cover1.literal_num(~lit0) );
  EXPECT_EQ(  1, cover1.literal_num( lit1) );
  EXPECT_EQ(  0, cover1.literal_num(~lit1) );
  EXPECT_EQ(  2, cover1.literal_num( lit2) );
  EXPECT_EQ(  0, cover1.literal_num(~lit2) );
  EXPECT_EQ(  0, cover1.literal_num( lit3) );
  EXPECT_EQ(  0, cover1.literal_num(~lit3) );
  EXPECT_EQ(  0, cover1.literal_num( lit4) );
  EXPECT_EQ(  0, cover1.literal_num(~lit4) );
  EXPECT_EQ(  0, cover1.literal_num( lit5) );
  EXPECT_EQ(  0, cover1.literal_num(~lit5) );
  EXPECT_EQ(  0, cover1.literal_num( lit6) );
  EXPECT_EQ(  0, cover1.literal_num(~lit6) );
  EXPECT_EQ(  0, cover1.literal_num( lit7) );
  EXPECT_EQ(  0, cover1.literal_num(~lit7) );
  EXPECT_EQ(  0, cover1.literal_num( lit8) );
  EXPECT_EQ(  0, cover1.literal_num(~lit8) );
  EXPECT_EQ(  0, cover1.literal_num( lit9) );
  EXPECT_EQ(  0, cover1.literal_num(~lit9) );

  auto cube_list1 = cover1.literal_list();
  EXPECT_EQ( 2, cube_list1.size() );
  EXPECT_EQ( 2, cube_list1[0].size() );
  EXPECT_EQ( 2, cube_list1[1].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit2, cube_list1[0][1] );
  EXPECT_EQ( lit1, cube_list1[1][0] );
  EXPECT_EQ( lit2, cube_list1[1][1] );

};

TEST_F(SopTest, Rcover_cube_product)
{
  Literal lit0(var0, false);
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  SopCover src1{nv, { {lit0}, {lit1} } };
  SopCube src2{nv,  {lit2} };

  SopCover cover1 = std::move(src1) * src2;

  EXPECT_EQ( 0, src1.cube_num() );

  EXPECT_EQ( nv, cover1.variable_num() );
  EXPECT_EQ(  2, cover1.cube_num() );
  EXPECT_EQ(  4, cover1.literal_num() );
  EXPECT_EQ(  1, cover1.literal_num( lit0) );
  EXPECT_EQ(  0, cover1.literal_num(~lit0) );
  EXPECT_EQ(  1, cover1.literal_num( lit1) );
  EXPECT_EQ(  0, cover1.literal_num(~lit1) );
  EXPECT_EQ(  2, cover1.literal_num( lit2) );
  EXPECT_EQ(  0, cover1.literal_num(~lit2) );
  EXPECT_EQ(  0, cover1.literal_num( lit3) );
  EXPECT_EQ(  0, cover1.literal_num(~lit3) );
  EXPECT_EQ(  0, cover1.literal_num( lit4) );
  EXPECT_EQ(  0, cover1.literal_num(~lit4) );
  EXPECT_EQ(  0, cover1.literal_num( lit5) );
  EXPECT_EQ(  0, cover1.literal_num(~lit5) );
  EXPECT_EQ(  0, cover1.literal_num( lit6) );
  EXPECT_EQ(  0, cover1.literal_num(~lit6) );
  EXPECT_EQ(  0, cover1.literal_num( lit7) );
  EXPECT_EQ(  0, cover1.literal_num(~lit7) );
  EXPECT_EQ(  0, cover1.literal_num( lit8) );
  EXPECT_EQ(  0, cover1.literal_num(~lit8) );
  EXPECT_EQ(  0, cover1.literal_num( lit9) );
  EXPECT_EQ(  0, cover1.literal_num(~lit9) );

  auto cube_list1 = cover1.literal_list();
  EXPECT_EQ( 2, cube_list1.size() );
  EXPECT_EQ( 2, cube_list1[0].size() );
  EXPECT_EQ( 2, cube_list1[1].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit2, cube_list1[0][1] );
  EXPECT_EQ( lit1, cube_list1[1][0] );
  EXPECT_EQ( lit2, cube_list1[1][1] );

};

TEST_F(SopTest, cube_cover_product)
{
  Literal lit0(var0, false);
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  SopCube src1{nv,  {lit2} };
  SopCover src2{nv, { {lit0}, {lit1} } };

  SopCover cover1 = src1 * src2;

  EXPECT_EQ( nv, cover1.variable_num() );
  EXPECT_EQ(  2, cover1.cube_num() );
  EXPECT_EQ(  4, cover1.literal_num() );
  EXPECT_EQ(  1, cover1.literal_num( lit0) );
  EXPECT_EQ(  0, cover1.literal_num(~lit0) );
  EXPECT_EQ(  1, cover1.literal_num( lit1) );
  EXPECT_EQ(  0, cover1.literal_num(~lit1) );
  EXPECT_EQ(  2, cover1.literal_num( lit2) );
  EXPECT_EQ(  0, cover1.literal_num(~lit2) );
  EXPECT_EQ(  0, cover1.literal_num( lit3) );
  EXPECT_EQ(  0, cover1.literal_num(~lit3) );
  EXPECT_EQ(  0, cover1.literal_num( lit4) );
  EXPECT_EQ(  0, cover1.literal_num(~lit4) );
  EXPECT_EQ(  0, cover1.literal_num( lit5) );
  EXPECT_EQ(  0, cover1.literal_num(~lit5) );
  EXPECT_EQ(  0, cover1.literal_num( lit6) );
  EXPECT_EQ(  0, cover1.literal_num(~lit6) );
  EXPECT_EQ(  0, cover1.literal_num( lit7) );
  EXPECT_EQ(  0, cover1.literal_num(~lit7) );
  EXPECT_EQ(  0, cover1.literal_num( lit8) );
  EXPECT_EQ(  0, cover1.literal_num(~lit8) );
  EXPECT_EQ(  0, cover1.literal_num( lit9) );
  EXPECT_EQ(  0, cover1.literal_num(~lit9) );

  auto cube_list1 = cover1.literal_list();
  EXPECT_EQ( 2, cube_list1.size() );
  EXPECT_EQ( 2, cube_list1[0].size() );
  EXPECT_EQ( 2, cube_list1[1].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit2, cube_list1[0][1] );
  EXPECT_EQ( lit1, cube_list1[1][0] );
  EXPECT_EQ( lit2, cube_list1[1][1] );

};

TEST_F(SopTest, cube_Rcover_product)
{
  Literal lit0(var0, false);
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  SopCube src1{nv,  {lit2} };
  SopCover src2{nv, { {lit0}, {lit1} } };

  SopCover cover1 = src1 * std::move(src2);

  EXPECT_EQ( 0, src2.cube_num() );

  EXPECT_EQ( nv, cover1.variable_num() );
  EXPECT_EQ(  2, cover1.cube_num() );
  EXPECT_EQ(  4, cover1.literal_num() );
  EXPECT_EQ(  1, cover1.literal_num( lit0) );
  EXPECT_EQ(  0, cover1.literal_num(~lit0) );
  EXPECT_EQ(  1, cover1.literal_num( lit1) );
  EXPECT_EQ(  0, cover1.literal_num(~lit1) );
  EXPECT_EQ(  2, cover1.literal_num( lit2) );
  EXPECT_EQ(  0, cover1.literal_num(~lit2) );
  EXPECT_EQ(  0, cover1.literal_num( lit3) );
  EXPECT_EQ(  0, cover1.literal_num(~lit3) );
  EXPECT_EQ(  0, cover1.literal_num( lit4) );
  EXPECT_EQ(  0, cover1.literal_num(~lit4) );
  EXPECT_EQ(  0, cover1.literal_num( lit5) );
  EXPECT_EQ(  0, cover1.literal_num(~lit5) );
  EXPECT_EQ(  0, cover1.literal_num( lit6) );
  EXPECT_EQ(  0, cover1.literal_num(~lit6) );
  EXPECT_EQ(  0, cover1.literal_num( lit7) );
  EXPECT_EQ(  0, cover1.literal_num(~lit7) );
  EXPECT_EQ(  0, cover1.literal_num( lit8) );
  EXPECT_EQ(  0, cover1.literal_num(~lit8) );
  EXPECT_EQ(  0, cover1.literal_num( lit9) );
  EXPECT_EQ(  0, cover1.literal_num(~lit9) );

  auto cube_list1 = cover1.literal_list();
  EXPECT_EQ( 2, cube_list1.size() );
  EXPECT_EQ( 2, cube_list1[0].size() );
  EXPECT_EQ( 2, cube_list1[1].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit2, cube_list1[0][1] );
  EXPECT_EQ( lit1, cube_list1[1][0] );
  EXPECT_EQ( lit2, cube_list1[1][1] );

};

TEST_F(SopTest, cover_literal_product)
{
  Literal lit0(var0, false);
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  SopCover src1{nv, { {lit0}, {lit1} } };

  SopCover cover1 = src1 * lit2;

  EXPECT_EQ( nv, cover1.variable_num() );
  EXPECT_EQ(  2, cover1.cube_num() );
  EXPECT_EQ(  4, cover1.literal_num() );
  EXPECT_EQ(  1, cover1.literal_num( lit0) );
  EXPECT_EQ(  0, cover1.literal_num(~lit0) );
  EXPECT_EQ(  1, cover1.literal_num( lit1) );
  EXPECT_EQ(  0, cover1.literal_num(~lit1) );
  EXPECT_EQ(  2, cover1.literal_num( lit2) );
  EXPECT_EQ(  0, cover1.literal_num(~lit2) );
  EXPECT_EQ(  0, cover1.literal_num( lit3) );
  EXPECT_EQ(  0, cover1.literal_num(~lit3) );
  EXPECT_EQ(  0, cover1.literal_num( lit4) );
  EXPECT_EQ(  0, cover1.literal_num(~lit4) );
  EXPECT_EQ(  0, cover1.literal_num( lit5) );
  EXPECT_EQ(  0, cover1.literal_num(~lit5) );
  EXPECT_EQ(  0, cover1.literal_num( lit6) );
  EXPECT_EQ(  0, cover1.literal_num(~lit6) );
  EXPECT_EQ(  0, cover1.literal_num( lit7) );
  EXPECT_EQ(  0, cover1.literal_num(~lit7) );
  EXPECT_EQ(  0, cover1.literal_num( lit8) );
  EXPECT_EQ(  0, cover1.literal_num(~lit8) );
  EXPECT_EQ(  0, cover1.literal_num( lit9) );
  EXPECT_EQ(  0, cover1.literal_num(~lit9) );

  auto cube_list1 = cover1.literal_list();
  EXPECT_EQ( 2, cube_list1.size() );
  EXPECT_EQ( 2, cube_list1[0].size() );
  EXPECT_EQ( 2, cube_list1[1].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit2, cube_list1[0][1] );
  EXPECT_EQ( lit1, cube_list1[1][0] );
  EXPECT_EQ( lit2, cube_list1[1][1] );

};

TEST_F(SopTest, Rcover_literal_product)
{
  Literal lit0(var0, false);
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  SopCover src1{nv, { {lit0}, {lit1} } };

  SopCover cover1 = std::move(src1) * lit2;

  EXPECT_EQ( 0, src1.cube_num() );

  EXPECT_EQ( nv, cover1.variable_num() );
  EXPECT_EQ(  2, cover1.cube_num() );
  EXPECT_EQ(  4, cover1.literal_num() );
  EXPECT_EQ(  1, cover1.literal_num( lit0) );
  EXPECT_EQ(  0, cover1.literal_num(~lit0) );
  EXPECT_EQ(  1, cover1.literal_num( lit1) );
  EXPECT_EQ(  0, cover1.literal_num(~lit1) );
  EXPECT_EQ(  2, cover1.literal_num( lit2) );
  EXPECT_EQ(  0, cover1.literal_num(~lit2) );
  EXPECT_EQ(  0, cover1.literal_num( lit3) );
  EXPECT_EQ(  0, cover1.literal_num(~lit3) );
  EXPECT_EQ(  0, cover1.literal_num( lit4) );
  EXPECT_EQ(  0, cover1.literal_num(~lit4) );
  EXPECT_EQ(  0, cover1.literal_num( lit5) );
  EXPECT_EQ(  0, cover1.literal_num(~lit5) );
  EXPECT_EQ(  0, cover1.literal_num( lit6) );
  EXPECT_EQ(  0, cover1.literal_num(~lit6) );
  EXPECT_EQ(  0, cover1.literal_num( lit7) );
  EXPECT_EQ(  0, cover1.literal_num(~lit7) );
  EXPECT_EQ(  0, cover1.literal_num( lit8) );
  EXPECT_EQ(  0, cover1.literal_num(~lit8) );
  EXPECT_EQ(  0, cover1.literal_num( lit9) );
  EXPECT_EQ(  0, cover1.literal_num(~lit9) );

  auto cube_list1 = cover1.literal_list();
  EXPECT_EQ( 2, cube_list1.size() );
  EXPECT_EQ( 2, cube_list1[0].size() );
  EXPECT_EQ( 2, cube_list1[1].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit2, cube_list1[0][1] );
  EXPECT_EQ( lit1, cube_list1[1][0] );
  EXPECT_EQ( lit2, cube_list1[1][1] );

};

TEST_F(SopTest, literal_cover_product)
{
  Literal lit0(var0, false);
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  SopCover src1{nv, { {lit0}, {lit1} } };

  SopCover cover1 = lit2 * src1;

  EXPECT_EQ( nv, cover1.variable_num() );
  EXPECT_EQ(  2, cover1.cube_num() );
  EXPECT_EQ(  4, cover1.literal_num() );
  EXPECT_EQ(  1, cover1.literal_num( lit0) );
  EXPECT_EQ(  0, cover1.literal_num(~lit0) );
  EXPECT_EQ(  1, cover1.literal_num( lit1) );
  EXPECT_EQ(  0, cover1.literal_num(~lit1) );
  EXPECT_EQ(  2, cover1.literal_num( lit2) );
  EXPECT_EQ(  0, cover1.literal_num(~lit2) );
  EXPECT_EQ(  0, cover1.literal_num( lit3) );
  EXPECT_EQ(  0, cover1.literal_num(~lit3) );
  EXPECT_EQ(  0, cover1.literal_num( lit4) );
  EXPECT_EQ(  0, cover1.literal_num(~lit4) );
  EXPECT_EQ(  0, cover1.literal_num( lit5) );
  EXPECT_EQ(  0, cover1.literal_num(~lit5) );
  EXPECT_EQ(  0, cover1.literal_num( lit6) );
  EXPECT_EQ(  0, cover1.literal_num(~lit6) );
  EXPECT_EQ(  0, cover1.literal_num( lit7) );
  EXPECT_EQ(  0, cover1.literal_num(~lit7) );
  EXPECT_EQ(  0, cover1.literal_num( lit8) );
  EXPECT_EQ(  0, cover1.literal_num(~lit8) );
  EXPECT_EQ(  0, cover1.literal_num( lit9) );
  EXPECT_EQ(  0, cover1.literal_num(~lit9) );

  auto cube_list1 = cover1.literal_list();
  EXPECT_EQ( 2, cube_list1.size() );
  EXPECT_EQ( 2, cube_list1[0].size() );
  EXPECT_EQ( 2, cube_list1[1].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit2, cube_list1[0][1] );
  EXPECT_EQ( lit1, cube_list1[1][0] );
  EXPECT_EQ( lit2, cube_list1[1][1] );

};

TEST_F(SopTest, literal_Rcover_product)
{
  Literal lit0(var0, false);
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  SopCover src1{nv, { {lit0}, {lit1} } };

  SopCover cover1 = lit2 * std::move(src1);

  EXPECT_EQ( 0, src1.cube_num() );

  EXPECT_EQ( nv, cover1.variable_num() );
  EXPECT_EQ(  2, cover1.cube_num() );
  EXPECT_EQ(  4, cover1.literal_num() );
  EXPECT_EQ(  1, cover1.literal_num( lit0) );
  EXPECT_EQ(  0, cover1.literal_num(~lit0) );
  EXPECT_EQ(  1, cover1.literal_num( lit1) );
  EXPECT_EQ(  0, cover1.literal_num(~lit1) );
  EXPECT_EQ(  2, cover1.literal_num( lit2) );
  EXPECT_EQ(  0, cover1.literal_num(~lit2) );
  EXPECT_EQ(  0, cover1.literal_num( lit3) );
  EXPECT_EQ(  0, cover1.literal_num(~lit3) );
  EXPECT_EQ(  0, cover1.literal_num( lit4) );
  EXPECT_EQ(  0, cover1.literal_num(~lit4) );
  EXPECT_EQ(  0, cover1.literal_num( lit5) );
  EXPECT_EQ(  0, cover1.literal_num(~lit5) );
  EXPECT_EQ(  0, cover1.literal_num( lit6) );
  EXPECT_EQ(  0, cover1.literal_num(~lit6) );
  EXPECT_EQ(  0, cover1.literal_num( lit7) );
  EXPECT_EQ(  0, cover1.literal_num(~lit7) );
  EXPECT_EQ(  0, cover1.literal_num( lit8) );
  EXPECT_EQ(  0, cover1.literal_num(~lit8) );
  EXPECT_EQ(  0, cover1.literal_num( lit9) );
  EXPECT_EQ(  0, cover1.literal_num(~lit9) );

  auto cube_list1 = cover1.literal_list();
  EXPECT_EQ( 2, cube_list1.size() );
  EXPECT_EQ( 2, cube_list1[0].size() );
  EXPECT_EQ( 2, cube_list1[1].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit2, cube_list1[0][1] );
  EXPECT_EQ( lit1, cube_list1[1][0] );
  EXPECT_EQ( lit2, cube_list1[1][1] );

};

TEST_F(SopTest, Scover_cover_product)
{
  Literal lit0(var0, false);
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  SopCover cover1{nv, { {lit0}, {lit1} } };
  SopCover src2{nv, { {lit2}, {lit3} } };

  cover1 = cover1 * src2;

  EXPECT_EQ( nv, cover1.variable_num() );
  EXPECT_EQ(  4, cover1.cube_num() );
  EXPECT_EQ(  8, cover1.literal_num() );
  EXPECT_EQ(  2, cover1.literal_num( lit0) );
  EXPECT_EQ(  0, cover1.literal_num(~lit0) );
  EXPECT_EQ(  2, cover1.literal_num( lit1) );
  EXPECT_EQ(  0, cover1.literal_num(~lit1) );
  EXPECT_EQ(  2, cover1.literal_num( lit2) );
  EXPECT_EQ(  0, cover1.literal_num(~lit2) );
  EXPECT_EQ(  2, cover1.literal_num( lit3) );
  EXPECT_EQ(  0, cover1.literal_num(~lit3) );
  EXPECT_EQ(  0, cover1.literal_num( lit4) );
  EXPECT_EQ(  0, cover1.literal_num(~lit4) );
  EXPECT_EQ(  0, cover1.literal_num( lit5) );
  EXPECT_EQ(  0, cover1.literal_num(~lit5) );
  EXPECT_EQ(  0, cover1.literal_num( lit6) );
  EXPECT_EQ(  0, cover1.literal_num(~lit6) );
  EXPECT_EQ(  0, cover1.literal_num( lit7) );
  EXPECT_EQ(  0, cover1.literal_num(~lit7) );
  EXPECT_EQ(  0, cover1.literal_num( lit8) );
  EXPECT_EQ(  0, cover1.literal_num(~lit8) );
  EXPECT_EQ(  0, cover1.literal_num( lit9) );
  EXPECT_EQ(  0, cover1.literal_num(~lit9) );

  auto cube_list1 = cover1.literal_list();
  EXPECT_EQ( 4, cube_list1.size() );
  EXPECT_EQ( 2, cube_list1[0].size() );
  EXPECT_EQ( 2, cube_list1[1].size() );
  EXPECT_EQ( 2, cube_list1[2].size() );
  EXPECT_EQ( 2, cube_list1[3].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit2, cube_list1[0][1] );
  EXPECT_EQ( lit0, cube_list1[1][0] );
  EXPECT_EQ( lit3, cube_list1[1][1] );
  EXPECT_EQ( lit1, cube_list1[2][0] );
  EXPECT_EQ( lit2, cube_list1[2][1] );
  EXPECT_EQ( lit1, cube_list1[3][0] );
  EXPECT_EQ( lit3, cube_list1[3][1] );

};

TEST_F(SopTest, cover_Scover_product)
{
  Literal lit0(var0, false);
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  SopCover src1{nv, { {lit0}, {lit1} } };
  SopCover cover1{nv, { {lit2}, {lit3} } };

  cover1 = src1 * cover1;

  EXPECT_EQ( nv, cover1.variable_num() );
  EXPECT_EQ(  4, cover1.cube_num() );
  EXPECT_EQ(  8, cover1.literal_num() );
  EXPECT_EQ(  2, cover1.literal_num( lit0) );
  EXPECT_EQ(  0, cover1.literal_num(~lit0) );
  EXPECT_EQ(  2, cover1.literal_num( lit1) );
  EXPECT_EQ(  0, cover1.literal_num(~lit1) );
  EXPECT_EQ(  2, cover1.literal_num( lit2) );
  EXPECT_EQ(  0, cover1.literal_num(~lit2) );
  EXPECT_EQ(  2, cover1.literal_num( lit3) );
  EXPECT_EQ(  0, cover1.literal_num(~lit3) );
  EXPECT_EQ(  0, cover1.literal_num( lit4) );
  EXPECT_EQ(  0, cover1.literal_num(~lit4) );
  EXPECT_EQ(  0, cover1.literal_num( lit5) );
  EXPECT_EQ(  0, cover1.literal_num(~lit5) );
  EXPECT_EQ(  0, cover1.literal_num( lit6) );
  EXPECT_EQ(  0, cover1.literal_num(~lit6) );
  EXPECT_EQ(  0, cover1.literal_num( lit7) );
  EXPECT_EQ(  0, cover1.literal_num(~lit7) );
  EXPECT_EQ(  0, cover1.literal_num( lit8) );
  EXPECT_EQ(  0, cover1.literal_num(~lit8) );
  EXPECT_EQ(  0, cover1.literal_num( lit9) );
  EXPECT_EQ(  0, cover1.literal_num(~lit9) );

  auto cube_list1 = cover1.literal_list();
  EXPECT_EQ( 4, cube_list1.size() );
  EXPECT_EQ( 2, cube_list1[0].size() );
  EXPECT_EQ( 2, cube_list1[1].size() );
  EXPECT_EQ( 2, cube_list1[2].size() );
  EXPECT_EQ( 2, cube_list1[3].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit2, cube_list1[0][1] );
  EXPECT_EQ( lit0, cube_list1[1][0] );
  EXPECT_EQ( lit3, cube_list1[1][1] );
  EXPECT_EQ( lit1, cube_list1[2][0] );
  EXPECT_EQ( lit2, cube_list1[2][1] );
  EXPECT_EQ( lit1, cube_list1[3][0] );
  EXPECT_EQ( lit3, cube_list1[3][1] );

};

TEST_F(SopTest, Scover_cube_product)
{
  Literal lit0(var0, false);
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  SopCover cover1{nv, { {lit0}, {lit1} } };
  SopCube src2{nv,  {lit2} };

  cover1 = cover1 * src2;

  EXPECT_EQ( nv, cover1.variable_num() );
  EXPECT_EQ(  2, cover1.cube_num() );
  EXPECT_EQ(  4, cover1.literal_num() );
  EXPECT_EQ(  1, cover1.literal_num( lit0) );
  EXPECT_EQ(  0, cover1.literal_num(~lit0) );
  EXPECT_EQ(  1, cover1.literal_num( lit1) );
  EXPECT_EQ(  0, cover1.literal_num(~lit1) );
  EXPECT_EQ(  2, cover1.literal_num( lit2) );
  EXPECT_EQ(  0, cover1.literal_num(~lit2) );
  EXPECT_EQ(  0, cover1.literal_num( lit3) );
  EXPECT_EQ(  0, cover1.literal_num(~lit3) );
  EXPECT_EQ(  0, cover1.literal_num( lit4) );
  EXPECT_EQ(  0, cover1.literal_num(~lit4) );
  EXPECT_EQ(  0, cover1.literal_num( lit5) );
  EXPECT_EQ(  0, cover1.literal_num(~lit5) );
  EXPECT_EQ(  0, cover1.literal_num( lit6) );
  EXPECT_EQ(  0, cover1.literal_num(~lit6) );
  EXPECT_EQ(  0, cover1.literal_num( lit7) );
  EXPECT_EQ(  0, cover1.literal_num(~lit7) );
  EXPECT_EQ(  0, cover1.literal_num( lit8) );
  EXPECT_EQ(  0, cover1.literal_num(~lit8) );
  EXPECT_EQ(  0, cover1.literal_num( lit9) );
  EXPECT_EQ(  0, cover1.literal_num(~lit9) );

  auto cube_list1 = cover1.literal_list();
  EXPECT_EQ( 2, cube_list1.size() );
  EXPECT_EQ( 2, cube_list1[0].size() );
  EXPECT_EQ( 2, cube_list1[1].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit2, cube_list1[0][1] );
  EXPECT_EQ( lit1, cube_list1[1][0] );
  EXPECT_EQ( lit2, cube_list1[1][1] );

};

TEST_F(SopTest, cube_Scover_product)
{
  Literal lit0(var0, false);
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  SopCube src1{nv,  {lit2} };
  SopCover cover1{nv, { {lit0}, {lit1} } };

  cover1 = src1 * cover1;

  EXPECT_EQ( nv, cover1.variable_num() );
  EXPECT_EQ(  2, cover1.cube_num() );
  EXPECT_EQ(  4, cover1.literal_num() );
  EXPECT_EQ(  1, cover1.literal_num( lit0) );
  EXPECT_EQ(  0, cover1.literal_num(~lit0) );
  EXPECT_EQ(  1, cover1.literal_num( lit1) );
  EXPECT_EQ(  0, cover1.literal_num(~lit1) );
  EXPECT_EQ(  2, cover1.literal_num( lit2) );
  EXPECT_EQ(  0, cover1.literal_num(~lit2) );
  EXPECT_EQ(  0, cover1.literal_num( lit3) );
  EXPECT_EQ(  0, cover1.literal_num(~lit3) );
  EXPECT_EQ(  0, cover1.literal_num( lit4) );
  EXPECT_EQ(  0, cover1.literal_num(~lit4) );
  EXPECT_EQ(  0, cover1.literal_num( lit5) );
  EXPECT_EQ(  0, cover1.literal_num(~lit5) );
  EXPECT_EQ(  0, cover1.literal_num( lit6) );
  EXPECT_EQ(  0, cover1.literal_num(~lit6) );
  EXPECT_EQ(  0, cover1.literal_num( lit7) );
  EXPECT_EQ(  0, cover1.literal_num(~lit7) );
  EXPECT_EQ(  0, cover1.literal_num( lit8) );
  EXPECT_EQ(  0, cover1.literal_num(~lit8) );
  EXPECT_EQ(  0, cover1.literal_num( lit9) );
  EXPECT_EQ(  0, cover1.literal_num(~lit9) );

  auto cube_list1 = cover1.literal_list();
  EXPECT_EQ( 2, cube_list1.size() );
  EXPECT_EQ( 2, cube_list1[0].size() );
  EXPECT_EQ( 2, cube_list1[1].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit2, cube_list1[0][1] );
  EXPECT_EQ( lit1, cube_list1[1][0] );
  EXPECT_EQ( lit2, cube_list1[1][1] );

};

TEST_F(SopTest, Scover_literal_product)
{
  Literal lit0(var0, false);
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  SopCover cover1{nv, { {lit0}, {lit1} } };

  cover1 = cover1 * lit2;

  EXPECT_EQ( nv, cover1.variable_num() );
  EXPECT_EQ(  2, cover1.cube_num() );
  EXPECT_EQ(  4, cover1.literal_num() );
  EXPECT_EQ(  1, cover1.literal_num( lit0) );
  EXPECT_EQ(  0, cover1.literal_num(~lit0) );
  EXPECT_EQ(  1, cover1.literal_num( lit1) );
  EXPECT_EQ(  0, cover1.literal_num(~lit1) );
  EXPECT_EQ(  2, cover1.literal_num( lit2) );
  EXPECT_EQ(  0, cover1.literal_num(~lit2) );
  EXPECT_EQ(  0, cover1.literal_num( lit3) );
  EXPECT_EQ(  0, cover1.literal_num(~lit3) );
  EXPECT_EQ(  0, cover1.literal_num( lit4) );
  EXPECT_EQ(  0, cover1.literal_num(~lit4) );
  EXPECT_EQ(  0, cover1.literal_num( lit5) );
  EXPECT_EQ(  0, cover1.literal_num(~lit5) );
  EXPECT_EQ(  0, cover1.literal_num( lit6) );
  EXPECT_EQ(  0, cover1.literal_num(~lit6) );
  EXPECT_EQ(  0, cover1.literal_num( lit7) );
  EXPECT_EQ(  0, cover1.literal_num(~lit7) );
  EXPECT_EQ(  0, cover1.literal_num( lit8) );
  EXPECT_EQ(  0, cover1.literal_num(~lit8) );
  EXPECT_EQ(  0, cover1.literal_num( lit9) );
  EXPECT_EQ(  0, cover1.literal_num(~lit9) );

  auto cube_list1 = cover1.literal_list();
  EXPECT_EQ( 2, cube_list1.size() );
  EXPECT_EQ( 2, cube_list1[0].size() );
  EXPECT_EQ( 2, cube_list1[1].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit2, cube_list1[0][1] );
  EXPECT_EQ( lit1, cube_list1[1][0] );
  EXPECT_EQ( lit2, cube_list1[1][1] );

};

TEST_F(SopTest, literal_Scover_product)
{
  Literal lit0(var0, false);
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  SopCover cover1{nv, { {lit0}, {lit1} } };

  cover1 = lit2 * cover1;

  EXPECT_EQ( nv, cover1.variable_num() );
  EXPECT_EQ(  2, cover1.cube_num() );
  EXPECT_EQ(  4, cover1.literal_num() );
  EXPECT_EQ(  1, cover1.literal_num( lit0) );
  EXPECT_EQ(  0, cover1.literal_num(~lit0) );
  EXPECT_EQ(  1, cover1.literal_num( lit1) );
  EXPECT_EQ(  0, cover1.literal_num(~lit1) );
  EXPECT_EQ(  2, cover1.literal_num( lit2) );
  EXPECT_EQ(  0, cover1.literal_num(~lit2) );
  EXPECT_EQ(  0, cover1.literal_num( lit3) );
  EXPECT_EQ(  0, cover1.literal_num(~lit3) );
  EXPECT_EQ(  0, cover1.literal_num( lit4) );
  EXPECT_EQ(  0, cover1.literal_num(~lit4) );
  EXPECT_EQ(  0, cover1.literal_num( lit5) );
  EXPECT_EQ(  0, cover1.literal_num(~lit5) );
  EXPECT_EQ(  0, cover1.literal_num( lit6) );
  EXPECT_EQ(  0, cover1.literal_num(~lit6) );
  EXPECT_EQ(  0, cover1.literal_num( lit7) );
  EXPECT_EQ(  0, cover1.literal_num(~lit7) );
  EXPECT_EQ(  0, cover1.literal_num( lit8) );
  EXPECT_EQ(  0, cover1.literal_num(~lit8) );
  EXPECT_EQ(  0, cover1.literal_num( lit9) );
  EXPECT_EQ(  0, cover1.literal_num(~lit9) );

  auto cube_list1 = cover1.literal_list();
  EXPECT_EQ( 2, cube_list1.size() );
  EXPECT_EQ( 2, cube_list1[0].size() );
  EXPECT_EQ( 2, cube_list1[1].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit2, cube_list1[0][1] );
  EXPECT_EQ( lit1, cube_list1[1][0] );
  EXPECT_EQ( lit2, cube_list1[1][1] );

};

TEST_F(SopTest, Icover_cover_product)
{
  Literal lit0(var0, false);
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  SopCover cover1{nv, { {lit0}, {lit1} } };
  SopCover src2{nv, { {lit2}, {lit3} } };

  cover1 *= src2;

  EXPECT_EQ( nv, cover1.variable_num() );
  EXPECT_EQ(  4, cover1.cube_num() );
  EXPECT_EQ(  8, cover1.literal_num() );
  EXPECT_EQ(  2, cover1.literal_num( lit0) );
  EXPECT_EQ(  0, cover1.literal_num(~lit0) );
  EXPECT_EQ(  2, cover1.literal_num( lit1) );
  EXPECT_EQ(  0, cover1.literal_num(~lit1) );
  EXPECT_EQ(  2, cover1.literal_num( lit2) );
  EXPECT_EQ(  0, cover1.literal_num(~lit2) );
  EXPECT_EQ(  2, cover1.literal_num( lit3) );
  EXPECT_EQ(  0, cover1.literal_num(~lit3) );
  EXPECT_EQ(  0, cover1.literal_num( lit4) );
  EXPECT_EQ(  0, cover1.literal_num(~lit4) );
  EXPECT_EQ(  0, cover1.literal_num( lit5) );
  EXPECT_EQ(  0, cover1.literal_num(~lit5) );
  EXPECT_EQ(  0, cover1.literal_num( lit6) );
  EXPECT_EQ(  0, cover1.literal_num(~lit6) );
  EXPECT_EQ(  0, cover1.literal_num( lit7) );
  EXPECT_EQ(  0, cover1.literal_num(~lit7) );
  EXPECT_EQ(  0, cover1.literal_num( lit8) );
  EXPECT_EQ(  0, cover1.literal_num(~lit8) );
  EXPECT_EQ(  0, cover1.literal_num( lit9) );
  EXPECT_EQ(  0, cover1.literal_num(~lit9) );

  auto cube_list1 = cover1.literal_list();
  EXPECT_EQ( 4, cube_list1.size() );
  EXPECT_EQ( 2, cube_list1[0].size() );
  EXPECT_EQ( 2, cube_list1[1].size() );
  EXPECT_EQ( 2, cube_list1[2].size() );
  EXPECT_EQ( 2, cube_list1[3].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit2, cube_list1[0][1] );
  EXPECT_EQ( lit0, cube_list1[1][0] );
  EXPECT_EQ( lit3, cube_list1[1][1] );
  EXPECT_EQ( lit1, cube_list1[2][0] );
  EXPECT_EQ( lit2, cube_list1[2][1] );
  EXPECT_EQ( lit1, cube_list1[3][0] );
  EXPECT_EQ( lit3, cube_list1[3][1] );

};

TEST_F(SopTest, Icover_cube_product)
{
  Literal lit0(var0, false);
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  SopCover cover1{nv, { {lit0}, {lit1} } };
  SopCube src2{nv,  {lit2} };

  cover1 *= src2;

  EXPECT_EQ( nv, cover1.variable_num() );
  EXPECT_EQ(  2, cover1.cube_num() );
  EXPECT_EQ(  4, cover1.literal_num() );
  EXPECT_EQ(  1, cover1.literal_num( lit0) );
  EXPECT_EQ(  0, cover1.literal_num(~lit0) );
  EXPECT_EQ(  1, cover1.literal_num( lit1) );
  EXPECT_EQ(  0, cover1.literal_num(~lit1) );
  EXPECT_EQ(  2, cover1.literal_num( lit2) );
  EXPECT_EQ(  0, cover1.literal_num(~lit2) );
  EXPECT_EQ(  0, cover1.literal_num( lit3) );
  EXPECT_EQ(  0, cover1.literal_num(~lit3) );
  EXPECT_EQ(  0, cover1.literal_num( lit4) );
  EXPECT_EQ(  0, cover1.literal_num(~lit4) );
  EXPECT_EQ(  0, cover1.literal_num( lit5) );
  EXPECT_EQ(  0, cover1.literal_num(~lit5) );
  EXPECT_EQ(  0, cover1.literal_num( lit6) );
  EXPECT_EQ(  0, cover1.literal_num(~lit6) );
  EXPECT_EQ(  0, cover1.literal_num( lit7) );
  EXPECT_EQ(  0, cover1.literal_num(~lit7) );
  EXPECT_EQ(  0, cover1.literal_num( lit8) );
  EXPECT_EQ(  0, cover1.literal_num(~lit8) );
  EXPECT_EQ(  0, cover1.literal_num( lit9) );
  EXPECT_EQ(  0, cover1.literal_num(~lit9) );

  auto cube_list1 = cover1.literal_list();
  EXPECT_EQ( 2, cube_list1.size() );
  EXPECT_EQ( 2, cube_list1[0].size() );
  EXPECT_EQ( 2, cube_list1[1].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit2, cube_list1[0][1] );
  EXPECT_EQ( lit1, cube_list1[1][0] );
  EXPECT_EQ( lit2, cube_list1[1][1] );

};

TEST_F(SopTest, Icover_literal_product)
{
  Literal lit0(var0, false);
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  SopCover cover1{nv, { {lit0}, {lit1} } };

  cover1 *= lit2;

  EXPECT_EQ( nv, cover1.variable_num() );
  EXPECT_EQ(  2, cover1.cube_num() );
  EXPECT_EQ(  4, cover1.literal_num() );
  EXPECT_EQ(  1, cover1.literal_num( lit0) );
  EXPECT_EQ(  0, cover1.literal_num(~lit0) );
  EXPECT_EQ(  1, cover1.literal_num( lit1) );
  EXPECT_EQ(  0, cover1.literal_num(~lit1) );
  EXPECT_EQ(  2, cover1.literal_num( lit2) );
  EXPECT_EQ(  0, cover1.literal_num(~lit2) );
  EXPECT_EQ(  0, cover1.literal_num( lit3) );
  EXPECT_EQ(  0, cover1.literal_num(~lit3) );
  EXPECT_EQ(  0, cover1.literal_num( lit4) );
  EXPECT_EQ(  0, cover1.literal_num(~lit4) );
  EXPECT_EQ(  0, cover1.literal_num( lit5) );
  EXPECT_EQ(  0, cover1.literal_num(~lit5) );
  EXPECT_EQ(  0, cover1.literal_num( lit6) );
  EXPECT_EQ(  0, cover1.literal_num(~lit6) );
  EXPECT_EQ(  0, cover1.literal_num( lit7) );
  EXPECT_EQ(  0, cover1.literal_num(~lit7) );
  EXPECT_EQ(  0, cover1.literal_num( lit8) );
  EXPECT_EQ(  0, cover1.literal_num(~lit8) );
  EXPECT_EQ(  0, cover1.literal_num( lit9) );
  EXPECT_EQ(  0, cover1.literal_num(~lit9) );

  auto cube_list1 = cover1.literal_list();
  EXPECT_EQ( 2, cube_list1.size() );
  EXPECT_EQ( 2, cube_list1[0].size() );
  EXPECT_EQ( 2, cube_list1[1].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit2, cube_list1[0][1] );
  EXPECT_EQ( lit1, cube_list1[1][0] );
  EXPECT_EQ( lit2, cube_list1[1][1] );

};

TEST_F(SopTest, cover_cover_quotient)
{
  Literal lit0(var0, false);
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  SopCover src1{nv, { {lit0, lit2}, {lit0, lit3}, {lit1, lit2}, {lit1, lit3} } };
  SopCover src2{nv, { {lit0}, {lit1} } };

  SopCover cover1 = src1 / src2;

  EXPECT_EQ( nv, cover1.variable_num() );
  EXPECT_EQ(  2, cover1.cube_num() );
  EXPECT_EQ(  2, cover1.literal_num() );
  EXPECT_EQ(  0, cover1.literal_num( lit0) );
  EXPECT_EQ(  0, cover1.literal_num(~lit0) );
  EXPECT_EQ(  0, cover1.literal_num( lit1) );
  EXPECT_EQ(  0, cover1.literal_num(~lit1) );
  EXPECT_EQ(  1, cover1.literal_num( lit2) );
  EXPECT_EQ(  0, cover1.literal_num(~lit2) );
  EXPECT_EQ(  1, cover1.literal_num( lit3) );
  EXPECT_EQ(  0, cover1.literal_num(~lit3) );
  EXPECT_EQ(  0, cover1.literal_num( lit4) );
  EXPECT_EQ(  0, cover1.literal_num(~lit4) );
  EXPECT_EQ(  0, cover1.literal_num( lit5) );
  EXPECT_EQ(  0, cover1.literal_num(~lit5) );
  EXPECT_EQ(  0, cover1.literal_num( lit6) );
  EXPECT_EQ(  0, cover1.literal_num(~lit6) );
  EXPECT_EQ(  0, cover1.literal_num( lit7) );
  EXPECT_EQ(  0, cover1.literal_num(~lit7) );
  EXPECT_EQ(  0, cover1.literal_num( lit8) );
  EXPECT_EQ(  0, cover1.literal_num(~lit8) );
  EXPECT_EQ(  0, cover1.literal_num( lit9) );
  EXPECT_EQ(  0, cover1.literal_num(~lit9) );

  auto cube_list1 = cover1.literal_list();
  EXPECT_EQ( 2, cube_list1.size() );
  EXPECT_EQ( 1, cube_list1[0].size() );
  EXPECT_EQ( 1, cube_list1[1].size() );
  EXPECT_EQ( lit2, cube_list1[0][0] );
  EXPECT_EQ( lit3, cube_list1[1][0] );

};

TEST_F(SopTest, cover_cube_quotient)
{
  Literal lit0(var0, false);
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  SopCover src1{nv, { {lit0, lit2}, {lit0, lit3}, {lit1, lit2}, {lit1, lit3} } };
  SopCube src2{nv, {lit0} };

  SopCover cover1 = src1 / src2;

  EXPECT_EQ( nv, cover1.variable_num() );
  EXPECT_EQ(  2, cover1.cube_num() );
  EXPECT_EQ(  2, cover1.literal_num() );
  EXPECT_EQ(  0, cover1.literal_num( lit0) );
  EXPECT_EQ(  0, cover1.literal_num(~lit0) );
  EXPECT_EQ(  0, cover1.literal_num( lit1) );
  EXPECT_EQ(  0, cover1.literal_num(~lit1) );
  EXPECT_EQ(  1, cover1.literal_num( lit2) );
  EXPECT_EQ(  0, cover1.literal_num(~lit2) );
  EXPECT_EQ(  1, cover1.literal_num( lit3) );
  EXPECT_EQ(  0, cover1.literal_num(~lit3) );
  EXPECT_EQ(  0, cover1.literal_num( lit4) );
  EXPECT_EQ(  0, cover1.literal_num(~lit4) );
  EXPECT_EQ(  0, cover1.literal_num( lit5) );
  EXPECT_EQ(  0, cover1.literal_num(~lit5) );
  EXPECT_EQ(  0, cover1.literal_num( lit6) );
  EXPECT_EQ(  0, cover1.literal_num(~lit6) );
  EXPECT_EQ(  0, cover1.literal_num( lit7) );
  EXPECT_EQ(  0, cover1.literal_num(~lit7) );
  EXPECT_EQ(  0, cover1.literal_num( lit8) );
  EXPECT_EQ(  0, cover1.literal_num(~lit8) );
  EXPECT_EQ(  0, cover1.literal_num( lit9) );
  EXPECT_EQ(  0, cover1.literal_num(~lit9) );

  auto cube_list1 = cover1.literal_list();
  EXPECT_EQ( 2, cube_list1.size() );
  EXPECT_EQ( 1, cube_list1[0].size() );
  EXPECT_EQ( 1, cube_list1[1].size() );
  EXPECT_EQ( lit2, cube_list1[0][0] );
  EXPECT_EQ( lit3, cube_list1[1][0] );

};

TEST_F(SopTest, cover_literal_quotient)
{
  Literal lit0(var0, false);
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  SopCover src1{nv, { {lit0, lit2}, {lit0, lit3}, {lit1, lit2}, {lit1, lit3} } };

  SopCover cover1 = src1 / lit0;

  EXPECT_EQ( nv, cover1.variable_num() );
  EXPECT_EQ(  2, cover1.cube_num() );
  EXPECT_EQ(  2, cover1.literal_num() );
  EXPECT_EQ(  0, cover1.literal_num( lit0) );
  EXPECT_EQ(  0, cover1.literal_num(~lit0) );
  EXPECT_EQ(  0, cover1.literal_num( lit1) );
  EXPECT_EQ(  0, cover1.literal_num(~lit1) );
  EXPECT_EQ(  1, cover1.literal_num( lit2) );
  EXPECT_EQ(  0, cover1.literal_num(~lit2) );
  EXPECT_EQ(  1, cover1.literal_num( lit3) );
  EXPECT_EQ(  0, cover1.literal_num(~lit3) );
  EXPECT_EQ(  0, cover1.literal_num( lit4) );
  EXPECT_EQ(  0, cover1.literal_num(~lit4) );
  EXPECT_EQ(  0, cover1.literal_num( lit5) );
  EXPECT_EQ(  0, cover1.literal_num(~lit5) );
  EXPECT_EQ(  0, cover1.literal_num( lit6) );
  EXPECT_EQ(  0, cover1.literal_num(~lit6) );
  EXPECT_EQ(  0, cover1.literal_num( lit7) );
  EXPECT_EQ(  0, cover1.literal_num(~lit7) );
  EXPECT_EQ(  0, cover1.literal_num( lit8) );
  EXPECT_EQ(  0, cover1.literal_num(~lit8) );
  EXPECT_EQ(  0, cover1.literal_num( lit9) );
  EXPECT_EQ(  0, cover1.literal_num(~lit9) );

  auto cube_list1 = cover1.literal_list();
  EXPECT_EQ( 2, cube_list1.size() );
  EXPECT_EQ( 1, cube_list1[0].size() );
  EXPECT_EQ( 1, cube_list1[1].size() );
  EXPECT_EQ( lit2, cube_list1[0][0] );
  EXPECT_EQ( lit3, cube_list1[1][0] );

};

TEST_F(SopTest, Rcover_cover_quotient)
{
  Literal lit0(var0, false);
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  SopCover src1{nv, { {lit0, lit2}, {lit0, lit3}, {lit1, lit2}, {lit1, lit3} } };
  SopCover src2{nv, { {lit0}, {lit1} } };

  SopCover cover1 = std::move(src1) / src2;

  EXPECT_EQ( 0, src1.cube_num() );

  EXPECT_EQ( nv, cover1.variable_num() );
  EXPECT_EQ(  2, cover1.cube_num() );
  EXPECT_EQ(  2, cover1.literal_num() );
  EXPECT_EQ(  0, cover1.literal_num( lit0) );
  EXPECT_EQ(  0, cover1.literal_num(~lit0) );
  EXPECT_EQ(  0, cover1.literal_num( lit1) );
  EXPECT_EQ(  0, cover1.literal_num(~lit1) );
  EXPECT_EQ(  1, cover1.literal_num( lit2) );
  EXPECT_EQ(  0, cover1.literal_num(~lit2) );
  EXPECT_EQ(  1, cover1.literal_num( lit3) );
  EXPECT_EQ(  0, cover1.literal_num(~lit3) );
  EXPECT_EQ(  0, cover1.literal_num( lit4) );
  EXPECT_EQ(  0, cover1.literal_num(~lit4) );
  EXPECT_EQ(  0, cover1.literal_num( lit5) );
  EXPECT_EQ(  0, cover1.literal_num(~lit5) );
  EXPECT_EQ(  0, cover1.literal_num( lit6) );
  EXPECT_EQ(  0, cover1.literal_num(~lit6) );
  EXPECT_EQ(  0, cover1.literal_num( lit7) );
  EXPECT_EQ(  0, cover1.literal_num(~lit7) );
  EXPECT_EQ(  0, cover1.literal_num( lit8) );
  EXPECT_EQ(  0, cover1.literal_num(~lit8) );
  EXPECT_EQ(  0, cover1.literal_num( lit9) );
  EXPECT_EQ(  0, cover1.literal_num(~lit9) );

  auto cube_list1 = cover1.literal_list();
  EXPECT_EQ( 2, cube_list1.size() );
  EXPECT_EQ( 1, cube_list1[0].size() );
  EXPECT_EQ( 1, cube_list1[1].size() );
  EXPECT_EQ( lit2, cube_list1[0][0] );
  EXPECT_EQ( lit3, cube_list1[1][0] );

};

TEST_F(SopTest, Rcover_cube_quotient)
{
  Literal lit0(var0, false);
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  SopCover src1{nv, { {lit0, lit2}, {lit0, lit3}, {lit1, lit2}, {lit1, lit3} } };
  SopCube src2{nv, {lit0} };

  SopCover cover1 = std::move(src1) / src2;

  EXPECT_EQ( 0, src1.cube_num() );

  EXPECT_EQ( nv, cover1.variable_num() );
  EXPECT_EQ(  2, cover1.cube_num() );
  EXPECT_EQ(  2, cover1.literal_num() );
  EXPECT_EQ(  0, cover1.literal_num( lit0) );
  EXPECT_EQ(  0, cover1.literal_num(~lit0) );
  EXPECT_EQ(  0, cover1.literal_num( lit1) );
  EXPECT_EQ(  0, cover1.literal_num(~lit1) );
  EXPECT_EQ(  1, cover1.literal_num( lit2) );
  EXPECT_EQ(  0, cover1.literal_num(~lit2) );
  EXPECT_EQ(  1, cover1.literal_num( lit3) );
  EXPECT_EQ(  0, cover1.literal_num(~lit3) );
  EXPECT_EQ(  0, cover1.literal_num( lit4) );
  EXPECT_EQ(  0, cover1.literal_num(~lit4) );
  EXPECT_EQ(  0, cover1.literal_num( lit5) );
  EXPECT_EQ(  0, cover1.literal_num(~lit5) );
  EXPECT_EQ(  0, cover1.literal_num( lit6) );
  EXPECT_EQ(  0, cover1.literal_num(~lit6) );
  EXPECT_EQ(  0, cover1.literal_num( lit7) );
  EXPECT_EQ(  0, cover1.literal_num(~lit7) );
  EXPECT_EQ(  0, cover1.literal_num( lit8) );
  EXPECT_EQ(  0, cover1.literal_num(~lit8) );
  EXPECT_EQ(  0, cover1.literal_num( lit9) );
  EXPECT_EQ(  0, cover1.literal_num(~lit9) );

  auto cube_list1 = cover1.literal_list();
  EXPECT_EQ( 2, cube_list1.size() );
  EXPECT_EQ( 1, cube_list1[0].size() );
  EXPECT_EQ( 1, cube_list1[1].size() );
  EXPECT_EQ( lit2, cube_list1[0][0] );
  EXPECT_EQ( lit3, cube_list1[1][0] );

};

TEST_F(SopTest, Rcover_literal_quotient)
{
  Literal lit0(var0, false);
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  SopCover src1{nv, { {lit0, lit2}, {lit0, lit3}, {lit1, lit2}, {lit1, lit3} } };

  SopCover cover1 = std::move(src1) / lit0;

  EXPECT_EQ( 0, src1.cube_num() );

  EXPECT_EQ( nv, cover1.variable_num() );
  EXPECT_EQ(  2, cover1.cube_num() );
  EXPECT_EQ(  2, cover1.literal_num() );
  EXPECT_EQ(  0, cover1.literal_num( lit0) );
  EXPECT_EQ(  0, cover1.literal_num(~lit0) );
  EXPECT_EQ(  0, cover1.literal_num( lit1) );
  EXPECT_EQ(  0, cover1.literal_num(~lit1) );
  EXPECT_EQ(  1, cover1.literal_num( lit2) );
  EXPECT_EQ(  0, cover1.literal_num(~lit2) );
  EXPECT_EQ(  1, cover1.literal_num( lit3) );
  EXPECT_EQ(  0, cover1.literal_num(~lit3) );
  EXPECT_EQ(  0, cover1.literal_num( lit4) );
  EXPECT_EQ(  0, cover1.literal_num(~lit4) );
  EXPECT_EQ(  0, cover1.literal_num( lit5) );
  EXPECT_EQ(  0, cover1.literal_num(~lit5) );
  EXPECT_EQ(  0, cover1.literal_num( lit6) );
  EXPECT_EQ(  0, cover1.literal_num(~lit6) );
  EXPECT_EQ(  0, cover1.literal_num( lit7) );
  EXPECT_EQ(  0, cover1.literal_num(~lit7) );
  EXPECT_EQ(  0, cover1.literal_num( lit8) );
  EXPECT_EQ(  0, cover1.literal_num(~lit8) );
  EXPECT_EQ(  0, cover1.literal_num( lit9) );
  EXPECT_EQ(  0, cover1.literal_num(~lit9) );

  auto cube_list1 = cover1.literal_list();
  EXPECT_EQ( 2, cube_list1.size() );
  EXPECT_EQ( 1, cube_list1[0].size() );
  EXPECT_EQ( 1, cube_list1[1].size() );
  EXPECT_EQ( lit2, cube_list1[0][0] );
  EXPECT_EQ( lit3, cube_list1[1][0] );

};

TEST_F(SopTest, Scover_cover_quotient)
{
  Literal lit0(var0, false);
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  SopCover cover1{nv, { {lit0, lit2}, {lit0, lit3}, {lit1, lit2}, {lit1, lit3} } };
  SopCover src2{nv, { {lit0}, {lit1} } };

  cover1 = cover1 / src2;

  EXPECT_EQ( nv, cover1.variable_num() );
  EXPECT_EQ(  2, cover1.cube_num() );
  EXPECT_EQ(  2, cover1.literal_num() );
  EXPECT_EQ(  0, cover1.literal_num( lit0) );
  EXPECT_EQ(  0, cover1.literal_num(~lit0) );
  EXPECT_EQ(  0, cover1.literal_num( lit1) );
  EXPECT_EQ(  0, cover1.literal_num(~lit1) );
  EXPECT_EQ(  1, cover1.literal_num( lit2) );
  EXPECT_EQ(  0, cover1.literal_num(~lit2) );
  EXPECT_EQ(  1, cover1.literal_num( lit3) );
  EXPECT_EQ(  0, cover1.literal_num(~lit3) );
  EXPECT_EQ(  0, cover1.literal_num( lit4) );
  EXPECT_EQ(  0, cover1.literal_num(~lit4) );
  EXPECT_EQ(  0, cover1.literal_num( lit5) );
  EXPECT_EQ(  0, cover1.literal_num(~lit5) );
  EXPECT_EQ(  0, cover1.literal_num( lit6) );
  EXPECT_EQ(  0, cover1.literal_num(~lit6) );
  EXPECT_EQ(  0, cover1.literal_num( lit7) );
  EXPECT_EQ(  0, cover1.literal_num(~lit7) );
  EXPECT_EQ(  0, cover1.literal_num( lit8) );
  EXPECT_EQ(  0, cover1.literal_num(~lit8) );
  EXPECT_EQ(  0, cover1.literal_num( lit9) );
  EXPECT_EQ(  0, cover1.literal_num(~lit9) );

  auto cube_list1 = cover1.literal_list();
  EXPECT_EQ( 2, cube_list1.size() );
  EXPECT_EQ( 1, cube_list1[0].size() );
  EXPECT_EQ( 1, cube_list1[1].size() );
  EXPECT_EQ( lit2, cube_list1[0][0] );
  EXPECT_EQ( lit3, cube_list1[1][0] );

};

TEST_F(SopTest, Scover_cube_quotient)
{
  Literal lit0(var0, false);
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  SopCover cover1{nv, { {lit0, lit2}, {lit0, lit3}, {lit1, lit2}, {lit1, lit3} } };
  SopCube src2{nv, {lit0} };

  cover1 = cover1 / src2;

  EXPECT_EQ( nv, cover1.variable_num() );
  EXPECT_EQ(  2, cover1.cube_num() );
  EXPECT_EQ(  2, cover1.literal_num() );
  EXPECT_EQ(  0, cover1.literal_num( lit0) );
  EXPECT_EQ(  0, cover1.literal_num(~lit0) );
  EXPECT_EQ(  0, cover1.literal_num( lit1) );
  EXPECT_EQ(  0, cover1.literal_num(~lit1) );
  EXPECT_EQ(  1, cover1.literal_num( lit2) );
  EXPECT_EQ(  0, cover1.literal_num(~lit2) );
  EXPECT_EQ(  1, cover1.literal_num( lit3) );
  EXPECT_EQ(  0, cover1.literal_num(~lit3) );
  EXPECT_EQ(  0, cover1.literal_num( lit4) );
  EXPECT_EQ(  0, cover1.literal_num(~lit4) );
  EXPECT_EQ(  0, cover1.literal_num( lit5) );
  EXPECT_EQ(  0, cover1.literal_num(~lit5) );
  EXPECT_EQ(  0, cover1.literal_num( lit6) );
  EXPECT_EQ(  0, cover1.literal_num(~lit6) );
  EXPECT_EQ(  0, cover1.literal_num( lit7) );
  EXPECT_EQ(  0, cover1.literal_num(~lit7) );
  EXPECT_EQ(  0, cover1.literal_num( lit8) );
  EXPECT_EQ(  0, cover1.literal_num(~lit8) );
  EXPECT_EQ(  0, cover1.literal_num( lit9) );
  EXPECT_EQ(  0, cover1.literal_num(~lit9) );

  auto cube_list1 = cover1.literal_list();
  EXPECT_EQ( 2, cube_list1.size() );
  EXPECT_EQ( 1, cube_list1[0].size() );
  EXPECT_EQ( 1, cube_list1[1].size() );
  EXPECT_EQ( lit2, cube_list1[0][0] );
  EXPECT_EQ( lit3, cube_list1[1][0] );

};

TEST_F(SopTest, Scover_literal_quotient)
{
  Literal lit0(var0, false);
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  SopCover cover1{nv, { {lit0, lit2}, {lit0, lit3}, {lit1, lit2}, {lit1, lit3} } };

  cover1 = cover1 / lit0;

  EXPECT_EQ( nv, cover1.variable_num() );
  EXPECT_EQ(  2, cover1.cube_num() );
  EXPECT_EQ(  2, cover1.literal_num() );
  EXPECT_EQ(  0, cover1.literal_num( lit0) );
  EXPECT_EQ(  0, cover1.literal_num(~lit0) );
  EXPECT_EQ(  0, cover1.literal_num( lit1) );
  EXPECT_EQ(  0, cover1.literal_num(~lit1) );
  EXPECT_EQ(  1, cover1.literal_num( lit2) );
  EXPECT_EQ(  0, cover1.literal_num(~lit2) );
  EXPECT_EQ(  1, cover1.literal_num( lit3) );
  EXPECT_EQ(  0, cover1.literal_num(~lit3) );
  EXPECT_EQ(  0, cover1.literal_num( lit4) );
  EXPECT_EQ(  0, cover1.literal_num(~lit4) );
  EXPECT_EQ(  0, cover1.literal_num( lit5) );
  EXPECT_EQ(  0, cover1.literal_num(~lit5) );
  EXPECT_EQ(  0, cover1.literal_num( lit6) );
  EXPECT_EQ(  0, cover1.literal_num(~lit6) );
  EXPECT_EQ(  0, cover1.literal_num( lit7) );
  EXPECT_EQ(  0, cover1.literal_num(~lit7) );
  EXPECT_EQ(  0, cover1.literal_num( lit8) );
  EXPECT_EQ(  0, cover1.literal_num(~lit8) );
  EXPECT_EQ(  0, cover1.literal_num( lit9) );
  EXPECT_EQ(  0, cover1.literal_num(~lit9) );

  auto cube_list1 = cover1.literal_list();
  EXPECT_EQ( 2, cube_list1.size() );
  EXPECT_EQ( 1, cube_list1[0].size() );
  EXPECT_EQ( 1, cube_list1[1].size() );
  EXPECT_EQ( lit2, cube_list1[0][0] );
  EXPECT_EQ( lit3, cube_list1[1][0] );

};

TEST_F(SopTest, Icover_cover_quotient)
{
  Literal lit0(var0, false);
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  SopCover cover1{nv, { {lit0, lit2}, {lit0, lit3}, {lit1, lit2}, {lit1, lit3} } };
  SopCover src2{nv, { {lit0}, {lit1} } };

  cover1 /= src2;

  EXPECT_EQ( nv, cover1.variable_num() );
  EXPECT_EQ(  2, cover1.cube_num() );
  EXPECT_EQ(  2, cover1.literal_num() );
  EXPECT_EQ(  0, cover1.literal_num( lit0) );
  EXPECT_EQ(  0, cover1.literal_num(~lit0) );
  EXPECT_EQ(  0, cover1.literal_num( lit1) );
  EXPECT_EQ(  0, cover1.literal_num(~lit1) );
  EXPECT_EQ(  1, cover1.literal_num( lit2) );
  EXPECT_EQ(  0, cover1.literal_num(~lit2) );
  EXPECT_EQ(  1, cover1.literal_num( lit3) );
  EXPECT_EQ(  0, cover1.literal_num(~lit3) );
  EXPECT_EQ(  0, cover1.literal_num( lit4) );
  EXPECT_EQ(  0, cover1.literal_num(~lit4) );
  EXPECT_EQ(  0, cover1.literal_num( lit5) );
  EXPECT_EQ(  0, cover1.literal_num(~lit5) );
  EXPECT_EQ(  0, cover1.literal_num( lit6) );
  EXPECT_EQ(  0, cover1.literal_num(~lit6) );
  EXPECT_EQ(  0, cover1.literal_num( lit7) );
  EXPECT_EQ(  0, cover1.literal_num(~lit7) );
  EXPECT_EQ(  0, cover1.literal_num( lit8) );
  EXPECT_EQ(  0, cover1.literal_num(~lit8) );
  EXPECT_EQ(  0, cover1.literal_num( lit9) );
  EXPECT_EQ(  0, cover1.literal_num(~lit9) );

  auto cube_list1 = cover1.literal_list();
  EXPECT_EQ( 2, cube_list1.size() );
  EXPECT_EQ( 1, cube_list1[0].size() );
  EXPECT_EQ( 1, cube_list1[1].size() );
  EXPECT_EQ( lit2, cube_list1[0][0] );
  EXPECT_EQ( lit3, cube_list1[1][0] );

};

TEST_F(SopTest, Icover_cube_quotient)
{
  Literal lit0(var0, false);
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  SopCover cover1{nv, { {lit0, lit2}, {lit0, lit3}, {lit1, lit2}, {lit1, lit3} } };
  SopCube src2{nv, {lit0} };

  cover1 /= src2;

  EXPECT_EQ( nv, cover1.variable_num() );
  EXPECT_EQ(  2, cover1.cube_num() );
  EXPECT_EQ(  2, cover1.literal_num() );
  EXPECT_EQ(  0, cover1.literal_num( lit0) );
  EXPECT_EQ(  0, cover1.literal_num(~lit0) );
  EXPECT_EQ(  0, cover1.literal_num( lit1) );
  EXPECT_EQ(  0, cover1.literal_num(~lit1) );
  EXPECT_EQ(  1, cover1.literal_num( lit2) );
  EXPECT_EQ(  0, cover1.literal_num(~lit2) );
  EXPECT_EQ(  1, cover1.literal_num( lit3) );
  EXPECT_EQ(  0, cover1.literal_num(~lit3) );
  EXPECT_EQ(  0, cover1.literal_num( lit4) );
  EXPECT_EQ(  0, cover1.literal_num(~lit4) );
  EXPECT_EQ(  0, cover1.literal_num( lit5) );
  EXPECT_EQ(  0, cover1.literal_num(~lit5) );
  EXPECT_EQ(  0, cover1.literal_num( lit6) );
  EXPECT_EQ(  0, cover1.literal_num(~lit6) );
  EXPECT_EQ(  0, cover1.literal_num( lit7) );
  EXPECT_EQ(  0, cover1.literal_num(~lit7) );
  EXPECT_EQ(  0, cover1.literal_num( lit8) );
  EXPECT_EQ(  0, cover1.literal_num(~lit8) );
  EXPECT_EQ(  0, cover1.literal_num( lit9) );
  EXPECT_EQ(  0, cover1.literal_num(~lit9) );

  auto cube_list1 = cover1.literal_list();
  EXPECT_EQ( 2, cube_list1.size() );
  EXPECT_EQ( 1, cube_list1[0].size() );
  EXPECT_EQ( 1, cube_list1[1].size() );
  EXPECT_EQ( lit2, cube_list1[0][0] );
  EXPECT_EQ( lit3, cube_list1[1][0] );

};

TEST_F(SopTest, Icover_literal_quotient)
{
  Literal lit0(var0, false);
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  SopCover cover1{nv, { {lit0, lit2}, {lit0, lit3}, {lit1, lit2}, {lit1, lit3} } };

  cover1 /= lit0;

  EXPECT_EQ( nv, cover1.variable_num() );
  EXPECT_EQ(  2, cover1.cube_num() );
  EXPECT_EQ(  2, cover1.literal_num() );
  EXPECT_EQ(  0, cover1.literal_num( lit0) );
  EXPECT_EQ(  0, cover1.literal_num(~lit0) );
  EXPECT_EQ(  0, cover1.literal_num( lit1) );
  EXPECT_EQ(  0, cover1.literal_num(~lit1) );
  EXPECT_EQ(  1, cover1.literal_num( lit2) );
  EXPECT_EQ(  0, cover1.literal_num(~lit2) );
  EXPECT_EQ(  1, cover1.literal_num( lit3) );
  EXPECT_EQ(  0, cover1.literal_num(~lit3) );
  EXPECT_EQ(  0, cover1.literal_num( lit4) );
  EXPECT_EQ(  0, cover1.literal_num(~lit4) );
  EXPECT_EQ(  0, cover1.literal_num( lit5) );
  EXPECT_EQ(  0, cover1.literal_num(~lit5) );
  EXPECT_EQ(  0, cover1.literal_num( lit6) );
  EXPECT_EQ(  0, cover1.literal_num(~lit6) );
  EXPECT_EQ(  0, cover1.literal_num( lit7) );
  EXPECT_EQ(  0, cover1.literal_num(~lit7) );
  EXPECT_EQ(  0, cover1.literal_num( lit8) );
  EXPECT_EQ(  0, cover1.literal_num(~lit8) );
  EXPECT_EQ(  0, cover1.literal_num( lit9) );
  EXPECT_EQ(  0, cover1.literal_num(~lit9) );

  auto cube_list1 = cover1.literal_list();
  EXPECT_EQ( 2, cube_list1.size() );
  EXPECT_EQ( 1, cube_list1[0].size() );
  EXPECT_EQ( 1, cube_list1[1].size() );
  EXPECT_EQ( lit2, cube_list1[0][0] );
  EXPECT_EQ( lit3, cube_list1[1][0] );

};

TEST_F(SopTest, common_cube)
{
  Literal lit0(var0, false);
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  SopCover src1{nv, { {lit0, lit2}, {lit0, lit3}, {lit0, ~lit4} } };

  SopCube cube1 = src1.common_cube();

  EXPECT_EQ( nv, cube1.variable_num() );
  EXPECT_EQ(  1, cube1.literal_num() );
  EXPECT_TRUE(  cube1.check_literal( lit0) );
  EXPECT_FALSE( cube1.check_literal(~lit0) );
  EXPECT_FALSE( cube1.check_literal( lit1) );
  EXPECT_FALSE( cube1.check_literal(~lit1) );
  EXPECT_FALSE( cube1.check_literal( lit2) );
  EXPECT_FALSE( cube1.check_literal(~lit2) );
  EXPECT_FALSE( cube1.check_literal( lit3) );
  EXPECT_FALSE( cube1.check_literal(~lit3) );
  EXPECT_FALSE( cube1.check_literal( lit4) );
  EXPECT_FALSE( cube1.check_literal(~lit4) );
  EXPECT_FALSE( cube1.check_literal( lit5) );
  EXPECT_FALSE( cube1.check_literal(~lit5) );
  EXPECT_FALSE( cube1.check_literal( lit6) );
  EXPECT_FALSE( cube1.check_literal(~lit6) );
  EXPECT_FALSE( cube1.check_literal( lit7) );
  EXPECT_FALSE( cube1.check_literal(~lit7) );
  EXPECT_FALSE( cube1.check_literal( lit8) );
  EXPECT_FALSE( cube1.check_literal(~lit8) );
  EXPECT_FALSE( cube1.check_literal( lit9) );
  EXPECT_FALSE( cube1.check_literal(~lit9) );
};

TEST_F(SopTest, to_expr1)
{
  Literal lit0{var0, false};
  Literal lit1{var1, false};
  Literal lit2{var2, false};

  SopCube cube1{3, {lit0, lit1}};
  SopCube cube2{3, {~lit2}};
  SopCover cover1{3, {cube1, cube2}};

  auto expr = cover1.expr();
  auto expr_str = expr.to_string();
  EXPECT_EQ( "( ( 0 & 1 ) | ~2 )", expr_str );
}

END_NAMESPACE_YM
