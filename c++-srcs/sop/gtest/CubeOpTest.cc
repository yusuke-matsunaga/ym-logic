
/// @file SopMgrTest.cc
/// @brief SopMgrTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "SopMgr.h"
#include "SopBlock.h"
#include "ym/Literal.h"


BEGIN_NAMESPACE_YM_SOP

TEST(SopMgrTest, constructor)
{
  const SizeType nv = 10;

  SopMgr mgr{nv};

  EXPECT_EQ( nv, mgr.variable_num() );
}

TEST(SopMgrTest, new_delete)
{
  const SizeType nv = 100;

  SopMgr mgr{nv};

  auto bv = mgr.new_cube();

  mgr.delete_cube(bv);
}

TEST(SopMgrTest, new_cube1)
{
  const SizeType nv = 10;

  SopMgr mgr{nv};

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

  Literal lit1{var0, false};
  Literal lit2{var1, true};
  Literal lit3{var2, false};
  vector<Literal> lit_list{lit1, lit2, lit3};

  auto bv = mgr.new_cube(lit_list);

  EXPECT_EQ( 3, mgr.literal_num(bv, 1) );

  EXPECT_EQ( SopPat::_1, mgr.get_pat(bv, 0, var0) );
  EXPECT_EQ( SopPat::_0, mgr.get_pat(bv, 0, var1) );
  EXPECT_EQ( SopPat::_1, mgr.get_pat(bv, 0, var2) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv, 0, var3) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv, 0, var4) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv, 0, var5) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv, 0, var6) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv, 0, var7) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv, 0, var8) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv, 0, var9) );
}

TEST(SopMgrTest, new_cube2)
{
  const SizeType nv = 10;

  SopMgr mgr{nv};

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

  auto bv = mgr.new_cube({Literal(var0, false), Literal(var1, true), Literal(var2, false)});

  EXPECT_EQ( SopPat::_1, mgr.get_pat(bv, 0, var0) );
  EXPECT_EQ( SopPat::_0, mgr.get_pat(bv, 0, var1) );
  EXPECT_EQ( SopPat::_1, mgr.get_pat(bv, 0, var2) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv, 0, var3) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv, 0, var4) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv, 0, var5) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv, 0, var6) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv, 0, var7) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv, 0, var8) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv, 0, var9) );
}

TEST(SopMgrTest, cube_clear)
{
  const SizeType nv = 10;

  SopMgr mgr{nv};

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

  auto bv = mgr.new_cube({Literal(var0, false), Literal(var1, true), Literal(var2, false)});
  mgr.cube_clear(bv);

  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv, 0, var0) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv, 0, var1) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv, 0, var2) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv, 0, var3) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv, 0, var4) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv, 0, var5) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv, 0, var6) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv, 0, var7) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv, 0, var8) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv, 0, var9) );
}

TEST(SopMgrTest, copy_cube)
{
  const SizeType nv = 10;

  SopMgr mgr{nv};

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

  auto bv1 = mgr.new_cube({Literal(var0, false), Literal(var1, true), Literal(var2, false)});

  auto bv2 = mgr.new_cube();
  mgr.cube_copy(bv2, bv1);

  EXPECT_EQ( SopPat::_1, mgr.get_pat(bv2, 0, var0) );
  EXPECT_EQ( SopPat::_0, mgr.get_pat(bv2, 0, var1) );
  EXPECT_EQ( SopPat::_1, mgr.get_pat(bv2, 0, var2) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv2, 0, var3) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv2, 0, var4) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv2, 0, var5) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv2, 0, var6) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv2, 0, var7) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv2, 0, var8) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv2, 0, var9) );
}

TEST(SopMgrTest, cube_compare)
{
  const SizeType nv = 10;

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

  SopMgr mgr{nv};

  auto bv1 = mgr.new_cube({Literal(var0, false), Literal(var1, true), Literal(var2, false)});
  auto bv2 = mgr.new_cube({Literal(var2, false), Literal(var0, false), Literal(var1, true)});
  EXPECT_EQ( 0, mgr.cube_compare(bv1, bv2) );

  auto bv3 = mgr.new_cube({Literal(var0, true), Literal(var1, true), Literal(var2, false)});

  EXPECT_EQ( -1, mgr.cube_compare(bv1, bv3) );
  EXPECT_EQ(  1, mgr.cube_compare(bv3, bv1) );

  auto bv4 = mgr.new_cube({Literal(var0, false), Literal(var1, true),
      Literal(var2, false), Literal(var9, false)});

  EXPECT_EQ( -1, mgr.cube_compare(bv1, bv4) );
}

TEST(SopMgrTest, cube_check_conflict)
{
  const SizeType nv = 10;

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

  SopMgr mgr{nv};

  auto bv1 = mgr.new_cube({Literal(var0, false), Literal(var1, true), Literal(var2, false)});

  auto bv2 = mgr.new_cube({Literal(var4, false), Literal(var1, true), Literal(var5, false)});

  EXPECT_FALSE( mgr.cube_check_conflict(bv1, bv2) );

  auto bv3 = mgr.new_cube({Literal(var4, false), Literal(var1, false), Literal(var5, false)});

  EXPECT_TRUE( mgr.cube_check_conflict(bv1, bv3) );

}

TEST(SopMgrTest, cube_check_containment)
{
  const SizeType nv = 10;

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

  SopMgr mgr{nv};

  auto bv1 = mgr.new_cube({Literal(var0, false), Literal(var1, true), Literal(var2, false)});

  auto bv2 = mgr.new_cube({Literal(var0, false), Literal(var1, true),
      Literal(var2, false), Literal(var8, false)});

  EXPECT_FALSE( mgr.cube_check_containment(bv1, bv2) );
  EXPECT_TRUE( mgr.cube_check_containment(bv2, bv1) );

  auto bv3 = mgr.new_cube({Literal(var0, false), Literal(var2, true), Literal(var5, false)});

  EXPECT_FALSE( mgr.cube_check_containment(bv1, bv3) );
  EXPECT_FALSE( mgr.cube_check_containment(bv3, bv1) );

}

TEST(SopMgrTest, cube_check_intersect)
{
  const SizeType nv = 10;

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

  SopMgr mgr{nv};

  auto bv1 = mgr.new_cube({Literal(var0, false), Literal(var1, true), Literal(var2, false)});

  auto bv2 = mgr.new_cube({Literal(var0, false), Literal(var1, true),
      Literal(var2, false), Literal(var8, false)});

  EXPECT_TRUE( mgr.cube_check_intersect(bv1, bv2) );

  auto bv3 = mgr.new_cube({Literal(var0, true), Literal(var2, true), Literal(var5, false)});

  EXPECT_FALSE( mgr.cube_check_intersect(bv1, bv3) );

}

TEST(SopMgrTest, cube_product)
{
  const SizeType nv = 10;

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

  SopMgr mgr{nv};

  auto bv1 = mgr.new_cube({Literal(var0, false), Literal(var2, true)});

  auto bv2 = mgr.new_cube({Literal(var1, true), Literal(var4, false)});

  auto bv3 = mgr.new_cube();
  bool stat1 = mgr.cube_product(bv3, bv1, bv2);
  EXPECT_TRUE( stat1 );

  EXPECT_EQ( SopPat::_1, mgr.get_pat(bv3, 0, var0) );
  EXPECT_EQ( SopPat::_0, mgr.get_pat(bv3, 0, var1) );
  EXPECT_EQ( SopPat::_0, mgr.get_pat(bv3, 0, var2) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var3) );
  EXPECT_EQ( SopPat::_1, mgr.get_pat(bv3, 0, var4) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var5) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var6) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var7) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var8) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var9) );

  auto bv4 = mgr.new_cube({Literal(var0, true), Literal(var4, false)});

  auto bv5 = mgr.new_cube();
  bool stat2 = mgr.cube_product(bv5, bv1, bv4);
  EXPECT_FALSE( stat2 );

  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv5, 0, var0) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv5, 0, var1) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv5, 0, var2) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv5, 0, var3) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv5, 0, var4) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv5, 0, var5) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv5, 0, var6) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv5, 0, var7) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv5, 0, var8) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv5, 0, var9) );
}

TEST(SopMgrTest, cube_quotient)
{
  const SizeType nv = 10;

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

  SopMgr mgr{nv};

  auto bv1 = mgr.new_cube({Literal(var0, false), Literal(var1, true), Literal(var2, true)});

  auto bv2 = mgr.new_cube({Literal(var1, true)});

  auto bv3 = mgr.new_cube();
  bool stat1 = mgr.cube_quotient(bv3, bv1, bv2);
  EXPECT_TRUE( stat1 );

  EXPECT_EQ( SopPat::_1, mgr.get_pat(bv3, 0, var0) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var1) );
  EXPECT_EQ( SopPat::_0, mgr.get_pat(bv3, 0, var2) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var3) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var4) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var5) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var6) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var7) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var8) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var9) );

  auto bv4 = mgr.new_cube({Literal(var1, true), Literal(var3, false)});

  auto bv5 = mgr.new_cube();
  bool stat2 = mgr.cube_quotient(bv5, bv1, bv4);
  EXPECT_FALSE( stat2 );

  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv5, 0, var0) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv5, 0, var1) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv5, 0, var2) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv5, 0, var3) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv5, 0, var4) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv5, 0, var5) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv5, 0, var6) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv5, 0, var7) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv5, 0, var8) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv5, 0, var9) );
}

TEST(SopMgrTest, cube_swap)
{
  const SizeType nv = 10;

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

  SopMgr mgr{nv};

  auto bv1 = mgr.new_cube({Literal(var0, false), Literal(var1, false), Literal(var2, true)});

  auto bv2 = mgr.new_cube({Literal(var1, true)});

  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv2, 0, var0) );
  EXPECT_EQ( SopPat::_0, mgr.get_pat(bv2, 0, var1) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv2, 0, var2) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv2, 0, var3) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv2, 0, var4) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv2, 0, var5) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv2, 0, var6) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv2, 0, var7) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv2, 0, var8) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv2, 0, var9) );

  EXPECT_EQ( SopPat::_1, mgr.get_pat(bv1, 0, var0) );
  EXPECT_EQ( SopPat::_1, mgr.get_pat(bv1, 0, var1) );
  EXPECT_EQ( SopPat::_0, mgr.get_pat(bv1, 0, var2) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 0, var3) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 0, var4) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 0, var5) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 0, var6) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 0, var7) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 0, var8) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 0, var9) );

  mgr.cube_swap(bv1, bv2);

  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 0, var0) );
  EXPECT_EQ( SopPat::_0, mgr.get_pat(bv1, 0, var1) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 0, var2) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 0, var3) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 0, var4) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 0, var5) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 0, var6) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 0, var7) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 0, var8) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 0, var9) );

  EXPECT_EQ( SopPat::_1, mgr.get_pat(bv2, 0, var0) );
  EXPECT_EQ( SopPat::_1, mgr.get_pat(bv2, 0, var1) );
  EXPECT_EQ( SopPat::_0, mgr.get_pat(bv2, 0, var2) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv2, 0, var3) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv2, 0, var4) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv2, 0, var5) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv2, 0, var6) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv2, 0, var7) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv2, 0, var8) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv2, 0, var9) );

}

TEST(SopMgrTest, cube_rotate3)
{
  const SizeType nv = 10;

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

  SopMgr mgr{nv};

  auto bv1 = mgr.new_cube({Literal(var0, false)});

  auto bv2 = mgr.new_cube({Literal(var1, false)});

  auto bv3 = mgr.new_cube({Literal(var2, false)});

  mgr.cube_rotate3(bv1, bv2, bv3);

  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 0, var0) );
  EXPECT_EQ( SopPat::_1, mgr.get_pat(bv1, 0, var1) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 0, var2) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 0, var3) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 0, var4) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 0, var5) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 0, var6) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 0, var7) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 0, var8) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 0, var9) );

  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv2, 0, var0) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv2, 0, var1) );
  EXPECT_EQ( SopPat::_1, mgr.get_pat(bv2, 0, var2) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv2, 0, var3) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv2, 0, var4) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv2, 0, var5) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv2, 0, var6) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv2, 0, var7) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv2, 0, var8) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv2, 0, var9) );

  EXPECT_EQ( SopPat::_1, mgr.get_pat(bv3, 0, var0) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var1) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var2) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var3) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var4) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var5) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var6) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var7) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var8) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var9) );

}

TEST(SopMgrTest, cube_rotate4)
{
  const SizeType nv = 10;

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

  SopMgr mgr{nv};

  auto bv1 = mgr.new_cube({Literal(var0, false)});

  auto bv2 = mgr.new_cube({Literal(var1, false)});

  auto bv3 = mgr.new_cube({Literal(var2, false)});

  auto bv4 = mgr.new_cube({Literal(var3, false)});

  mgr.cube_rotate4(bv1, bv2, bv3, bv4);

  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 0, var0) );
  EXPECT_EQ( SopPat::_1, mgr.get_pat(bv1, 0, var1) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 0, var2) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 0, var3) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 0, var4) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 0, var5) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 0, var6) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 0, var7) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 0, var8) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 0, var9) );

  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv2, 0, var0) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv2, 0, var1) );
  EXPECT_EQ( SopPat::_1, mgr.get_pat(bv2, 0, var2) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv2, 0, var3) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv2, 0, var4) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv2, 0, var5) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv2, 0, var6) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv2, 0, var7) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv2, 0, var8) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv2, 0, var9) );

  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var0) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var1) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var2) );
  EXPECT_EQ( SopPat::_1, mgr.get_pat(bv3, 0, var3) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var4) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var5) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var6) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var7) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var8) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var9) );

  EXPECT_EQ( SopPat::_1, mgr.get_pat(bv4, 0, var0) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv4, 0, var1) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv4, 0, var2) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv4, 0, var3) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv4, 0, var4) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv4, 0, var5) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv4, 0, var6) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv4, 0, var7) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv4, 0, var8) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv4, 0, var9) );

}

TEST(SopMgrTest, new_cover1)
{
  const SizeType nv = 10;

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

  SopMgr mgr{nv};

  Literal lit0{var0, false};
  Literal lit1{var1, true};
  auto block1 = mgr.new_cover({{lit0}, {lit1}});
  EXPECT_EQ( 2, block1.cube_num);
  EXPECT_EQ( 2, mgr.literal_num(block1) );

  auto bv1 = block1.body;

  EXPECT_EQ( SopPat::_1, mgr.get_pat(bv1, 0, var0) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 0, var1) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 0, var2) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 0, var3) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 0, var4) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 0, var5) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 0, var6) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 0, var7) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 0, var8) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 0, var9) );

  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 1, var0) );
  EXPECT_EQ( SopPat::_0, mgr.get_pat(bv1, 1, var1) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 1, var2) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 1, var3) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 1, var4) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 1, var5) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 1, var6) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 1, var7) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 1, var8) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 1, var9) );
}

TEST(SopMgrTest, new_cover2)
{
  const SizeType nv = 10;

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

  SopMgr mgr{nv};

  auto block1 = mgr.new_cover({{Literal(var1, false)}, {Literal(var0, true)}});
  EXPECT_EQ( 2, block1.cube_num );
  EXPECT_EQ( 2, mgr.literal_num(block1) );

  auto bv1 = block1.body;

  EXPECT_EQ( SopPat::_0, mgr.get_pat(bv1, 0, var0) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 0, var1) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 0, var2) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 0, var3) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 0, var4) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 0, var5) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 0, var6) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 0, var7) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 0, var8) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 0, var9) );

  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 1, var0) );
  EXPECT_EQ( SopPat::_1, mgr.get_pat(bv1, 1, var1) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 1, var2) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 1, var3) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 1, var4) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 1, var5) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 1, var6) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 1, var7) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 1, var8) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv1, 1, var9) );
}

TEST(SopMgrTest, sum1)
{
  const SizeType nv = 10;

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

  SopMgr mgr{nv};

  auto bv1 = mgr.new_cube({Literal(var0, false)});

  auto bv2 = mgr.new_cube({Literal(var1, false)});

  auto block1 = mgr.cover_sum(SopBlock{1, bv1}, SopBlock{1, bv2});
  EXPECT_EQ( 2, block1.cube_num );
  auto bv3 = block1.body;

  EXPECT_EQ( SopPat::_1, mgr.get_pat(bv3, 0, var0) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var1) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var2) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var3) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var4) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var5) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var6) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var7) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var8) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var9) );

  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 1, var0) );
  EXPECT_EQ( SopPat::_1, mgr.get_pat(bv3, 1, var1) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 1, var2) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 1, var3) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 1, var4) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 1, var5) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 1, var6) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 1, var7) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 1, var8) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 1, var9) );

}

#if 0
TEST(SopMgrTest, sum2)
{
  const SizeType nv = 10;

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

  SopMgr mgr{nv};

  SopBitVect* bv1 = mgr.new_body(2);
  mgr.cover_set(bv1, {{Literal(var0, false)}, {Literal(var1, true)}});

  SopBitVect* bv2 = mgr.new_body(1);
  mgr.cube_set(bv2, {Literal(var0, false)});

  SopBitVect* bv3 = mgr.new_body(2);
  int nc = mgr.cover_sum(bv3, SopBlock{2, bv1}, SopBlock{1, bv2});

  EXPECT_EQ( 2, nc );

  EXPECT_EQ( SopPat::_1, mgr.get_pat(bv3, 0, var0) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var1) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var2) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var3) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var4) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var5) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var6) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var7) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var8) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var9) );

  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 1, var0) );
  EXPECT_EQ( SopPat::_0, mgr.get_pat(bv3, 1, var1) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 1, var2) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 1, var3) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 1, var4) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 1, var5) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 1, var6) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 1, var7) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 1, var8) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 1, var9) );
}

TEST(SopMgrTest, diff)
{
  const SizeType nv = 10;

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

  SopMgr mgr{nv};

  SopBitVect* bv1 = mgr.new_body(2);
  mgr.cover_set(bv1, {{Literal(var0, false)}, {Literal(var1, true)}});

  SopBitVect* bv2 = mgr.new_body(2);
  mgr.cover_set(bv2, {{Literal(var0, false)}, {Literal(var2, true)}});

  SopBitVect* bv3 = mgr.new_body(2);
  int nc = mgr.cover_diff(bv3, SopBlock{2, bv1}, SopBlock{2, bv2});
  EXPECT_EQ( 1, nc );

  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var0) );
  EXPECT_EQ( SopPat::_0, mgr.get_pat(bv3, 0, var1) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var2) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var3) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var4) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var5) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var6) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var7) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var8) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var9) );
}

TEST(SopMgrTest, product1)
{
  const SizeType nv = 10;

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

  SopMgr mgr{nv};

  SopBitVect* bv1 = mgr.new_body(2);
  mgr.cover_set(bv1, {{Literal(var0, false)}, {Literal(var1, true)}});

  SopBitVect* bv2 = mgr.new_body(2);
  mgr.cover_set(bv2, {{Literal(var2, false)}, {Literal(var3, true)}});

  SopBitVect* bv3 = mgr.new_body(4);
  int nc = mgr.cover_product(bv3, SopBlock{2, bv1}, SopBlock{2, bv2});
  EXPECT_EQ( 4, nc );

  EXPECT_EQ( SopPat::_1, mgr.get_pat(bv3, 0, var0) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var1) );
  EXPECT_EQ( SopPat::_1, mgr.get_pat(bv3, 0, var2) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var3) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var4) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var5) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var6) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var7) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var8) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var9) );

  EXPECT_EQ( SopPat::_1, mgr.get_pat(bv3, 1, var0) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 1, var1) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 1, var2) );
  EXPECT_EQ( SopPat::_0, mgr.get_pat(bv3, 1, var3) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 1, var4) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 1, var5) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 1, var6) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 1, var7) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 1, var8) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 1, var9) );

  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 2, var0) );
  EXPECT_EQ( SopPat::_0, mgr.get_pat(bv3, 2, var1) );
  EXPECT_EQ( SopPat::_1, mgr.get_pat(bv3, 2, var2) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 2, var3) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 2, var4) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 2, var5) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 2, var6) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 2, var7) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 2, var8) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 2, var9) );

  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 3, var0) );
  EXPECT_EQ( SopPat::_0, mgr.get_pat(bv3, 3, var1) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 3, var2) );
  EXPECT_EQ( SopPat::_0, mgr.get_pat(bv3, 3, var3) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 3, var4) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 3, var5) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 3, var6) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 3, var7) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 3, var8) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 3, var9) );
}

TEST(SopMgrTest, product2)
{
  const SizeType nv = 10;

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

  SopMgr mgr{nv};

  SopBitVect* bv1 = mgr.new_body(2);
  mgr.cover_set(bv1, {{Literal(var0, false)}, {Literal(var1, true)}});

  SopBitVect* bv2 = mgr.new_body(2);
  mgr.cover_set(bv2, {{Literal(var0, true)}, {Literal(var3, true)}});

  SopBitVect* bv3 = mgr.new_body(4);
  int nc = mgr.cover_product(bv3, SopBlock{2, bv1}, SopBlock{2, bv2});
  EXPECT_EQ( 3, nc );

  EXPECT_EQ( SopPat::_0, mgr.get_pat(bv3, 0, var0) );
  EXPECT_EQ( SopPat::_0, mgr.get_pat(bv3, 0, var1) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var2) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var3) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var4) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var5) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var6) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var7) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var8) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var9) );

  EXPECT_EQ( SopPat::_1, mgr.get_pat(bv3, 1, var0) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 1, var1) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 1, var2) );
  EXPECT_EQ( SopPat::_0, mgr.get_pat(bv3, 1, var3) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 1, var4) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 1, var5) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 1, var6) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 1, var7) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 1, var8) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 1, var9) );

  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 2, var0) );
  EXPECT_EQ( SopPat::_0, mgr.get_pat(bv3, 2, var1) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 2, var2) );
  EXPECT_EQ( SopPat::_0, mgr.get_pat(bv3, 2, var3) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 2, var4) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 2, var5) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 2, var6) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 2, var7) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 2, var8) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 2, var9) );
}

TEST(SopMgrTest, product3)
{
  const SizeType nv = 10;

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

  SopMgr mgr{nv};

  SopBitVect* bv1 = mgr.new_body(2);
  mgr.cover_set(bv1, {{Literal(var0, false)}, {Literal(var1, true)}});

  SopBitVect* bv3 = mgr.new_body(2);
  int nc = mgr.cover_product(bv3, SopBlock{2, bv1}, Literal(var1, false));
  EXPECT_EQ( 1, nc );

  EXPECT_EQ( SopPat::_1, mgr.get_pat(bv3, 0, var0) );
  EXPECT_EQ( SopPat::_1, mgr.get_pat(bv3, 0, var1) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var2) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var3) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var4) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var5) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var6) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var7) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var8) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var9) );
}

TEST(SopMgrTest, quotient1)
{
  const SizeType nv = 10;

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

  SopMgr mgr{nv};

  std::initializer_list<std::initializer_list<Literal>> cube_list1
    {
     { Literal{var0, false}, Literal{var2, false} },
     { Literal{var0, false}, Literal{var3, true}  },
     { Literal{var1, true},  Literal{var2, false} },
     { Literal{var1, true},  Literal{var3, true}  }
    };
  SizeType nc1 = cube_list1.size();
  SopBitVect* bv1 = mgr.new_body(nc1);
  mgr.cover_set(bv1, cube_list1);
  SopBlock block1{nc1, bv1};

  std::initializer_list<std::initializer_list<Literal>> cube_list2
    {
     { Literal{var2, false} },
     { Literal{var3, true} }
    };
  SizeType nc2 = cube_list2.size();
  SopBitVect* bv2 = mgr.new_body(cube_list2.size());
  mgr.cover_set(bv2, cube_list2);
  SopBlock block2{nc2, bv2};

  SopBitVect* bv3 = mgr.new_body(2);
  SizeType nc = mgr.cover_quotient(bv3, block1, block2);
  EXPECT_EQ( 2, nc );

  EXPECT_EQ( SopPat::_1, mgr.get_pat(bv3, 0, var0) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var1) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var2) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var3) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var4) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var5) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var6) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var7) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var8) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var9) );

  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 1, var0) );
  EXPECT_EQ( SopPat::_0, mgr.get_pat(bv3, 1, var1) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 1, var2) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 1, var3) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 1, var4) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 1, var5) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 1, var6) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 1, var7) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 1, var8) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 1, var9) );
}

TEST(SopMgrTest, quotient2)
{
  const SizeType nv = 10;

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

  SopMgr mgr{nv};

  std::initializer_list<std::initializer_list<Literal>> cube_list1
    {
     { Literal{var0, false}, Literal{var2, false} },
     { Literal{var0, false}, Literal{var3, true}  },
     { Literal{var1, true},  Literal{var2, false} },
     { Literal{var1, true},  Literal{var3, true}  }
    };
  SizeType nc1 = cube_list1.size();
  SopBitVect* bv1 = mgr.new_body(nc1);
  mgr.cover_set(bv1, cube_list1);
  SopBlock block1{nc1, bv1};

  SopBitVect* bv3 = mgr.new_body(2);
  SizeType nc = mgr.cover_quotient(bv3, block1, Literal{var1, true});
  EXPECT_EQ( 2, nc );

  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var0) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var1) );
  EXPECT_EQ( SopPat::_1, mgr.get_pat(bv3, 0, var2) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var3) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var4) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var5) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var6) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var7) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var8) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 0, var9) );

  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 1, var0) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 1, var1) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 1, var2) );
  EXPECT_EQ( SopPat::_0, mgr.get_pat(bv3, 1, var3) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 1, var4) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 1, var5) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 1, var6) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 1, var7) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 1, var8) );
  EXPECT_EQ( SopPat::_X, mgr.get_pat(bv3, 1, var9) );
}

TEST(SopMgrTest, common_cube)
{
  const SizeType nv = 10;

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

  SopMgr mgr{nv};

  std::initializer_list<std::initializer_list<Literal>> cube_list1
    {
     { Literal{var0, false}, Literal{var2, false}, Literal{var4, false} },
     { Literal{var0, false}, Literal{var3, true}  },
    };
  SizeType nc1 = cube_list1.size();
  SopBitVect* bv1 = mgr.new_body(nc1);
  mgr.cover_set(bv1, cube_list1);
  SopBlock block1{nc1, bv1};

  SopCube ccube = mgr.common_cube(block1);

  EXPECT_EQ( SopPat::_1, ccube.get_pat(var0) );
  EXPECT_EQ( SopPat::_X, ccube.get_pat(var1) );
  EXPECT_EQ( SopPat::_X, ccube.get_pat(var2) );
  EXPECT_EQ( SopPat::_X, ccube.get_pat(var3) );
  EXPECT_EQ( SopPat::_X, ccube.get_pat(var4) );
  EXPECT_EQ( SopPat::_X, ccube.get_pat(var5) );
  EXPECT_EQ( SopPat::_X, ccube.get_pat(var6) );
  EXPECT_EQ( SopPat::_X, ccube.get_pat(var7) );
  EXPECT_EQ( SopPat::_X, ccube.get_pat(var8) );
  EXPECT_EQ( SopPat::_X, ccube.get_pat(var9) );
}

TEST(SopMgrTest, cover_compare1)
{
  const SizeType nv = 10;

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

  SopMgr mgr{nv};

  std::initializer_list<std::initializer_list<Literal>> cube_list1
    {
     { Literal{var0, false}, Literal{var2, false} },
     { Literal{var0, false}, Literal{var3, true}  },
     { Literal{var1, true},  Literal{var2, false} },
     { Literal{var1, true},  Literal{var3, true}  }
    };
  SizeType nc1 = cube_list1.size();
  SopBitVect* bv1 = mgr.new_body(nc1);
  mgr.cover_set(bv1, cube_list1);
  SopBlock block1{nc1, bv1};

  std::initializer_list<std::initializer_list<Literal>> cube_list2
    {
     { Literal{var0, false}, Literal{var2, false} },
     { Literal{var1, true},  Literal{var3, true}  },
     { Literal{var1, true},  Literal{var2, false} },
     { Literal{var0, false}, Literal{var3, true}  }
    };
  SizeType nc2 = cube_list2.size();
  SopBitVect* bv2 = mgr.new_body(nc2);
  mgr.cover_set(bv2, cube_list2);
  SopBlock block2{nc2, bv2};

  int stat = mgr.cover_compare(block1, block2);
  EXPECT_EQ( 0, stat );
}

TEST(SopMgrTest, cover_compare2)
{
  const SizeType nv = 10;

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

  SopMgr mgr{nv};

  std::initializer_list<std::initializer_list<Literal>> cube_list1
    {
     { Literal{var0, false}, Literal{var2, false} },
     { Literal{var0, false}, Literal{var3, true}  },
     { Literal{var1, true},  Literal{var2, false} },
     { Literal{var1, true},  Literal{var3, true}  }
    };
  SizeType nc1 = cube_list1.size();
  SopBitVect* bv1 = mgr.new_body(nc1);
  mgr.cover_set(bv1, cube_list1);
  SopBlock block1{nc1, bv1};

  std::initializer_list<std::initializer_list<Literal>> cube_list2
    {
     { Literal{var0, false}, Literal{var2, false} },
     { Literal{var1, true},  Literal{var3, true}  },
     { Literal{var1, true},  Literal{var2, false} }
    };
  SizeType nc2 = cube_list2.size();
  SopBitVect* bv2 = mgr.new_body(nc2);
  mgr.cover_set(bv2, cube_list2);
  SopBlock block2{nc2, bv2};

  int stat1 = mgr.cover_compare(block1, block2);
  EXPECT_EQ( 1, stat1 );

  int stat2 = mgr.cover_compare(block2, block1);
  EXPECT_EQ( -1, stat2 );
}

TEST(SopMgrTest, cover_compare3)
{
  const SizeType nv = 10;

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

  SopMgr mgr{nv};

  std::initializer_list<std::initializer_list<Literal>> cube_list1
    {
     { Literal{var0, false}, Literal{var2, false} },
     { Literal{var0, false}, Literal{var3, true}  },
     { Literal{var1, true},  Literal{var2, false} },
     { Literal{var1, true},  Literal{var3, true}  }
    };
  auto block1 = mgr.new_cover(cube_list1);

  std::initializer_list<std::initializer_list<Literal>> cube_list2
    {
     { Literal{var0, false}, Literal{var2, false} },
     { Literal{var1, true},  Literal{var3, true}  },
     { Literal{var1, true},  Literal{var2, false} },
     { Literal{var0, false}, Literal{var4, true}  }
    };
  auto block2 = mgr.new_cover(cube_list2);


  int stat1 = mgr.cover_compare(block1, block2);
  EXPECT_EQ( 1, stat1 );

  int stat2 = mgr.cover_compare(block2, block1);
  EXPECT_EQ( -1, stat2 );
}
#endif

END_NAMESPACE_YM_SOP
