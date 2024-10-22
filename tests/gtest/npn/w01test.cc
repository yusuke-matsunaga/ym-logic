
/// @file w01test.cc
/// @brief w01test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017, 2023 Yusuke Matsunaga
/// All rights reserved.

#include "gtest/gtest.h"
#include "ym/TvFunc.h"
#include "ym/NpnMap.h"
#include "InputInfo.h"


BEGIN_NAMESPACE_YM_LOGIC

extern
bool
walsh_01_normalize(
  const TvFunc& func,
  NpnMap& xmap,
  InputInfo& input_info
);

TEST(w01test, walsh_01_normalize_and2)
{
  auto lit0 = TvFunc::literal(2, 0, false);
  auto lit1 = TvFunc::literal(2, 1, false);

  auto and2 = lit0 & lit1;

  NpnMap xmap;
  InputInfo input_info;
  bool opol_fixed = walsh_01_normalize(and2, xmap, input_info);

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

  SizeType var0{0};
  EXPECT_EQ( var0, xmap.imap(var0).var() );
  EXPECT_FALSE( xmap.imap(var0).inv() );

  SizeType var1{1};
  EXPECT_EQ( var1, xmap.imap(var1).var() );
  EXPECT_FALSE( xmap.imap(var1).inv() );
}

TEST(w01test, walsh_01_normalize_xor2)
{
  auto lit0 = TvFunc::literal(2, Literal{0, false});
  auto lit1 = TvFunc::literal(2, Literal{1, false});

  auto xor2 = (lit0 & ~lit1) | (~lit0 & lit1);

  NpnMap xmap;
  InputInfo input_info;
  bool opol_fixed = walsh_01_normalize(xor2, xmap, input_info);

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

  SizeType var0(0);
  EXPECT_EQ( var0, xmap.imap(var0).var() );
  EXPECT_FALSE( xmap.imap(var0).inv() );

  SizeType var1(1);
  EXPECT_EQ( var1, xmap.imap(var1).var() );
  EXPECT_FALSE( xmap.imap(var1).inv() );
}

TEST(w01test, walsh_01_normalize_ao221)
{
  auto lit0 = TvFunc::literal(5, Literal(0, false));
  auto lit1 = TvFunc::literal(5, Literal(1, false));
  auto lit2 = TvFunc::literal(5, Literal(2, false));
  auto lit3 = TvFunc::literal(5, Literal(3, false));
  auto lit4 = TvFunc::literal(5, Literal(4, false));

  auto ao221 = (lit0 & lit1) | (~lit2 & ~lit3) | lit4;

  NpnMap xmap;
  InputInfo input_info;
  bool opol_fixed = walsh_01_normalize(ao221, xmap, input_info);

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

  SizeType var0{0};
  EXPECT_EQ( var0, xmap.imap(var0).var() );
  EXPECT_TRUE( xmap.imap(var0).inv() );

  SizeType var1{1};
  EXPECT_EQ( var1, xmap.imap(var1).var() );
  EXPECT_TRUE( xmap.imap(var1).inv() );
}

END_NAMESPACE_YM_LOGIC
