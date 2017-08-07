
/// @file TvFuncTest.cc
/// @brief TvFuncTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.


#include "gtest/gtest.h"
#include "ym/TvFunc.h"
#include "ym/NpnMap.h"
#include "ym/RandGen.h"
#include "ym/RandPermGen.h"


BEGIN_NAMESPACE_YM

bool debug = false;

class TvFuncTestWithParam :
  public ::testing::TestWithParam<int>
{
public:

  void
  init_values()
  {
    ymuint ni = GetParam();
    ymuint ni_exp = 1 << ni;
    mValues.clear();
    mValues.resize(ni_exp, 0);
  }

public:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ベクタの値を格納するバッファ
  vector<int> mValues;

};


void
check_func(const TvFunc& func,
	   const vector<int>& exp_values,
	   const string& str = string())
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
  int w0_2;
  if ( ni < 16 ) {
    w0_2 = func.walsh_012(vec1, vec2);
  }

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
  if ( ni < 16 ) {
    EXPECT_EQ( exp_w0, w0_2 );
  }

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
    if ( ni < 16 ) {
      EXPECT_EQ( exp_w1, vec1[i] );
    }
  }

  if ( ni < 16 ) {
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
  }

  if ( debug ) {
    cout << "func:";
    func.print(cout);
    cout << endl;
  }
  // walsh_w0(), walsh_w1() のテスト
  RandGen rg;
  // 10回ランダムな極性割り当てを試す．
  for (ymuint c = 0; c < 10; ++ c) {
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
      if ( debug ) {
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
      int ww1[20][21];
      if ( ni < 16 ) {
	for (ymuint j = 0; j < ni; ++ j) {
	  for (ymuint k = 0; k <= ni; ++ k) {
	    ww1[j][k] = 0;
	  }
	}
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
	if ( ni < 16 ) {
	  for (ymuint j = 0; j < ni; ++ j) {
	    ymuint i_bit = 1U << j;
	    int v1 = v;
	    if ( q & i_bit ) {
	      v1 = 1 - v1;
	    }
	    if ( v1 ) {
	      ww1[j][cur_w] -= 1;
	    }
	    else {
	      ww1[j][cur_w] += 1;
	    }
	  }
	}
      }
      for (ymuint w = 0; w <= ni; ++ w) {
	if ( debug ) {
	  cout << "  w = " << w << endl;
	}
	EXPECT_EQ( ww0[w], func.walsh_w0(w, oinv, ibits) );
	if ( ni < 16 ) {
	  for (ymuint j = 0; j < ni; ++ j) {
	    VarId var(j);
	    EXPECT_EQ( ww1[j][w], func.walsh_w1(var, w, oinv, ibits) );
	  }
	}
	if ( debug ) {
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
  vector<int> exp_and(ni_exp);
  vector<int> exp_or(ni_exp);
  vector<int> exp_xor(ni_exp);
  for (ymuint p = 0; p < ni_exp; ++ p) {
    int v1 = func1.value(p);
    int v2 = func2.value(p);
    exp_and[p] = v1 & v2;
    exp_or[p]  = v1 | v2;
    exp_xor[p] = v1 ^ v2;
  }
  TvFunc f_and = func1 & func2;
  TvFunc f_or  = func1 | func2;
  TvFunc f_xor = func1 ^ func2;

  TvFunc exp_f_and(ni, exp_and);
  EXPECT_EQ( exp_f_and, f_and );

  TvFunc exp_f_or(ni, exp_or);
  EXPECT_EQ( exp_f_or, f_or );

  TvFunc exp_f_xor(ni, exp_xor);
  EXPECT_EQ( exp_f_xor, f_xor );

  TvFunc f_and2(func1);
  f_and2 &= func2;
  EXPECT_EQ( exp_f_and, f_and2 );

  TvFunc f_or2(func1);
  f_or2 |= func2;
  EXPECT_EQ( exp_f_or, f_or2 );

  TvFunc f_xor2(func1);
  f_xor2 ^= func2;
  EXPECT_EQ( exp_f_xor, f_xor2 );
}

TEST_P(TvFuncTestWithParam, input_num)
{
  ymuint ni = GetParam();
  TvFunc f(ni);
  EXPECT_EQ( ni, f.input_num() );
}

TEST_P(TvFuncTestWithParam, empty_constr)
{
  init_values();
  ymuint ni = GetParam();
  TvFunc f0(ni);
  ostringstream buf;
  buf << "TvFunc(" << ni << ")";
  check_func(f0, mValues, buf.str());
}

TEST_P(TvFuncTestWithParam, const_zero)
{
  init_values();
  ymuint ni = GetParam();
  TvFunc f0 = TvFunc::const_zero(ni);
  ostringstream buf;
  buf << "TvFunc::const_zero(" << ni << ")";
  check_func(f0, mValues, buf.str());
}

TEST_P(TvFuncTestWithParam, const_one)
{
  init_values();
  ymuint ni = GetParam();
  ymuint ni_exp = 1U << ni;
  for (ymuint p = 0; p < ni_exp; ++ p) {
    mValues[p] = 1;
  }
  TvFunc f0 = TvFunc::const_one(ni);
  ostringstream buf;
  buf << "TvFunc::const_one(" << ni << ")";
  check_func(f0, mValues, buf.str());
}

TEST_P(TvFuncTestWithParam, literal1)
{
  init_values();
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
  init_values();
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
  init_values();
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
  init_values();
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

TEST_P(TvFuncTestWithParam, random_func)
{
  RandGen rg;
  init_values();
  ymuint ni = GetParam();
  ymuint ni_exp = 1 << ni;
  for (ymuint n = 0; n < 100; ++ n) {
    for (ymuint p = 0; p < ni_exp; ++ p) {
      if ( rg.real1() > 0.5 ) {
	mValues[p] = 1;
      }
    }
    TvFunc func(ni, mValues);
    check_func(func, mValues);
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

TEST_P(TvFuncTestWithParam, check_sup1)
{
  RandGen rg;
  ymuint ni = GetParam();
  ymuint ni_exp = 1 << ni;
  vector<int> values(ni_exp, 0);
  for (ymuint n = 0; n < 100; ++ n) {
    for (ymuint i = 0; i < ni; ++ i) {
      ymuint i_bit = 1U << i;
      // i 番目の変数を独立にする．
      for (ymuint p = 0; p < ni_exp; ++ p) {
	if ( p & i_bit ) {
	  values[p] = values[p ^ i_bit];
	}
	else if ( rg.real1() > 0.5 ) {
	  values[p] = 1;
	}
	else {
	  values[p] = 0;
	}
      }
      TvFunc func(ni, values);
      for (ymuint j = 0; j < ni; ++ j) {
	bool exp_ans = false;
	ymuint j_bit = 1U << j;
	for (ymuint p = 0; p < ni_exp; ++ p) {
	  if ( (p & j_bit) == j_bit ) {
	    if ( values[p] != values[p ^ j_bit] ) {
	      exp_ans = true;
	      break;
	    }
	  }
	}
	EXPECT_EQ( exp_ans, func.check_sup(VarId(j)) );
      }
    }
  }
}

TEST_P(TvFuncTestWithParam, check_sup2)
{
  RandGen rg;
  ymuint ni = GetParam();
  ymuint ni_exp = 1 << ni;
  vector<int> values(ni_exp, 0);
  for (ymuint n = 0; n < 200; ++ n) {
    ymuint i_bits = 0;
    for (ymuint i = 0; i < ni; ++ i) {
      if ( rg.real1() > 0.5 ) {
	i_bits |= (1U << i);
      }
    }
    // i_bits に含まれる変数を独立にする．
    for (ymuint p = 0; p < ni_exp; ++ p) {
      if ( p & i_bits ) {
	values[p] = values[p & ~i_bits];
      }
      else if ( rg.real1() > 0.5 ) {
	values[p] = 1;
      }
      else {
	values[p] = 0;
      }
    }
    TvFunc func(ni, values);
    for (ymuint j = 0; j < ni; ++ j) {
      bool exp_ans = false;
      ymuint j_bit = 1U << j;
      for (ymuint p = 0; p < ni_exp; ++ p) {
	if ( (p & j_bit) == j_bit ) {
	  if ( values[p] != values[p ^ j_bit] ) {
	    exp_ans = true;
	    break;
	  }
	}
      }
      EXPECT_EQ( exp_ans, func.check_sup(VarId(j)) );
    }
  }
}

ymuint
check_sym(const vector<int>& values,
	  ymuint i1,
	  ymuint i2)
{
  ymuint sym0 = 1U;
  ymuint sym1 = 2U;
  ymuint i1_bit = 1U << i1;
  ymuint i2_bit = 1U << i2;
  ymuint ni_exp = values.size();
  for (ymuint p = 0; p < ni_exp; ++ p) {
    if ( (p & i1_bit) == 0U && (p & i2_bit) == i2_bit ) {
      if ( values[p] != values[p ^ i1_bit ^ i2_bit] ) {
	sym0 = 0U;
      }
    }
    else if ( (p & i1_bit) == 0U && (p & i2_bit) == 0U ) {
      if ( values[p] != values[p ^ i1_bit ^ i2_bit] ) {
	sym1 = 0U;
      }
    }
    if ( (sym0 | sym1) == 0U ) {
      break;
    }
  }
  return sym0 | sym1;
}

TEST_P(TvFuncTestWithParam, check_sym)
{
  RandGen rg;
  ymuint ni = GetParam();
  if ( ni > 15 ) {
    return;
  }
  ymuint ni_exp = 1 << ni;
  vector<int> values(ni_exp, 0);
  ymuint c0 = 0;
  ymuint c1 = 0;
  for (ymuint n = 0; n < 50; ++ n) {
    for (ymuint i1 = 0; i1 < ni; ++ i1) {
      ymuint i1_bit = 1U << i1;
      for (ymuint i2 = i1 + 1; i2 < ni; ++ i2) {
	ymuint i2_bit = 1U << i2;
	for (ymuint inv = 0; inv < 4; ++ inv) {
	  for (ymuint p = 0; p < ni_exp; ++ p) {
	    if ( (inv & 1) == 1 && (p & i1_bit) == 0U && (p & i2_bit) == i2_bit ) {
	      values[p] = values[p ^ i1_bit ^ i2_bit];
	    }
	    else if ( (inv & 2) == 2 && (p & i1_bit) == i1_bit && (p & i2_bit) == i2_bit ) {
	      values[p] = values[p ^ i1_bit ^ i2_bit];
	    }
	    else if ( rg.real1() > 0.5 ) {
	      values[p] = 1;
	    }
	    else {
	      values[p] = 0;
	    }
	  }
	  TvFunc func(ni, values);
	  ymuint exp_sym = check_sym(values, i1, i2);
	  bool exp_sym0 = exp_sym & 1;
	  bool exp_sym1 = (exp_sym >> 1) & 1;
	  EXPECT_EQ( exp_sym0, func.check_sym(VarId(i1), VarId(i2), false) );
	  EXPECT_EQ( exp_sym0, func.check_sym(VarId(i2), VarId(i1), false) );
	  EXPECT_EQ( exp_sym1, func.check_sym(VarId(i1), VarId(i2), true) );
	  EXPECT_EQ( exp_sym1, func.check_sym(VarId(i2), VarId(i1), true) );
	  for (ymuint c = 0; c < 50; ++ c) {
	    ymuint j1 = rg.int31() % ni;
	    ymuint j2 = rg.int31() % ni;
	    if ( j1 == j2 ) {
	      continue;
	    }
	    ymuint exp_sym = check_sym(values, j1, j2);
	    bool exp_sym0 = exp_sym & 1;
	    bool exp_sym1 = (exp_sym >> 1) & 1;
	    EXPECT_EQ( exp_sym0, func.check_sym(VarId(j1), VarId(j2), false) );
	    EXPECT_EQ( exp_sym1, func.check_sym(VarId(j1), VarId(j2), true) );
	  }
	}
      }
    }
  }
}

TEST_P(TvFuncTestWithParam, cofactor)
{
  RandGen rg;
  ymuint ni = GetParam();
  ymuint ni_exp = 1 << ni;
  vector<int> values(ni_exp, 0);
  for (ymuint c = 0; c < 100; ++ c) {
    for (ymuint p = 0; p < ni_exp; ++ p) {
      if ( rg.real1() > 0.5 ) {
	values[p] = 1;
      }
      else {
	values[p] = 0;
      }
    }
    TvFunc func(ni, values);
    for (ymuint i = 0; i < ni; ++ i) {
      VarId var(i);
      TvFunc func0 = func.cofactor(var, true);
      TvFunc func1 = func.cofactor(var, false);
      vector<int> values0(ni_exp, 0);
      vector<int> values1(ni_exp, 0);
      ymuint i_bit = 1U << i;
      for (ymuint p = 0; p < ni_exp; ++ p) {
	if ( p & i_bit ) {
	  values0[p] = values[p ^ i_bit];
	  values1[p] = values[p];
	}
	else {
	  values1[p] = values[p ^ i_bit];
	  values0[p] = values[p];
	}
      }
      TvFunc exp_func0(ni, values0);
      TvFunc exp_func1(ni, values1);
      EXPECT_EQ( exp_func0, func0 );
      EXPECT_EQ( exp_func1, func1 );

      TvFunc cfunc0(func);
      cfunc0.set_cofactor(var, true);
      EXPECT_EQ( exp_func0, cfunc0 );

      TvFunc cfunc1(func);
      cfunc1.set_cofactor(var, false);
      EXPECT_EQ( exp_func1, cfunc1 );
    }
  }
}

TEST_P(TvFuncTestWithParam, xform)
{
  RandGen rg;
  ymuint ni = GetParam();
  ymuint ni_exp = 1 << ni;
  vector<int> values(ni_exp, 0);
  for (ymuint c1 = 0; c1 < 20; ++ c1) {
    for (ymuint p = 0; p < ni_exp; ++ p) {
      if ( rg.real1() > 0.5 ) {
	values[p] = 1;
      }
      else {
	values[p] = 0;
      }
    }
    TvFunc func(ni, values);
    RandPermGen rpg(ni);
    for (ymuint c2 = 0; c2 < 20; ++ c2) {
      bool oinv = (rg.real1() > 0.5);
      NpnMap map(ni, oinv);
      rpg.generate(rg);
      for (ymuint i = 0; i < ni; ++ i) {
	bool iinv = (rg.real1() > 0.5);
	map.set(VarId(i), VarId(rpg.elem(i)), iinv);
      }
      TvFunc xfunc = func.xform(map);

      vector<int> exp_values(ni_exp);
      for (ymuint p = 0; p < ni_exp; ++ p) {
	ymuint q = 0;
	for (ymuint i = 0; i < ni; ++ i) {
	  NpnVmap vmap = map.imap(VarId(i));
	  ymuint j = vmap.var().val();
	  bool iinv = vmap.inv();
	  if ( p & (1 << i) ) {
	    if ( !iinv ) {
	      q |= (1 << j);
	    }
	  }
	  else {
	    if ( iinv ) {
	      q |= (1 << j);
	    }
	  }
	}
	if ( map.oinv() ) {
	  exp_values[q] = 1 - values[p];
	}
	else {
	  exp_values[q] = values[p];
	}
      }
      TvFunc exp_xfunc(ni, exp_values);
      EXPECT_EQ( exp_xfunc, xfunc );
    }
  }
}

INSTANTIATE_TEST_CASE_P(Test0to20,
			TvFuncTestWithParam,
			::testing::Range(0, 21));

END_NAMESPACE_YM
