
/// @file TvFuncTest.cc
/// @brief TvFuncTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.


#include "gtest/gtest.h"
#include "ym/TvFunc.h"
#include "ym/NpnMap.h"
#include "ym/RandPermGen.h"
#include "ym/Range.h"
#include <random>


BEGIN_NAMESPACE_YM

bool debug = false;

class TvFuncTestWithParam :
  public ::testing::TestWithParam<int>
{
public:

  // コンストラクタ
  TvFuncTestWithParam();

  void
  init_values()
  {
    int ni = GetParam();
    int ni_exp = 1 << ni;
    mValues.clear();
    mValues.resize(ni_exp, 0);
  }

  void
  check_func(const TvFunc& func,
	     const vector<int>& exp_values,
	     bool fast,
	     const string& str = string());

public:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ベクタの値を格納するバッファ
  vector<int> mValues;

  // 乱数生成器
  std::mt19937 mRandGen;

  // 0, 1 の一様分布生成器
  std::uniform_int_distribution<int> mRandDist;

};


// コンストラクタ
TvFuncTestWithParam::TvFuncTestWithParam() :
  mRandDist(0, 1)
{
}

void
TvFuncTestWithParam::check_func(const TvFunc& func,
				const vector<int>& exp_values,
				bool fast,
				const string& str)
{
  // value() のテスト
  // 同時に0と1の数も数えておく．
  int ni = func.input_num();
  int ni_exp = 1U << ni;
  int n0 = 0;
  int n1 = 0;
  for ( int p: Range(ni_exp) ) {
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

  // invert_int() のテスト
  TvFunc fn(func);
  fn.invert_int();
  for ( int p: Range(ni_exp) ) {
    int exp_value = 1 - exp_values[p];
    EXPECT_EQ( exp_value, fn.value(p) ) << str;
  }

  if ( fast ) {
    return;
  }

  // walsh_01() のテスト
  int vec[20];
  int w0 = func.walsh_01(vec);

  // walsh_012() のテスト
  int vec1[20];
  int vec2[20 * 20];
  int w0_2;
  if ( ni < 14 ) {
    w0_2 = func.walsh_012(vec1, vec2);
  }

  // walsh_0() のテスト
  int exp_w0 = 0;
  for ( int p: Range(ni_exp) ) {
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
  if ( ni < 14 ) {
    EXPECT_EQ( exp_w0, w0_2 );
  }

  // walsh_1() のテスト
  for ( int i: Range(ni) ) {
    VarId vid(i);
    int i_bit = 1U << i;
    int exp_w1 = 0;
    for ( int p: Range(ni_exp) ) {
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
    if ( ni < 14 ) {
      EXPECT_EQ( exp_w1, vec1[i] );
    }
  }

  if ( ni < 14 ) {
    // walsh_2() のテスト
    for ( int i1: Range(ni) ) {
      VarId vid1(i1);
      int i1_bit = 1U << i1;
      for ( int i2: Range(i1 + 1, ni) ) {
	VarId vid2(i2);
	int i2_bit = 1U << i2;
	int exp_w2 = 0;
	for ( int p: Range(ni_exp) ) {
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
  // ランダムな極性割り当てを試す．
  int n = ni < 14 ? 10 : 1;
  for ( int c: Range(n) ) {
    for ( int oinv: {0, 1} ) {
      int ibits = 0U;
      for ( int j: Range(ni) ) {
	if ( mRandDist(mRandGen) ) {
	  ibits |= (1U << j);
	}
      }
      if ( c == 0 ) {
	ibits = 0U;
      }
      if ( debug ) {
	cout << "oinv = " << oinv << endl;
	cout << "ibits = ";
	for (int i = 0; i < ni; ++ i) {
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
      for ( int j: Range(ni + 1) ) {
	ww0[j] = 0;
      }
      int ww1[20][21];
      if ( ni < 14 ) {
	for ( int j: Range(ni) ) {
	  for (int k: Range(ni + 1) ) {
	    ww1[j][k] = 0;
	  }
	}
      }
      for ( int p: Range(ni_exp) ) {
	int q = p ^ ibits;
	int cur_w = 0;
	for ( int j: Range(ni) ) {
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
	if ( ni < 14 ) {
	  for ( int j: Range(ni) ) {
	    int i_bit = 1U << j;
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
      for ( int w: Range(ni + 1) ) {
	if ( debug ) {
	  cout << "  w = " << w << endl;
	}
	EXPECT_EQ( ww0[w], func.walsh_w0(w, oinv, ibits) );
	if ( ni < 14 ) {
	  for ( int j: Range(ni) ) {
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
  int ni = func1.input_num();
  int ni_exp = 1U << ni;
  vector<int> exp_and(ni_exp);
  vector<int> exp_or(ni_exp);
  vector<int> exp_xor(ni_exp);
  for ( int p: Range(ni_exp) ) {
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
  int ni = GetParam();
  TvFunc f(ni);
  EXPECT_EQ( ni, f.input_num() );
}

TEST_P(TvFuncTestWithParam, empty_constr)
{
  init_values();
  int ni = GetParam();
  TvFunc f0(ni);
  ostringstream buf;
  buf << "TvFunc(" << ni << ")";
  check_func(f0, mValues, false, buf.str());
}

TEST_P(TvFuncTestWithParam, const_zero)
{
  init_values();
  int ni = GetParam();
  TvFunc f0 = TvFunc::make_zero(ni);
  ostringstream buf;
  buf << "TvFunc::make_zero(" << ni << ")";
  check_func(f0, mValues, false, buf.str());
}

TEST_P(TvFuncTestWithParam, const_one)
{
  init_values();
  int ni = GetParam();
  int ni_exp = 1U << ni;
  for ( int p: Range(ni_exp) ) {
    mValues[p] = 1;
  }
  TvFunc f0 = TvFunc::make_one(ni);
  ostringstream buf;
  buf << "TvFunc::make_one(" << ni << ")";
  check_func(f0, mValues, false, buf.str());
}

TEST_P(TvFuncTestWithParam, literal1)
{
  init_values();
  int ni = GetParam();
  int ni_exp = 1U << ni;
  for ( int i: Range(ni) ) {
    TvFunc f1 = TvFunc::make_literal(ni, VarId(i), false);
    for ( int p: Range(ni_exp) ) {
      if ( p & (1U << i) ) {
	mValues[p] = 1;
      }
      else {
	mValues[p] = 0;
      }
    }
    ostringstream buf1;
    buf1 << "TvFunc::make_literal(" << ni << ", " << i << ", false)";
    check_func(f1, mValues, false, buf1.str());

    TvFunc f2 = TvFunc::make_literal(ni, VarId(i), true);
    for ( int p: Range(ni_exp) ) {
      if ( p & (1U << i) ) {
	mValues[p] = 0;
      }
      else {
	mValues[p] = 1;
      }
    }
    ostringstream buf2;
    buf2 << "TvFunc::make_literal(" << ni << ", " << i << ", true)";
    check_func(f2, mValues, false, buf2.str());
  }
}

TEST_P(TvFuncTestWithParam, literal2)
{
  init_values();
  int ni = GetParam();
  int ni_exp = 1U << ni;
  for ( int i: Range(ni) ) {
    Literal lit(VarId(i), false);
    TvFunc f1 = TvFunc::make_literal(ni, lit);
    for ( int p: Range(ni_exp) ) {
      if ( p & (1U << i) ) {
	mValues[p] = 1;
      }
      else {
	mValues[p] = 0;
      }
    }
    ostringstream buf1;
    buf1 << "TvFunc::make_literal(" << ni << ", Literal(" << i << ", false))";
    check_func(f1, mValues, true, buf1.str());

    lit = Literal(VarId(i), true);
    TvFunc f2 = TvFunc::make_literal(ni, lit);
    for ( int p: Range(ni_exp) ) {
      if ( p & (1U << i) ) {
	mValues[p] = 0;
      }
      else {
	mValues[p] = 1;
      }
    }
    ostringstream buf2;
    buf2 << "TvFunc::make_literal(" << ni << ", Literal(" << i << ", true))";
    check_func(f2, mValues, true, buf2.str());
  }
}

TEST_P(TvFuncTestWithParam, posi_literal)
{
  init_values();
  int ni = GetParam();
  int ni_exp = 1U << ni;
  for ( int i: Range(ni) ) {
    TvFunc f1 = TvFunc::make_posi_literal(ni, VarId(i));
    for ( int p: Range(ni_exp) ) {
      if ( p & (1U << i) ) {
	mValues[p] = 1;
      }
      else {
	mValues[p] = 0;
      }
    }
    ostringstream buf1;
    buf1 << "TvFunc::posi_literal(" << ni << ", " << i << ")";
    check_func(f1, mValues, true, buf1.str());
  }
}

TEST_P(TvFuncTestWithParam, nega_literal)
{
  init_values();
  int ni = GetParam();
  int ni_exp = 1U << ni;
  for ( int i: Range(ni) ) {
    TvFunc f1 = TvFunc::make_nega_literal(ni, VarId(i));
    for ( int p: Range(ni_exp) ) {
      if ( p & (1U << i) ) {
	mValues[p] = 0;
      }
      else {
	mValues[p] = 1;
      }
    }
    ostringstream buf1;
    buf1 << "TvFunc::posi_literal(" << ni << ", " << i << ")";
    check_func(f1, mValues, true, buf1.str());
  }
}

TEST_P(TvFuncTestWithParam, random_func)
{
  init_values();
  int ni = GetParam();
  int ni_exp = 1 << ni;
  int n = ni < 13 ? 100 : ni < 14 ? 20 : 5;
  for ( int c: Range(n) ) {
    for ( int p: Range(ni_exp) ) {
      if ( mRandDist(mRandGen) ) {
	mValues[p] = 1;
      }
    }
    TvFunc func(ni, mValues);
    bool fast = (ni > 14);
    check_func(func, mValues, fast);
  }
}

TEST(TvFuncTest, check_op1)
{
  vector<int> values1(4);
  vector<int> values2(4);
  for ( int v1: Range(14) ) {
    for ( int p: {0, 1, 2, 3} ) {
      if ( v1 & (1 << p) ) {
	values1[p] = 1;
      }
      else {
	values1[p] = 0;
      }
    }
    TvFunc func1(2, values1);
    for ( int v2: Range(14) ) {
      for ( int p: {0, 1, 2, 3} ) {
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
  int ni = GetParam();
  int ni_exp = 1 << ni;
  vector<int> values1(ni_exp, 0);
  vector<int> values2(ni_exp, 0);
  int n = ni < 19 ? 100 : 1;
  for ( int c: Range(n) ) {
    for ( int p: Range(ni_exp) ) {
      if ( mRandDist(mRandGen) ) {
	values1[p] = 1;
      }
      if ( mRandDist(mRandGen) ) {
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
  int ni = GetParam();
  int ni_exp = 1 << ni;
  vector<int> values(ni_exp, 0);
  int n = ni < 17 ? 100 : ni < 19 ? 10 : 1;
  for ( int c: Range(n) ) {
    for ( int i: Range(ni) ) {
      int i_bit = 1U << i;
      // i 番目の変数を独立にする．
      for ( int p: Range(ni_exp) ) {
	if ( p & i_bit ) {
	  values[p] = values[p ^ i_bit];
	}
	else if ( mRandDist(mRandGen) ) {
	  values[p] = 1;
	}
	else {
	  values[p] = 0;
	}
      }
      TvFunc func(ni, values);
      for ( int j: Range(ni) ) {
	bool exp_ans = false;
	int j_bit = 1U << j;
	for ( int p: Range(ni_exp) ) {
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
  int ni = GetParam();
  int ni_exp = 1 << ni;
  vector<int> values(ni_exp, 0);
  int n = ni < 19 ? 200 : 2;
  for ( int c: Range(n) ) {
    int i_bits = 0;
    for ( int i: Range(ni) ) {
      if ( mRandDist(mRandGen) ) {
	i_bits |= (1U << i);
      }
    }
    // i_bits に含まれる変数を独立にする．
    for ( int p: Range(ni_exp) ) {
      if ( p & i_bits ) {
	values[p] = values[p & ~i_bits];
      }
      else if ( mRandDist(mRandGen) ) {
	values[p] = 1;
      }
      else {
	values[p] = 0;
      }
    }
    TvFunc func(ni, values);
    for ( int j: Range(ni) ) {
      bool exp_ans = false;
      int j_bit = 1U << j;
      for ( int p: Range(ni_exp) ) {
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

int
check_sym(const vector<int>& values,
	  int i1,
	  int i2)
{
  int sym0 = 1U;
  int sym1 = 2U;
  int i1_bit = 1U << i1;
  int i2_bit = 1U << i2;
  int ni_exp = values.size();
  for ( int p: Range(ni_exp) ) {
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
  int ni = GetParam();
  if ( ni > 15 ) {
    return;
  }
  std::uniform_int_distribution<int> rd(0, ni - 1);
  int ni_exp = 1 << ni;
  vector<int> values(ni_exp, 0);
  int n1 = ni < 11 ? 50 : 10;
  int n2 = ni < 11 ? 50 : 10;
  for ( int c1: Range(n1) ) {
    for ( int i1: Range(ni) ) {
      int i1_bit = 1U << i1;
      for ( int i2: Range(i1 + 1, ni) ) {
	int i2_bit = 1U << i2;
	for ( int inv: {0, 1, 2, 3} ) {
	  for ( int p: Range(ni_exp) ) {
	    if ( (inv & 1) == 1 && (p & i1_bit) == 0U && (p & i2_bit) == i2_bit ) {
	      values[p] = values[p ^ i1_bit ^ i2_bit];
	    }
	    else if ( (inv & 2) == 2 && (p & i1_bit) == i1_bit && (p & i2_bit) == i2_bit ) {
	      values[p] = values[p ^ i1_bit ^ i2_bit];
	    }
	    else if ( mRandDist(mRandGen) ) {
	      values[p] = 1;
	    }
	    else {
	      values[p] = 0;
	    }
	  }
	  TvFunc func(ni, values);
	  int exp_sym = check_sym(values, i1, i2);
	  bool exp_sym0 = exp_sym & 1;
	  bool exp_sym1 = (exp_sym >> 1) & 1;
	  EXPECT_EQ( exp_sym0, func.check_sym(VarId(i1), VarId(i2), false) );
	  EXPECT_EQ( exp_sym0, func.check_sym(VarId(i2), VarId(i1), false) );
	  EXPECT_EQ( exp_sym1, func.check_sym(VarId(i1), VarId(i2), true) );
	  EXPECT_EQ( exp_sym1, func.check_sym(VarId(i2), VarId(i1), true) );
	  for ( int c2: Range(n2) ) {
	    int j1 = rd(mRandGen);
	    int j2 = rd(mRandGen);
	    if ( j1 == j2 ) {
	      continue;
	    }
	    int exp_sym = check_sym(values, j1, j2);
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
  int ni = GetParam();
  int ni_exp = 1 << ni;
  vector<int> values(ni_exp, 0);
  int n = ni < 17 ? 100 : ni < 19 ? 10 : 1;
  for ( int c: Range(n) ) {
    for ( int p: Range(ni_exp) ) {
      if ( mRandDist(mRandGen) ) {
	values[p] = 1;
      }
      else {
	values[p] = 0;
      }
    }
    TvFunc func(ni, values);
    for ( int i: Range(ni) ) {
      VarId var(i);
      TvFunc func0 = func.cofactor(var, true);
      TvFunc func1 = func.cofactor(var, false);
      vector<int> values0(ni_exp, 0);
      vector<int> values1(ni_exp, 0);
      int i_bit = 1U << i;
      for ( int p: Range(ni_exp) ) {
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
      cfunc0.cofactor_int(var, true);
      EXPECT_EQ( exp_func0, cfunc0 );

      TvFunc cfunc1(func);
      cfunc1.cofactor_int(var, false);
      EXPECT_EQ( exp_func1, cfunc1 );
    }
  }
}

TEST_P(TvFuncTestWithParam, xform)
{
  int ni = GetParam();
  int ni_exp = 1 << ni;
  vector<int> values(ni_exp, 0);
  int n1 = ni < 12 ? 30 : ni < 19 ? 5 : 1;
  int n2 = ni < 15 ? 30 : 5;
  for ( int c1: Range(n1) ) {
    for ( int p: Range(ni_exp) ) {
      if ( mRandDist(mRandGen) ) {
	values[p] = 1;
      }
      else {
	values[p] = 0;
      }
    }
    TvFunc func(ni, values);
    RandPermGen rpg(ni);
    for ( int c2: Range(n2) ) {
      bool oinv = mRandDist(mRandGen) ? true : false;
      NpnMap map(ni);
      map.set_oinv(oinv);
      rpg.generate(mRandGen);
      for ( int i: Range(ni) ) {
	bool iinv = mRandDist(mRandGen) ? true : false;
	map.set(VarId(i), VarId(rpg.elem(i)), iinv);
      }
      TvFunc xfunc = func.xform(map);

      vector<int> exp_values(ni_exp);
      for ( int p: Range(ni_exp) ) {
	int q = 0;
	for ( int i: Range(ni) ) {
	  NpnVmap vmap = map.imap(VarId(i));
	  int j = vmap.var().val();
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

TEST(TvFuncTest, shrink_0)
{
  int ni = 10;
  TvFunc func0 = TvFunc::make_zero(ni);

  NpnMap map = func0.shrink_map();
  TvFunc func1 = func0.xform(map);

  EXPECT_EQ( TvFunc::make_zero(0), func1 );

  EXPECT_EQ( ni, map.input_num() );
  for ( int i: Range(ni) ) {
    NpnVmap imap = map.imap(VarId(i));
    EXPECT_TRUE( imap.is_invalid() );
  }
}

TEST(TvFuncTest, shrink_1)
{
  int ni = 10;
  TvFunc func0 = TvFunc::make_one(ni);

  NpnMap map = func0.shrink_map();
  TvFunc func1 = func0.xform(map);

  EXPECT_EQ( TvFunc::make_one(0), func1 );

  EXPECT_EQ( ni, map.input_num() );
  for ( int i: Range(ni) ) {
    NpnVmap imap = map.imap(VarId(i));
    EXPECT_TRUE( imap.is_invalid() );
  }
}

TEST(TvFuncTest, shrink_lit1)
{
  int ni = 10;
  TvFunc func0 = TvFunc::make_literal(ni, VarId(0), false);

  NpnMap map = func0.shrink_map();;
  TvFunc func1 = func0.xform(map);

  EXPECT_EQ( TvFunc::make_literal(1, VarId(0), false), func1);

  EXPECT_EQ( ni, map.input_num() );

  {
    NpnVmap imap = map.imap(VarId(0));
    EXPECT_FALSE( imap.is_invalid() );
    EXPECT_EQ( VarId(0), imap.var() );
    EXPECT_FALSE( imap.inv() );
  }

  for ( int i: Range(1, ni) ) {
    NpnVmap imap = map.imap(VarId(i));
    EXPECT_TRUE( imap.is_invalid() );
  }
}

TEST(TvFuncTest, shrink_lit2)
{
  int ni = 10;
  TvFunc func0 = TvFunc::make_literal(ni, VarId(2), false);

  NpnMap map = func0.shrink_map();
  TvFunc func1 = func0.xform(map);

  EXPECT_EQ( TvFunc::make_literal(1, VarId(0), false), func1);

  EXPECT_EQ( ni, map.input_num() );

  {
    NpnVmap imap = map.imap(VarId(2));
    EXPECT_FALSE( imap.is_invalid() );
    EXPECT_EQ( VarId(0), imap.var() );
    EXPECT_FALSE( imap.inv() );
  }

  for ( int i: Range(ni) ) {
    if ( i == 2 ) {
      continue;
    }
    NpnVmap imap = map.imap(VarId(i));
    EXPECT_TRUE( imap.is_invalid() );
  }
}

TEST(TvFuncTest, shrink_lit3)
{
  int ni = 10;
  TvFunc lit1 = TvFunc::make_literal(ni, VarId(2), false);
  TvFunc lit2 = TvFunc::make_literal(ni, VarId(5), false);
  TvFunc func0 = lit1 & ~lit2;

  NpnMap map = func0.shrink_map();
  TvFunc func1 = func0.xform(map);

  TvFunc func2;
  {
    TvFunc lit1 = TvFunc::make_literal(2, VarId(0), false);
    TvFunc lit2 = TvFunc::make_literal(2, VarId(1), false);
    func2 = lit1 & ~lit2;
  }

  EXPECT_EQ( func2, func1);

  EXPECT_EQ( ni, map.input_num() );

  {
    NpnVmap imap = map.imap(VarId(2));
    EXPECT_FALSE( imap.is_invalid() );
    EXPECT_EQ( VarId(0), imap.var() );
    EXPECT_FALSE( imap.inv() );
  }

  {
    NpnVmap imap = map.imap(VarId(5));
    EXPECT_FALSE( imap.is_invalid() );
    EXPECT_EQ( VarId(1), imap.var() );
    EXPECT_FALSE( imap.inv() );
  }

  for ( int i: Range(ni) ) {
    if ( i == 2 || i == 5 ) {
      continue;
    }
    NpnVmap imap = map.imap(VarId(i));
    EXPECT_TRUE( imap.is_invalid() );
  }
}

TEST(TvFuncTest, expand_0)
{
  TvFunc func0 = TvFunc::make_zero(0);

  NpnMap map(0, 10);
  TvFunc func1 = func0.xform(map);

  EXPECT_EQ( TvFunc::make_zero(10), func1 );
}

TEST(TvFuncTest, expand_1)
{
  TvFunc func0 = TvFunc::make_one(0);

  NpnMap map(0, 10);
  TvFunc func1 = func0.xform(map);

  EXPECT_EQ( TvFunc::make_one(10), func1 );
}

TEST(TvFuncTest, expand_lit1)
{
  TvFunc func0 = TvFunc::make_literal(1, VarId(0), false);

  NpnMap map(1, 10);
  map.set(VarId(0), VarId(0), false);
  TvFunc func1 = func0.xform(map);

  EXPECT_EQ( TvFunc::make_literal(10, VarId(0), false), func1 );
}

TEST(TvFuncTest, expand_lit2)
{
  TvFunc func0 = TvFunc::make_literal(2, VarId(1), false);

  NpnMap map(2, 10);
  map.set(VarId(1), VarId(2), false);
  TvFunc func1 = func0.xform(map);

  EXPECT_EQ( TvFunc::make_literal(10, VarId(2), false), func1 );
}

INSTANTIATE_TEST_CASE_P(Test0to20,
			TvFuncTestWithParam,
			::testing::Range(0, 21));

END_NAMESPACE_YM
