
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
  bool opol_fixed = npnmgr.walsh01_normalize(and2, xmap);

  EXPECT_TRUE( opol_fixed );

#if 0
  EXPECT_EQ( 1, iinfo.group_num() );
  EXPECT_EQ( 0, iinfo.polundet_num() );
  EXPECT_EQ( 2, iinfo.w1(0) );
  EXPECT_EQ( 2, iinfo.elem_num(0) );
  EXPECT_EQ( 0, iinfo.elem(0, 0) );
  EXPECT_EQ( 1, iinfo.elem(0, 1) );
  EXPECT_FALSE( iinfo.bisym(0) );
#endif

  EXPECT_EQ( 2, xmap.input_num() );
  EXPECT_FALSE( xmap.oinv() );

  VarId var0(0);
  EXPECT_EQ( var0, xmap.imap(var0).var() );
  EXPECT_FALSE( xmap.imap(var0).inv() );

  VarId var1(1);
  EXPECT_EQ( var1, xmap.imap(var1).var() );
  EXPECT_FALSE( xmap.imap(var1).inv() );
}

END_NAMESPACE_YM_LOGIC
