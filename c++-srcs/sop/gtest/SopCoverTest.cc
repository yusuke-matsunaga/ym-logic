
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
#include <random>


BEGIN_NAMESPACE_YM

TEST_F(SopTest, constructor1)
{
  auto cover1 = SopCover{nv};

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
  auto cube1 = SopCube{nv, {lit0, lit1}};
  auto cube2 = SopCube{nv, {lit2, lit3}};

  vector<SopCube> cube_list{cube1, cube2};

  auto cover1 = SopCover{nv, cube_list};

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

TEST_F(SopTest, constructor2_bad)
{
  auto cube1 = SopCube{nv + 1, {lit0, lit1}};
  auto cube2 = SopCube{nv, {lit2, lit3}};

  vector<SopCube> cube_list{cube1, cube2};

  ASSERT_THROW( (SopCover{nv, cube_list}), std::invalid_argument );
}

TEST_F(SopTest, constructor3)
{
  vector<vector<Literal>> cube_list{ {lit0, lit1}, {lit2, lit3} };
  auto cover1 = SopCover{nv, cube_list};

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
  auto cover1 = SopCover{nv, { {lit0, lit1}, {lit2, lit3} }};

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
  auto src_cover = SopCover{nv, { {lit0, lit1}, {lit2, lit3} }};

  auto cover1 = SopCover{src_cover};

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
  auto src_cover = SopCover{nv, { {lit0, lit1}, {lit2, lit3} }};

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

TEST_F(SopTest, move_constructor)
{
  auto src_cover = SopCover{nv, { {lit0, lit1}, {lit2, lit3} }};

  auto cover1 = SopCover{std::move(src_cover)};

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
  auto src_cover = SopCover{nv, { {lit0, lit1}, {lit2, lit3} }};

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

TEST_F(SopTest, cube_constructor)
{
  auto src_cube = SopCube{nv, {lit0, lit1}};

  auto cover1 = SopCover{src_cube};

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
TEST_F(SopTest, cube_move_constructor)
{
  auto src_cube = SopCube{nv, {lit0, lit1}};

  auto cover1 = SopCover{std::move(src_cube)};

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

TEST_F(SopTest, cover_cover_sum)
{
  auto src1 = SopCover{nv, { {lit0, lit1} } };
  auto src2 = SopCover{nv, { {lit2, lit3} } };

  auto cover1 = src1 | src2;

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

TEST_F(SopTest, cover_cover_sum_bad)
{
  auto src1 = SopCover{nv, { {lit0, lit1} } };
  auto src2 = SopCover{nv + 1, { {lit2, lit3} } };

  ASSERT_THROW( src1 | src2, std::invalid_argument );
}

TEST_F(SopTest, cover_cube_sum)
{
  auto src1 = SopCover{nv, { {lit0, lit1} } };
  auto src2 = SopCube{nv, {lit2, lit3} };

  auto cover1 = src1 | src2;

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

TEST_F(SopTest, cover_cube_sum_bad)
{
  auto src1 = SopCover{nv, { {lit0, lit1} } };
  auto src2 = SopCube{nv + 1, {lit2, lit3} };

  ASSERT_THROW( src1 | src2, std::invalid_argument );
}

TEST_F(SopTest, cube_cover_sum)
{
  auto src1 = SopCube{nv, {lit0, lit1} };
  auto src2 = SopCover{nv, { {lit2, lit3} } };

  auto cover1 = src1 | src2;

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

TEST_F(SopTest, cube_cover_sum_bad)
{
  auto src1 = SopCube{nv, {lit0, lit1} };
  auto src2 = SopCover{nv + 1, { {lit2, lit3} } };

  ASSERT_THROW( src1 | src2, std::invalid_argument );
}

TEST_F(SopTest, Scover_cover_sum)
{
  auto cover1 = SopCover{nv, { {lit0, lit1} } };
  auto src2 = SopCover{nv, { {lit2, lit3} } };

  cover1 = cover1 | src2;

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
  auto cover1 = SopCover{nv, { {lit0, lit1} }};
  auto src2 = SopCube{nv, {lit2, lit3}};

  cover1 = cover1 | src2;

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
  auto src1 = SopCube{nv, {lit0, lit1} };
  auto cover1 = SopCover{nv, { {lit2, lit3} } };

  cover1 = src1 | cover1;

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
  auto cover1 = SopCover{nv, { {lit0, lit1} } };
  auto src2 = SopCover{nv, { {lit2, lit3} } };

  cover1 |= src2;

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

TEST_F(SopTest, Icover_cover_sum_bad)
{
  auto cover1 = SopCover{nv, { {lit0, lit1} } };
  auto src2 = SopCover{nv + 1, { {lit2, lit3} } };

  ASSERT_THROW( cover1 |= src2, std::invalid_argument );
}

TEST_F(SopTest, Icover_cube_sum)
{
  auto cover1 = SopCover{nv, { {lit0, lit1} }};
  auto src2 = SopCube{nv, {lit2, lit3}};

  cover1 |= src2;

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

TEST_F(SopTest, Icover_cube_sum_bad)
{
  auto cover1 = SopCover{nv, { {lit0, lit1} }};
  auto src2 = SopCube{nv + 1, {lit2, lit3}};

  ASSERT_THROW( cover1 |= src2, std::invalid_argument );
}

TEST_F(SopTest, sort1)
{
  auto cover1 = SopCover{3, { {~lit1,  lit2},
			      { lit1, ~lit2},
			      {~lit0,  lit2},
			      {~lit0,  lit1},
			      { lit0, ~lit2},
			      { lit0, ~lit1} } };
  ostringstream buf;
  cover1.print(buf);

  auto str = buf.str();
  auto expr_str = "v0 v1' + v0 v2' + v0' v1 + v0' v2 + v1 v2' + v1' v2";
  EXPECT_EQ( expr_str, str );
}

TEST_F(SopTest, sort2)
{
  auto cover1 = SopCover{3, { {~lit1,  lit2},
			      { lit1, ~lit2},
			      {~lit0,  lit2},
			      {~lit0,  lit2},
			      { lit0, ~lit2},
			      { lit0, ~lit1} } };
  ostringstream buf;
  cover1.print(buf);

  auto str = buf.str();
  auto expr_str = "v0 v1' + v0 v2' + v0' v2 + v1 v2' + v1' v2";
  EXPECT_EQ( expr_str, str );
}

TEST_F(SopTest, sort3)
{
  // サンプル数
  SizeType nsample = 1000;

  // キューブ数
  SizeType ncube = 1000;

  // キューブあたりのリテラル数
  SizeType nlit = 100;

  // 乱数生成器
  std::mt19937 rand_gen;

  // int 型の一様分布生成器
  std::uniform_int_distribution<int> rand_dist;

  SizeType nv = 30;
  vector<Literal> lit_set(nv);
  for ( SizeType i = 0; i < nv; ++ i ) {
    lit_set[i] = Literal{i, false};
  }

  for ( SizeType i = 0; i < nsample; ++ i ) {
    vector<vector<Literal>> cube_list;
    cube_list.reserve(ncube);
    for ( SizeType j = 0; j < ncube; ++ j ) {
      vector<Literal> lit_list;
      lit_list.reserve(nlit);
      std::sample(lit_set.begin(), lit_set.end(),
		  std::back_inserter(lit_list),
		  nlit, rand_gen);
      vector<Literal> lit_list2;
      lit_list.reserve(nlit);
      for ( auto lit: lit_list ) {
	if ( rand_dist(rand_gen) % 2 ) {
	  lit_list2.push_back(lit);
	}
	else {
	  lit_list2.push_back(~lit);
	}
      }
      cube_list.push_back(lit_list2);
    }
    auto cover = SopCover{nv, cube_list};

    auto nc = cover.cube_num();
    for ( SizeType i = 1; i < nc; ++ i ) {
      auto cube1 = cover.get_cube(i - 1);
      auto cube2 = cover.get_cube(i);
      EXPECT_TRUE( cube1 > cube2 );
    }
  }
}

TEST_F(SopTest, to_expr1)
{
  auto cube1 = SopCube{3, {lit0, lit1}};
  auto cube2 = SopCube{3, {~lit2}};
  auto cover1 = SopCover{3, {cube1, cube2}};

  auto expr = cover1.expr();
  auto expr_str = expr.to_string();
  EXPECT_EQ( "( ( 0 & 1 ) | ~2 )", expr_str );
}

END_NAMESPACE_YM
