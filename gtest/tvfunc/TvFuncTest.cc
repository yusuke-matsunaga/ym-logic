
/// @file TvFuncTest.cc
/// @brief TvFuncTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.


#include "gtest/gtest.h"
#include "ym/TvFunc.h"
#include "ym/RandGen.h"


BEGIN_NAMESPACE_YM

class TvFuncTestWithParam :
  public ::testing::TestWithParam<int>
{
public:

public:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ベクタの値を格納するバッファ
  int mValues[1024 * 1024];

};


void
check_func(const TvFunc& func,
	   int exp_values[],
	   const string& str)
{
  // value() のテスト
  // 同時に0と1の数も数えておく．
  ymuint ni = func.input_num();
  ymuint ni_exp = 1U << ni;
  ymuint n0 = 0;
  ymuint n1 = 0;
  for (ymuint p = 0; p < ni_exp; ++ p) {
    EXPECT_EQ( exp_values[p], func.value(p) ) << str;
    if ( exp_values[p] ) {
      ++ n1;
    }
    else {
      ++ n0;
    }
  }
  // count_zero(), count_one() のテスト
  EXPECT_EQ( n0, func.count_zero() ) << str;
  EXPECT_EQ( n1, func.count_one() ) << str;

  // negate() のテスト
  TvFunc fn(func);
  fn.negate();
  for (ymuint p = 0; p < ni_exp; ++ p) {
    int exp_value = 1 - exp_values[p];
    EXPECT_EQ( exp_value, fn.value(p) ) << str;
  }

  // walsh_01() のテスト
  int vec[20];
  int w0 = func.walsh_01(vec);

  // walsh_012() のテスト
  int vec1[20];
  int vec2[20 * 20];
  int w0_2 = func.walsh_012(vec1, vec2);

  // walsh_0() のテスト
  int exp_w0 = 0;
  for (ymuint p = 0; p < ni_exp; ++ p) {
    int v = exp_values[p];
    if ( v == 0 ) {
      exp_w0 += 1;
    }
    else {
      exp_w0 -= 1;
    }
  }
  EXPECT_EQ( exp_w0, func.walsh_0() );
  EXPECT_EQ( exp_w0, w0 );
  EXPECT_EQ( exp_w0, w0_2 );

  // walsh_1() のテスト
  for (ymuint i = 0; i < ni; ++ i) {
    VarId vid(i);
    ymuint i_bit = 1U << i;
    int exp_w1 = 0;
    for (ymuint p = 0; p < ni_exp; ++ p) {
      int v = exp_values[p];
      if ( p & i_bit ) {
	v = 1 - v;
      }
      if ( v == 0 ) {
	exp_w1 += 1;
      }
      else {
	exp_w1 -= 1;
      }
    }
    EXPECT_EQ( exp_w1, func.walsh_1(vid) );
    EXPECT_EQ( exp_w1, vec[i] );
    EXPECT_EQ( exp_w1, vec1[i] );
  }

  // walsh_2() のテスト
  for (ymuint i1 = 0; i1 < ni; ++ i1) {
    VarId vid1(i1);
    ymuint i1_bit = 1U << i1;
    for (ymuint i2 = i1 + 1; i2 < ni; ++ i2) {
      VarId vid2(i2);
      ymuint i2_bit = 1U << i2;
      int exp_w2 = 0;
      for (ymuint p = 0; p < ni_exp; ++ p) {
	int v = exp_values[p];
	bool i1_on = (p & i1_bit) == i1_bit;
	bool i2_on = (p & i2_bit) == i2_bit;
	if ( i1_on ^ i2_on ) {
	  v = 1 - v;
	}
	if ( v == 0 ) {
	  exp_w2 += 1;
	}
	else {
	  exp_w2 -= 1;
	}
      }
      EXPECT_EQ( exp_w2, func.walsh_2(vid1, vid2) );
      EXPECT_EQ( exp_w2, vec2[i1 * ni + i2] );
    }
  }

  if ( 1 ) {
    cout << "func:";
    func.print(cout);
    cout << endl;
  }
  // walsh_w0() のテスト
  RandGen rg;
  // 100回ランダムな極性割り当てを試す．
  for (ymuint c = 0; c < 100; ++ c) {
    for (int oinv = 0; oinv < 2; ++ oinv) {
      ymuint ibits = 0U;
      for (ymuint j = 0; j < ni; ++ j) {
	if ( rg.real1() > 0.5 ) {
	  ibits |= (1U << j);
	}
      }
      if ( c == 0 ) {
	ibits = 0U;
      }
      if ( 1 ) {
	cout << "oinv = " << oinv << endl;
	cout << "ibits = ";
	for (ymuint i = 0; i < ni; ++ i) {
	  if ( ibits & (1 << i) ) {
	    cout << "1";
	  }
	  else {
	    cout << "0";
	  }
	}
	cout << endl;
      }
      int ww0[21];
      for (ymuint j = 0; j <= ni; ++ j) {
	ww0[j] = 0;
      }
      for (ymuint p = 0; p < ni_exp; ++ p) {
	ymuint q = p ^ ibits;
	ymuint cur_w = 0;
	for (ymuint j = 0; j < ni; ++ j) {
	  if ( q & (1U << j) ) {
	    ++ cur_w;
	  }
	}
	int v = func.value(p);
	if ( oinv ) {
	  v = 1 - v;
	}
	if ( v ) {
	  ww0[cur_w] -= 1;
	}
	else {
	  ww0[cur_w] += 1;
	}
      }
      for (ymuint w = 0; w <= ni; ++ w) {
	if ( 1 ) {
	  cout << "  w = " << w << endl;
	}
	EXPECT_EQ( ww0[w], func.walsh_w0(w, oinv, ibits) );
	if ( 1 ) {
	  cout << endl;
	}
      }
    }
  }
}

void
check_op(const TvFunc& func1,
	 const TvFunc& func2)
{
  ymuint ni = func1.input_num();
  ymuint ni_exp = 1U << ni;
  int exp_and[1024 * 1024];
  int exp_or[1024 * 1024];
  int exp_xor[1024 * 1024];
  for (ymuint p = 0; p < ni_exp; ++ p) {
    int v1 = func1.value(p);
    int v2 = func2.value(p);
    exp_and[p] = v1 & v2;
    exp_or[p]  = v1 | v2;
    exp_xor[p] = v1 ^ v2;
  }
  TvFunc f_and = func1 & func2;
  check_func(f_and, exp_and, "func1 & func2");

  TvFunc f_or  = func1 | func2;
  check_func(f_or, exp_or, "func1 | func2");

  TvFunc f_xor = func1 ^ func2;
  check_func(f_xor, exp_xor, "func1 ^ func2");
}

TEST_P(TvFuncTestWithParam, input_num)
{
  ymuint ni = GetParam();
  TvFunc f(ni);
  EXPECT_EQ( ni, f.input_num() );
}

TEST_P(TvFuncTestWithParam, empty_constr)
{
  ymuint ni = GetParam();
  ymuint ni_exp = 1U << ni;
  for (ymuint i = 0; i < ni_exp; ++ i) {
    mValues[i] = 0;
  }
  TvFunc f0(ni);
  ostringstream buf;
  buf << "TvFunc(" << ni << ")";
  check_func(f0, mValues, buf.str());
}

TEST_P(TvFuncTestWithParam, const_zero)
{
  ymuint ni = GetParam();
  ymuint ni_exp = 1U << ni;
  for (ymuint i = 0; i < ni_exp; ++ i) {
    mValues[i] = 0;
  }
  TvFunc f0 = TvFunc::const_zero(ni);
  ostringstream buf;
  buf << "TvFunc::const_zero(" << ni << ")";
  check_func(f0, mValues, buf.str());
}

TEST_P(TvFuncTestWithParam, const_one)
{
  ymuint ni = GetParam();
  ymuint ni_exp = 1U << ni;
  for (ymuint i = 0; i < ni_exp; ++ i) {
    mValues[i] = 0;
  }
  TvFunc f0 = TvFunc::const_one(ni);
  ostringstream buf;
  buf << "TvFunc::const_one(" << ni << ")";
  check_func(f0, mValues, buf.str());
}

TEST_P(TvFuncTestWithParam, literal1)
{
  ymuint ni = GetParam();
  ymuint ni_exp = 1U << ni;
  for (ymuint i = 0; i < ni; ++ i) {
    TvFunc f1 = TvFunc::literal(ni, VarId(i), false);
    for (ymuint p = 0; p < ni_exp; ++ p) {
      if ( p & (1U << i) ) {
	mValues[p] = 1;
      }
      else {
	mValues[p] = 0;
      }
    }
    ostringstream buf1;
    buf1 << "TvFunc::literal(" << ni << ", " << i << ", false)";
    check_func(f1, mValues, buf1.str());

    TvFunc f2 = TvFunc::literal(ni, VarId(i), true);
    for (ymuint p = 0; p < ni_exp; ++ p) {
      if ( p & (1U << i) ) {
	mValues[p] = 0;
      }
      else {
	mValues[p] = 1;
      }
    }
    ostringstream buf2;
    buf2 << "TvFunc::literal(" << ni << ", " << i << ", true)";
    check_func(f2, mValues, buf2.str());
  }
}

TEST_P(TvFuncTestWithParam, literal2)
{
  ymuint ni = GetParam();
  ymuint ni_exp = 1U << ni;
  for (ymuint i = 0; i < ni; ++ i) {
    Literal lit(VarId(i), false);
    TvFunc f1 = TvFunc::literal(ni, lit);
    for (ymuint p = 0; p < ni_exp; ++ p) {
      if ( p & (1U << i) ) {
	mValues[p] = 1;
      }
      else {
	mValues[p] = 0;
      }
    }
    ostringstream buf1;
    buf1 << "TvFunc::literal(" << ni << ", Literal(" << i << ", false))";
    check_func(f1, mValues, buf1.str());

    lit = Literal(VarId(i), true);
    TvFunc f2 = TvFunc::literal(ni, lit);
    for (ymuint p = 0; p < ni_exp; ++ p) {
      if ( p & (1U << i) ) {
	mValues[p] = 0;
      }
      else {
	mValues[p] = 1;
      }
    }
    ostringstream buf2;
    buf2 << "TvFunc::literal(" << ni << ", Literal(" << i << ", true))";
    check_func(f2, mValues, buf2.str());
  }
}

TEST_P(TvFuncTestWithParam, posi_literal)
{
  ymuint ni = GetParam();
  ymuint ni_exp = 1U << ni;
  for (ymuint i = 0; i < ni; ++ i) {
    TvFunc f1 = TvFunc::posi_literal(ni, VarId(i));
    for (ymuint p = 0; p < ni_exp; ++ p) {
      if ( p & (1U << i) ) {
	mValues[p] = 1;
      }
      else {
	mValues[p] = 0;
      }
    }
    ostringstream buf1;
    buf1 << "TvFunc::posi_literal(" << ni << ", " << i << ")";
    check_func(f1, mValues, buf1.str());
  }
}

TEST_P(TvFuncTestWithParam, nega_literal)
{
  ymuint ni = GetParam();
  ymuint ni_exp = 1U << ni;
  for (ymuint i = 0; i < ni; ++ i) {
    TvFunc f1 = TvFunc::nega_literal(ni, VarId(i));
    for (ymuint p = 0; p < ni_exp; ++ p) {
      if ( p & (1U << i) ) {
	mValues[p] = 0;
      }
      else {
	mValues[p] = 1;
      }
    }
    ostringstream buf1;
    buf1 << "TvFunc::posi_literal(" << ni << ", " << i << ")";
    check_func(f1, mValues, buf1.str());
  }
}

TEST(TvFuncTest, check_op1)
{
  vector<int> values1(4);
  vector<int> values2(4);
  for (ymuint v1 = 0; v1 < 16; ++ v1) {
    for (ymuint p = 0; p < 4; ++ p) {
      if ( v1 & (1 << p) ) {
	values1[p] = 1;
      }
      else {
	values1[p] = 0;
      }
    }
    TvFunc func1(2, values1);
    for (ymuint v2 = 0; v2 < 16; ++ v2) {
      for (ymuint p = 0; p < 4; ++ p) {
	if ( v2 & (1 << p) ) {
	  values2[p] = 1;
	}
	else {
	  values2[p] = 0;
	}
      }
      TvFunc func2(2, values2);
      check_op(func1, func2);
    }
  }
}

TEST_P(TvFuncTestWithParam, check_op2)
{
  RandGen rg;
  ymuint ni = GetParam();
  ymuint ni_exp = 1 << ni;
  vector<int> values1(ni_exp, 0);
  vector<int> values2(ni_exp, 0);
  for (ymuint n = 0; n < 100; ++ n) {
    for (ymuint p = 0; p < ni_exp; ++ p) {
      if ( rg.real1() > 0.5 ) {
	values1[p] = 1;
      }
      if ( rg.real1() > 0.5 ) {
	values2[p] = 1;
      }
    }
    TvFunc func1(ni, values1);
    TvFunc func2(ni, values2);
    check_op(func1, func2);
  }
}

INSTANTIATE_TEST_CASE_P(Test0to20,
			TvFuncTestWithParam,
			::testing::Range(0, 21));

END_NAMESPACE_YM
