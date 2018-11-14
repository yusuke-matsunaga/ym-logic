
/// @file AlgCoverTest.cc
/// @brief AlgCoverTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.


#include <gtest/gtest.h>
#include "ym/AlgCover.h"
#include "ym/AlgCube.h"


BEGIN_NAMESPACE_YM

TEST(AlgCoverTest, constructor1)
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
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  AlgCover cover1(nv);

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

TEST(AlgCoverTest, constructor2)
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
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  AlgCube cube1{nv, {lit0, lit1}};
  AlgCube cube2{nv, {lit2, lit3}};

  vector<AlgCube> cube_list{cube1, cube2};

  AlgCover cover1{nv, cube_list};

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

  vector<vector<Literal>> cube_list1;
  cover1.to_literal_list(cube_list1);
  EXPECT_EQ( 2, cube_list1.size() );
  EXPECT_EQ( 2, cube_list1[0].size() );
  EXPECT_EQ( 2, cube_list1[1].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit1, cube_list1[0][1] );
  EXPECT_EQ( lit2, cube_list1[1][0] );
  EXPECT_EQ( lit3, cube_list1[1][1] );

};

TEST(AlgCoverTest, constructor3)
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
  AlgCover cover1{nv, cube_list};

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

  vector<vector<Literal>> cube_list1;
  cover1.to_literal_list(cube_list1);
  EXPECT_EQ( 2, cube_list1.size() );
  EXPECT_EQ( 2, cube_list1[0].size() );
  EXPECT_EQ( 2, cube_list1[1].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit1, cube_list1[0][1] );
  EXPECT_EQ( lit2, cube_list1[1][0] );
  EXPECT_EQ( lit3, cube_list1[1][1] );

};

TEST(AlgCoverTest, constructor4)
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
  AlgCover cover1{nv, cube_list};

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

  vector<vector<Literal>> cube_list1;
  cover1.to_literal_list(cube_list1);
  EXPECT_EQ( 2, cube_list1.size() );
  EXPECT_EQ( 2, cube_list1[0].size() );
  EXPECT_EQ( 2, cube_list1[1].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit1, cube_list1[0][1] );
  EXPECT_EQ( lit2, cube_list1[1][0] );
  EXPECT_EQ( lit3, cube_list1[1][1] );

};

TEST(AlgCoverTest, copy_constructor)
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
  AlgCover src_cover{nv, cube_list};

  AlgCover cover1{src_cover};

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

  vector<vector<Literal>> cube_list1;
  cover1.to_literal_list(cube_list1);
  EXPECT_EQ( 2, cube_list1.size() );
  EXPECT_EQ( 2, cube_list1[0].size() );
  EXPECT_EQ( 2, cube_list1[1].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit1, cube_list1[0][1] );
  EXPECT_EQ( lit2, cube_list1[1][0] );
  EXPECT_EQ( lit3, cube_list1[1][1] );

};

TEST(AlgCoverTest, copy_assignment)
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
  AlgCover src_cover{nv, cube_list};

  AlgCover cover1 = src_cover;

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

  vector<vector<Literal>> cube_list1;
  cover1.to_literal_list(cube_list1);
  EXPECT_EQ( 2, cube_list1.size() );
  EXPECT_EQ( 2, cube_list1[0].size() );
  EXPECT_EQ( 2, cube_list1[1].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit1, cube_list1[0][1] );
  EXPECT_EQ( lit2, cube_list1[1][0] );
  EXPECT_EQ( lit3, cube_list1[1][1] );

};

TEST(AlgCoverTest, move_constructor)
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
  AlgCover src_cover{nv, cube_list};

  AlgCover cover1{std::move(src_cover)};

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

  vector<vector<Literal>> cube_list1;
  cover1.to_literal_list(cube_list1);
  EXPECT_EQ( 2, cube_list1.size() );
  EXPECT_EQ( 2, cube_list1[0].size() );
  EXPECT_EQ( 2, cube_list1[1].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit1, cube_list1[0][1] );
  EXPECT_EQ( lit2, cube_list1[1][0] );
  EXPECT_EQ( lit3, cube_list1[1][1] );

};

TEST(AlgCoverTest, move_assignment)
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
  AlgCover src_cover{nv, cube_list};

  AlgCover cover1 = std::move(src_cover);

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

  vector<vector<Literal>> cube_list1;
  cover1.to_literal_list(cube_list1);
  EXPECT_EQ( 2, cube_list1.size() );
  EXPECT_EQ( 2, cube_list1[0].size() );
  EXPECT_EQ( 2, cube_list1[1].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit1, cube_list1[0][1] );
  EXPECT_EQ( lit2, cube_list1[1][0] );
  EXPECT_EQ( lit3, cube_list1[1][1] );

};

TEST(AlgCoverTest, cube_constructor)
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
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  AlgCube src_cube{nv, {lit0, lit1}};

  AlgCover cover1{src_cube};

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

  vector<vector<Literal>> cube_list1;
  cover1.to_literal_list(cube_list1);
  EXPECT_EQ( 1, cube_list1.size() );
  EXPECT_EQ( 2, cube_list1[0].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit1, cube_list1[0][1] );

};

TEST(AlgCoverTest, cube_move_constructor)
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
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  AlgCube src_cube{nv, {lit0, lit1}};

  AlgCover cover1{std::move(src_cube)};

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

  vector<vector<Literal>> cube_list1;
  cover1.to_literal_list(cube_list1);
  EXPECT_EQ( 1, cube_list1.size() );
  EXPECT_EQ( 2, cube_list1[0].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit1, cube_list1[0][1] );

};

TEST(AlgCoverTest, sum)
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
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  std::initializer_list<std::initializer_list<Literal>> cube_list1{ {lit0, lit1} };
  AlgCover src1{nv, cube_list1};

  std::initializer_list<std::initializer_list<Literal>> cube_list2{ {lit2, lit3} };
  AlgCover src2{nv, cube_list2};

  AlgCover cover1 = src1 + src2;

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

  vector<vector<Literal>> tmp_list;
  cover1.to_literal_list(tmp_list);
  EXPECT_EQ( 2, tmp_list.size() );
  EXPECT_EQ( 2, tmp_list[0].size() );
  EXPECT_EQ( 2, tmp_list[1].size() );
  EXPECT_EQ( lit0, tmp_list[0][0] );
  EXPECT_EQ( lit1, tmp_list[0][1] );
  EXPECT_EQ( lit2, tmp_list[1][0] );
  EXPECT_EQ( lit3, tmp_list[1][1] );

};

TEST(AlgCoverTest, cube_sum1)
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
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  AlgCover src1{nv, { {lit0, lit1} }};
  AlgCube src2{nv, {lit2, lit3}};

  AlgCover cover1 = src1 + src2;

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

  vector<vector<Literal>> tmp_list;
  cover1.to_literal_list(tmp_list);
  EXPECT_EQ( 2, tmp_list.size() );
  EXPECT_EQ( 2, tmp_list[0].size() );
  EXPECT_EQ( 2, tmp_list[1].size() );
  EXPECT_EQ( lit0, tmp_list[0][0] );
  EXPECT_EQ( lit1, tmp_list[0][1] );
  EXPECT_EQ( lit2, tmp_list[1][0] );
  EXPECT_EQ( lit3, tmp_list[1][1] );

};

TEST(AlgCoverTest, cube_sum2)
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
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  AlgCube src1{nv, {lit0, lit1} };
  AlgCover src2{nv, { {lit2, lit3} } };

  AlgCover cover1 = src1 + src2;

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

  vector<vector<Literal>> tmp_list;
  cover1.to_literal_list(tmp_list);
  EXPECT_EQ( 2, tmp_list.size() );
  EXPECT_EQ( 2, tmp_list[0].size() );
  EXPECT_EQ( 2, tmp_list[1].size() );
  EXPECT_EQ( lit0, tmp_list[0][0] );
  EXPECT_EQ( lit1, tmp_list[0][1] );
  EXPECT_EQ( lit2, tmp_list[1][0] );
  EXPECT_EQ( lit3, tmp_list[1][1] );

};

TEST(AlgCoverTest, ssum)
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
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  AlgCover cover1{nv, { {lit0, lit1} } };
  AlgCover src2{nv, { {lit2, lit3} } };

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

  vector<vector<Literal>> tmp_list;
  cover1.to_literal_list(tmp_list);
  EXPECT_EQ( 2, tmp_list.size() );
  EXPECT_EQ( 2, tmp_list[0].size() );
  EXPECT_EQ( 2, tmp_list[1].size() );
  EXPECT_EQ( lit0, tmp_list[0][0] );
  EXPECT_EQ( lit1, tmp_list[0][1] );
  EXPECT_EQ( lit2, tmp_list[1][0] );
  EXPECT_EQ( lit3, tmp_list[1][1] );

};

TEST(AlgCoverTest, scube_sum1)
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
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  AlgCover cover1{nv, { {lit0, lit1} }};
  AlgCube src2{nv, {lit2, lit3}};

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

  vector<vector<Literal>> tmp_list;
  cover1.to_literal_list(tmp_list);
  EXPECT_EQ( 2, tmp_list.size() );
  EXPECT_EQ( 2, tmp_list[0].size() );
  EXPECT_EQ( 2, tmp_list[1].size() );
  EXPECT_EQ( lit0, tmp_list[0][0] );
  EXPECT_EQ( lit1, tmp_list[0][1] );
  EXPECT_EQ( lit2, tmp_list[1][0] );
  EXPECT_EQ( lit3, tmp_list[1][1] );

};

TEST(AlgCoverTest, scube_sum2)
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
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  AlgCube src1{nv, {lit0, lit1} };
  AlgCover cover1{nv, { {lit2, lit3} } };

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

  vector<vector<Literal>> tmp_list;
  cover1.to_literal_list(tmp_list);
  EXPECT_EQ( 2, tmp_list.size() );
  EXPECT_EQ( 2, tmp_list[0].size() );
  EXPECT_EQ( 2, tmp_list[1].size() );
  EXPECT_EQ( lit0, tmp_list[0][0] );
  EXPECT_EQ( lit1, tmp_list[0][1] );
  EXPECT_EQ( lit2, tmp_list[1][0] );
  EXPECT_EQ( lit3, tmp_list[1][1] );

};

TEST(AlgCoverTest, isum)
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
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  AlgCover cover1{nv, { {lit0, lit1} } };
  AlgCover src2{nv, { {lit2, lit3} } };

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

  vector<vector<Literal>> tmp_list;
  cover1.to_literal_list(tmp_list);
  EXPECT_EQ( 2, tmp_list.size() );
  EXPECT_EQ( 2, tmp_list[0].size() );
  EXPECT_EQ( 2, tmp_list[1].size() );
  EXPECT_EQ( lit0, tmp_list[0][0] );
  EXPECT_EQ( lit1, tmp_list[0][1] );
  EXPECT_EQ( lit2, tmp_list[1][0] );
  EXPECT_EQ( lit3, tmp_list[1][1] );

};

TEST(AlgCoverTest, icube_sum1)
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
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  AlgCover cover1{nv, { {lit0, lit1} }};
  AlgCube src2{nv, {lit2, lit3}};

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

  vector<vector<Literal>> tmp_list;
  cover1.to_literal_list(tmp_list);
  EXPECT_EQ( 2, tmp_list.size() );
  EXPECT_EQ( 2, tmp_list[0].size() );
  EXPECT_EQ( 2, tmp_list[1].size() );
  EXPECT_EQ( lit0, tmp_list[0][0] );
  EXPECT_EQ( lit1, tmp_list[0][1] );
  EXPECT_EQ( lit2, tmp_list[1][0] );
  EXPECT_EQ( lit3, tmp_list[1][1] );

};

TEST(AlgCoverTest, diff)
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
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  AlgCover src1{nv, { {lit0, lit1}, {lit2, lit3} } };
  AlgCover src2{nv, { {lit3, lit2}, {lit0, ~lit1} } };

  AlgCover cover1 = src1 - src2;

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

  vector<vector<Literal>> cube_list1;
  cover1.to_literal_list(cube_list1);
  EXPECT_EQ( 1, cube_list1.size() );
  EXPECT_EQ( 2, cube_list1[0].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit1, cube_list1[0][1] );

};

TEST(AlgCoverTest, cube_diff)
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
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  AlgCover src1{nv, { {lit0, lit1}, {lit2, lit3} } };
  AlgCube src2{nv, {lit3, lit2} };

  AlgCover cover1 = src1 - src2;

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

  vector<vector<Literal>> cube_list1;
  cover1.to_literal_list(cube_list1);
  EXPECT_EQ( 1, cube_list1.size() );
  EXPECT_EQ( 2, cube_list1[0].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit1, cube_list1[0][1] );

};

TEST(AlgCoverTest, sdiff)
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
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  AlgCover cover1{nv, { {lit0, lit1}, {lit2, lit3} } };
  AlgCover src2{nv, { {lit3, lit2}, {lit0, ~lit1} } };

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

  vector<vector<Literal>> cube_list1;
  cover1.to_literal_list(cube_list1);
  EXPECT_EQ( 1, cube_list1.size() );
  EXPECT_EQ( 2, cube_list1[0].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit1, cube_list1[0][1] );

};

TEST(AlgCoverTest, scube_diff)
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
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  AlgCover cover1{nv, { {lit0, lit1}, {lit2, lit3} } };
  AlgCube src2{nv, {lit3, lit2} };

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

  vector<vector<Literal>> cube_list1;
  cover1.to_literal_list(cube_list1);
  EXPECT_EQ( 1, cube_list1.size() );
  EXPECT_EQ( 2, cube_list1[0].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit1, cube_list1[0][1] );

};

TEST(AlgCoverTest, idiff)
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
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  AlgCover cover1{nv, { {lit0, lit1}, {lit2, lit3} } };
  AlgCover src2{nv, { {lit3, lit2}, {lit0, ~lit1} } };

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

  vector<vector<Literal>> cube_list1;
  cover1.to_literal_list(cube_list1);
  EXPECT_EQ( 1, cube_list1.size() );
  EXPECT_EQ( 2, cube_list1[0].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit1, cube_list1[0][1] );

};

TEST(AlgCoverTest, icube_diff)
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
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  AlgCover cover1{nv, { {lit0, lit1}, {lit2, lit3} } };
  AlgCube src2{nv, {lit3, lit2} };

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

  vector<vector<Literal>> cube_list1;
  cover1.to_literal_list(cube_list1);
  EXPECT_EQ( 1, cube_list1.size() );
  EXPECT_EQ( 2, cube_list1[0].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit1, cube_list1[0][1] );

};

TEST(AlgCoverTest, product)
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
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  AlgCover src1{nv, { {lit0}, {lit1} } };
  AlgCover src2{nv, { {lit2}, {lit3} } };

  AlgCover cover1 = src1 * src2;

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

  vector<vector<Literal>> cube_list1;
  cover1.to_literal_list(cube_list1);
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

TEST(AlgCoverTest, cube_product1)
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
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  AlgCover src1{nv, { {lit0}, {lit1} } };
  AlgCube src2{nv,  {lit2} };

  AlgCover cover1 = src1 * src2;

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

  vector<vector<Literal>> cube_list1;
  cover1.to_literal_list(cube_list1);
  EXPECT_EQ( 2, cube_list1.size() );
  EXPECT_EQ( 2, cube_list1[0].size() );
  EXPECT_EQ( 2, cube_list1[1].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit2, cube_list1[0][1] );
  EXPECT_EQ( lit1, cube_list1[1][0] );
  EXPECT_EQ( lit2, cube_list1[1][1] );

};

TEST(AlgCoverTest, cube_product2)
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
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  AlgCube src1{nv,  {lit2} };
  AlgCover src2{nv, { {lit0}, {lit1} } };

  AlgCover cover1 = src1 * src2;

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

  vector<vector<Literal>> cube_list1;
  cover1.to_literal_list(cube_list1);
  EXPECT_EQ( 2, cube_list1.size() );
  EXPECT_EQ( 2, cube_list1[0].size() );
  EXPECT_EQ( 2, cube_list1[1].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit2, cube_list1[0][1] );
  EXPECT_EQ( lit1, cube_list1[1][0] );
  EXPECT_EQ( lit2, cube_list1[1][1] );

};

TEST(AlgCoverTest, literal_product1)
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
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  AlgCover src1{nv, { {lit0}, {lit1} } };

  AlgCover cover1 = src1 * lit2;

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

  vector<vector<Literal>> cube_list1;
  cover1.to_literal_list(cube_list1);
  EXPECT_EQ( 2, cube_list1.size() );
  EXPECT_EQ( 2, cube_list1[0].size() );
  EXPECT_EQ( 2, cube_list1[1].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit2, cube_list1[0][1] );
  EXPECT_EQ( lit1, cube_list1[1][0] );
  EXPECT_EQ( lit2, cube_list1[1][1] );

};

TEST(AlgCoverTest, literal_product2)
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
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  AlgCover src1{nv, { {lit0}, {lit1} } };

  AlgCover cover1 = lit2 * src1;

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

  vector<vector<Literal>> cube_list1;
  cover1.to_literal_list(cube_list1);
  EXPECT_EQ( 2, cube_list1.size() );
  EXPECT_EQ( 2, cube_list1[0].size() );
  EXPECT_EQ( 2, cube_list1[1].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit2, cube_list1[0][1] );
  EXPECT_EQ( lit1, cube_list1[1][0] );
  EXPECT_EQ( lit2, cube_list1[1][1] );

};

TEST(AlgCoverTest, sproduct)
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
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  AlgCover cover1{nv, { {lit0}, {lit1} } };
  AlgCover src2{nv, { {lit2}, {lit3} } };

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

  vector<vector<Literal>> cube_list1;
  cover1.to_literal_list(cube_list1);
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

TEST(AlgCoverTest, scube_product1)
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
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  AlgCover cover1{nv, { {lit0}, {lit1} } };
  AlgCube src2{nv,  {lit2} };

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

  vector<vector<Literal>> cube_list1;
  cover1.to_literal_list(cube_list1);
  EXPECT_EQ( 2, cube_list1.size() );
  EXPECT_EQ( 2, cube_list1[0].size() );
  EXPECT_EQ( 2, cube_list1[1].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit2, cube_list1[0][1] );
  EXPECT_EQ( lit1, cube_list1[1][0] );
  EXPECT_EQ( lit2, cube_list1[1][1] );

};

TEST(AlgCoverTest, scube_product2)
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
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  AlgCube src1{nv,  {lit2} };
  AlgCover cover1{nv, { {lit0}, {lit1} } };

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

  vector<vector<Literal>> cube_list1;
  cover1.to_literal_list(cube_list1);
  EXPECT_EQ( 2, cube_list1.size() );
  EXPECT_EQ( 2, cube_list1[0].size() );
  EXPECT_EQ( 2, cube_list1[1].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit2, cube_list1[0][1] );
  EXPECT_EQ( lit1, cube_list1[1][0] );
  EXPECT_EQ( lit2, cube_list1[1][1] );

};

TEST(AlgCoverTest, sliteral_product1)
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
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  AlgCover cover1{nv, { {lit0}, {lit1} } };

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

  vector<vector<Literal>> cube_list1;
  cover1.to_literal_list(cube_list1);
  EXPECT_EQ( 2, cube_list1.size() );
  EXPECT_EQ( 2, cube_list1[0].size() );
  EXPECT_EQ( 2, cube_list1[1].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit2, cube_list1[0][1] );
  EXPECT_EQ( lit1, cube_list1[1][0] );
  EXPECT_EQ( lit2, cube_list1[1][1] );

};

TEST(AlgCoverTest, sliteral_product2)
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
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  AlgCover cover1{nv, { {lit0}, {lit1} } };

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

  vector<vector<Literal>> cube_list1;
  cover1.to_literal_list(cube_list1);
  EXPECT_EQ( 2, cube_list1.size() );
  EXPECT_EQ( 2, cube_list1[0].size() );
  EXPECT_EQ( 2, cube_list1[1].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit2, cube_list1[0][1] );
  EXPECT_EQ( lit1, cube_list1[1][0] );
  EXPECT_EQ( lit2, cube_list1[1][1] );

};

TEST(AlgCoverTest, iproduct)
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
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  AlgCover cover1{nv, { {lit0}, {lit1} } };
  AlgCover src2{nv, { {lit2}, {lit3} } };

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

  vector<vector<Literal>> cube_list1;
  cover1.to_literal_list(cube_list1);
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

TEST(AlgCoverTest, icube_product1)
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
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  AlgCover cover1{nv, { {lit0}, {lit1} } };
  AlgCube src2{nv,  {lit2} };

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

  vector<vector<Literal>> cube_list1;
  cover1.to_literal_list(cube_list1);
  EXPECT_EQ( 2, cube_list1.size() );
  EXPECT_EQ( 2, cube_list1[0].size() );
  EXPECT_EQ( 2, cube_list1[1].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit2, cube_list1[0][1] );
  EXPECT_EQ( lit1, cube_list1[1][0] );
  EXPECT_EQ( lit2, cube_list1[1][1] );

};

TEST(AlgCoverTest, iliteral_product1)
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
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  AlgCover cover1{nv, { {lit0}, {lit1} } };

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

  vector<vector<Literal>> cube_list1;
  cover1.to_literal_list(cube_list1);
  EXPECT_EQ( 2, cube_list1.size() );
  EXPECT_EQ( 2, cube_list1[0].size() );
  EXPECT_EQ( 2, cube_list1[1].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit2, cube_list1[0][1] );
  EXPECT_EQ( lit1, cube_list1[1][0] );
  EXPECT_EQ( lit2, cube_list1[1][1] );

};

TEST(AlgCoverTest, quotient)
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
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  AlgCover src1{nv, { {lit0, lit2}, {lit0, lit3}, {lit1, lit2}, {lit1, lit3} } };
  AlgCover src2{nv, { {lit0}, {lit1} } };

  AlgCover cover1 = src1 / src2;

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

  vector<vector<Literal>> cube_list1;
  cover1.to_literal_list(cube_list1);
  EXPECT_EQ( 2, cube_list1.size() );
  EXPECT_EQ( 1, cube_list1[0].size() );
  EXPECT_EQ( 1, cube_list1[1].size() );
  EXPECT_EQ( lit2, cube_list1[0][0] );
  EXPECT_EQ( lit3, cube_list1[1][0] );

};

TEST(AlgCoverTest, cube_quotient)
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
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  AlgCover src1{nv, { {lit0, lit2}, {lit0, lit3}, {lit1, lit2}, {lit1, lit3} } };
  AlgCube src2{nv, {lit0} };

  AlgCover cover1 = src1 / src2;

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

  vector<vector<Literal>> cube_list1;
  cover1.to_literal_list(cube_list1);
  EXPECT_EQ( 2, cube_list1.size() );
  EXPECT_EQ( 1, cube_list1[0].size() );
  EXPECT_EQ( 1, cube_list1[1].size() );
  EXPECT_EQ( lit2, cube_list1[0][0] );
  EXPECT_EQ( lit3, cube_list1[1][0] );

};

TEST(AlgCoverTest, literal_quotient)
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
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  AlgCover src1{nv, { {lit0, lit2}, {lit0, lit3}, {lit1, lit2}, {lit1, lit3} } };

  AlgCover cover1 = src1 / lit0;

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

  vector<vector<Literal>> cube_list1;
  cover1.to_literal_list(cube_list1);
  EXPECT_EQ( 2, cube_list1.size() );
  EXPECT_EQ( 1, cube_list1[0].size() );
  EXPECT_EQ( 1, cube_list1[1].size() );
  EXPECT_EQ( lit2, cube_list1[0][0] );
  EXPECT_EQ( lit3, cube_list1[1][0] );

};

TEST(AlgCoverTest, iquotient)
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
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  AlgCover cover1{nv, { {lit0, lit2}, {lit0, lit3}, {lit1, lit2}, {lit1, lit3} } };
  AlgCover src2{nv, { {lit0}, {lit1} } };

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

  vector<vector<Literal>> cube_list1;
  cover1.to_literal_list(cube_list1);
  EXPECT_EQ( 2, cube_list1.size() );
  EXPECT_EQ( 1, cube_list1[0].size() );
  EXPECT_EQ( 1, cube_list1[1].size() );
  EXPECT_EQ( lit2, cube_list1[0][0] );
  EXPECT_EQ( lit3, cube_list1[1][0] );

};

TEST(AlgCoverTest, icube_quotient)
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
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  AlgCover cover1{nv, { {lit0, lit2}, {lit0, lit3}, {lit1, lit2}, {lit1, lit3} } };
  AlgCube src2{nv, {lit0} };

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

  vector<vector<Literal>> cube_list1;
  cover1.to_literal_list(cube_list1);
  EXPECT_EQ( 2, cube_list1.size() );
  EXPECT_EQ( 1, cube_list1[0].size() );
  EXPECT_EQ( 1, cube_list1[1].size() );
  EXPECT_EQ( lit2, cube_list1[0][0] );
  EXPECT_EQ( lit3, cube_list1[1][0] );

};

TEST(AlgCoverTest, iliteral_quotient)
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
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  AlgCover cover1{nv, { {lit0, lit2}, {lit0, lit3}, {lit1, lit2}, {lit1, lit3} } };

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

  vector<vector<Literal>> cube_list1;
  cover1.to_literal_list(cube_list1);
  EXPECT_EQ( 2, cube_list1.size() );
  EXPECT_EQ( 1, cube_list1[0].size() );
  EXPECT_EQ( 1, cube_list1[1].size() );
  EXPECT_EQ( lit2, cube_list1[0][0] );
  EXPECT_EQ( lit3, cube_list1[1][0] );

};

TEST(AlgCoverTest, common_cube)
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
  Literal lit1(var1, false);
  Literal lit2(var2, false);
  Literal lit3(var3, false);
  Literal lit4(var4, false);
  Literal lit5(var5, false);
  Literal lit6(var6, false);
  Literal lit7(var7, false);
  Literal lit8(var8, false);
  Literal lit9(var9, false);

  AlgCover src1{nv, { {lit0, lit2}, {lit0, lit3}, {lit0, ~lit4} } };

  AlgCube cube1 = src1.common_cube();

  EXPECT_EQ( nv, cube1.variable_num() );
  EXPECT_EQ(  1, cube1.literal_num() );
  EXPECT_TRUE(  cube1.has_literal( lit0) );
  EXPECT_FALSE( cube1.has_literal(~lit0) );
  EXPECT_FALSE( cube1.has_literal( lit1) );
  EXPECT_FALSE( cube1.has_literal(~lit1) );
  EXPECT_FALSE( cube1.has_literal( lit2) );
  EXPECT_FALSE( cube1.has_literal(~lit2) );
  EXPECT_FALSE( cube1.has_literal( lit3) );
  EXPECT_FALSE( cube1.has_literal(~lit3) );
  EXPECT_FALSE( cube1.has_literal( lit4) );
  EXPECT_FALSE( cube1.has_literal(~lit4) );
  EXPECT_FALSE( cube1.has_literal( lit5) );
  EXPECT_FALSE( cube1.has_literal(~lit5) );
  EXPECT_FALSE( cube1.has_literal( lit6) );
  EXPECT_FALSE( cube1.has_literal(~lit6) );
  EXPECT_FALSE( cube1.has_literal( lit7) );
  EXPECT_FALSE( cube1.has_literal(~lit7) );
  EXPECT_FALSE( cube1.has_literal( lit8) );
  EXPECT_FALSE( cube1.has_literal(~lit8) );
  EXPECT_FALSE( cube1.has_literal( lit9) );
  EXPECT_FALSE( cube1.has_literal(~lit9) );
};


END_NAMESPACE_YM
