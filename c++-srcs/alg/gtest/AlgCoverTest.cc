
/// @file AlgCoverTest.cc
/// @brief AlgCoverTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "AlgTest.h"
#include "ym/AlgCover.h"
#include "ym/AlgCube.h"


BEGIN_NAMESPACE_YM

TEST_F(AlgTest, constructor1)
{
  auto cover1 = AlgCover{nv};

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

TEST_F(AlgTest, constructor2)
{
  auto cube1 = AlgCube{nv, {lit0, lit1}};
  auto cube2 = AlgCube{nv, {lit2, lit3}};

  auto cube_list = vector<AlgCube>{cube1, cube2};

  auto cover1 = AlgCover{nv, cube_list};

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

TEST_F(AlgTest, constructor2_bad)
{
  auto cube1 = AlgCube{nv + 1, {lit0, lit1}};
  auto cube2 = AlgCube{nv, {lit2, lit3}};

  auto cube_list = vector<AlgCube>{cube1, cube2};

  ASSERT_THROW( (AlgCover{nv, cube_list}), std::invalid_argument );
}

TEST_F(AlgTest, constructor3)
{
  auto cube_list = vector<vector<Literal>>{ {lit0, lit1},
					    {lit2, lit3} };
  auto cover1 = AlgCover{nv, cube_list};

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

TEST_F(AlgTest, constructor4)
{
  auto cover1 = AlgCover{nv, { {lit0, lit1},
			       {lit2, lit3} }};

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

TEST_F(AlgTest, copy_constructor)
{
  auto src_cover = AlgCover{nv, { {lit0, lit1},
				  {lit2, lit3} }};

  auto cover1 = AlgCover{src_cover};

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

TEST_F(AlgTest, copy_assignment)
{
  auto src_cover = AlgCover{nv,{ {lit0, lit1},
				 {lit2, lit3} }};

  auto cover1 = src_cover;

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

TEST_F(AlgTest, move_constructor)
{
  auto src_cover = AlgCover{nv, { {lit0, lit1},
				  {lit2, lit3} }};

  auto cover1 = AlgCover{std::move(src_cover)};

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

TEST_F(AlgTest, move_assignment)
{
  auto src_cover = AlgCover{nv, { {lit0, lit1},
				  {lit2, lit3} }};

  auto cover1 = std::move(src_cover);

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

TEST_F(AlgTest, cube_constructor)
{
  auto src_cube = AlgCube{nv, {lit0, lit1}};

  auto cover1 = AlgCover{src_cube};

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

#if 0
TEST_F(AlgTest, cube_move_constructor)
{
  auto lit0 = Literal(var0, false);
  auto lit1 = Literal(var1, false);
  auto lit2 = Literal(var2, false);
  auto lit3 = Literal(var3, false);
  auto lit4 = Literal(var4, false);
  auto lit5 = Literal(var5, false);
  auto lit6 = Literal(var6, false);
  auto lit7 = Literal(var7, false);
  auto lit8 = Literal(var8, false);
  auto lit9 = Literal(var9, false);

  auto src = AlgCube_cube{nv, {lit0, lit1}};

  AlgCover cover1{std::move(src_cube)};

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
#endif

TEST_F(AlgTest, cover_cover_sum)
{
  auto src1 = AlgCover{nv, { {lit0, lit1} } };
  auto src2 = AlgCover{nv, { {lit2, lit3} } };

  auto cover1 = src1 + src2;

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

TEST_F(AlgTest, cover_cover_sum_bad)
{
  auto src1 = AlgCover{nv, { {lit0, lit1} } };
  auto src2 = AlgCover{nv + 1, { {lit2, lit3} } };

  ASSERT_THROW( src1 + src2, std::invalid_argument );
}

TEST_F(AlgTest, cover_cube_sum)
{
  auto src1 = AlgCover{nv, { {lit0, lit1} } };
  auto src2 = AlgCube{nv, {lit2, lit3} };

  auto cover1 = src1 + src2;

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

TEST_F(AlgTest, cover_cube_sum_bad)
{
  auto src1 = AlgCover{nv, { {lit0, lit1} } };
  auto src2 = AlgCube{nv + 1, {lit2, lit3} };

  ASSERT_THROW( src1 + src2, std::invalid_argument );
}

TEST_F(AlgTest, cube_cover_sum)
{
  auto src1 = AlgCube{nv, {lit0, lit1} };
  auto src2 = AlgCover{nv, { {lit2, lit3} } };

  auto cover1 = src1 + src2;

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

TEST_F(AlgTest, cube_cover_sum_bad)
{
  auto src1 = AlgCube{nv, {lit0, lit1} };
  auto src2 = AlgCover{nv + 1, { {lit2, lit3} } };

  ASSERT_THROW( src1 + src2, std::invalid_argument );
}

TEST_F(AlgTest, Scover_cover_sum)
{
  auto cover1 = AlgCover{nv, { {lit0, lit1} } };
  auto src2 = AlgCover{nv, { {lit2, lit3} } };

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

TEST_F(AlgTest, Scover_cube_sum1)
{
  auto cover1 = AlgCover{nv, { {lit0, lit1} }};
  auto src2 = AlgCube{nv, {lit2, lit3}};

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

TEST_F(AlgTest, cube_Scover_sum)
{
  auto src1 = AlgCube{nv, {lit0, lit1} };
  auto cover1 = AlgCover{nv, { {lit2, lit3} } };

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

TEST_F(AlgTest, Icover_cover_sum)
{
  auto cover1 = AlgCover{nv, { {lit0, lit1} } };
  auto src2 = AlgCover{nv, { {lit2, lit3} } };

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

TEST_F(AlgTest, Icover_cover_sum_bad)
{
  auto cover1 = AlgCover{nv, { {lit0, lit1} } };
  auto src2 = AlgCover{nv + 1, { {lit2, lit3} } };

  ASSERT_THROW( cover1 += src2, std::invalid_argument );
}

TEST_F(AlgTest, Icover_cube_sum)
{
  auto cover1 = AlgCover{nv, { {lit0, lit1} }};
  auto src2 = AlgCube{nv, {lit2, lit3}};

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

TEST_F(AlgTest, Icover_cube_sum_bad)
{
  auto cover1 = AlgCover{nv, { {lit0, lit1} }};
  auto src2 = AlgCube{nv + 1, {lit2, lit3}};

  ASSERT_THROW( cover1 += src2, std::invalid_argument );
}

TEST_F(AlgTest, cover_cover_diff)
{
  auto src1 = AlgCover{nv, { {lit0, lit1},
			     {lit2, lit3} } };
  auto src2 = AlgCover{nv, { {lit3, lit2},
			     {lit0, ~lit1} } };

  auto cover1 = src1 - src2;

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

TEST_F(AlgTest, cover_cover_diff_bad)
{
  auto src1 = AlgCover{nv, { {lit0, lit1},
			     {lit2, lit3} } };
  auto src2 = AlgCover{nv + 1, { {lit3, lit2},
				 {lit0, ~lit1} } };

  ASSERT_THROW( src1 - src2, std::invalid_argument );
}

TEST_F(AlgTest, Rcover_cover_diff)
{
  auto src1 = AlgCover{nv, { {lit0, lit1},
			     {lit2, lit3} } };
  auto src2 = AlgCover{nv, { {lit3, lit2},
			     {lit0, ~lit1} } };

  auto cover1 = std::move(src1) - src2;

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

TEST_F(AlgTest, cover_cube_diff)
{
  auto src1 = AlgCover{nv, { {lit0, lit1},
			     {lit2, lit3} } };
  auto src2 = AlgCube{nv, {lit3, lit2} };

  auto cover1 = src1 - src2;

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

TEST_F(AlgTest, cover_cube_diff_bad)
{
  auto src1 = AlgCover{nv, { {lit0, lit1},
			     {lit2, lit3} } };
  auto src2 = AlgCube{nv + 1, {lit3, lit2} };

  ASSERT_THROW( src1 - src2, std::invalid_argument );
}

TEST_F(AlgTest, Scover_cover_diff)
{
  auto cover1 = AlgCover{nv, { {lit0, lit1},
			       {lit2, lit3} } };
  auto src2 = AlgCover{nv, { {lit3, lit2},
			     {lit0, ~lit1} } };

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

TEST_F(AlgTest, Scover_cube_diff)
{
  auto cover1 = AlgCover{nv, { {lit0, lit1},
			       {lit2, lit3} } };
  auto src2 = AlgCube{nv, {lit3, lit2} };

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

TEST_F(AlgTest, Icover_cover_diff)
{
  auto cover1 = AlgCover{nv, { {lit0, lit1},
			       {lit2, lit3} } };
  auto src2 = AlgCover{nv, { {lit3, lit2},
			     {lit0, ~lit1} } };

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

TEST_F(AlgTest, Icover_cover_diff_bad)
{
  auto cover1 = AlgCover{nv, { {lit0, lit1},
			       {lit2, lit3} } };
  auto src2 = AlgCover{nv + 1, { {lit3, lit2},
				 {lit0, ~lit1} } };

  ASSERT_THROW( cover1 -= src2, std::invalid_argument );
}

TEST_F(AlgTest, Icover_cube_diff)
{
  auto cover1 = AlgCover{nv, { {lit0, lit1},
			       {lit2, lit3} } };
  auto src2 = AlgCube{nv, {lit3, lit2} };

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

TEST_F(AlgTest, Icover_cube_diff_bad)
{
  auto cover1 = AlgCover{nv, { {lit0, lit1},
			       {lit2, lit3} } };
  auto src2 = AlgCube{nv + 1, {lit3, lit2} };

  ASSERT_THROW( cover1 -= src2, std::invalid_argument );
}

TEST_F(AlgTest, cover_cover_product)
{
  auto src1 = AlgCover{nv, { {lit0}, {lit1} } };
  auto src2 = AlgCover{nv, { {lit2}, {lit3} } };

  auto cover1 = src1 * src2;

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

TEST_F(AlgTest, cover_cover_product_bad)
{
  auto src1 = AlgCover{nv, { {lit0}, {lit1} } };
  auto src2 = AlgCover{nv + 1, { {lit2}, {lit3} } };

  ASSERT_THROW( src1 * src2, std::invalid_argument );
}

TEST_F(AlgTest, cover_cube_product)
{
  auto src1 = AlgCover{nv, { {lit0}, {lit1} } };
  auto src2 = AlgCube{nv,  {lit2} };

  auto cover1 = src1 * src2;

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

TEST_F(AlgTest, cover_cube_product_bad)
{
  auto src1 = AlgCover{nv, { {lit0}, {lit1} } };
  auto src2 = AlgCube{nv + 1,  {lit2} };

  ASSERT_THROW( src1 * src2, std::invalid_argument );
}

TEST_F(AlgTest, Rcover_cube_product)
{
  auto src1 = AlgCover{nv, { {lit0}, {lit1} } };
  auto src2 = AlgCube{nv,  {lit2} };

  auto cover1 = std::move(src1) * src2;

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

TEST_F(AlgTest, cube_cover_product)
{
  auto src1 = AlgCube{nv,  {lit2} };
  auto src2 = AlgCover{nv, { {lit0}, {lit1} } };

  auto cover1 = src1 * src2;

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

TEST_F(AlgTest, cube_cover_product_bad)
{
  auto src1 = AlgCube{nv,  {lit2} };
  auto src2 = AlgCover{nv + 1, { {lit0}, {lit1} } };

  ASSERT_THROW( src1 * src2, std::invalid_argument );
}

TEST_F(AlgTest, cube_Rcover_product)
{
  auto src1 = AlgCube{nv,  {lit2} };
  auto src2 = AlgCover{nv, { {lit0}, {lit1} } };

  auto cover1 = src1 * std::move(src2);

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

TEST_F(AlgTest, cover_literal_product)
{
  auto src1 = AlgCover{nv, { {lit0}, {lit1} } };

  auto cover1 = src1 * lit2;

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

TEST_F(AlgTest, Rcover_literal_product)
{
  auto src1 = AlgCover{nv, { {lit0}, {lit1} } };

  auto cover1 = std::move(src1) * lit2;

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

TEST_F(AlgTest, literal_cover_product)
{
  auto src1 = AlgCover{nv, { {lit0}, {lit1} } };

  auto cover1 = lit2 * src1;

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

TEST_F(AlgTest, literal_Rcover_product)
{
  auto src1 = AlgCover{nv, { {lit0}, {lit1} } };

  auto cover1 = lit2 * std::move(src1);

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

TEST_F(AlgTest, Scover_cover_product)
{
  auto cover1 = AlgCover{nv, { {lit0}, {lit1} } };
  auto src2 = AlgCover{nv, { {lit2}, {lit3} } };

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

TEST_F(AlgTest, cover_Scover_product)
{
  auto src1 = AlgCover{nv, { {lit0}, {lit1} } };
  auto cover1 = AlgCover{nv, { {lit2}, {lit3} } };

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

TEST_F(AlgTest, Scover_cube_product)
{
  auto cover1 = AlgCover{nv, { {lit0}, {lit1} } };
  auto src2 = AlgCube{nv,  {lit2} };

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

TEST_F(AlgTest, cube_Scover_product)
{
  auto src1 = AlgCube{nv,  {lit2} };
  auto cover1 = AlgCover{nv, { {lit0}, {lit1} } };

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

TEST_F(AlgTest, Scover_literal_product)
{
  auto cover1 = AlgCover{nv, { {lit0}, {lit1} } };

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

TEST_F(AlgTest, literal_Scover_product)
{
  auto cover1 = AlgCover{nv, { {lit0}, {lit1} } };

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

TEST_F(AlgTest, Icover_cover_product)
{
  auto cover1 = AlgCover{nv, { {lit0}, {lit1} } };
  auto src2 = AlgCover{nv, { {lit2}, {lit3} } };

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

TEST_F(AlgTest, Icover_cover_product_bad)
{
  auto cover1 = AlgCover{nv, { {lit0}, {lit1} } };
  auto src2 = AlgCover{nv + 1, { {lit2}, {lit3} } };

  ASSERT_THROW( cover1 *= src2, std::invalid_argument );
}

TEST_F(AlgTest, Icover_cube_product)
{
  auto cover1 = AlgCover{nv, { {lit0}, {lit1} } };
  auto src2 = AlgCube{nv,  {lit2} };

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

TEST_F(AlgTest, Icover_cube_product_bad)
{
  auto cover1 = AlgCover{nv, { {lit0}, {lit1} } };
  auto src2 = AlgCube{nv + 1,  {lit2} };

  ASSERT_THROW( cover1 *= src2, std::invalid_argument );
}

TEST_F(AlgTest, Icover_literal_product)
{
  auto cover1 = AlgCover{nv, { {lit0}, {lit1} } };

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

TEST_F(AlgTest, cover_cover_quotient)
{
  auto src1 = AlgCover{nv, { {lit0, lit2},
			     {lit0, lit3},
			     {lit1, lit2},
			     {lit1, lit3} } };
  auto src2 = AlgCover{nv, { {lit0}, {lit1} } };

  auto cover1 = src1 / src2;

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

TEST_F(AlgTest, cover_cover_quotient_bad)
{
  auto src1 = AlgCover{nv, { {lit0, lit2},
			     {lit0, lit3},
			     {lit1, lit2},
			     {lit1, lit3} } };
  auto src2 = AlgCover{nv + 1, { {lit0}, {lit1} } };

  ASSERT_THROW( src1 / src2, std::invalid_argument );
}

TEST_F(AlgTest, cover_cube_quotient)
{
  auto src1 = AlgCover{nv, { {lit0, lit2},
			     {lit0, lit3},
			     {lit1, lit2},
			     {lit1, lit3} } };
  auto src2 = AlgCube{nv, {lit0} };

  auto cover1 = src1 / src2;

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

TEST_F(AlgTest, cover_cube_quotient_bad)
{
  auto src1 = AlgCover{nv, { {lit0, lit2},
			     {lit0, lit3},
			     {lit1, lit2},
			     {lit1, lit3} } };
  auto src2 = AlgCube{nv + 1, {lit0} };

  ASSERT_THROW( src1 / src2, std::invalid_argument );
}

TEST_F(AlgTest, cover_literal_quotient)
{
  auto src1 = AlgCover{nv, { {lit0, lit2},
			     {lit0, lit3},
			     {lit1, lit2},
			     {lit1, lit3} } };

  auto cover1 =  src1 / lit0;

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

TEST_F(AlgTest, Rcover_cover_quotient)
{
  auto src1 = AlgCover{nv, { {lit0, lit2},
			     {lit0, lit3},
			     {lit1, lit2},
			     {lit1, lit3} } };
  auto src2 = AlgCover{nv, { {lit0}, {lit1} } };

  auto cover1 = std::move(src1) / src2;

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

TEST_F(AlgTest, Rcover_cube_quotient)
{
  auto src1 = AlgCover{nv, { {lit0, lit2},
			     {lit0, lit3},
			     {lit1, lit2},
			     {lit1, lit3} } };
  auto src2 = AlgCube{nv, {lit0} };

  auto cover1 = std::move(src1) / src2;

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

TEST_F(AlgTest, Rcover_literal_quotient)
{
  auto src1 = AlgCover{nv, { {lit0, lit2},
			     {lit0, lit3},
			     {lit1, lit2},
			     {lit1, lit3} } };

  auto cover1 = std::move(src1) / lit0;

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

TEST_F(AlgTest, Scover_cover_quotient)
{
  auto cover1 = AlgCover{nv, { {lit0, lit2},
			       {lit0, lit3},
			       {lit1, lit2},
			       {lit1, lit3} } };
  auto src2 = AlgCover{nv, { {lit0}, {lit1} } };

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

TEST_F(AlgTest, Scover_cube_quotient)
{
  auto cover1 = AlgCover{nv, { {lit0, lit2},
			       {lit0, lit3},
			       {lit1, lit2},
			       {lit1, lit3} } };
  auto src2 = AlgCube{nv, {lit0} };

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

TEST_F(AlgTest, Scover_literal_quotient)
{
  auto cover1 = AlgCover{nv, { {lit0, lit2},
			       {lit0, lit3},
			       {lit1, lit2},
			       {lit1, lit3} } };

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

TEST_F(AlgTest, Icover_cover_quotient)
{
  auto cover1 = AlgCover{nv, { {lit0, lit2},
			       {lit0, lit3},
			       {lit1, lit2},
			       {lit1, lit3} } };
  auto src2 = AlgCover{nv, { {lit0}, {lit1} } };

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

TEST_F(AlgTest, Icover_cover_quotient_bad)
{
  auto cover1 = AlgCover{nv, { {lit0, lit2},
			       {lit0, lit3},
			       {lit1, lit2},
			       {lit1, lit3} } };
  auto src2 = AlgCover{nv + 1, { {lit0}, {lit1} } };

  ASSERT_THROW( cover1 /= src2, std::invalid_argument );
}

TEST_F(AlgTest, Icover_cube_quotient)
{
  auto cover1 = AlgCover{nv, { {lit0, lit2},
			       {lit0, lit3},
			       {lit1, lit2},
			       {lit1, lit3} } };
  auto src2 = AlgCube{nv, {lit0} };

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

TEST_F(AlgTest, Icover_cube_quotient_bad)
{
  auto cover1 = AlgCover{nv, { {lit0, lit2},
			       {lit0, lit3},
			       {lit1, lit2},
			       {lit1, lit3} } };
  auto src2 = AlgCube{nv + 1, {lit0} };

  ASSERT_THROW( cover1 /= src2, std::invalid_argument );
}

TEST_F(AlgTest, Icover_literal_quotient)
{
  auto cover1 = AlgCover{nv, { {lit0, lit2},
			       {lit0, lit3},
			       {lit1, lit2},
			       {lit1, lit3} } };

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

TEST_F(AlgTest, comp1)
{
  auto cover1 = AlgCover{nv, { {lit0, lit2},
			       {lit0, lit3},
			       {lit0, ~lit4} } };
  auto cover2 = AlgCover{nv, { {lit0, lit2},
			       {lit0, lit3},
			       {lit0, ~lit4} } };

  EXPECT_EQ( cover1, cover2 );
  EXPECT_TRUE( cover1 == cover2 );
  EXPECT_FALSE( cover1 != cover2 );
  EXPECT_FALSE( cover1 < cover2 );
  EXPECT_FALSE( cover1 > cover2 );
  EXPECT_TRUE( cover1 <= cover2 );
  EXPECT_TRUE( cover1 >= cover2 );
}

TEST_F(AlgTest, comp2)
{
  auto cover1 = AlgCover{nv, { {lit0, lit2},
			       {lit0, lit3},
			       {lit0, ~lit4} } };
  auto cover2 = AlgCover{nv, { {lit0, lit2},
			       {lit1, lit3},
			       {lit0, ~lit4} } };

  EXPECT_NE( cover1, cover2 );
  EXPECT_FALSE( cover1 == cover2 );
  EXPECT_TRUE( cover1 != cover2 );
  EXPECT_FALSE( cover1 < cover2 );
  EXPECT_TRUE( cover1 > cover2 );
  EXPECT_FALSE( cover1 <= cover2 );
  EXPECT_TRUE( cover1 >= cover2 );
}

TEST_F(AlgTest, comp3)
{
  auto cover1 = AlgCover{nv, { {lit0, lit2},
			       {lit0, lit3},
			       {lit0, ~lit4} } };
  auto cover2 = AlgCover{nv, { {lit0, lit2},
			       {lit0, lit3} } };

  EXPECT_NE( cover1, cover2 );
  EXPECT_FALSE( cover1 == cover2 );
  EXPECT_TRUE( cover1 != cover2 );
  EXPECT_FALSE( cover1 < cover2 );
  EXPECT_TRUE( cover1 > cover2 );
  EXPECT_FALSE( cover1 <= cover2 );
  EXPECT_TRUE( cover1 >= cover2 );
}

TEST_F(AlgTest, comp4)
{
  auto cover1 = AlgCover{nv, { {lit0, lit2} } };
  auto cover2 = AlgCover{nv, { {lit0, lit3} } };

  EXPECT_NE( cover1, cover2 );
  EXPECT_FALSE( cover1 == cover2 );
  EXPECT_TRUE( cover1 != cover2 );
  EXPECT_FALSE( cover1 < cover2 );
  EXPECT_TRUE( cover1 > cover2 );
  EXPECT_FALSE( cover1 <= cover2 );
  EXPECT_TRUE( cover1 >= cover2 );
}

TEST_F(AlgTest, common_cube)
{
  auto src1 = AlgCover{nv, { {lit0, lit2},
			     {lit0, lit3},
			     {lit0, ~lit4} } };

  auto cube1 = src1.common_cube();

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

TEST_F(AlgTest, to_expr1)
{
  auto cube1 = AlgCube{3, {lit0, lit1}};
  auto cube2 = AlgCube{3, {~lit2}};
  auto cover1 = AlgCover{3, {cube1, cube2}};

  auto expr = cover1.expr();
  auto expr_str = expr.to_string();
  EXPECT_EQ( "( ( 0 & 1 ) | ~2 )", expr_str );
}

TEST_F(AlgTest, all_kernels1)
{
  auto cover1 = AlgCover{nv, { {lit0, lit2},
			       {lit1, lit2} }};
  auto kernel_list = cover1.all_kernels();

  vector<string> exp_list{
    "v0 + v1, v2"
  };

  SizeType n = kernel_list.size();
  EXPECT_EQ( exp_list.size(), n );
  for ( SizeType i = 0; i < n; ++ i ) {
    auto& ki = kernel_list[i];
    auto& kernel = ki.first;
    auto& cokernels = ki.second;
    ostringstream tmp;
    tmp << kernel << ", " << cokernels;
    string tmp_str = tmp.str();
    EXPECT_EQ( exp_list[i], tmp_str );
  }
}

TEST_F(AlgTest, all_kernels2)
{
  auto lit_a = Literal(var0, false);
  auto lit_b = Literal(var1, false);
  auto lit_c = Literal(var2, false);
  auto lit_d = Literal(var3, false);
  auto lit_e = Literal(var4, false);
  auto lit_f = Literal(var5, false);
  auto lit_g = Literal(var6, false);
  auto lit_h = Literal(var7, false);
  auto lit_i = Literal(var8, false);
  auto lit_j = Literal(var9, false);

  // adf + aef + bdf + bef + cdf + cef + bfg + h
  auto cover1 = AlgCover{nv,
			 { { lit_a, lit_d, lit_f },
			   { lit_a, lit_e, lit_f },
			   { lit_b, lit_d, lit_f },
			   { lit_b, lit_e, lit_f },
			   { lit_c, lit_d, lit_f },
			   { lit_c, lit_e, lit_f },
			   { lit_b, lit_f, lit_g },
			   { lit_h } } };

  auto kernel_list = cover1.all_kernels();

  vector<string> exp_list{
    "v3 + v4, v0 v5 + v2 v5",
    "v3 + v4 + v6, v1 v5",
    "v0 + v1 + v2, v3 v5 + v4 v5",
    "v0 v3 + v0 v4 + v1 v3 + v1 v4 + v1 v6 + v2 v3 + v2 v4, v5",
    "v0 v3 v5 + v0 v4 v5 + v1 v3 v5 + v1 v4 v5 + v1 v5 v6 + v2 v3 v5 + v2 v4 v5 + v7, "
  };

  SizeType n = kernel_list.size();
  EXPECT_EQ( exp_list.size(), n );
  for ( SizeType i = 0; i < n; ++ i ) {
    auto& ki = kernel_list[i];
    auto& kernel = ki.first;
    auto& cokernels = ki.second;
    ostringstream tmp;
    tmp << kernel << ", " << cokernels;
    string tmp_str = tmp.str();
    EXPECT_EQ( exp_list[i], tmp_str );
  }
}

END_NAMESPACE_YM
