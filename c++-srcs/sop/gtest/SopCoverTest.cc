
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

// SopCover(SizeType) のテスト
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

// SopCover{SizeType, const vector<SopCube>} のテスト
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
  ASSERT_EQ( 2, cube_list1.size() );
  ASSERT_EQ( 2, cube_list1[0].size() );
  ASSERT_EQ( 2, cube_list1[1].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit1, cube_list1[0][1] );
  EXPECT_EQ( lit2, cube_list1[1][0] );
  EXPECT_EQ( lit3, cube_list1[1][1] );

};

// SopCover{SizeType, const vector<SopCube>} のテスト
// 変数の数が合わないのでエラーとなる．
TEST_F(SopTest, constructor2_bad)
{
  auto cube1 = SopCube{nv + 1, {lit0, lit1}};
  auto cube2 = SopCube{nv, {lit2, lit3}};

  vector<SopCube> cube_list{cube1, cube2};

  EXPECT_THROW( (SopCover{nv, cube_list}), std::invalid_argument );
}

// SopCover(SizeType, vector<vector<Literal>>) のテスト
TEST_F(SopTest, constructor3)
{
  vector<vector<Literal>> cube_list{ {lit0, lit1},
				     {lit2, lit3} };
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
  ASSERT_EQ( 2, cube_list1.size() );
  ASSERT_EQ( 2, cube_list1[0].size() );
  ASSERT_EQ( 2, cube_list1[1].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit1, cube_list1[0][1] );
  EXPECT_EQ( lit2, cube_list1[1][0] );
  EXPECT_EQ( lit3, cube_list1[1][1] );

};

// SopCover(SizeType, vector<vector<Literal>>) のテスト
// リテラルが範囲外なのでエラーとなる．
TEST_F(SopTest, constructor3_bad)
{
  vector<vector<Literal>> cube_list{ {lit100, lit1},
				     {lit2, lit3} };
  EXPECT_THROW( (SopCover{nv, cube_list}),
		std::out_of_range );
}

// SopCover(SizeType, initializer_list<initializer_list<Literal>>) のテスト
TEST_F(SopTest, constructor4)
{
  auto cover1 = SopCover{nv, { {lit0, lit1},
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
  ASSERT_EQ( 2, cube_list1.size() );
  ASSERT_EQ( 2, cube_list1[0].size() );
  ASSERT_EQ( 2, cube_list1[1].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit1, cube_list1[0][1] );
  EXPECT_EQ( lit2, cube_list1[1][0] );
  EXPECT_EQ( lit3, cube_list1[1][1] );

};

// SopCover(SizeType, initializer_list<initializer_list<Literal>>) のテスト
// リテラルが範囲外なのでエラーとなる．
TEST_F(SopTest, constructor4_bad)
{
  EXPECT_THROW( (SopCover{nv, { {lit100, lit1},
				{lit2, lit3} }}),
    std::out_of_range );
}

// コピーコンストラクタのテスト
TEST_F(SopTest, copy_constructor)
{
  auto src_cover = SopCover{nv, { {lit0, lit1},
				  {lit2, lit3} }};

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
  ASSERT_EQ( 2, cube_list1.size() );
  ASSERT_EQ( 2, cube_list1[0].size() );
  ASSERT_EQ( 2, cube_list1[1].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit1, cube_list1[0][1] );
  EXPECT_EQ( lit2, cube_list1[1][0] );
  EXPECT_EQ( lit3, cube_list1[1][1] );

};

// コピー代入演算のテスト
TEST_F(SopTest, copy_assignment)
{
  auto src_cover = SopCover{nv, { {lit0, lit1},
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
  ASSERT_EQ( 2, cube_list1.size() );
  ASSERT_EQ( 2, cube_list1[0].size() );
  ASSERT_EQ( 2, cube_list1[1].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit1, cube_list1[0][1] );
  EXPECT_EQ( lit2, cube_list1[1][0] );
  EXPECT_EQ( lit3, cube_list1[1][1] );

};

// ムーブコンストラクタのテスト
TEST_F(SopTest, move_constructor)
{
  auto src_cover = SopCover{nv, { {lit0, lit1},
				  {lit2, lit3} }};

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
  ASSERT_EQ( 2, cube_list1.size() );
  ASSERT_EQ( 2, cube_list1[0].size() );
  ASSERT_EQ( 2, cube_list1[1].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit1, cube_list1[0][1] );
  EXPECT_EQ( lit2, cube_list1[1][0] );
  EXPECT_EQ( lit3, cube_list1[1][1] );

};

// ムーブ代入演算のテスト
TEST_F(SopTest, move_assignment)
{
  auto src_cover = SopCover{nv, { {lit0, lit1},
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
  ASSERT_EQ( 2, cube_list1.size() );
  ASSERT_EQ( 2, cube_list1[0].size() );
  ASSERT_EQ( 2, cube_list1[1].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit1, cube_list1[0][1] );
  EXPECT_EQ( lit2, cube_list1[1][0] );
  EXPECT_EQ( lit3, cube_list1[1][1] );

};

// SopCover(SopCube) のテスト
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
  ASSERT_EQ( 1, cube_list1.size() );
  ASSERT_EQ( 2, cube_list1[0].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit1, cube_list1[0][1] );

};

// SopCover | SopCover のテスト
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
  ASSERT_EQ( 2, tmp_list.size() );
  ASSERT_EQ( 2, tmp_list[0].size() );
  ASSERT_EQ( 2, tmp_list[1].size() );
  EXPECT_EQ( lit0, tmp_list[0][0] );
  EXPECT_EQ( lit1, tmp_list[0][1] );
  EXPECT_EQ( lit2, tmp_list[1][0] );
  EXPECT_EQ( lit3, tmp_list[1][1] );

};

// SopCover | SopCover のテスト
// 変数の数が合わないのでエラーとなる．
TEST_F(SopTest, cover_cover_sum_bad)
{
  auto src1 = SopCover{nv, { {lit0, lit1} } };
  auto src2 = SopCover{nv + 1, { {lit2, lit3} } };

  EXPECT_THROW( src1 | src2,
		std::invalid_argument );
}

// SopCover | SopCube のテスト
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
  ASSERT_EQ( 2, tmp_list.size() );
  ASSERT_EQ( 2, tmp_list[0].size() );
  ASSERT_EQ( 2, tmp_list[1].size() );
  EXPECT_EQ( lit0, tmp_list[0][0] );
  EXPECT_EQ( lit1, tmp_list[0][1] );
  EXPECT_EQ( lit2, tmp_list[1][0] );
  EXPECT_EQ( lit3, tmp_list[1][1] );

};

// SopCover | SopCube のテスト
// 変数の数が合わないのでエラーとなる．
TEST_F(SopTest, cover_cube_sum_bad)
{
  auto src1 = SopCover{nv, { {lit0, lit1} } };
  auto src2 = SopCube{nv + 1, {lit2, lit3} };

  EXPECT_THROW( src1 | src2, std::invalid_argument );
}

// SopCube | SopCover のテスト
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
  ASSERT_EQ( 2, tmp_list.size() );
  ASSERT_EQ( 2, tmp_list[0].size() );
  ASSERT_EQ( 2, tmp_list[1].size() );
  EXPECT_EQ( lit0, tmp_list[0][0] );
  EXPECT_EQ( lit1, tmp_list[0][1] );
  EXPECT_EQ( lit2, tmp_list[1][0] );
  EXPECT_EQ( lit3, tmp_list[1][1] );

};

// SopCube | SopCover のテスト
// 変数の数が合わないのでエラーとなる．
TEST_F(SopTest, cube_cover_sum_bad)
{
  auto src1 = SopCube{nv, {lit0, lit1} };
  auto src2 = SopCover{nv + 1, { {lit2, lit3} } };

  EXPECT_THROW( src1 | src2, std::invalid_argument );
}

// SopCover | SopCover のテスト
// 左辺と第1オペランドが同じ
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
  ASSERT_EQ( 2, tmp_list.size() );
  ASSERT_EQ( 2, tmp_list[0].size() );
  ASSERT_EQ( 2, tmp_list[1].size() );
  EXPECT_EQ( lit0, tmp_list[0][0] );
  EXPECT_EQ( lit1, tmp_list[0][1] );
  EXPECT_EQ( lit2, tmp_list[1][0] );
  EXPECT_EQ( lit3, tmp_list[1][1] );

};

// SopCover | SopCube のテスト
// 左辺と第1オペランドが等しいケース
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
  ASSERT_EQ( 2, tmp_list.size() );
  ASSERT_EQ( 2, tmp_list[0].size() );
  ASSERT_EQ( 2, tmp_list[1].size() );
  EXPECT_EQ( lit0, tmp_list[0][0] );
  EXPECT_EQ( lit1, tmp_list[0][1] );
  EXPECT_EQ( lit2, tmp_list[1][0] );
  EXPECT_EQ( lit3, tmp_list[1][1] );

};

// SopCube | SopCover のテスト
// 左辺と第2オペランドが等しいケース
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
  ASSERT_EQ( 2, tmp_list.size() );
  ASSERT_EQ( 2, tmp_list[0].size() );
  ASSERT_EQ( 2, tmp_list[1].size() );
  EXPECT_EQ( lit0, tmp_list[0][0] );
  EXPECT_EQ( lit1, tmp_list[0][1] );
  EXPECT_EQ( lit2, tmp_list[1][0] );
  EXPECT_EQ( lit3, tmp_list[1][1] );

};

// SopCover |= SopCover のテスト
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
  ASSERT_EQ( 2, tmp_list.size() );
  ASSERT_EQ( 2, tmp_list[0].size() );
  ASSERT_EQ( 2, tmp_list[1].size() );
  EXPECT_EQ( lit0, tmp_list[0][0] );
  EXPECT_EQ( lit1, tmp_list[0][1] );
  EXPECT_EQ( lit2, tmp_list[1][0] );
  EXPECT_EQ( lit3, tmp_list[1][1] );

};

// SopCover |= SopCover のテスト
// 変数の数が合わないのでエラーとなる．
TEST_F(SopTest, Icover_cover_sum_bad)
{
  auto cover1 = SopCover{nv, { {lit0, lit1} } };
  auto src2 = SopCover{nv + 1, { {lit2, lit3} } };

  EXPECT_THROW( cover1 |= src2, std::invalid_argument );
}

// SopCover |= SopCube のテスト
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
  ASSERT_EQ( 2, tmp_list.size() );
  ASSERT_EQ( 2, tmp_list[0].size() );
  ASSERT_EQ( 2, tmp_list[1].size() );
  EXPECT_EQ( lit0, tmp_list[0][0] );
  EXPECT_EQ( lit1, tmp_list[0][1] );
  EXPECT_EQ( lit2, tmp_list[1][0] );
  EXPECT_EQ( lit3, tmp_list[1][1] );

};

// SopCover |= SopCube のテスト
// 変数の数が合わないのでエラーとなる．
TEST_F(SopTest, Icover_cube_sum_bad)
{
  auto cover1 = SopCover{nv, { {lit0, lit1} }};
  auto src2 = SopCube{nv + 1, {lit2, lit3}};

  EXPECT_THROW( cover1 |= src2, std::invalid_argument );
}

TEST_F(SopTest, cover_cover_diff)
{
  auto src1 = SopCover{nv, { {lit0, lit1},
			     {lit2, lit3} } };
  auto src2 = SopCover{nv, { {lit3, lit2},
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
  ASSERT_EQ( 1, cube_list1.size() );
  ASSERT_EQ( 2, cube_list1[0].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit1, cube_list1[0][1] );

};

TEST_F(SopTest, cover_cover_diff_bad)
{
  auto src1 = SopCover{nv, { {lit0, lit1},
			     {lit2, lit3} } };
  auto src2 = SopCover{nv + 1, { {lit3, lit2},
				 {lit0, ~lit1} } };

  EXPECT_THROW( src1 - src2, std::invalid_argument );
}

TEST_F(SopTest, Rcover_cover_diff)
{
  auto src1 = SopCover{nv, { {lit0, lit1},
			     {lit2, lit3} } };
  auto src2 = SopCover{nv, { {lit3, lit2},
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
  ASSERT_EQ( 1, cube_list1.size() );
  ASSERT_EQ( 2, cube_list1[0].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit1, cube_list1[0][1] );

};

TEST_F(SopTest, cover_cube_diff)
{
  auto src1 = SopCover{nv, { {lit0, lit1},
			     {lit2, lit3} } };
  auto src2 = SopCube{nv, {lit3, lit2} };

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
  ASSERT_EQ( 1, cube_list1.size() );
  ASSERT_EQ( 2, cube_list1[0].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit1, cube_list1[0][1] );

};

TEST_F(SopTest, cover_cube_diff_bad)
{
  auto src1 = SopCover{nv, { {lit0, lit1},
			     {lit2, lit3} } };
  auto src2 = SopCube{nv + 1, {lit3, lit2} };

  EXPECT_THROW( src1 - src2, std::invalid_argument );
}

TEST_F(SopTest, Scover_cover_diff)
{
  auto cover1 = SopCover{nv, { {lit0, lit1},
			       {lit2, lit3} } };
  auto src2 = SopCover{nv, { {lit3, lit2},
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
  ASSERT_EQ( 1, cube_list1.size() );
  ASSERT_EQ( 2, cube_list1[0].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit1, cube_list1[0][1] );

};

TEST_F(SopTest, Scover_cube_diff)
{
  auto cover1 = SopCover{nv, { {lit0, lit1},
			       {lit2, lit3} } };
  auto src2 = SopCube{nv, {lit3, lit2} };

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
  ASSERT_EQ( 1, cube_list1.size() );
  ASSERT_EQ( 2, cube_list1[0].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit1, cube_list1[0][1] );

};

TEST_F(SopTest, Icover_cover_diff)
{
  auto cover1 = SopCover{nv, { {lit0, lit1},
			       {lit2, lit3} } };
  auto src2 = SopCover{nv, { {lit3, lit2},
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
  ASSERT_EQ( 1, cube_list1.size() );
  ASSERT_EQ( 2, cube_list1[0].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit1, cube_list1[0][1] );

};

TEST_F(SopTest, Icover_cover_diff_bad)
{
  auto cover1 = SopCover{nv, { {lit0, lit1},
			       {lit2, lit3} } };
  auto src2 = SopCover{nv + 1, { {lit3, lit2},
				 {lit0, ~lit1} } };

  EXPECT_THROW( cover1 -= src2, std::invalid_argument );
}

TEST_F(SopTest, Icover_cube_diff)
{
  auto cover1 = SopCover{nv, { {lit0, lit1},
			       {lit2, lit3} } };
  auto src2 = SopCube{nv, {lit3, lit2} };

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
  ASSERT_EQ( 1, cube_list1.size() );
  ASSERT_EQ( 2, cube_list1[0].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit1, cube_list1[0][1] );

};

TEST_F(SopTest, Icover_cube_diff_bad)
{
  auto cover1 = SopCover{nv, { {lit0, lit1},
			       {lit2, lit3} } };
  auto src2 = SopCube{nv + 1, {lit3, lit2} };

  EXPECT_THROW( cover1 -= src2, std::invalid_argument );
}

TEST_F(SopTest, cover_cover_product)
{
  auto src1 = SopCover{nv, { {lit0}, {lit1} } };
  auto src2 = SopCover{nv, { {lit2}, {lit3} } };

  auto cover1 = src1 & src2;

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
  ASSERT_EQ( 4, cube_list1.size() );
  ASSERT_EQ( 2, cube_list1[0].size() );
  ASSERT_EQ( 2, cube_list1[1].size() );
  ASSERT_EQ( 2, cube_list1[2].size() );
  ASSERT_EQ( 2, cube_list1[3].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit2, cube_list1[0][1] );
  EXPECT_EQ( lit0, cube_list1[1][0] );
  EXPECT_EQ( lit3, cube_list1[1][1] );
  EXPECT_EQ( lit1, cube_list1[2][0] );
  EXPECT_EQ( lit2, cube_list1[2][1] );
  EXPECT_EQ( lit1, cube_list1[3][0] );
  EXPECT_EQ( lit3, cube_list1[3][1] );

};

TEST_F(SopTest, cover_cover_product_bad)
{
  auto src1 = SopCover{nv, { {lit0}, {lit1} } };
  auto src2 = SopCover{nv + 1, { {lit2}, {lit3} } };

  EXPECT_THROW( src1 & src2, std::invalid_argument );
}

TEST_F(SopTest, cover_cube_product)
{
  auto src1 = SopCover{nv, { {lit0}, {lit1} } };
  auto src2 = SopCube{nv,  {lit2} };

  auto cover1 = src1 & src2;

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
  ASSERT_EQ( 2, cube_list1.size() );
  ASSERT_EQ( 2, cube_list1[0].size() );
  ASSERT_EQ( 2, cube_list1[1].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit2, cube_list1[0][1] );
  EXPECT_EQ( lit1, cube_list1[1][0] );
  EXPECT_EQ( lit2, cube_list1[1][1] );

};

TEST_F(SopTest, cover_cube_product_bad)
{
  auto src1 = SopCover{nv, { {lit0}, {lit1} } };
  auto src2 = SopCube{nv + 1,  {lit2} };

  EXPECT_THROW( src1 & src2, std::invalid_argument );
}

TEST_F(SopTest, Rcover_cube_product)
{
  auto src1 = SopCover{nv, { {lit0}, {lit1} } };
  auto src2 = SopCube{nv,  {lit2} };

  auto cover1 = std::move(src1) & src2;

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
  ASSERT_EQ( 2, cube_list1.size() );
  ASSERT_EQ( 2, cube_list1[0].size() );
  ASSERT_EQ( 2, cube_list1[1].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit2, cube_list1[0][1] );
  EXPECT_EQ( lit1, cube_list1[1][0] );
  EXPECT_EQ( lit2, cube_list1[1][1] );

};

TEST_F(SopTest, cube_cover_product)
{
  auto src1 = SopCube{nv,  {lit2} };
  auto src2 = SopCover{nv, { {lit0}, {lit1} } };

  auto cover1 = src1 & src2;

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
  ASSERT_EQ( 2, cube_list1.size() );
  ASSERT_EQ( 2, cube_list1[0].size() );
  ASSERT_EQ( 2, cube_list1[1].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit2, cube_list1[0][1] );
  EXPECT_EQ( lit1, cube_list1[1][0] );
  EXPECT_EQ( lit2, cube_list1[1][1] );

};

TEST_F(SopTest, cube_cover_product_bad)
{
  auto src1 = SopCube{nv,  {lit2} };
  auto src2 = SopCover{nv + 1, { {lit0}, {lit1} } };

  EXPECT_THROW( src1 & src2, std::invalid_argument );
}

TEST_F(SopTest, cube_Rcover_product)
{
  auto src1 = SopCube{nv,  {lit2} };
  auto src2 = SopCover{nv, { {lit0}, {lit1} } };

  auto cover1 = src1 & std::move(src2);

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
  ASSERT_EQ( 2, cube_list1.size() );
  ASSERT_EQ( 2, cube_list1[0].size() );
  ASSERT_EQ( 2, cube_list1[1].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit2, cube_list1[0][1] );
  EXPECT_EQ( lit1, cube_list1[1][0] );
  EXPECT_EQ( lit2, cube_list1[1][1] );

};

TEST_F(SopTest, cover_literal_product)
{
  auto src1 = SopCover{nv, { {lit0}, {lit1} } };

  auto cover1 = src1 & lit2;

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
  ASSERT_EQ( 2, cube_list1.size() );
  ASSERT_EQ( 2, cube_list1[0].size() );
  ASSERT_EQ( 2, cube_list1[1].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit2, cube_list1[0][1] );
  EXPECT_EQ( lit1, cube_list1[1][0] );
  EXPECT_EQ( lit2, cube_list1[1][1] );

};

TEST_F(SopTest, Rcover_literal_product)
{
  auto src1 = SopCover{nv, { {lit0}, {lit1} } };

  auto cover1 = std::move(src1) & lit2;

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
  ASSERT_EQ( 2, cube_list1.size() );
  ASSERT_EQ( 2, cube_list1[0].size() );
  ASSERT_EQ( 2, cube_list1[1].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit2, cube_list1[0][1] );
  EXPECT_EQ( lit1, cube_list1[1][0] );
  EXPECT_EQ( lit2, cube_list1[1][1] );

};

TEST_F(SopTest, literal_cover_product)
{
  auto src1 = SopCover{nv, { {lit0}, {lit1} } };

  auto cover1 = lit2 & src1;

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
  ASSERT_EQ( 2, cube_list1.size() );
  ASSERT_EQ( 2, cube_list1[0].size() );
  ASSERT_EQ( 2, cube_list1[1].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit2, cube_list1[0][1] );
  EXPECT_EQ( lit1, cube_list1[1][0] );
  EXPECT_EQ( lit2, cube_list1[1][1] );

};

TEST_F(SopTest, literal_Rcover_product)
{
  auto src1 = SopCover{nv, { {lit0}, {lit1} } };

  auto cover1 = lit2 & std::move(src1);

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
  ASSERT_EQ( 2, cube_list1.size() );
  ASSERT_EQ( 2, cube_list1[0].size() );
  ASSERT_EQ( 2, cube_list1[1].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit2, cube_list1[0][1] );
  EXPECT_EQ( lit1, cube_list1[1][0] );
  EXPECT_EQ( lit2, cube_list1[1][1] );

};

TEST_F(SopTest, Scover_cover_product)
{
  auto cover1 = SopCover{nv, { {lit0}, {lit1} } };
  auto src2 = SopCover{nv, { {lit2}, {lit3} } };

  cover1 = cover1 & src2;

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
  ASSERT_EQ( 4, cube_list1.size() );
  ASSERT_EQ( 2, cube_list1[0].size() );
  ASSERT_EQ( 2, cube_list1[1].size() );
  ASSERT_EQ( 2, cube_list1[2].size() );
  ASSERT_EQ( 2, cube_list1[3].size() );
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
  auto src1 = SopCover{nv, { {lit0}, {lit1} } };
  auto cover1 = SopCover{nv, { {lit2}, {lit3} } };

  cover1 = src1 & cover1;

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
  ASSERT_EQ( 4, cube_list1.size() );
  ASSERT_EQ( 2, cube_list1[0].size() );
  ASSERT_EQ( 2, cube_list1[1].size() );
  ASSERT_EQ( 2, cube_list1[2].size() );
  ASSERT_EQ( 2, cube_list1[3].size() );
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
  auto cover1 = SopCover{nv, { {lit0}, {lit1} } };
  auto src2 = SopCube{nv,  {lit2} };

  cover1 = cover1 & src2;

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
  ASSERT_EQ( 2, cube_list1.size() );
  ASSERT_EQ( 2, cube_list1[0].size() );
  ASSERT_EQ( 2, cube_list1[1].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit2, cube_list1[0][1] );
  EXPECT_EQ( lit1, cube_list1[1][0] );
  EXPECT_EQ( lit2, cube_list1[1][1] );

};

TEST_F(SopTest, cube_Scover_product)
{
  auto src1 = SopCube{nv,  {lit2} };
  auto cover1 = SopCover{nv, { {lit0}, {lit1} } };

  cover1 = src1 & cover1;

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
  ASSERT_EQ( 2, cube_list1.size() );
  ASSERT_EQ( 2, cube_list1[0].size() );
  ASSERT_EQ( 2, cube_list1[1].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit2, cube_list1[0][1] );
  EXPECT_EQ( lit1, cube_list1[1][0] );
  EXPECT_EQ( lit2, cube_list1[1][1] );

};

TEST_F(SopTest, Scover_literal_product)
{
  auto cover1 = SopCover{nv, { {lit0}, {lit1} } };

  cover1 = cover1 & lit2;

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
  ASSERT_EQ( 2, cube_list1.size() );
  ASSERT_EQ( 2, cube_list1[0].size() );
  ASSERT_EQ( 2, cube_list1[1].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit2, cube_list1[0][1] );
  EXPECT_EQ( lit1, cube_list1[1][0] );
  EXPECT_EQ( lit2, cube_list1[1][1] );

};

TEST_F(SopTest, literal_Scover_product)
{
  auto cover1 = SopCover{nv, { {lit0}, {lit1} } };

  cover1 = lit2 & cover1;

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
  ASSERT_EQ( 2, cube_list1.size() );
  ASSERT_EQ( 2, cube_list1[0].size() );
  ASSERT_EQ( 2, cube_list1[1].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit2, cube_list1[0][1] );
  EXPECT_EQ( lit1, cube_list1[1][0] );
  EXPECT_EQ( lit2, cube_list1[1][1] );

};

TEST_F(SopTest, Icover_cover_product)
{
  auto cover1 = SopCover{nv, { {lit0}, {lit1} } };
  auto src2 = SopCover{nv, { {lit2}, {lit3} } };

  cover1 &= src2;

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
  ASSERT_EQ( 4, cube_list1.size() );
  ASSERT_EQ( 2, cube_list1[0].size() );
  ASSERT_EQ( 2, cube_list1[1].size() );
  ASSERT_EQ( 2, cube_list1[2].size() );
  ASSERT_EQ( 2, cube_list1[3].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit2, cube_list1[0][1] );
  EXPECT_EQ( lit0, cube_list1[1][0] );
  EXPECT_EQ( lit3, cube_list1[1][1] );
  EXPECT_EQ( lit1, cube_list1[2][0] );
  EXPECT_EQ( lit2, cube_list1[2][1] );
  EXPECT_EQ( lit1, cube_list1[3][0] );
  EXPECT_EQ( lit3, cube_list1[3][1] );

};

TEST_F(SopTest, Icover_cover_product_bad)
{
  auto cover1 = SopCover{nv, { {lit0}, {lit1} } };
  auto src2 = SopCover{nv + 1, { {lit2}, {lit3} } };

  EXPECT_THROW( cover1 &= src2, std::invalid_argument );
}

TEST_F(SopTest, Icover_cube_product)
{
  auto cover1 = SopCover{nv, { {lit0}, {lit1} } };
  auto src2 = SopCube{nv,  {lit2} };

  cover1 &= src2;

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
  ASSERT_EQ( 2, cube_list1.size() );
  ASSERT_EQ( 2, cube_list1[0].size() );
  ASSERT_EQ( 2, cube_list1[1].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit2, cube_list1[0][1] );
  EXPECT_EQ( lit1, cube_list1[1][0] );
  EXPECT_EQ( lit2, cube_list1[1][1] );

};

TEST_F(SopTest, Icover_cube_product_bad)
{
  auto cover1 = SopCover{nv, { {lit0}, {lit1} } };
  auto src2 = SopCube{nv + 1,  {lit2} };

  EXPECT_THROW( cover1 &= src2, std::invalid_argument );
}

TEST_F(SopTest, Icover_literal_product)
{
  auto cover1 = SopCover{nv, { {lit0}, {lit1} } };

  cover1 &= lit2;

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
  ASSERT_EQ( 2, cube_list1.size() );
  ASSERT_EQ( 2, cube_list1[0].size() );
  ASSERT_EQ( 2, cube_list1[1].size() );
  EXPECT_EQ( lit0, cube_list1[0][0] );
  EXPECT_EQ( lit2, cube_list1[0][1] );
  EXPECT_EQ( lit1, cube_list1[1][0] );
  EXPECT_EQ( lit2, cube_list1[1][1] );

};

TEST_F(SopTest, cover_cover_algdiv)
{
  auto src1 = SopCover{nv, { {lit0, lit2},
			     {lit0, lit3},
			     {lit1, lit2},
			     {lit1, lit3} } };
  auto src2 = SopCover{nv, { {lit0}, {lit1} } };

  auto cover1 = src1.algdiv(src2);

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
  ASSERT_EQ( 2, cube_list1.size() );
  ASSERT_EQ( 1, cube_list1[0].size() );
  ASSERT_EQ( 1, cube_list1[1].size() );
  EXPECT_EQ( lit2, cube_list1[0][0] );
  EXPECT_EQ( lit3, cube_list1[1][0] );

};

TEST_F(SopTest, cover_cover_algdiv_bad)
{
  auto src1 = SopCover{nv, { {lit0, lit2},
			     {lit0, lit3},
			     {lit1, lit2},
			     {lit1, lit3} } };
  auto src2 = SopCover{nv + 1, { {lit0}, {lit1} } };

  EXPECT_THROW( src1.algdiv(src2), std::invalid_argument );
}

TEST_F(SopTest, cover_cube_algdiv)
{
  auto src1 = SopCover{nv, { {lit0, lit2},
			     {lit0, lit3},
			     {lit1, lit2},
			     {lit1, lit3} } };
  auto src2 = SopCube{nv, {lit0} };

  auto cover1 = src1.algdiv(src2);

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
  ASSERT_EQ( 2, cube_list1.size() );
  ASSERT_EQ( 1, cube_list1[0].size() );
  ASSERT_EQ( 1, cube_list1[1].size() );
  EXPECT_EQ( lit2, cube_list1[0][0] );
  EXPECT_EQ( lit3, cube_list1[1][0] );

};

TEST_F(SopTest, cover_cube_algdiv_bad)
{
  auto src1 = SopCover{nv, { {lit0, lit2},
			     {lit0, lit3},
			     {lit1, lit2},
			     {lit1, lit3} } };
  auto src2 = SopCube{nv + 1, {lit0} };

  EXPECT_THROW( src1.algdiv(src2), std::invalid_argument );
}

TEST_F(SopTest, cover_literal_algdiv)
{
  auto src1 = SopCover{nv, { {lit0, lit2},
			     {lit0, lit3},
			     {lit1, lit2},
			     {lit1, lit3} } };

  auto cover1 =  src1.algdiv(lit0);

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
  ASSERT_EQ( 2, cube_list1.size() );
  ASSERT_EQ( 1, cube_list1[0].size() );
  ASSERT_EQ( 1, cube_list1[1].size() );
  EXPECT_EQ( lit2, cube_list1[0][0] );
  EXPECT_EQ( lit3, cube_list1[1][0] );

};

TEST_F(SopTest, Scover_cover_algdiv)
{
  auto cover1 = SopCover{nv, { {lit0, lit2},
			       {lit0, lit3},
			       {lit1, lit2},
			       {lit1, lit3} } };
  auto src2 = SopCover{nv, { {lit0}, {lit1} } };

  cover1 = cover1.algdiv(src2);

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
  ASSERT_EQ( 2, cube_list1.size() );
  ASSERT_EQ( 1, cube_list1[0].size() );
  ASSERT_EQ( 1, cube_list1[1].size() );
  EXPECT_EQ( lit2, cube_list1[0][0] );
  EXPECT_EQ( lit3, cube_list1[1][0] );

};

TEST_F(SopTest, Scover_cube_algdiv)
{
  auto cover1 = SopCover{nv, { {lit0, lit2},
			       {lit0, lit3},
			       {lit1, lit2},
			       {lit1, lit3} } };
  auto src2 = SopCube{nv, {lit0} };

  cover1 = cover1.algdiv(src2);

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
  ASSERT_EQ( 2, cube_list1.size() );
  ASSERT_EQ( 1, cube_list1[0].size() );
  ASSERT_EQ( 1, cube_list1[1].size() );
  EXPECT_EQ( lit2, cube_list1[0][0] );
  EXPECT_EQ( lit3, cube_list1[1][0] );

};

TEST_F(SopTest, Scover_literal_algdiv)
{
  auto cover1 = SopCover{nv, { {lit0, lit2},
			       {lit0, lit3},
			       {lit1, lit2},
			       {lit1, lit3} } };

  cover1 = cover1.algdiv(lit0);

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
  ASSERT_EQ( 2, cube_list1.size() );
  ASSERT_EQ( 1, cube_list1[0].size() );
  ASSERT_EQ( 1, cube_list1[1].size() );
  EXPECT_EQ( lit2, cube_list1[0][0] );
  EXPECT_EQ( lit3, cube_list1[1][0] );

};

TEST_F(SopTest, Icover_cover_algdiv)
{
  auto cover1 = SopCover{nv, { {lit0, lit2},
			       {lit0, lit3},
			       {lit1, lit2},
			       {lit1, lit3} } };
  auto src2 = SopCover{nv, { {lit0}, {lit1} } };

  cover1.algdiv_int(src2);

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
  ASSERT_EQ( 2, cube_list1.size() );
  ASSERT_EQ( 1, cube_list1[0].size() );
  ASSERT_EQ( 1, cube_list1[1].size() );
  EXPECT_EQ( lit2, cube_list1[0][0] );
  EXPECT_EQ( lit3, cube_list1[1][0] );

};

TEST_F(SopTest, Icover_cover_algdiv_bad)
{
  auto cover1 = SopCover{nv, { {lit0, lit2},
			       {lit0, lit3},
			       {lit1, lit2},
			       {lit1, lit3} } };
  auto src2 = SopCover{nv + 1, { {lit0}, {lit1} } };

  EXPECT_THROW( cover1.algdiv_int(src2), std::invalid_argument );
}

TEST_F(SopTest, Icover_cube_algdiv)
{
  auto cover1 = SopCover{nv, { {lit0, lit2},
			       {lit0, lit3},
			       {lit1, lit2},
			       {lit1, lit3} } };
  auto src2 = SopCube{nv, {lit0} };

  cover1.algdiv_int(src2);

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
  ASSERT_EQ( 2, cube_list1.size() );
  ASSERT_EQ( 1, cube_list1[0].size() );
  ASSERT_EQ( 1, cube_list1[1].size() );
  EXPECT_EQ( lit2, cube_list1[0][0] );
  EXPECT_EQ( lit3, cube_list1[1][0] );

};

TEST_F(SopTest, Icover_cube_algdiv_bad)
{
  auto cover1 = SopCover{nv, { {lit0, lit2},
			       {lit0, lit3},
			       {lit1, lit2},
			       {lit1, lit3} } };
  auto src2 = SopCube{nv + 1, {lit0} };

  EXPECT_THROW( cover1.algdiv_int(src2), std::invalid_argument );
}

TEST_F(SopTest, Icover_literal_algdiv)
{
  auto cover1 = SopCover{nv, { {lit0, lit2},
			       {lit0, lit3},
			       {lit1, lit2},
			       {lit1, lit3} } };

  cover1.algdiv_int(lit0);

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
  ASSERT_EQ( 2, cube_list1.size() );
  ASSERT_EQ( 1, cube_list1[0].size() );
  ASSERT_EQ( 1, cube_list1[1].size() );
  EXPECT_EQ( lit2, cube_list1[0][0] );
  EXPECT_EQ( lit3, cube_list1[1][0] );

};

// common_cube() のテスト
TEST_F(SopTest, common_cube1)
{
  auto cover1 = SopCover{nv, { {lit0, lit2},
			       {lit0, lit3},
			       {lit0, ~lit4} } };

  auto cube1 = cover1.common_cube();

  EXPECT_TRUE( cube1.is_valid() );
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

// common_cube() のテスト
// 結果は不正となる．
TEST_F(SopTest, common_cube2)
{
  auto cover1 = SopCover{nv, { {lit0, lit2},
			       {~lit0, lit3},
			       {lit0, ~lit4} } };

  auto cube1 = cover1.common_cube();

  EXPECT_TRUE( cube1.is_invalid() );
};

TEST_F(SopTest, cofactor_cube1)
{
  auto cover1 = SopCover{nv, { { lit0, ~lit2},
			       { lit1, ~lit3},
			       {~lit0, ~lit1},
			       {~lit1,  lit2} }};
  auto cube1 = SopCube{nv, {lit0, lit3}};

  auto ans_cover = cover1.cofactor(cube1);

  EXPECT_EQ( 2, ans_cover.cube_num() );
  auto lits_list = ans_cover.literal_list();
  ASSERT_EQ( 2, lits_list.size() );
  ASSERT_EQ( 2, lits_list[0].size() );
  ASSERT_EQ( 1, lits_list[1].size() );
  EXPECT_EQ( ~lit1, lits_list[0][0] );
  EXPECT_EQ(  lit2, lits_list[0][1] );
  EXPECT_EQ( ~lit2, lits_list[1][0] );
}

TEST_F(SopTest, cofactor_cube_bad)
{
  auto cover1 = SopCover{nv, { { lit0, ~lit2},
			       { lit1, ~lit3},
			       {~lit0, ~lit1},
			       {~lit1,  lit2} }};
  auto cube1 = SopCube{nv + 1, {lit0, lit3}};

  EXPECT_THROW( cover1.cofactor(cube1), std::invalid_argument );
}

TEST_F(SopTest, cofactor_int_cube1)
{
  auto ans_cover = SopCover{nv, { { lit0, ~lit2},
				  { lit1, ~lit3},
				  {~lit0, ~lit1},
				  {~lit1,  lit2} }};
  auto cube1 = SopCube{nv, {lit0, lit3}};

  ans_cover.cofactor_int(cube1);

  EXPECT_EQ( 2, ans_cover.cube_num() );
  auto lits_list = ans_cover.literal_list();
  ASSERT_EQ( 2, lits_list.size() );
  ASSERT_EQ( 2, lits_list[0].size() );
  ASSERT_EQ( 1, lits_list[1].size() );
  EXPECT_EQ( ~lit1, lits_list[0][0] );
  EXPECT_EQ(  lit2, lits_list[0][1] );
  EXPECT_EQ( ~lit2, lits_list[1][0] );
}

TEST_F(SopTest, cofactor_int_cube_bad)
{
  auto cover1 = SopCover{nv, { { lit0, ~lit2},
			       { lit1, ~lit3},
			       {~lit0, ~lit1},
			       {~lit1,  lit2} }};
  auto cube1 = SopCube{nv + 1, {lit0, lit3}};

  EXPECT_THROW( cover1.cofactor_int(cube1), std::invalid_argument );
}

TEST_F(SopTest, cofactor_int_lit1)
{
  auto ans_cover = SopCover{nv, { { lit0, ~lit2},
				  { lit1, ~lit3},
				  {~lit0, ~lit1},
				  {~lit1,  lit2} }};

  ans_cover.cofactor_int(lit0);

  EXPECT_EQ( 3, ans_cover.cube_num() );
  auto lits_list = ans_cover.literal_list();
  ASSERT_EQ( 3, lits_list.size() );
  ASSERT_EQ( 2, lits_list[0].size() );
  ASSERT_EQ( 2, lits_list[1].size() );
  ASSERT_EQ( 1, lits_list[2].size() );
  EXPECT_EQ(  lit1, lits_list[0][0] );
  EXPECT_EQ( ~lit3, lits_list[0][1] );
  EXPECT_EQ( ~lit1, lits_list[1][0] );
  EXPECT_EQ(  lit2, lits_list[1][1] );
  EXPECT_EQ( ~lit2, lits_list[2][0] );
}

TEST_F(SopTest, cofactor_int_lit_bad)
{
  auto cover1 = SopCover{nv, { { lit0, ~lit2},
			       { lit1, ~lit3},
			       {~lit0, ~lit1},
			       {~lit1,  lit2} }};

  EXPECT_THROW( cover1.cofactor_int(lit100), std::out_of_range );
}

// ソートのテスト
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

// ソートのテスト
// 重複したキューブが取り除かれるかのテスト
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

// ソートのテスト
// ランダムに生成したカバーを用いる．
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

// expr() のテスト
TEST_F(SopTest, expr)
{
  auto cover1 = SopCover{3};
  auto expr1 = cover1.expr();
  EXPECT_TRUE( expr1.is_zero() );

  auto cover2 = SopCover{3, vector<vector<Literal>>{{}} };
  auto expr2 = cover2.expr();
  EXPECT_TRUE( expr2.is_one() );

  auto cube1 = SopCube{3, {lit0, lit1}};
  auto cube2 = SopCube{3, {~lit2}};
  auto cover3 = SopCover{3, {cube1, cube2}};

  auto expr3 = cover3.expr();
  auto expr3_str = expr3.to_string();
  EXPECT_EQ( "( ( 0 & 1 ) | ~2 )", expr3_str );
}

// tvfunc() のテスト
TEST_F(SopTest, tvfunc)
{
  SizeType nv = 2;
  auto cover1 = SopCover{nv};
  auto func1 = cover1.tvfunc();
  EXPECT_EQ( nv, func1.input_num() );
  EXPECT_TRUE( func1.is_zero() );

  auto cover2 = SopCover{nv, vector<vector<Literal>>{{}} };
  auto func2 = cover2.tvfunc();
  EXPECT_EQ( nv, func2.input_num() );
  EXPECT_TRUE( func2.is_one() );

  auto cover3 = SopCover{nv, {{lit0}, {~lit1}} };
  auto func3 = cover3.tvfunc();
  EXPECT_EQ( nv, func3.input_num() );
  EXPECT_EQ( 1, func3.value(0) );
  EXPECT_EQ( 1, func3.value(1) );
  EXPECT_EQ( 0, func3.value(2) );
  EXPECT_EQ( 1, func3.value(3) );
}

END_NAMESPACE_YM
