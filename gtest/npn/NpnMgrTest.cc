
/// @file NpnMgrTest.cc
/// @brief NpnMgrTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.


#include "gtest/gtest.h"
#include "NpnMgr.h"
#include "ym/TvFunc.h"
#include "ym/HashSet.h"
#include "ym/PermGen.h"
#include "ym/RandGen.h"
#include "ym/RandPermGen.h"
#include "InputInfo.h"


BEGIN_NAMESPACE_YM_LOGIC

TEST(NpnMgrTest, walsh01_normalize_and2)
{
  TvFunc lit0 = TvFunc::literal(2, Literal(VarId(0)));
  TvFunc lit1 = TvFunc::literal(2, Literal(VarId(1)));

  TvFunc and2 = lit0 & lit1;

  NpnMap xmap;
  InputInfo input_info;
  bool opol_fixed = NpnMgr::walsh01_normalize(and2, xmap, input_info);

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

  NpnMap xmap;
  InputInfo input_info;
  bool opol_fixed = NpnMgr::walsh01_normalize(xor2, xmap, input_info);

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

  NpnMap xmap;
  InputInfo input_info;
  bool opol_fixed = NpnMgr::walsh01_normalize(ao221, xmap, input_info);

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

  EXPECT_TRUE( xmap.oinv() );
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

  EXPECT_FALSE( xmap.oinv() );
}

class NpnMgrTestWithParam :
  public ::testing::TestWithParam<int>
{
};

TEST_P(NpnMgrTestWithParam, func_test)
{
  ymuint ni = GetParam();
  ymuint ni_exp = 1U << ni;
  ymuint ni_exp_exp = 1U << ni_exp;
  HashSet<TvFunc> func_hash;
  for (ymuint sig = 0; sig < ni_exp_exp; ++ sig) {
    vector<int> values(ni_exp);
    for (ymuint p = 0; p < ni_exp; ++ p) {
      if ( sig & (1U << p) ) {
	values[p] = 1;
      }
      else {
	values[p] = 0;
      }
    }
    TvFunc f(ni, values);

    NpnMgr npnmgr;
    TvFunc cfunc0 = npnmgr.cannonical(f);

    vector<NpnMap> map_list;
    npnmgr.all_cmap(map_list);
    if ( map_list.empty() ) {
      cout << "F     = " << f << endl
	   << "cfunc = " << cfunc0 << endl;
    }

    NpnMap map = map_list[0];
    TvFunc cfunc = f.xform(map);

    if ( !func_hash.find(cfunc) ) {
      func_hash.add(cfunc);
    }
  }

  ymuint exp_num[] = {
    1,
    2,
    4,
    14,
    222,
  };

  EXPECT_EQ( exp_num[ni], func_hash.num() );
}

TEST_P(NpnMgrTestWithParam, func_test2)
{
  ymuint ni = GetParam();
  ymuint ni_exp = 1U << ni;
  ymuint ni_exp_exp = 1U << ni_exp;
  HashSet<TvFunc> func_hash;
  for (ymuint sig = 0; sig < ni_exp_exp; ++ sig) {
    vector<int> values(ni_exp);
    for (ymuint p = 0; p < ni_exp; ++ p) {
      if ( sig & (1U << p) ) {
	values[p] = 1;
      }
      else {
	values[p] = 0;
      }
    }
    TvFunc f(ni, values);
    if ( func_hash.check(f) ) {
      continue;
    }
    if ( 0 ) {
      cout << "F  = " << f << endl;
    }

    NpnMgr npnmgr;
    TvFunc cfunc = npnmgr.cannonical(f);

    {
      vector<NpnMap> map_list;
      npnmgr.all_cmap(map_list);

      NpnMap map = map_list[0];
      TvFunc xformed_cfunc = f.xform(map);
      EXPECT_EQ( cfunc, xformed_cfunc );
    }

    if ( ni > 0 ) {
      for (PermGen pg(ni, ni); !pg.is_end(); ++ pg) {
	for (ymuint op = 0; op < 2; ++ op) {
	  for (ymuint ip = 0; ip < ni_exp; ++ ip) {
	    NpnMap map(ni);
	    bool oinv = op ? true : false;
	    map.set_oinv(oinv);
	    for (ymuint i = 0; i < ni; ++ i) {
	      bool iinv = (ip & (1U << i)) ? true : false;
	      map.set(VarId(i), VarId(pg(i)), iinv);
	    }
	    TvFunc f1 = f.xform(map);
	    func_hash.add(f1);
	    if ( 0 ) {
	      cout << "F1 = " << f1 << endl
		   << " map = " << map << endl;
	    }

	    NpnMgr npnmgr;
	    TvFunc cfunc1 = npnmgr.cannonical(f1);

	    {
	      vector<NpnMap> map_list;
	      npnmgr.all_cmap(map_list);

	      NpnMap cmap = map_list[0];
	      TvFunc xformed_cfunc1 = f1.xform(cmap);
	      EXPECT_EQ( cfunc1, xformed_cfunc1 );
	    }
	    if ( cfunc != cfunc1 ) {
	      cout << "f  = " << f << endl
		   << "f1 = " << f1 << endl
		   << "map = " << map << endl;
	    }
	    EXPECT_EQ( cfunc, cfunc1 );
	  }
	}
      }
    }
    else {
      for (ymuint op = 0; op < 2; ++ op) {
	NpnMap map(0);
	bool oinv = op ? true : false;
	map.set_oinv(oinv);
	TvFunc f1 = f.xform(map);
	if ( 0 ) {
	  cout << "F1 = " << f1 << endl
	       << " map = " << map << endl;
	}

	NpnMgr npnmgr;
	TvFunc cfunc1 = npnmgr.cannonical(f1);

	{
	  vector<NpnMap> map_list;
	  npnmgr.all_cmap(map_list);

	  NpnMap cmap = map_list[0];
	  TvFunc xformed_cfunc1 = f1.xform(cmap);
	  EXPECT_EQ( cfunc1, xformed_cfunc1 );
	}
	EXPECT_EQ( cfunc, cfunc1 );
      }
    }
  }
}

class NpnMgrRandomTest :
  public ::testing::TestWithParam<int>
{
};

TEST_P(NpnMgrRandomTest, func_test)
{
  ymuint ni = GetParam();
  ymuint ni_exp = 1U << ni;
  ymuint ni_exp_exp = 1U << ni_exp;
  ymuint nfunc = 100;
  ymuint nperm = 50;
  ymuint nphase = 10;

  if ( ni == 0 ) {
    return;
  }

  RandGen rg;
  for (ymuint i = 0; i < nfunc; ++ i) {
    vector<int> values(ni_exp);
    for (ymuint p = 0; p < ni_exp; ++ p) {
      if ( rg.int32() % 2 ) {
	values[p] = 1;
      }
      else {
	values[p] = 0;
      }
    }
    TvFunc f(ni, values);
    if ( 0 ) {
      cout << "F  = " << f << endl;
    }

    NpnMgr npnmgr;
    TvFunc cfunc = npnmgr.cannonical(f);

    {
      vector<NpnMap> map_list;
      npnmgr.all_cmap(map_list);

      if ( map_list.empty() ) {
	cout << "f = " << f << endl
	     << "cfunc = " << cfunc << endl;
      }

      NpnMap map = map_list[0];
      TvFunc xformed_cfunc = f.xform(map);
      EXPECT_EQ( cfunc, xformed_cfunc );
    }

    RandPermGen rpg(ni);
    for (ymuint j = 0; j < nperm; ++ j) {
      vector<int> dst_map(ni);
      rpg.generate(rg);
      for (ymuint k = 0; k < ni; ++ k) {
	dst_map[k] = rpg.elem(k);
      }
      for (ymuint op = 0; op < 2; ++ op) {
	bool oinv = op ? true : false;
	for (ymuint k = 0; k < nphase; ++ k) {
	  NpnMap map(ni);
	  map.set_oinv(oinv);
	  for (ymuint i = 0; i < ni; ++ i) {
	    bool iinv = (rg.int32() % 2) ? true : false;
	    map.set(VarId(i), VarId(dst_map[i]), iinv);
	  }
	  TvFunc f1 = f.xform(map);
	  if ( 0 ) {
	    cout << "F1 = " << f1 << endl
		 << " map = " << map << endl;
	  }

	  NpnMgr npnmgr;
	  TvFunc cfunc1 = npnmgr.cannonical(f1);

	  {
	    vector<NpnMap> map_list;
	    npnmgr.all_cmap(map_list);

	    NpnMap cmap = map_list[0];
	    TvFunc xformed_cfunc1 = f1.xform(cmap);
	    EXPECT_EQ( cfunc1, xformed_cfunc1 );
	  }

	  if ( cfunc != cfunc1 ) {
	    cout << "f  = " << f << endl
		 << "f1 = " << f1 << endl
		 << "map = " << map << endl;
	  }
	  EXPECT_EQ( cfunc, cfunc1 );
	}
      }
    }
  }
}

INSTANTIATE_TEST_CASE_P(Test0to5,
			NpnMgrTestWithParam,
			::testing::Range(0, 5));

INSTANTIATE_TEST_CASE_P(Test5to20,
			NpnMgrRandomTest,
			::testing::Range(5, 20));

TvFunc
str_to_func(ymuint ni,
	    const string& func_str)
{
  ymuint ni_exp = 1 << ni;
  vector<int> values(ni_exp);
  for (ymuint i = 0; i < ni_exp; ++ i) {
    values[i] = func_str[i] == '1' ? 1 : 0;
  }
  return TvFunc(ni, values);
}

TEST(NpnMgrTest1, bug1)
{
  TvFunc func = str_to_func(4, "1011111011000000");

  NpnMgr npnmgr;
  TvFunc cfunc = npnmgr.cannonical(func);

  NpnMap map1(4);
  map1.set_oinv(true);
  TvFunc func1 = func.xform(map1);

  TvFunc cfunc1 = npnmgr.cannonical(func1);

  EXPECT_EQ( cfunc, cfunc1 );
}

TEST(NpnMgrTest1, bug2)
{
  TvFunc func = str_to_func(4, "1101100000000000");

  NpnMgr npnmgr;
  TvFunc cfunc = npnmgr.cannonical(func);

  NpnMap map1(4);
  map1.set_oinv(true);
  TvFunc func1 = func.xform(map1);

  TvFunc cfunc1 = npnmgr.cannonical(func1);

  EXPECT_EQ( cfunc, cfunc1 );
}

TEST(NpnMgrTest1, bug3)
{
  TvFunc func = str_to_func(4, "1011100001000000");

  NpnMgr npnmgr;
  TvFunc cfunc = npnmgr.cannonical(func);

  NpnMap map1(4);
  map1.set(VarId(0), VarId(0), false);
  map1.set(VarId(1), VarId(1), false);
  map1.set(VarId(2), VarId(3), false);
  map1.set(VarId(3), VarId(2), false);

  TvFunc func1 = func.xform(map1);

  TvFunc cfunc1 = npnmgr.cannonical(func1);

  EXPECT_EQ( cfunc, cfunc1 );
}

TEST(NpnMgrTest1, bug4)
{
  TvFunc func = str_to_func(4, "1000100101100000");

  NpnMgr npnmgr;
  TvFunc cfunc = npnmgr.cannonical(func);

  NpnMap map1(4);
  map1.set(VarId(0), VarId(0), false);
  map1.set(VarId(1), VarId(1), false);
  map1.set(VarId(2), VarId(3), false);
  map1.set(VarId(3), VarId(2), false);

  TvFunc func1 = func.xform(map1);

  TvFunc cfunc1 = npnmgr.cannonical(func1);

  EXPECT_EQ( cfunc, cfunc1 );
}

TEST(NpnMgrTest1, bug5)
{
  TvFunc func = str_to_func(10, "1011000100100101001000011011110011101000011001001110111001001100110011110010111101111111101100101100010001100010101111011010101011110000000110001011001111100010100110001001100010000000001000110011101110000100111111000110100000010001010011111111100101001000011010100010111111011111100111010011110111100001100000110111000000101111000111001100001011110101000010101011011111000100100110101011101010001000011010110101111111101111001010011001010000101101111111000110001110000011000100010100101111001111010111110011000011101000000000011101111010110100000100010010011000110111000001010010011111000011001000111100001101101001111100101010001111010110100100001011011001101101000001111101111010000100101001011010111100100101110000000011000001010001011011101011000001100011111011001001110001011000000101100100100001000011000001001011011110101001010000101000100111100011010010101101000000000010010111001101011101011110011010101011000000100110000110100011110110001110110100010110010010011010101100001110100010100001011000100100010010001111");

  NpnMgr npnmgr;
  TvFunc cfunc = npnmgr.cannonical(func);

  vector<NpnMap> map_list;
  npnmgr.all_cmap(map_list);
  if ( map_list.empty() ) {
    cout << "func = " << func << endl
	 << "cfunc = " << cfunc << endl;
  }
  EXPECT_FALSE( map_list.empty() );
}

END_NAMESPACE_YM_LOGIC
