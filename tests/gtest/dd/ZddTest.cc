
/// @file ZddTest.cc
/// @brief ZddTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/ZddMgr.h"
#include "ym/Zdd.h"


BEGIN_NAMESPACE_YM

TEST(ZddTest, invalid)
{
  Zdd zdd;

  EXPECT_FALSE( zdd.is_valid() );
  EXPECT_TRUE( zdd.is_invalid() );
  EXPECT_FALSE( zdd.is_zero() );
  EXPECT_FALSE( zdd.is_one() );
}

TEST(ZddTest, zero)
{
  ZddMgr mgr;

  auto zdd = mgr.zero();

  EXPECT_TRUE( zdd.is_valid() );
  EXPECT_FALSE( zdd.is_invalid() );
  EXPECT_TRUE( zdd.is_zero() );
  EXPECT_FALSE( zdd.is_one() );
  EXPECT_EQ( 0, zdd.count() );

  Zdd f0;
  Zdd f1;
  auto index = zdd.root_decomp(f0, f1);
  EXPECT_EQ( BAD_VARID, index );
  EXPECT_EQ( zdd, f0 );
  EXPECT_EQ( zdd, f1 );
}

TEST(ZddTest, one)
{
  ZddMgr mgr;

  auto zdd = mgr.one();

  EXPECT_TRUE( zdd.is_valid() );
  EXPECT_FALSE( zdd.is_invalid() );
  EXPECT_FALSE( zdd.is_zero() );
  EXPECT_TRUE( zdd.is_one() );
  EXPECT_EQ( 1, zdd.count() );

  Zdd f0;
  Zdd f1;
  auto index = zdd.root_decomp(f0, f1);
  EXPECT_EQ( BAD_VARID, index );
  EXPECT_EQ( zdd, f0 );
  EXPECT_EQ( zdd, f1 );
}

TEST(ZddTest, make_set1)
{
  ZddMgr mgr;

  auto zdd = mgr.make_set({0});

  EXPECT_TRUE( zdd.is_valid() );
  EXPECT_FALSE( zdd.is_invalid() );
  EXPECT_FALSE( zdd.is_zero() );
  EXPECT_FALSE( zdd.is_one() );
  EXPECT_EQ( 1, zdd.count() );

  Zdd f0;
  Zdd f1;
  auto index = zdd.root_decomp(f0, f1);
  EXPECT_EQ( 0, index );
  EXPECT_TRUE( f0.is_zero() );
  EXPECT_TRUE( f1.is_one() );
}

TEST(ZddTest, make_set2)
{
  ZddMgr mgr;

  vector<SizeType> elem_list{0, 2};

  auto zdd = mgr.make_set(elem_list);

  EXPECT_TRUE( zdd.is_valid() );
  EXPECT_FALSE( zdd.is_invalid() );
  EXPECT_FALSE( zdd.is_zero() );
  EXPECT_FALSE( zdd.is_one() );
  EXPECT_EQ( 1, zdd.count() );

  Zdd f0;
  Zdd f1;
  auto index = zdd.root_decomp(f0, f1);
  EXPECT_EQ( 0, index );
  EXPECT_TRUE( f0.is_zero() );

  Zdd f10;
  Zdd f11;
  auto index2 = f1.root_decomp(f10, f11);
  EXPECT_EQ( 2, index2 );
  EXPECT_TRUE( f10.is_zero() );
  EXPECT_TRUE( f11.is_one() );
}

TEST(ZddTest, make_set3)
{
  ZddMgr mgr;

  auto zdd = mgr.make_set({});
  EXPECT_EQ( 1, zdd.count() );

  EXPECT_TRUE( zdd.is_valid() );
  EXPECT_FALSE( zdd.is_invalid() );
  EXPECT_TRUE( zdd.is_one() );
}

TEST(ZddTest, cup1)
{
  ZddMgr mgr;

  auto zdd1 = mgr.make_set({0});
  auto zdd2 = mgr.make_set({1});

  auto zdd = zdd1 | zdd2;

  EXPECT_TRUE( zdd.is_valid() );
  EXPECT_FALSE( zdd.is_invalid() );
  EXPECT_FALSE( zdd.is_zero() );
  EXPECT_FALSE( zdd.is_one() );
  EXPECT_EQ( 2, zdd.count() );

  Zdd f0;
  Zdd f1;
  auto index = zdd.root_decomp(f0, f1);
  EXPECT_EQ( 0, index );
  EXPECT_TRUE( f1.is_one() );

  Zdd f00;
  Zdd f01;
  auto index2 = f0.root_decomp(f00, f01);
  EXPECT_EQ( 1, index2 );
  EXPECT_TRUE( f00.is_zero() );
  EXPECT_TRUE( f01.is_one() );
}

TEST(ZddTest, cup2)
{
  ZddMgr mgr;

  auto zdd1 = mgr.make_set({0, 2});
  auto zdd2 = mgr.make_set({0, 1});

  auto zdd = zdd1 | zdd2;

  EXPECT_TRUE( zdd.is_valid() );
  EXPECT_FALSE( zdd.is_invalid() );
  EXPECT_FALSE( zdd.is_zero() );
  EXPECT_FALSE( zdd.is_one() );
  EXPECT_EQ( 2, zdd.count() );

  Zdd f0;
  Zdd f1;
  auto index = zdd.root_decomp(f0, f1);
  EXPECT_EQ( 0, index );
  EXPECT_TRUE( f0.is_zero() );

  Zdd f10;
  Zdd f11;
  auto index1 = f1.root_decomp(f10, f11);
  EXPECT_EQ( 1, index1 );
  EXPECT_TRUE( f11.is_one() );

  Zdd f100;
  Zdd f101;
  auto index2 = f10.root_decomp(f100, f101);
  EXPECT_EQ( 2, index2 );
  EXPECT_TRUE( f100.is_zero() );
  EXPECT_TRUE( f101.is_one() );
}

TEST(ZddTest, cup3)
{
  ZddMgr mgr;

  auto zdd1 = mgr.make_set({0, 2});
  auto zdd2 = mgr.make_set({1, 3});

  auto zdd = zdd1 | zdd2;

  EXPECT_TRUE( zdd.is_valid() );
  EXPECT_FALSE( zdd.is_invalid() );
  EXPECT_FALSE( zdd.is_zero() );
  EXPECT_FALSE( zdd.is_one() );
  EXPECT_EQ( 2, zdd.count() );

  Zdd f0;
  Zdd f1;
  auto index = zdd.root_decomp(f0, f1);
  EXPECT_EQ( 0, index );

  Zdd f00;
  Zdd f01;
  auto index1 = f0.root_decomp(f00, f01);
  EXPECT_EQ( 1, index1 );
  EXPECT_TRUE( f00.is_zero() );

  Zdd f010;
  Zdd f011;
  auto index2 = f01.root_decomp(f010, f011);
  EXPECT_EQ( 3, index2 );
  EXPECT_TRUE( f010.is_zero() );
  EXPECT_TRUE( f011.is_one() );

  Zdd f10;
  Zdd f11;
  auto index4 = f1.root_decomp(f10, f11);
  EXPECT_EQ( 2, index4 );
  EXPECT_TRUE( f10.is_zero() );
  EXPECT_TRUE( f11.is_one() );
}

TEST(ZddTest, cup4)
{
  ZddMgr mgr;

  auto zdd2 = mgr.make_set({2, 3});
  auto zdd3 = mgr.make_set({0, 3});

  auto zdd = zdd2 | zdd3;

  EXPECT_EQ( 2, zdd.count() );

  Zdd f0;
  Zdd f1;
  auto index = zdd.root_decomp(f0, f1);
  EXPECT_EQ( 0, index );

  Zdd f00;
  Zdd f01;
  auto index2 = f0.root_decomp(f00, f01);
  EXPECT_EQ( 2, index2 );
  EXPECT_TRUE( f00.is_zero() );

  Zdd f010;
  Zdd f011;
  auto index3 = f01.root_decomp(f010, f011);
  EXPECT_EQ( 3, index3 );
  EXPECT_TRUE( f010.is_zero() );
  EXPECT_TRUE( f011.is_one() );

  Zdd f10;
  Zdd f11;
  auto index4 = f1.root_decomp(f10, f11);
  EXPECT_EQ( 3, index4 );
  EXPECT_TRUE( f10.is_zero() );
  EXPECT_TRUE( f11.is_one() );
}

TEST(ZddTest, cup5)
{
  ZddMgr mgr;

  auto zdd1 = mgr.make_set({2, 3});
  auto zdd2 = mgr.one();

  auto zdd = zdd1 | zdd2;

  EXPECT_EQ( 2, zdd.count() );

  Zdd f0;
  Zdd f1;
  auto index = zdd.root_decomp(f0, f1);
  EXPECT_EQ( 2, index );
  EXPECT_TRUE( f0.is_one() );
  EXPECT_EQ( zdd1.root_cofactor1(), f1 );
}

TEST(ZddTest, cap1)
{
  ZddMgr mgr;

  auto zdd1 = mgr.make_set({0, 1});
  auto zdd2 = mgr.make_set({2, 3});
  auto zdd3 = mgr.make_set({0, 3});

  auto zdd4 = zdd1 | zdd2 | zdd3;

  auto zdd5 = mgr.make_set({0, 2});

  auto zdd6 = zdd2 | zdd5;

  auto zdd = zdd4 & zdd6;

  EXPECT_EQ( 1, zdd.count() );

  EXPECT_EQ( zdd2, zdd );
}

TEST(ZddTest, cap2)
{
  ZddMgr mgr;

  auto zdd1 = mgr.make_set({0, 1});
  auto zdd2 = mgr.make_set({2, 3});
  auto zdd3 = mgr.make_set({0, 3});

  auto zdd4 = zdd1 | zdd2 | zdd3;

  auto zdd5 = mgr.make_set({0, 2});

  auto zdd6 = zdd2 | zdd5 | zdd3;

  auto zdd = zdd4 & zdd6;

  EXPECT_EQ( 2, zdd.count() );

  auto ref_zdd = zdd2 | zdd3;

  EXPECT_EQ( ref_zdd, zdd );
}

TEST(ZddTest, cap3)
{
  ZddMgr mgr;

  auto zdd1 = mgr.make_set({0, 1});
  auto zdd2 = mgr.make_set({2, 3});
  auto zdd3 = mgr.make_set({0, 3});

  auto zdd4 = zdd1 | zdd2 | zdd3 | mgr.one();

  auto zdd5 = mgr.make_set({0, 2});

  auto zdd6 = zdd2 | zdd5 | zdd3;

  auto zdd = zdd4 & zdd6;

  EXPECT_EQ( 2, zdd.count() );

  auto ref_zdd = zdd2 | zdd3;

  EXPECT_EQ( ref_zdd, zdd );
}

TEST(ZddTest, cap4)
{
  ZddMgr mgr;

  auto uzdd = mgr.one();
  auto zdd1 = mgr.make_set({0, 1});
  auto zdd2 = mgr.make_set({2, 3});
  auto zdd3 = mgr.make_set({0, 3});

  auto zdd4 = zdd1 | zdd2 | zdd3 | uzdd;

  auto zdd5 = mgr.make_set({0, 2});

  auto zdd6 = zdd2 | zdd5 | zdd3 | uzdd;

  auto zdd = zdd4 & zdd6;

  EXPECT_EQ( 3, zdd.count() );

  auto ref_zdd = zdd2 | zdd3 | uzdd;

  EXPECT_EQ( ref_zdd, zdd );
}

TEST(ZddTest, diff1)
{
  ZddMgr mgr;

  auto zdd1 = mgr.make_set({0, 1});
  auto zdd2 = mgr.make_set({2, 3});
  auto zdd3 = mgr.make_set({0, 3});

  auto zdd4 = zdd1 | zdd2 | zdd3;

  auto zdd = zdd4 - zdd3;

  EXPECT_EQ( 2, zdd.count() );

  auto ref_zdd = zdd1 | zdd2;

  EXPECT_EQ( ref_zdd, zdd );
}

TEST(ZddTest, diff2)
{
  ZddMgr mgr;

  auto zdd1 = mgr.make_set({0, 1});
  auto zdd2 = mgr.make_set({2, 3});
  auto zdd3 = mgr.make_set({0, 3});

  auto zdd4 = zdd2 | zdd3;
  auto zdd5 = zdd1 | zdd2;
  auto zdd = zdd4 - zdd5;

  EXPECT_EQ( 1, zdd.count() );

  auto ref_zdd = zdd3;

  EXPECT_EQ( ref_zdd, zdd );
}

TEST(ZddTest, product1)
{
  ZddMgr mgr;

  auto zdd1 = mgr.make_set({0});
  auto zdd2 = mgr.make_set({1});

  auto zdd = zdd1 * zdd2;

  EXPECT_EQ( 1, zdd.count() );

  Zdd f0;
  Zdd f1;
  auto index = zdd.root_decomp(f0, f1);
  EXPECT_EQ( 0, index );
  EXPECT_TRUE( f0.is_zero() );

  Zdd f10;
  Zdd f11;
  auto index2 = f1.root_decomp(f10, f11);
  EXPECT_EQ( 1, index2 );
  EXPECT_TRUE( f10.is_zero() );
  EXPECT_TRUE( f11.is_one() );
}

TEST(ZddTest, product2)
{
  ZddMgr mgr;

  auto zdd10 = mgr.make_set({0});
  auto zdd11 = mgr.make_set({2});
  auto zdd1 = zdd10 | zdd11;
  auto zdd20 = mgr.make_set({1});
  auto zdd21 = mgr.make_set({3});
  auto zdd2 = zdd20 | zdd21;

  auto zdd = zdd1 * zdd2;

  EXPECT_EQ( 4, zdd.count() );

  Zdd f0;
  Zdd f1;
  auto index = zdd.root_decomp(f0, f1);
  EXPECT_EQ( 0, index );

  Zdd f00;
  Zdd f01;
  auto index2 = f0.root_decomp(f00, f01);
  EXPECT_EQ( 1, index2 );

  Zdd f000;
  Zdd f001;
  auto index3 = f00.root_decomp(f000, f001);
  EXPECT_EQ( 2, index3 );
  EXPECT_TRUE( f000.is_zero() );

  Zdd f0010;
  Zdd f0011;
  auto index4 = f001.root_decomp(f0010, f0011);
  EXPECT_EQ( 3, index4 );
  EXPECT_TRUE( f0010.is_zero() );
  EXPECT_TRUE( f0011.is_one() );

  Zdd f010;
  Zdd f011;
  auto index5 = f01.root_decomp(f010, f011);
  EXPECT_EQ( 2, index5 );
  EXPECT_TRUE( f010.is_zero() );
  EXPECT_TRUE( f011.is_one() );

  Zdd f10;
  Zdd f11;
  auto index6 = f1.root_decomp(f10, f11);
  EXPECT_EQ( 1, index6 );
  EXPECT_TRUE( f11.is_one() );

  Zdd f100;
  Zdd f101;
  auto index7 = f10.root_decomp(f100, f101);
  EXPECT_EQ( 3, index7 );
  EXPECT_TRUE( f100.is_zero() );
  EXPECT_TRUE( f101.is_one() );
}

TEST(ZddTest, mgr_copy)
{
  ZddMgr mgr1;

  auto zdd = mgr1.make_set({0});

  auto mgr2 = zdd.mgr();

  EXPECT_EQ( mgr1, mgr2 );
}

END_NAMESPACE_YM
