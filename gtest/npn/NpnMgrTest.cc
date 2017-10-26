
/// @file NpnMgrTest.cc
/// @brief NpnMgrTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.


#include "gtest/gtest.h"
#include "NpnMgr.h"
#include "ym/TvFunc.h"
#include "InputInfo.h"


BEGIN_NAMESPACE_YM_LOGIC

TEST(NpnMgrTest, walsh01_normalize_and2)
{
  TvFunc lit0 = TvFunc::literal(2, Literal(VarId(0)));
  TvFunc lit1 = TvFunc::literal(2, Literal(VarId(1)));

  TvFunc and2 = lit0 & lit1;

  NpnMgr npnmgr;
  NpnMap xmap;
  InputInfo input_info;
  bool opol_fixed = npnmgr.walsh01_normalize(and2, xmap, input_info);

  EXPECT_TRUE( opol_fixed );

  EXPECT_EQ( 1, input_info.group_num() );
  EXPECT_EQ( 0, input_info.polundet_num() );
  EXPECT_EQ( 2, input_info.w1(0) );
  EXPECT_EQ( 2, input_info.elem_num(0) );
  EXPECT_EQ( 0, input_info.elem(0, 0) );
  EXPECT_EQ( 1, input_info.elem(0, 1) );
  EXPECT_FALSE( input_info.bisym(0) );

  EXPECT_EQ( 2, xmap.input_num() );
  EXPECT_FALSE( xmap.oinv() );

  VarId var0(0);
  EXPECT_EQ( var0, xmap.imap(var0).var() );
  EXPECT_FALSE( xmap.imap(var0).inv() );

  VarId var1(1);
  EXPECT_EQ( var1, xmap.imap(var1).var() );
  EXPECT_FALSE( xmap.imap(var1).inv() );
}

TEST(NpnMgrTest, walsh01_normalize_xor2)
{
  TvFunc lit0 = TvFunc::literal(2, Literal(VarId(0)));
  TvFunc lit1 = TvFunc::literal(2, Literal(VarId(1)));

  TvFunc xor2 = (lit0 & ~lit1) | (~lit0 & lit1);

  NpnMgr npnmgr;
  NpnMap xmap;
  InputInfo input_info;
  bool opol_fixed = npnmgr.walsh01_normalize(xor2, xmap, input_info);

  EXPECT_FALSE( opol_fixed );

  EXPECT_EQ( 1, input_info.group_num() );
  EXPECT_EQ( 1, input_info.polundet_num() );
  EXPECT_EQ( 0, input_info.w1(0) );
  EXPECT_EQ( 2, input_info.elem_num(0) );
  EXPECT_EQ( 0, input_info.elem(0, 0) );
  EXPECT_EQ( 1, input_info.elem(0, 1) );
  EXPECT_TRUE( input_info.bisym(0) );

  EXPECT_EQ( 2, xmap.input_num() );
  EXPECT_FALSE( xmap.oinv() );

  VarId var0(0);
  EXPECT_EQ( var0, xmap.imap(var0).var() );
  EXPECT_FALSE( xmap.imap(var0).inv() );

  VarId var1(1);
  EXPECT_EQ( var1, xmap.imap(var1).var() );
  EXPECT_FALSE( xmap.imap(var1).inv() );
}

TEST(NpnMgrTest, walsh01_normalize_ao221)
{
  TvFunc lit0 = TvFunc::literal(5, Literal(VarId(0)));
  TvFunc lit1 = TvFunc::literal(5, Literal(VarId(1)));
  TvFunc lit2 = TvFunc::literal(5, Literal(VarId(2)));
  TvFunc lit3 = TvFunc::literal(5, Literal(VarId(3)));
  TvFunc lit4 = TvFunc::literal(5, Literal(VarId(4)));

  TvFunc ao221 = (lit0 & lit1) | (~lit2 & ~lit3) | lit4;

  NpnMgr npnmgr;
  NpnMap xmap;
  InputInfo input_info;
  bool opol_fixed = npnmgr.walsh01_normalize(ao221, xmap, input_info);

  EXPECT_TRUE( opol_fixed );

  EXPECT_EQ( 3, input_info.group_num() );
  EXPECT_EQ( 0, input_info.polundet_num() );
  EXPECT_EQ( 6, input_info.w1(0) );
  EXPECT_EQ( 6, input_info.w1(1) );
  EXPECT_EQ( 18, input_info.w1(2) );
  EXPECT_EQ( 2, input_info.elem_num(0) );
  EXPECT_EQ( 0, input_info.elem(0, 0) );
  EXPECT_EQ( 1, input_info.elem(0, 1) );
  EXPECT_FALSE( input_info.bisym(0) );
  EXPECT_EQ( 2, input_info.elem_num(1) );
  EXPECT_EQ( 2, input_info.elem(1, 0) );
  EXPECT_EQ( 3, input_info.elem(1, 1) );
  EXPECT_FALSE( input_info.bisym(0) );
  EXPECT_EQ( 1, input_info.elem_num(2) );
  EXPECT_EQ( 4, input_info.elem(2, 0) );
  EXPECT_FALSE( input_info.bisym(0) );

  EXPECT_EQ( 5, xmap.input_num() );
  EXPECT_TRUE( xmap.oinv() );

  VarId var0(0);
  EXPECT_EQ( var0, xmap.imap(var0).var() );
  EXPECT_TRUE( xmap.imap(var0).inv() );

  VarId var1(1);
  EXPECT_EQ( var1, xmap.imap(var1).var() );
  EXPECT_TRUE( xmap.imap(var1).inv() );
}

TEST(NpnMgrTest, cannonical_const0)
{
  TvFunc func = TvFunc::const_zero(0);

  NpnMgr npnmgr;
  npnmgr.cannonical(func);

  vector<NpnMap> map_list;
  npnmgr.all_cmap(map_list);

  EXPECT_EQ( 1, map_list.size() );

  const NpnMap& xmap = map_list[0];
  EXPECT_EQ( 0, xmap.input_num() );

  EXPECT_FALSE( xmap.oinv() );
}

TEST(NpnMgrTest, cannonical_const1)
{
  TvFunc func = TvFunc::const_one(0);

  NpnMgr npnmgr;
  npnmgr.cannonical(func);

  vector<NpnMap> map_list;
  npnmgr.all_cmap(map_list);

  EXPECT_EQ( 1, map_list.size() );

  const NpnMap& xmap = map_list[0];
  EXPECT_EQ( 0, xmap.input_num() );

  EXPECT_TRUE( xmap.oinv() );
}

END_NAMESPACE_YM_LOGIC
