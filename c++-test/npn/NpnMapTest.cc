
/// @file NpnMapTest.cc
/// @brief NpnMapTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.


#include "gtest/gtest.h"
#include "ym/NpnMap.h"


BEGIN_NAMESPACE_YM_LOGIC

TEST(NpnMapTest, identity)
{
  ymuint ni = 5;
  NpnMap map = NpnMap::identity(ni);
  EXPECT_EQ( ni, map.input_num() );
  EXPECT_EQ( ni, map.input_num2() );
  EXPECT_EQ( false, map.oinv() );
  for (ymuint i = 0; i < ni; ++ i) {
    NpnVmap imap = map.imap(VarId(i));
    EXPECT_EQ( VarId(i), imap.var() );
    EXPECT_FALSE( imap.inv() );
  }
}

TEST(NpnMapTest, identity2)
{
  ymuint ni = 10;
  NpnMap map = NpnMap::identity(ni, true);
  EXPECT_EQ( ni, map.input_num() );
  EXPECT_EQ( ni, map.input_num2() );
  EXPECT_EQ( true, map.oinv() );
  for (ymuint i = 0; i < ni; ++ i) {
    NpnVmap imap = map.imap(VarId(i));
    EXPECT_EQ( VarId(i), imap.var() );
    EXPECT_FALSE( imap.inv() );
  }
}

TEST(NpnMapTest, base)
{
  // (3', 1, 4, 2', 0)'
  NpnMap map1(5);
  map1.set_oinv(true);
  map1.set(VarId(0), VarId(4), false);
  map1.set(VarId(1), VarId(1), false);
  map1.set(VarId(2), VarId(3), true);
  map1.set(VarId(3), VarId(0), true);
  map1.set(VarId(4), VarId(2), false);

  EXPECT_EQ( 5, map1.input_num() );
  EXPECT_EQ( true, map1.oinv() );

  EXPECT_EQ( VarId(4), map1.imap(VarId(0)).var() );
  EXPECT_EQ( false, map1.imap(VarId(0)).inv() );

  EXPECT_EQ( VarId(1), map1.imap(VarId(1)).var() );
  EXPECT_EQ( false, map1.imap(VarId(1)).inv() );

  EXPECT_EQ( VarId(3), map1.imap(VarId(2)).var() );
  EXPECT_EQ( true, map1.imap(VarId(2)).inv() );

  EXPECT_EQ( VarId(0), map1.imap(VarId(3)).var() );
  EXPECT_EQ( true, map1.imap(VarId(3)).inv() );

  EXPECT_EQ( VarId(2), map1.imap(VarId(4)).var() );
  EXPECT_EQ( false, map1.imap(VarId(4)).inv() );

  NpnMap map2(5);
  map2.set_oinv(true);
  map2.set(VarId(0), VarId(4), false);
  map2.set(VarId(1), VarId(1), false);
  map2.set(VarId(2), VarId(3), true);
  map2.set(VarId(3), VarId(0), true);
  map2.set(VarId(4), VarId(2), false);

  EXPECT_EQ( map1, map2 );

  NpnMap map3(5);

  EXPECT_NE( map3, map1);
}

TEST(NpnMapTest, inverse)
{
  // (3', 1, 4, 2', 0)'
  NpnMap map1(5);
  map1.set_oinv(true);
  map1.set(VarId(0), VarId(4), false);
  map1.set(VarId(1), VarId(1), false);
  map1.set(VarId(2), VarId(3), true);
  map1.set(VarId(3), VarId(0), true);
  map1.set(VarId(4), VarId(2), false);

  NpnMap imap = inverse(map1);

  NpnMap map2 = map1 * imap;

  NpnMap ident(5);
  EXPECT_EQ( ident, map2 );

  NpnMap map3 = imap * map1;
  EXPECT_EQ( ident, map3);
}

TEST(NpnMapTest, compose)
{
  // (9', 5, 3)'
  NpnMap map1(3, 10);
  map1.set_oinv(true);
  map1.set(VarId(0), VarId(9), true);
  map1.set(VarId(1), VarId(5), false);
  map1.set(VarId(2), VarId(3), false);

  // (--, --, --, 2', --, 1, --, --, --, 0)
  NpnMap map2(10, 3);
  map2.set_oinv(false);
  map2.set(VarId(3), VarId(2), true);
  map2.set(VarId(5), VarId(1), false);
  map2.set(VarId(9), VarId(0), false);

  NpnMap map3 = map1 * map2;
  EXPECT_EQ( 3, map3.input_num() );
  EXPECT_EQ( 3, map3.input_num2() );
  EXPECT_EQ( true, map3.oinv() );

  NpnVmap imap0 = map3.imap(VarId(0));
  EXPECT_EQ( VarId(0), imap0.var() );
  EXPECT_EQ( true, imap0.inv() );

  NpnVmap imap1 = map3.imap(VarId(1));
  EXPECT_EQ( VarId(1), imap1.var() );
  EXPECT_EQ( false, imap1.inv() );

  NpnVmap imap2 = map3.imap(VarId(2));
  EXPECT_EQ( VarId(2), imap2.var() );
  EXPECT_EQ( true, imap2.inv() );
}

END_NAMESPACE_YM_LOGIC
