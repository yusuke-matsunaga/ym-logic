
/// @file AlgMgrTest.cc
/// @brief AlgMgrTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "AlgMgr.h"
#include "AlgBlock.h"
#include "ym/VarId.h"
#include "ym/Literal.h"


BEGIN_NAMESPACE_YM_LOGIC

TEST(AlgMgrTest, constructor)
{
  const int nv = 10;

  AlgMgr mgr(nv);

  EXPECT_EQ( nv, mgr.variable_num() );
}

TEST(AlgMgrTest, new_delete)
{
  const int nv = 100;

  AlgMgr mgr(nv);

  AlgBitVect* bv = mgr.new_body(10);

  mgr.delete_body(bv, 10);
}

TEST(AlgMgrTest, cube_set1)
{
  const int nv = 10;

  AlgMgr mgr(nv);

  AlgBitVect* bv = mgr.new_body(1);

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

  vector<Literal> lit_list{Literal(var0, false), Literal(var1, true), Literal(var2, false)};

  mgr.cube_set(bv, lit_list);

  EXPECT_EQ( AlgPol::P, mgr.get_pol(bv, 0, var0) );
  EXPECT_EQ( AlgPol::N, mgr.get_pol(bv, 0, var1) );
  EXPECT_EQ( AlgPol::P, mgr.get_pol(bv, 0, var2) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv, 0, var3) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv, 0, var4) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv, 0, var5) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv, 0, var6) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv, 0, var7) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv, 0, var8) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv, 0, var9) );
}

TEST(AlgMgrTest, cube_set2)
{
  const int nv = 10;

  AlgMgr mgr(nv);

  AlgBitVect* bv = mgr.new_body(1);

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

  mgr.cube_set(bv, {Literal(var0, false), Literal(var1, true), Literal(var2, false)});

  EXPECT_EQ( AlgPol::P, mgr.get_pol(bv, 0, var0) );
  EXPECT_EQ( AlgPol::N, mgr.get_pol(bv, 0, var1) );
  EXPECT_EQ( AlgPol::P, mgr.get_pol(bv, 0, var2) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv, 0, var3) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv, 0, var4) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv, 0, var5) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv, 0, var6) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv, 0, var7) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv, 0, var8) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv, 0, var9) );
}

TEST(AlgMgrTest, cube_clear)
{
  const int nv = 10;

  AlgMgr mgr(nv);

  AlgBitVect* bv = mgr.new_body(1);

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

  mgr.cube_set(bv, {Literal(var0, false), Literal(var1, true), Literal(var2, false)});
  mgr.cube_clear(bv);

  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv, 0, var0) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv, 0, var1) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv, 0, var2) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv, 0, var3) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv, 0, var4) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv, 0, var5) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv, 0, var6) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv, 0, var7) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv, 0, var8) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv, 0, var9) );
}

TEST(AlgMgrTest, cube_compare)
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

  AlgMgr mgr(nv);

  AlgBitVect* bv1 = mgr.new_body(1);
  mgr.cube_set(bv1, {Literal(var0, false), Literal(var1, true), Literal(var2, false)});

  AlgBitVect* bv2 = mgr.new_body(1);
  mgr.cube_set(bv2, {Literal(var2, false), Literal(var0, false), Literal(var1, true)});

  EXPECT_EQ( 0, mgr.cube_compare(bv1, bv2) );

  AlgBitVect* bv3 = mgr.new_body(1);
  mgr.cube_set(bv3, {Literal(var0, true), Literal(var1, true), Literal(var2, false)});

  EXPECT_EQ( -1, mgr.cube_compare(bv1, bv3) );
  EXPECT_EQ(  1, mgr.cube_compare(bv3, bv1) );

  AlgBitVect* bv4 = mgr.new_body(1);
  mgr.cube_set(bv4, {Literal(var0, false), Literal(var1, true),
			       Literal(var2, false), Literal(var9, false)});

  EXPECT_EQ( -1, mgr.cube_compare(bv1, bv4) );
}

TEST(AlgMgrTest, cube_check_conflict)
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

  AlgMgr mgr(nv);

  AlgBitVect* bv1 = mgr.new_body(1);
  mgr.cube_set(bv1, {Literal(var0, false), Literal(var1, true), Literal(var2, false)});

  AlgBitVect* bv2 = mgr.new_body(1);
  mgr.cube_set(bv2, {Literal(var4, false), Literal(var1, true), Literal(var5, false)});

  EXPECT_FALSE( mgr.cube_check_conflict(bv1, bv2) );

  AlgBitVect* bv3 = mgr.new_body(1);
  mgr.cube_set(bv3, {Literal(var4, false), Literal(var1, false), Literal(var5, false)});

  EXPECT_TRUE( mgr.cube_check_conflict(bv1, bv3) );

}

TEST(AlgMgrTest, cube_check_containment)
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

  AlgMgr mgr(nv);

  AlgBitVect* bv1 = mgr.new_body(1);
  mgr.cube_set(bv1, {Literal(var0, false), Literal(var1, true), Literal(var2, false)});

  AlgBitVect* bv2 = mgr.new_body(1);
  mgr.cube_set(bv2, {Literal(var0, false), Literal(var1, true),
			       Literal(var2, false), Literal(var8, false)});

  EXPECT_FALSE( mgr.cube_check_containment(bv1, bv2) );
  EXPECT_TRUE( mgr.cube_check_containment(bv2, bv1) );

  AlgBitVect* bv3 = mgr.new_body(1);
  mgr.cube_set(bv3, {Literal(var0, false), Literal(var2, true), Literal(var5, false)});

  EXPECT_FALSE( mgr.cube_check_containment(bv1, bv3) );
  EXPECT_FALSE( mgr.cube_check_containment(bv3, bv1) );

}

TEST(AlgMgrTest, cube_check_intersect)
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

  AlgMgr mgr(nv);

  AlgBitVect* bv1 = mgr.new_body(1);
  mgr.cube_set(bv1, {Literal(var0, false), Literal(var1, true), Literal(var2, false)});

  AlgBitVect* bv2 = mgr.new_body(1);
  mgr.cube_set(bv2, {Literal(var0, false), Literal(var1, true),
			       Literal(var2, false), Literal(var8, false)});

  EXPECT_TRUE( mgr.cube_check_intersect(bv1, bv2) );

  AlgBitVect* bv3 = mgr.new_body(1);
  mgr.cube_set(bv3, {Literal(var0, true), Literal(var2, true), Literal(var5, false)});

  EXPECT_FALSE( mgr.cube_check_intersect(bv1, bv3) );

}

TEST(AlgMgrTest, cube_product)
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

  AlgMgr mgr(nv);

  AlgBitVect* bv1 = mgr.new_body(1);
  mgr.cube_set(bv1, {Literal(var0, false), Literal(var2, true)});

  AlgBitVect* bv2 = mgr.new_body(1);
  mgr.cube_set(bv2, {Literal(var1, true), Literal(var4, false)});

  AlgBitVect* bv3 = mgr.new_body(1);
  bool stat1 = mgr.cube_product(bv3, bv1, bv2);
  EXPECT_TRUE( stat1 );

  EXPECT_EQ( AlgPol::P, mgr.get_pol(bv3, 0, var0) );
  EXPECT_EQ( AlgPol::N, mgr.get_pol(bv3, 0, var1) );
  EXPECT_EQ( AlgPol::N, mgr.get_pol(bv3, 0, var2) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var3) );
  EXPECT_EQ( AlgPol::P, mgr.get_pol(bv3, 0, var4) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var5) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var6) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var7) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var8) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var9) );

  AlgBitVect* bv4 = mgr.new_body(1);
  mgr.cube_set(bv4, {Literal(var0, true), Literal(var4, false)});

  AlgBitVect* bv5 = mgr.new_body(1);
  bool stat2 = mgr.cube_product(bv5, bv1, bv4);
  EXPECT_FALSE( stat2 );

  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv5, 0, var0) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv5, 0, var1) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv5, 0, var2) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv5, 0, var3) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv5, 0, var4) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv5, 0, var5) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv5, 0, var6) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv5, 0, var7) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv5, 0, var8) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv5, 0, var9) );
}

TEST(AlgMgrTest, cube_quotient)
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

  AlgMgr mgr(nv);

  AlgBitVect* bv1 = mgr.new_body(1);
  mgr.cube_set(bv1, {Literal(var0, false), Literal(var1, true), Literal(var2, true)});

  AlgBitVect* bv2 = mgr.new_body(1);
  mgr.cube_set(bv2, {Literal(var1, true)});

  AlgBitVect* bv3 = mgr.new_body(1);
  bool stat1 = mgr.cube_quotient(bv3, bv1, bv2);
  EXPECT_TRUE( stat1 );

  EXPECT_EQ( AlgPol::P, mgr.get_pol(bv3, 0, var0) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var1) );
  EXPECT_EQ( AlgPol::N, mgr.get_pol(bv3, 0, var2) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var3) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var4) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var5) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var6) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var7) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var8) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var9) );

  AlgBitVect* bv4 = mgr.new_body(1);
  mgr.cube_set(bv4, {Literal(var1, true), Literal(var3, false)});

  AlgBitVect* bv5 = mgr.new_body(1);
  bool stat2 = mgr.cube_quotient(bv5, bv1, bv4);
  EXPECT_FALSE( stat2 );

  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv5, 0, var0) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv5, 0, var1) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv5, 0, var2) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv5, 0, var3) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv5, 0, var4) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv5, 0, var5) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv5, 0, var6) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv5, 0, var7) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv5, 0, var8) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv5, 0, var9) );
}

TEST(AlgMgrTest, cube_swap)
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

  AlgMgr mgr(nv);

  AlgBitVect* bv1 = mgr.new_body(1);
  mgr.cube_set(bv1, {Literal(var0, false), Literal(var1, false), Literal(var2, true)});

  AlgBitVect* bv2 = mgr.new_body(1);
  mgr.cube_set(bv2, {Literal(var1, true)});

  mgr.cube_swap(bv1, bv2);

  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv1, 0, var0) );
  EXPECT_EQ( AlgPol::N, mgr.get_pol(bv1, 0, var1) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv1, 0, var2) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv1, 0, var3) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv1, 0, var4) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv1, 0, var5) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv1, 0, var6) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv1, 0, var7) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv1, 0, var8) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv1, 0, var9) );

  EXPECT_EQ( AlgPol::P, mgr.get_pol(bv2, 0, var0) );
  EXPECT_EQ( AlgPol::P, mgr.get_pol(bv2, 0, var1) );
  EXPECT_EQ( AlgPol::N, mgr.get_pol(bv2, 0, var2) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv2, 0, var3) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv2, 0, var4) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv2, 0, var5) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv2, 0, var6) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv2, 0, var7) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv2, 0, var8) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv2, 0, var9) );

}

TEST(AlgMgrTest, cube_rotate3)
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

  AlgMgr mgr(nv);

  AlgBitVect* bv1 = mgr.new_body(1);
  mgr.cube_set(bv1, {Literal(var0, false)});

  AlgBitVect* bv2 = mgr.new_body(1);
  mgr.cube_set(bv2, {Literal(var1, false)});

  AlgBitVect* bv3 = mgr.new_body(1);
  mgr.cube_set(bv3, {Literal(var2, false)});

  mgr.cube_rotate3(bv1, bv2, bv3);

  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv1, 0, var0) );
  EXPECT_EQ( AlgPol::P, mgr.get_pol(bv1, 0, var1) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv1, 0, var2) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv1, 0, var3) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv1, 0, var4) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv1, 0, var5) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv1, 0, var6) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv1, 0, var7) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv1, 0, var8) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv1, 0, var9) );

  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv2, 0, var0) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv2, 0, var1) );
  EXPECT_EQ( AlgPol::P, mgr.get_pol(bv2, 0, var2) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv2, 0, var3) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv2, 0, var4) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv2, 0, var5) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv2, 0, var6) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv2, 0, var7) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv2, 0, var8) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv2, 0, var9) );

  EXPECT_EQ( AlgPol::P, mgr.get_pol(bv3, 0, var0) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var1) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var2) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var3) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var4) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var5) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var6) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var7) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var8) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var9) );

}

TEST(AlgMgrTest, cube_rotate4)
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

  AlgMgr mgr(nv);

  AlgBitVect* bv1 = mgr.new_body(1);
  mgr.cube_set(bv1, {Literal(var0, false)});

  AlgBitVect* bv2 = mgr.new_body(1);
  mgr.cube_set(bv2, {Literal(var1, false)});

  AlgBitVect* bv3 = mgr.new_body(1);
  mgr.cube_set(bv3, {Literal(var2, false)});

  AlgBitVect* bv4 = mgr.new_body(1);
  mgr.cube_set(bv4, {Literal(var3, false)});

  mgr.cube_rotate4(bv1, bv2, bv3, bv4);

  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv1, 0, var0) );
  EXPECT_EQ( AlgPol::P, mgr.get_pol(bv1, 0, var1) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv1, 0, var2) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv1, 0, var3) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv1, 0, var4) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv1, 0, var5) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv1, 0, var6) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv1, 0, var7) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv1, 0, var8) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv1, 0, var9) );

  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv2, 0, var0) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv2, 0, var1) );
  EXPECT_EQ( AlgPol::P, mgr.get_pol(bv2, 0, var2) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv2, 0, var3) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv2, 0, var4) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv2, 0, var5) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv2, 0, var6) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv2, 0, var7) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv2, 0, var8) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv2, 0, var9) );

  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var0) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var1) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var2) );
  EXPECT_EQ( AlgPol::P, mgr.get_pol(bv3, 0, var3) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var4) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var5) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var6) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var7) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var8) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var9) );

  EXPECT_EQ( AlgPol::P, mgr.get_pol(bv4, 0, var0) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv4, 0, var1) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv4, 0, var2) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv4, 0, var3) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv4, 0, var4) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv4, 0, var5) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv4, 0, var6) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv4, 0, var7) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv4, 0, var8) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv4, 0, var9) );

}

TEST(AlgMgrTest, cover_set1)
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

  AlgMgr mgr(nv);

  AlgBitVect* bv1 = mgr.new_body(2);
  mgr.cover_set(bv1, {{Literal(var0, false)}, {Literal(var1, true)}});

  EXPECT_EQ( AlgPol::P, mgr.get_pol(bv1, 0, var0) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv1, 0, var1) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv1, 0, var2) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv1, 0, var3) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv1, 0, var4) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv1, 0, var5) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv1, 0, var6) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv1, 0, var7) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv1, 0, var8) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv1, 0, var9) );

  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv1, 1, var0) );
  EXPECT_EQ( AlgPol::N, mgr.get_pol(bv1, 1, var1) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv1, 1, var2) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv1, 1, var3) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv1, 1, var4) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv1, 1, var5) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv1, 1, var6) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv1, 1, var7) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv1, 1, var8) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv1, 1, var9) );
}

TEST(AlgMgrTest, cover_set2)
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

  AlgMgr mgr(nv);

  AlgBitVect* bv1 = mgr.new_body(2);
  mgr.cover_set(bv1, {{Literal(var1, false)}, {Literal(var0, true)}});

  EXPECT_EQ( AlgPol::N, mgr.get_pol(bv1, 0, var0) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv1, 0, var1) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv1, 0, var2) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv1, 0, var3) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv1, 0, var4) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv1, 0, var5) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv1, 0, var6) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv1, 0, var7) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv1, 0, var8) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv1, 0, var9) );

  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv1, 1, var0) );
  EXPECT_EQ( AlgPol::P, mgr.get_pol(bv1, 1, var1) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv1, 1, var2) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv1, 1, var3) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv1, 1, var4) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv1, 1, var5) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv1, 1, var6) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv1, 1, var7) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv1, 1, var8) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv1, 1, var9) );
}

TEST(AlgMgrTest, sum1)
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

  AlgMgr mgr(nv);

  AlgBitVect* bv1 = mgr.new_body(1);
  mgr.cube_set(bv1, {Literal(var0, false)});

  AlgBitVect* bv2 = mgr.new_body(1);
  mgr.cube_set(bv2, {Literal(var1, false)});

  AlgBitVect* bv3 = mgr.new_body(2);
  int nc = mgr.cover_sum(bv3, AlgBlock(1, bv1), AlgBlock(1, bv2));
  EXPECT_EQ( 2, nc );

  EXPECT_EQ( AlgPol::P, mgr.get_pol(bv3, 0, var0) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var1) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var2) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var3) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var4) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var5) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var6) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var7) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var8) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var9) );

  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 1, var0) );
  EXPECT_EQ( AlgPol::P, mgr.get_pol(bv3, 1, var1) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 1, var2) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 1, var3) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 1, var4) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 1, var5) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 1, var6) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 1, var7) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 1, var8) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 1, var9) );

}

TEST(AlgMgrTest, sum2)
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

  AlgMgr mgr(nv);

  AlgBitVect* bv1 = mgr.new_body(2);
  mgr.cover_set(bv1, {{Literal(var0, false)}, {Literal(var1, true)}});

  AlgBitVect* bv2 = mgr.new_body(1);
  mgr.cube_set(bv2, {Literal(var0, false)});

  AlgBitVect* bv3 = mgr.new_body(2);
  int nc = mgr.cover_sum(bv3, AlgBlock{2, bv1}, AlgBlock{1, bv2});

  EXPECT_EQ( 2, nc );

  EXPECT_EQ( AlgPol::P, mgr.get_pol(bv3, 0, var0) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var1) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var2) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var3) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var4) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var5) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var6) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var7) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var8) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var9) );

  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 1, var0) );
  EXPECT_EQ( AlgPol::N, mgr.get_pol(bv3, 1, var1) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 1, var2) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 1, var3) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 1, var4) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 1, var5) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 1, var6) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 1, var7) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 1, var8) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 1, var9) );
}

TEST(AlgMgrTest, diff)
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

  AlgMgr mgr(nv);

  AlgBitVect* bv1 = mgr.new_body(2);
  mgr.cover_set(bv1, {{Literal(var0, false)}, {Literal(var1, true)}});

  AlgBitVect* bv2 = mgr.new_body(2);
  mgr.cover_set(bv2, {{Literal(var0, false)}, {Literal(var2, true)}});

  AlgBitVect* bv3 = mgr.new_body(2);
  int nc = mgr.cover_diff(bv3, AlgBlock{2, bv1}, AlgBlock{2, bv2});
  EXPECT_EQ( 1, nc );

  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var0) );
  EXPECT_EQ( AlgPol::N, mgr.get_pol(bv3, 0, var1) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var2) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var3) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var4) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var5) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var6) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var7) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var8) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var9) );
}

TEST(AlgMgrTest, product1)
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

  AlgMgr mgr(nv);

  AlgBitVect* bv1 = mgr.new_body(2);
  mgr.cover_set(bv1, {{Literal(var0, false)}, {Literal(var1, true)}});

  AlgBitVect* bv2 = mgr.new_body(2);
  mgr.cover_set(bv2, {{Literal(var2, false)}, {Literal(var3, true)}});

  AlgBitVect* bv3 = mgr.new_body(4);
  int nc = mgr.cover_product(bv3, AlgBlock{2, bv1}, AlgBlock{2, bv2});
  EXPECT_EQ( 4, nc );

  EXPECT_EQ( AlgPol::P, mgr.get_pol(bv3, 0, var0) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var1) );
  EXPECT_EQ( AlgPol::P, mgr.get_pol(bv3, 0, var2) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var3) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var4) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var5) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var6) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var7) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var8) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var9) );

  EXPECT_EQ( AlgPol::P, mgr.get_pol(bv3, 1, var0) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 1, var1) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 1, var2) );
  EXPECT_EQ( AlgPol::N, mgr.get_pol(bv3, 1, var3) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 1, var4) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 1, var5) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 1, var6) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 1, var7) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 1, var8) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 1, var9) );

  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 2, var0) );
  EXPECT_EQ( AlgPol::N, mgr.get_pol(bv3, 2, var1) );
  EXPECT_EQ( AlgPol::P, mgr.get_pol(bv3, 2, var2) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 2, var3) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 2, var4) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 2, var5) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 2, var6) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 2, var7) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 2, var8) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 2, var9) );

  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 3, var0) );
  EXPECT_EQ( AlgPol::N, mgr.get_pol(bv3, 3, var1) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 3, var2) );
  EXPECT_EQ( AlgPol::N, mgr.get_pol(bv3, 3, var3) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 3, var4) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 3, var5) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 3, var6) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 3, var7) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 3, var8) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 3, var9) );
}

TEST(AlgMgrTest, product2)
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

  AlgMgr mgr(nv);

  AlgBitVect* bv1 = mgr.new_body(2);
  mgr.cover_set(bv1, {{Literal(var0, false)}, {Literal(var1, true)}});

  AlgBitVect* bv2 = mgr.new_body(2);
  mgr.cover_set(bv2, {{Literal(var0, true)}, {Literal(var3, true)}});

  AlgBitVect* bv3 = mgr.new_body(4);
  int nc = mgr.cover_product(bv3, AlgBlock{2, bv1}, AlgBlock{2, bv2});
  EXPECT_EQ( 3, nc );

  EXPECT_EQ( AlgPol::P, mgr.get_pol(bv3, 0, var0) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var1) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var2) );
  EXPECT_EQ( AlgPol::N, mgr.get_pol(bv3, 0, var3) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var4) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var5) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var6) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var7) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var8) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var9) );

  EXPECT_EQ( AlgPol::N, mgr.get_pol(bv3, 1, var0) );
  EXPECT_EQ( AlgPol::N, mgr.get_pol(bv3, 1, var1) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 1, var2) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 1, var3) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 1, var4) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 1, var5) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 1, var6) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 1, var7) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 1, var8) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 1, var9) );

  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 2, var0) );
  EXPECT_EQ( AlgPol::N, mgr.get_pol(bv3, 2, var1) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 2, var2) );
  EXPECT_EQ( AlgPol::N, mgr.get_pol(bv3, 2, var3) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 2, var4) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 2, var5) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 2, var6) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 2, var7) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 2, var8) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 2, var9) );
}

TEST(AlgMgrTest, product3)
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

  AlgMgr mgr(nv);

  AlgBitVect* bv1 = mgr.new_body(2);
  mgr.cover_set(bv1, {{Literal(var0, false)}, {Literal(var1, true)}});

  AlgBitVect* bv3 = mgr.new_body(2);
  int nc = mgr.cover_product(bv3, AlgBlock{2, bv1}, Literal(var1, false));
  EXPECT_EQ( 1, nc );

  EXPECT_EQ( AlgPol::P, mgr.get_pol(bv3, 0, var0) );
  EXPECT_EQ( AlgPol::P, mgr.get_pol(bv3, 0, var1) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var2) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var3) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var4) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var5) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var6) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var7) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var8) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var9) );
}

TEST(AlgMgrTest, quotient1)
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

  AlgMgr mgr(nv);

  std::initializer_list<std::initializer_list<Literal>> cube_list1
    {
     { Literal{var0, false}, Literal{var2, false} },
     { Literal{var0, false}, Literal{var3, true}  },
     { Literal{var1, true},  Literal{var2, false} },
     { Literal{var1, true},  Literal{var3, true}  }
    };
  int nc1 = cube_list1.size();
  AlgBitVect* bv1 = mgr.new_body(nc1);
  mgr.cover_set(bv1, cube_list1);
  AlgBlock block1{nc1, bv1};

  std::initializer_list<std::initializer_list<Literal>> cube_list2
    {
     { Literal{var2, false} },
     { Literal{var3, true} }
    };
  int nc2 = cube_list2.size();
  AlgBitVect* bv2 = mgr.new_body(cube_list2.size());
  mgr.cover_set(bv2, cube_list2);
  AlgBlock block2{nc2, bv2};

  AlgBitVect* bv3 = mgr.new_body(2);
  int nc = mgr.cover_quotient(bv3, block1, block2);
  EXPECT_EQ( 2, nc );

  EXPECT_EQ( AlgPol::P, mgr.get_pol(bv3, 0, var0) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var1) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var2) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var3) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var4) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var5) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var6) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var7) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var8) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var9) );

  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 1, var0) );
  EXPECT_EQ( AlgPol::N, mgr.get_pol(bv3, 1, var1) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 1, var2) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 1, var3) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 1, var4) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 1, var5) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 1, var6) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 1, var7) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 1, var8) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 1, var9) );
}

TEST(AlgMgrTest, quotient2)
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

  AlgMgr mgr(nv);

  std::initializer_list<std::initializer_list<Literal>> cube_list1
    {
     { Literal{var0, false}, Literal{var2, false} },
     { Literal{var0, false}, Literal{var3, true}  },
     { Literal{var1, true},  Literal{var2, false} },
     { Literal{var1, true},  Literal{var3, true}  }
    };
  int nc1 = cube_list1.size();
  AlgBitVect* bv1 = mgr.new_body(nc1);
  mgr.cover_set(bv1, cube_list1);
  AlgBlock block1{nc1, bv1};

  AlgBitVect* bv3 = mgr.new_body(2);
  int nc = mgr.cover_quotient(bv3, block1, Literal{var1, true});
  EXPECT_EQ( 2, nc );

  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var0) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var1) );
  EXPECT_EQ( AlgPol::P, mgr.get_pol(bv3, 0, var2) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var3) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var4) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var5) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var6) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var7) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var8) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var9) );

  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 1, var0) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 1, var1) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 1, var2) );
  EXPECT_EQ( AlgPol::N, mgr.get_pol(bv3, 1, var3) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 1, var4) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 1, var5) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 1, var6) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 1, var7) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 1, var8) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 1, var9) );
}

TEST(AlgMgrTest, common_cube)
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

  AlgMgr mgr(nv);

  std::initializer_list<std::initializer_list<Literal>> cube_list1
    {
     { Literal{var0, false}, Literal{var2, false}, Literal{var4, false} },
     { Literal{var0, false}, Literal{var3, true}  },
    };
  int nc1 = cube_list1.size();
  AlgBitVect* bv1 = mgr.new_body(nc1);
  mgr.cover_set(bv1, cube_list1);
  AlgBlock block1{nc1, bv1};

  AlgBitVect* bv3 = mgr.new_body(2);
  mgr.common_cube(bv3, block1);

  EXPECT_EQ( AlgPol::P, mgr.get_pol(bv3, 0, var0) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var1) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var2) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var3) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var4) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var5) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var6) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var7) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var8) );
  EXPECT_EQ( AlgPol::X, mgr.get_pol(bv3, 0, var9) );
}

TEST(AlgMgrTest, cover_compare1)
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

  AlgMgr mgr(nv);

  std::initializer_list<std::initializer_list<Literal>> cube_list1
    {
     { Literal{var0, false}, Literal{var2, false} },
     { Literal{var0, false}, Literal{var3, true}  },
     { Literal{var1, true},  Literal{var2, false} },
     { Literal{var1, true},  Literal{var3, true}  }
    };
  int nc1 = cube_list1.size();
  AlgBitVect* bv1 = mgr.new_body(nc1);
  mgr.cover_set(bv1, cube_list1);
  AlgBlock block1{nc1, bv1};

  std::initializer_list<std::initializer_list<Literal>> cube_list2
    {
     { Literal{var0, false}, Literal{var2, false} },
     { Literal{var1, true},  Literal{var3, true}  },
     { Literal{var1, true},  Literal{var2, false} },
     { Literal{var0, false}, Literal{var3, true}  }
    };
  int nc2 = cube_list2.size();
  AlgBitVect* bv2 = mgr.new_body(nc2);
  mgr.cover_set(bv2, cube_list2);
  AlgBlock block2{nc2, bv2};

  int stat = mgr.cover_compare(block1, block2);
  EXPECT_EQ( 0, stat );
}

TEST(AlgMgrTest, cover_compare2)
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

  AlgMgr mgr(nv);

  std::initializer_list<std::initializer_list<Literal>> cube_list1
    {
     { Literal{var0, false}, Literal{var2, false} },
     { Literal{var0, false}, Literal{var3, true}  },
     { Literal{var1, true},  Literal{var2, false} },
     { Literal{var1, true},  Literal{var3, true}  }
    };
  int nc1 = cube_list1.size();
  AlgBitVect* bv1 = mgr.new_body(nc1);
  mgr.cover_set(bv1, cube_list1);
  AlgBlock block1{nc1, bv1};

  std::initializer_list<std::initializer_list<Literal>> cube_list2
    {
     { Literal{var0, false}, Literal{var2, false} },
     { Literal{var1, true},  Literal{var3, true}  },
     { Literal{var1, true},  Literal{var2, false} }
    };
  int nc2 = cube_list2.size();
  AlgBitVect* bv2 = mgr.new_body(nc2);
  mgr.cover_set(bv2, cube_list2);
  AlgBlock block2{nc2, bv2};

  int stat1 = mgr.cover_compare(block1, block2);
  EXPECT_EQ( 1, stat1 );

  int stat2 = mgr.cover_compare(block2, block1);
  EXPECT_EQ( -1, stat2 );
}

TEST(AlgMgrTest, cover_compare3)
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

  AlgMgr mgr(nv);

  std::initializer_list<std::initializer_list<Literal>> cube_list1
    {
     { Literal{var0, false}, Literal{var2, false} },
     { Literal{var0, false}, Literal{var3, true}  },
     { Literal{var1, true},  Literal{var2, false} },
     { Literal{var1, true},  Literal{var3, true}  }
    };
  int nc1 = cube_list1.size();
  AlgBitVect* bv1 = mgr.new_body(nc1);
  mgr.cover_set(bv1, cube_list1);
  AlgBlock block1{nc1, bv1};

  std::initializer_list<std::initializer_list<Literal>> cube_list2
    {
     { Literal{var0, false}, Literal{var2, false} },
     { Literal{var1, true},  Literal{var3, true}  },
     { Literal{var1, true},  Literal{var2, false} },
     { Literal{var0, false}, Literal{var4, true}  }
    };
  int nc2 = cube_list2.size();
  AlgBitVect* bv2 = mgr.new_body(nc2);
  mgr.cover_set(bv2, cube_list2);
  AlgBlock block2{nc2, bv2};

  int stat1 = mgr.cover_compare(block1, block2);
  EXPECT_EQ( 1, stat1 );

  int stat2 = mgr.cover_compare(block2, block1);
  EXPECT_EQ( -1, stat2 );
}

END_NAMESPACE_YM_LOGIC
