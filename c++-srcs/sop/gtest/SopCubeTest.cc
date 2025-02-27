
/// @file SopCubeTest.cc
/// @brief SopCubeTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/SopCube.h"
#include "ym/Literal.h"
#include "SopTest.h"


BEGIN_NAMESPACE_YM

// 変数の数のみのコンストラクタのテスト
TEST_F(SopTest, constructor1)
{
  auto cube1 = SopCube{nv};

  EXPECT_EQ( nv, cube1.variable_num() );
  EXPECT_EQ( 0,  cube1.literal_num() );

  auto lit_list = cube1.literal_list();
  EXPECT_TRUE( lit_list.empty() );
};

// 変数の数とリテラルを指定したコンストラクタのテスト
TEST_F(SopTest, constructor2)
{
  auto cube1 = SopCube{nv, ~lit5};

  EXPECT_TRUE( cube1.is_valid() );
  EXPECT_EQ( nv, cube1.variable_num() );
  EXPECT_EQ( 1,  cube1.literal_num() );

  auto lit_list = cube1.literal_list();
  ASSERT_EQ( 1, lit_list.size() );
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

  EXPECT_FALSE( cube1.check_literal(var0, false) );
  EXPECT_FALSE( cube1.check_literal(var0, true) );
  EXPECT_FALSE( cube1.check_literal(var1, false) );
  EXPECT_FALSE( cube1.check_literal(var1, true) );
  EXPECT_FALSE( cube1.check_literal(var2, false) );
  EXPECT_FALSE( cube1.check_literal(var2, true) );
  EXPECT_FALSE( cube1.check_literal(var3, false) );
  EXPECT_FALSE( cube1.check_literal(var3, true) );
  EXPECT_FALSE( cube1.check_literal(var4, false) );
  EXPECT_FALSE( cube1.check_literal(var4, true) );
  EXPECT_FALSE( cube1.check_literal(var5, false) );
  EXPECT_TRUE(  cube1.check_literal(var5, true) );
  EXPECT_FALSE( cube1.check_literal(var6, false) );
  EXPECT_FALSE( cube1.check_literal(var6, true) );
  EXPECT_FALSE( cube1.check_literal(var7, false) );
  EXPECT_FALSE( cube1.check_literal(var7, true) );
  EXPECT_FALSE( cube1.check_literal(var8, false) );
  EXPECT_FALSE( cube1.check_literal(var8, true) );
  EXPECT_FALSE( cube1.check_literal(var9, false) );
  EXPECT_FALSE( cube1.check_literal(var9, true) );

};

TEST_F(SopTest, constructor2_2)
{
  auto lit10 = Literal(10);
  auto lit11 = Literal(11);
  auto lit12 = Literal(12);
  auto lit26 = Literal(26);
  auto lit27 = Literal(27);
  auto lit29 = Literal(29);
  auto lit32 = Literal(32);
  auto cube1 = SopCube(64, { ~lit0, lit1, lit2, lit3, ~lit4, lit5, ~lit6, lit7, lit8, ~lit9, lit10, ~lit11, ~lit12, ~lit26, ~lit27, ~lit29, ~lit32 });

  auto lit_list = cube1.literal_list();
  ASSERT_EQ( 17, lit_list.size() );
  EXPECT_EQ( ~lit0, lit_list.front() );
}

// 変数の数とリテラルを指定したコンストラクタのテスト
// リテラルが範囲外でエラーとなる．
TEST_F(SopTest, constructor2_bad)
{
  EXPECT_THROW( (SopCube{nv, lit100}),
		std::out_of_range );
};

// 変数の数とリテラルの vector を指定したコンストラクタのテスト
TEST_F(SopTest, constructor3)
{
  auto lit_list = vector<Literal>{~lit5, lit0};
  auto cube1 = SopCube{nv, lit_list};

  EXPECT_TRUE( cube1.is_valid() );
  auto nl = lit_list.size();
  EXPECT_EQ( nv, cube1.variable_num() );
  EXPECT_EQ( nl, cube1.literal_num() );

  auto lit_list1 = cube1.literal_list();
  ASSERT_EQ( nl, lit_list1.size() );
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

// 変数の数とリテラルの vector を指定したコンストラクタのテスト
// リテラルが範囲外でエラーとなる．
TEST_F(SopTest, constructor3_bad)
{
  auto lit_list = vector<Literal>{~lit5, lit100};
  EXPECT_THROW( (SopCube{nv, lit_list}),
		std::out_of_range );
}

// 変数の数とリテラルのリスト初期化子を指定したコンストラクタのテスト
TEST_F(SopTest, constructor4)
{
  auto cube1 = SopCube{nv, {~lit5, lit0}};

  EXPECT_TRUE( cube1.is_valid() );
  auto nl = 2;
  EXPECT_EQ( nv, cube1.variable_num() );
  EXPECT_EQ( nl,  cube1.literal_num() );

  auto lit_list1 = cube1.literal_list();
  ASSERT_EQ( nl, lit_list1.size() );
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

// 変数の数とリテラルのリスト初期化子を指定したコンストラクタのテスト
// リテラルが範囲外でエラーとなる．
TEST_F(SopTest, constructor4_bad)
{
  EXPECT_THROW( (SopCube{nv, {~lit5, lit100}}),
		std::out_of_range );
}

// コピーコンストラクタのテスト
TEST_F(SopTest, copy_constructor)
{
  auto nl = 2;
  auto cube1 = SopCube{nv, {~lit5, lit0}};

  auto cube2 = SopCube{cube1};
  EXPECT_TRUE( cube2.is_valid() );
  EXPECT_EQ( nv, cube2.variable_num() );
  EXPECT_EQ( nl,  cube2.literal_num() );

  auto lit_list1 = cube2.literal_list();
  ASSERT_EQ( nl, lit_list1.size() );
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

// コピー代入演算のテスト
TEST_F(SopTest, copy_assignment)
{
  auto nl = 2;
  auto cube1 = SopCube{nv, {~lit5, lit0}};

  auto cube2 = SopCube{nv};
  EXPECT_TRUE( cube2.is_valid() );
  EXPECT_EQ( 0, cube2.literal_num() );

  cube2 = cube1;
  EXPECT_EQ( nv, cube2.variable_num() );
  EXPECT_EQ( nl, cube2.literal_num() );

  auto lit_list1 = cube2.literal_list();
  ASSERT_EQ( nl, lit_list1.size() );
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

// ムーブコンストラクタのテスト
TEST_F(SopTest, move_constructor)
{
  auto nl = 2;
  auto cube1 = SopCube{nv, {~lit5, lit0}};
  auto cube2 = SopCube{std::move(cube1)};

  EXPECT_EQ( 0, cube1.literal_num() );

  EXPECT_TRUE( cube2.is_valid() );
  EXPECT_EQ( nv, cube2.variable_num() );
  EXPECT_EQ( nl, cube2.literal_num() );

  auto lit_list1 = cube2.literal_list();
  ASSERT_EQ( nl, lit_list1.size() );
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

// ムーブ代入演算のテスト
TEST_F(SopTest, move_assignment)
{
  auto nl = 2;
  auto cube1 = SopCube{nv, {~lit5, lit0}};

  auto cube2 = SopCube{nv};
  EXPECT_TRUE( cube2.is_valid() );
  EXPECT_EQ( 0, cube2.literal_num() );

  cube2 = std::move(cube1);

  EXPECT_EQ( 0, cube1.literal_num() );

  EXPECT_EQ( nv, cube2.variable_num() );
  EXPECT_EQ( nl,  cube2.literal_num() );

  auto lit_list1 = cube2.literal_list();
  ASSERT_EQ( nl, lit_list1.size() );
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

// invalid() のテスト
TEST_F(SopTest, invalid)
{
  auto cube1 = SopCube::invalid(nv);

  EXPECT_EQ( nv, cube1.variable_num() );
  EXPECT_TRUE( cube1.is_invalid() );
}

// is_tautology() のテスト
TEST_F(SopTest, is_tautology)
{
  auto cube1 = SopCube{nv};
  auto cube2 = SopCube{nv, {~lit5, lit0}};

  EXPECT_TRUE( cube1.is_valid() );
  EXPECT_TRUE( cube1.is_tautology() );
  EXPECT_TRUE( cube2.is_valid() );
  EXPECT_FALSE( cube2.is_tautology() );
}

// get_pat() のテスト
TEST_F(SopTest, get_pat)
{
  auto cube1 = SopCube{nv, {~lit5, lit0}};

  EXPECT_TRUE( cube1.is_valid() );
  EXPECT_EQ( SopPat::_1, cube1.get_pat(0) );
  EXPECT_EQ( SopPat::_X, cube1.get_pat(1) );
  EXPECT_EQ( SopPat::_X, cube1.get_pat(2) );
  EXPECT_EQ( SopPat::_X, cube1.get_pat(3) );
  EXPECT_EQ( SopPat::_X, cube1.get_pat(4) );
  EXPECT_EQ( SopPat::_0, cube1.get_pat(5) );
  EXPECT_EQ( SopPat::_X, cube1.get_pat(6) );
  EXPECT_EQ( SopPat::_X, cube1.get_pat(7) );
  EXPECT_EQ( SopPat::_X, cube1.get_pat(8) );
  EXPECT_EQ( SopPat::_X, cube1.get_pat(9) );
  EXPECT_THROW( cube1.get_pat(10),
		std::out_of_range );
}

// check_literal() の範囲外エラーのテスト
TEST_F(SopTest, check_literal_bad)
{
  auto cube1 = SopCube{nv, {~lit5, lit0}};

  EXPECT_THROW( cube1.check_literal(lit100),
		std::out_of_range );
  EXPECT_THROW( cube1.check_literal(100, true),
		std::out_of_range );
}

// check_containment のテスト
TEST_F(SopTest, check_containment)
{
  auto cube1 = SopCube{nv, {~lit5, lit0}};
  auto cube2 = SopCube{nv, {~lit5}};

  EXPECT_TRUE(  cube1.check_containment(cube2) );
  EXPECT_FALSE( cube2.check_containment(cube1) );

  auto cube3 = SopCube{nv, {lit0}};
  EXPECT_FALSE( cube2.check_containment(cube3) );
  EXPECT_FALSE( cube3.check_containment(cube2) );
};

// check_containment のテスト
// キューブのサイズが合わないのでエラーとなる．
TEST_F(SopTest, check_containment_bad)
{
  auto cube1 = SopCube{nv, {~lit5, lit0}};
  auto cube2 = SopCube{nv + 1, {lit1}};

  EXPECT_THROW( cube1.check_containment(cube2),
		std::invalid_argument );
  EXPECT_THROW( cube2.check_containment(cube1),
		std::invalid_argument );
};

// SopCube & SopCube のテスト
TEST_F(SopTest, cube_cube_product1)
{
  auto cube1 = SopCube{nv, {~lit5, ~lit7}};
  auto cube2 = SopCube{nv, {lit0, ~lit5}};
  auto cube3 = cube1 & cube2;

  EXPECT_TRUE( cube3.is_valid() );
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

// SopCube & SopCube のテスト
// 2つのキューブがコンフリクトしているので結果は空となる．
TEST_F(SopTest, cube_cube_product2)
{
  auto cube1 = SopCube{nv, {~lit5, ~lit7}};
  auto cube2 = SopCube{nv, {lit0, lit5}};
  auto cube3 = cube1 & cube2;

  EXPECT_TRUE(  cube3.is_invalid() );
};

// SopCube & SopCube のテスト
// 変数の数が合わないのでエラーとなる．
TEST_F(SopTest, cube_cube_product_bad)
{
  auto cube1 = SopCube{nv, {~lit5, ~lit7}};
  auto cube2 = SopCube{nv + 1, {lit0, ~lit5}};

  EXPECT_THROW( cube1 & cube2,
		std::invalid_argument );
}

// SopCube & SopCube のテスト
// 第1オペランドが右辺値のケース
TEST_F(SopTest, Rcube_cube_product1)
{
  auto cube1 = SopCube{nv, {~lit5, ~lit7}};
  auto cube2 = SopCube{nv, {lit0, ~lit5}};
  auto cube3 = std::move(cube1) & cube2;

  EXPECT_EQ( 0, cube1.literal_num() );

  EXPECT_TRUE(  cube3.is_valid() );
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

// SopCube & SopCube のテスト
// 第2オペランドが右辺値のケース
TEST_F(SopTest, cube_Rcube_product1)
{
  auto cube1 = SopCube{nv, {~lit5, ~lit7}};
  auto cube2 = SopCube{nv, {lit0, ~lit5}};
  auto cube3 = cube1 & std::move(cube2);

  EXPECT_EQ( 0, cube2.literal_num() );

  EXPECT_TRUE(  cube3.is_valid() );
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

// SopCube & SopCube のテスト
// 第1オペランドと第2オペランドが右辺値のケース
TEST_F(SopTest, Rcube_Rcube_product1)
{
  auto cube1 = SopCube{nv, {~lit5, ~lit7}};
  auto cube2 = SopCube{nv, {lit0, ~lit5}};
  auto cube3 = std::move(cube1) & std::move(cube2);

  EXPECT_EQ( 0, cube1.literal_num() );

  EXPECT_TRUE(  cube3.is_valid() );
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

// Literal & SopCube のテスト
TEST_F(SopTest, lit_cube_product1)
{
  auto cube1 = SopCube{nv, {~lit5, ~lit7}};
  auto cube3 = lit0 & cube1;

  EXPECT_TRUE(  cube3.is_valid() );
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

// Literal & SopCube のテスト
// 第2オペランドが右辺値のケース
TEST_F(SopTest, lit_Rcube_product1)
{
  auto cube1 = SopCube{nv, {~lit5, ~lit7}};
  auto cube3 = lit0 & std::move(cube1);

  EXPECT_EQ( 0, cube1.literal_num() );

  EXPECT_TRUE(  cube3.is_valid() );
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

// Literal & SopCube のテスト
// コンフリクトしているケース
TEST_F(SopTest, lit_cube_product2)
{
  auto cube1 = SopCube{nv, {~lit5, ~lit7}};
  auto cube3 = lit5 & cube1;

  EXPECT_TRUE(  cube3.is_invalid() );
};

// SopCube & Literal のテスト
TEST_F(SopTest, cube_lit_product1)
{
  auto cube1 = SopCube{nv, {~lit5, ~lit7}};
  auto cube3 = cube1 & lit0;

  EXPECT_TRUE(  cube3.is_valid() );
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

// SopCube & Literal のテスト
// 第1オペランドが右辺値のケース
TEST_F(SopTest, Rcube_lit_product1)
{
  auto cube1 = SopCube{nv, {~lit5, ~lit7}};
  auto cube3 = std::move(cube1) & lit0;

  EXPECT_EQ( 0, cube1.literal_num() );

  EXPECT_TRUE(  cube3.is_valid() );
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

// SopCube & Literal のテスト
// コンフリクトしているケース
TEST_F(SopTest, cube_lit_product2)
{
  auto cube1 = SopCube{nv, {~lit5, ~lit7}};
  auto cube3 = cube1 & lit5;

  EXPECT_TRUE(  cube3.is_invalid() );
};

// SopCube & SopCube のテスト
// 左辺と第1オペランドが同一のケース
TEST_F(SopTest, Scube_cube_product1)
{
  auto cube1 = SopCube{nv, {~lit5, ~lit7}};
  auto cube2 = SopCube{nv, {lit0, ~lit5}};
  cube1 = cube1 & cube2;

  EXPECT_TRUE(  cube1.is_valid() );
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

// SopCube & SopCube のテスト
// 左辺と第1オペランドが同一のケース
// ただしコンフリクトしている．
TEST_F(SopTest, Scube_cube_product2)
{
  auto cube1 = SopCube{nv, {~lit5, ~lit7}};
  auto cube2 = SopCube{nv, {lit0, lit5}};
  cube1 = cube1 & cube2;

  EXPECT_TRUE(  cube1.is_invalid() );
};

// SopCube & Literal のテスト
// 左辺と第1オペランドが同一のケース
TEST_F(SopTest, Scube_literal_product1)
{
  auto cube1 = SopCube{nv, {~lit5, ~lit7}};
  cube1 = cube1 & lit0;

  EXPECT_TRUE(  cube1.is_valid() );
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

// SopCube & Literal のテスト
// 左辺と第1オペランドが同一のケース
// コンフリクトしている．
TEST_F(SopTest, Scube_literal_product2)
{
  auto cube1 = SopCube{nv, {~lit5, ~lit7}};
  cube1 = cube1 & lit5;

  EXPECT_TRUE(  cube1.is_invalid() );
};

// Literal & SopCube のテスト
// 左辺と第2オペランドが同一のケース
TEST_F(SopTest, literal_Scube_product1)
{
  auto cube1 = SopCube{nv, {~lit5, ~lit7}};
  cube1 = lit0 & cube1;

  EXPECT_TRUE(  cube1.is_valid() );
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

// Literal & SopCube のテスト
// 左辺と第2オペランドが同一のケース
// コンフリクトしている．
TEST_F(SopTest, literal_Scube_product2)
{
  auto cube1 = SopCube{nv, {~lit5, ~lit7}};

  cube1 = lit5 & cube1;

  EXPECT_TRUE(  cube1.is_invalid() );
};

// SopCube &= SopCube のテスト
TEST_F(SopTest, Icube_cube_product1)
{
  auto cube1 = SopCube{nv, {~lit5, ~lit7}};
  auto cube2 = SopCube{nv, {lit0, ~lit5}};

  cube1 &= cube2;

  EXPECT_TRUE(  cube1.is_valid() );
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

// SopCube &= SopCube のテスト
// コンフリクトしている．
TEST_F(SopTest, Icube_cube_product2)
{
  auto cube1 = SopCube{nv, {~lit5, ~lit7}};
  auto cube2 = SopCube{nv, {lit0, lit5}};

  cube1 &= cube2;

  EXPECT_TRUE(  cube1.is_invalid() );
};

// SopCube &= Literal のテスト
TEST_F(SopTest, Icube_literal_product1)
{
  auto cube1 = SopCube{nv, {~lit5, ~lit7}};

  cube1 &= lit0;

  EXPECT_TRUE(  cube1.is_valid() );
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

// SopCube &= Literal のテスト
// コンフリクトしている．
TEST_F(SopTest, Icube_literal_product2)
{
  auto cube1 = SopCube{nv, {~lit5, ~lit7}};

  cube1 &= lit5;

  EXPECT_TRUE(  cube1.is_invalid() );
};

TEST_F(SopTest, cube_cube_algdiv1)
{
  auto cube1 = SopCube{nv, {lit0, ~lit5, ~lit7}};
  auto cube2 = SopCube{nv, {lit0, ~lit7}};
  auto cube3 = cube1 / cube2;

  EXPECT_TRUE( cube3.is_valid() );
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

TEST_F(SopTest, cube_cube_algdiv1_bad)
{
  auto cube1 = SopCube{nv, {lit0, ~lit5, ~lit7}};
  auto cube2 = SopCube{nv + 1, {lit0, ~lit7}};
  EXPECT_THROW( cube1 / cube2, std::invalid_argument );
}

TEST_F(SopTest, Rcube_cube_algdiv1)
{
  auto cube1 = SopCube{nv, {lit0, ~lit5, ~lit7}};
  auto cube2 = SopCube{nv, {lit0, ~lit7}};
  auto cube3 = std::move(cube1) / cube2;

  EXPECT_EQ( 0, cube1.literal_num() );

  EXPECT_TRUE( cube3.is_valid() );
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

TEST_F(SopTest, cube_cube_algdiv2)
{
  auto cube1 = SopCube{nv, {lit0, ~lit5}};
  auto cube2 = SopCube{nv, {~lit7}};
  auto cube3 = cube1 / cube2;

  EXPECT_TRUE( cube3.is_invalid() );
};

TEST_F(SopTest, cube_lit_algdiv1)
{
  auto cube1 = SopCube{nv, {lit0, ~lit5, ~lit7}};
  auto cube3 = cube1 / lit0;

  EXPECT_TRUE( cube3.is_valid() );
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

TEST_F(SopTest, cube_lit_algdiv1_bad)
{
  auto cube1 = SopCube{nv, {lit0, ~lit5, ~lit7}};
  EXPECT_THROW( cube1 / lit100, std::out_of_range );
}

TEST_F(SopTest, Rcube_lit_algdiv1)
{
  auto cube1 = SopCube{nv, {lit0, ~lit5, ~lit7}};
  auto cube3 = std::move(cube1) / lit0;

  EXPECT_EQ( 0, cube1.literal_num() );

  EXPECT_TRUE( cube3.is_valid() );
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

TEST_F(SopTest, cube_lit_algdiv2)
{
  auto cube1 = SopCube{nv, {lit0, ~lit5, ~lit7}};
  auto cube3 = cube1 / lit5;

  EXPECT_TRUE( cube3.is_invalid() );
};

TEST_F(SopTest, Scube_cube_algdiv1)
{
  auto cube1 = SopCube{nv, {lit0, ~lit5, ~lit7}};
  auto cube2 = SopCube{nv, {lit0, ~lit7}};
  cube1 = cube1 / cube2;

  EXPECT_TRUE( cube1.is_valid() );
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

TEST_F(SopTest, Scube_cube_algdiv2)
{
  auto cube1 = SopCube{nv, {lit0, ~lit5}};
  auto cube2 = SopCube{nv, {~lit7}};
  cube1 = cube1 / cube2;

  EXPECT_TRUE( cube1.is_invalid() );
};

TEST_F(SopTest, Scube_lit_algdiv1)
{
  auto cube1 = SopCube{nv, {lit0, ~lit5, ~lit7}};
  cube1 = cube1 / lit0;

  EXPECT_TRUE( cube1.is_valid() );
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

TEST_F(SopTest, Scube_lit_algdiv2)
{
  auto cube1 = SopCube{nv, {lit4, ~lit5, ~lit7}};
  cube1 = cube1 / lit5;

  EXPECT_TRUE( cube1.is_invalid() );
};

TEST_F(SopTest, Icube_cube_algdiv1)
{
  auto cube1 = SopCube{nv, {lit0, ~lit5, ~lit7}};
  auto cube2 = SopCube{nv, {lit0, ~lit7}};
  cube1 /= cube2;

  EXPECT_TRUE( cube1.is_valid() );
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

TEST_F(SopTest, Icube_cube_algdiv1_bad)
{
  auto cube1 = SopCube{nv, {lit0, ~lit5, ~lit7}};
  auto cube2 = SopCube{nv + 1, {lit0, ~lit7}};
  EXPECT_THROW( cube1 /= cube2, std::invalid_argument );
}

TEST_F(SopTest, Icube_cube_algdiv2)
{
  auto cube1 = SopCube{nv, {lit0, ~lit5}};
  auto cube2 = SopCube{nv, {~lit7}};
  cube1 /= cube2;

  EXPECT_TRUE( cube1.is_invalid() );
};

TEST_F(SopTest, Icube_lit_algdiv1)
{
  auto cube1 = SopCube{nv, {lit0, ~lit5, ~lit7}};
  cube1 /= lit0;

  EXPECT_TRUE( cube1.is_valid() );
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

TEST_F(SopTest, Icube_lit_algdiv1_bad)
{
  auto cube1 = SopCube{nv, {lit0, ~lit5, ~lit7}};
  EXPECT_THROW( cube1 /= lit100, std::out_of_range );
}

TEST_F(SopTest, Icube_lit_algdiv2)
{
  auto cube1 = SopCube{nv, {~lit5, lit1, lit2}};

  cube1 /= lit5;

  EXPECT_TRUE( cube1.is_invalid() );
};

// compare() のテスト
TEST_F(SopTest, compare1)
{
  const int nv1 = 100;
  SizeType var80{80};

  auto lit0 = Literal{var0, false};
  auto lit1 = Literal{var80, false};

  auto cube1 = SopCube{nv1, { lit0,  lit1}};
  auto cube2 = SopCube{nv1, { lit0, ~lit1}};
  auto cube3 = SopCube{nv1, {~lit0,  lit1}};

  EXPECT_EQ(  0, SopCube::compare(cube1, cube1) );
  EXPECT_EQ(  1, SopCube::compare(cube1, cube2) );
  EXPECT_EQ(  1, SopCube::compare(cube1, cube3) );
  EXPECT_EQ( -1, SopCube::compare(cube2, cube1) );
  EXPECT_EQ(  0, SopCube::compare(cube2, cube2) );
  EXPECT_EQ(  1, SopCube::compare(cube2, cube3) );
  EXPECT_EQ( -1, SopCube::compare(cube3, cube1) );
  EXPECT_EQ( -1, SopCube::compare(cube3, cube2) );
  EXPECT_EQ(  0, SopCube::compare(cube3, cube3) );
};

// SopCube の比較演算のテスト
TEST_F(SopTest, compare2)
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
  EXPECT_FALSE( cube1 <  cube2 );
  EXPECT_TRUE(  cube1 >  cube2 );
  EXPECT_FALSE( cube1 <= cube2 );
  EXPECT_TRUE(  cube1 >= cube2 );

  EXPECT_FALSE( cube1 == cube3 );
  EXPECT_FALSE( cube1 <  cube3 );
  EXPECT_TRUE(  cube1 >  cube3 );
  EXPECT_FALSE( cube1 <= cube3 );
  EXPECT_TRUE(  cube1 >= cube3 );

  EXPECT_FALSE( cube2 == cube1 );
  EXPECT_TRUE(  cube2 <  cube1 );
  EXPECT_FALSE( cube2 >  cube1 );
  EXPECT_TRUE(  cube2 <= cube1 );
  EXPECT_FALSE( cube2 >= cube1 );

  EXPECT_TRUE(  cube2 == cube2 );
  EXPECT_FALSE( cube2 <  cube2 );
  EXPECT_FALSE( cube2 >  cube2 );
  EXPECT_TRUE(  cube2 <= cube2 );
  EXPECT_TRUE(  cube2 >= cube2 );

  EXPECT_FALSE( cube2 == cube3 );
  EXPECT_FALSE( cube2 <  cube3 );
  EXPECT_TRUE(  cube2 >  cube3 );
  EXPECT_FALSE( cube2 <= cube3 );
  EXPECT_TRUE(  cube2 >= cube3 );

  EXPECT_FALSE( cube3 == cube1 );
  EXPECT_TRUE(  cube3 <  cube1 );
  EXPECT_FALSE( cube3 >  cube1 );
  EXPECT_TRUE(  cube3 <= cube1 );
  EXPECT_FALSE( cube3 >= cube1 );

  EXPECT_FALSE( cube3 == cube2 );
  EXPECT_TRUE(  cube3 <  cube2 );
  EXPECT_FALSE( cube3 >  cube2 );
  EXPECT_TRUE(  cube3 <= cube2 );
  EXPECT_FALSE( cube3 >= cube2 );

  EXPECT_TRUE(  cube3 == cube3 );
  EXPECT_FALSE( cube3 <  cube3 );
  EXPECT_FALSE( cube3 >  cube3 );
  EXPECT_TRUE(  cube3 <= cube3 );
  EXPECT_TRUE(  cube3 >= cube3 );

};

// 比較演算のテスト
// 変数の数が合わないのでエラーとなる．
TEST_F(SopTest, compare_bad)
{
  const int nv1 = 100;
  SizeType var80{80};

  auto lit0 = Literal{var0, false};
  auto lit1 = Literal{var80, false};

  auto cube1 = SopCube{nv1, { lit0,  lit1}};
  auto cube2 = SopCube{nv1 + 1, { lit0, ~lit1}};

  EXPECT_THROW( SopCube::compare(cube1, cube2),
		std::invalid_argument );
  EXPECT_THROW( auto r = (cube1 == cube2), std::invalid_argument );
  EXPECT_THROW( auto r = (cube1 < cube2),  std::invalid_argument );
  EXPECT_THROW( auto r = (cube1 > cube2),  std::invalid_argument );
  EXPECT_THROW( auto r = (cube1 <= cube2), std::invalid_argument );
  EXPECT_THROW( auto r = (cube1 >= cube2), std::invalid_argument );
}

// expr() のテスト
TEST_F(SopTest, expr)
{
  SizeType nv = 3;

  auto cube0 = SopCube{nv};
  auto expr0 = cube0.expr();
  EXPECT_TRUE( expr0.is_one() );

  auto cube1 = SopCube{nv, {lit0, lit1, ~lit2}};
  auto expr1 = cube1.expr();
  auto expr1_str = expr1.to_string();
  EXPECT_EQ( "( 0 & 1 & ~2 )", expr1_str );
}

// tvfunc のテスト
TEST_F(SopTest, tvfunc)
{
  SizeType nv = 2;

  // 空のキューブ
  auto cube1 = SopCube{nv};
  auto func1 = cube1.tvfunc();
  EXPECT_EQ( nv, func1.input_num() );
  EXPECT_TRUE( func1.is_one() );

  // lit0 & ~lit1 のキューブ
  auto cube2 = SopCube{nv, {lit0, ~lit1}};
  auto func2 = cube2.tvfunc();
  EXPECT_EQ( nv, func2.input_num() );
  EXPECT_EQ( 0, func2.value(0) );
  EXPECT_EQ( 1, func2.value(1) );
  EXPECT_EQ( 0, func2.value(2) );
  EXPECT_EQ( 0, func2.value(3) );
}

// SopCube のリストを TvFunc に変換する関数のテスト
TEST_F(SopTest, tvfunc2)
{
  SizeType nv = 2;
  auto cube1 = SopCube{nv, {lit0}};
  auto cube2 = SopCube{nv, {~lit1}};
  auto func1 = SopCube::tvfunc(nv, {cube1, cube2});
  EXPECT_EQ( 2, func1.input_num() );
  EXPECT_EQ( 1, func1.value(0) );
  EXPECT_EQ( 1, func1.value(1) );
  EXPECT_EQ( 0, func1.value(2) );
  EXPECT_EQ( 1, func1.value(3) );
}

// SopCube のリストを TvFunc に変換する関数のテスト
// 変数の数が合わないのでエラーとなる．
TEST_F(SopTest, tvfunc2_bad)
{
  SizeType nv = 2;
  auto cube1 = SopCube{nv + 1, {lit0}};
  auto cube2 = SopCube{nv, {~lit1}};
  EXPECT_THROW( SopCube::tvfunc(nv, {cube1, cube2}),
		std::invalid_argument );
}

END_NAMESPACE_YM
