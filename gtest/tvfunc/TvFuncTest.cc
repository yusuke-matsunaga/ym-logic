
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

void
check_func(const TvFunc& func,
	   int exp_values[],
	   const string& str)
{
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
  EXPECT_EQ( n0, func.count_zero() ) << str;
  EXPECT_EQ( n1, func.count_one() ) << str;

  TvFunc fn(func);
  fn.negate();
  for (ymuint p = 0; p < ni_exp; ++ p) {
    int exp_value = 1 - exp_values[p];
    EXPECT_EQ( exp_value, fn.value(p) ) << str;
  }
}

void
check_op(const TvFunc& func1,
	 const TvFunc& func2)
{
  ymuint ni = func1.input_num();
  ymuint ni_exp = 1U << ni;
  TvFunc f_and = func1 & func2;
  TvFunc f_or  = func1 | func2;
  TvFunc f_xor = func1 ^ func2;
  for (ymuint p = 0; p < ni_exp; ++ p) {
    int v1 = func1.value(p);
    int v2 = func2.value(p);
    int exp_and = v1 & v2;
    EXPECT_EQ( exp_and, f_and.value(p) ) << "func1 & func2";
    int exp_or  = v1 | v2;
    EXPECT_EQ( exp_or, f_or.value(p) ) << "func1 | func2";
    int exp_xor = v1 ^ v2;
    EXPECT_EQ( exp_xor, f_xor.value(p) ) << "func1 ^ func2";
  }
}

TEST(TvFuncTest, input_num)
{
  for (ymuint ni = 0; ni < 10; ++ ni) {
    TvFunc f(ni);
    EXPECT_EQ( ni, f.input_num() );
  }
}

TEST(TvFuncTest, empty_constr)
{
  int values[1024];
  for (ymuint i = 0; i < 1024; ++ i) {
    values[i] = 0;
  }
  for (ymuint i = 0; i < 10; ++ i) {
    TvFunc f0(i);
    ostringstream buf;
    buf << "TvFunc(" << i << ")";
    check_func(f0, values, buf.str());
  }
}

TEST(TvFuncTest, const_zero)
{
  int values[1024];
  for (ymuint i = 0; i < 1024; ++ i) {
    values[i] = 0;
  }
  for (ymuint i = 0; i < 10; ++ i) {
    TvFunc f0 = TvFunc::const_zero(i);
    ostringstream buf;
    buf << "TvFunc::const_zero(" << i << ")";
    check_func(f0, values, buf.str());
  }
}

TEST(TvFuncTest, const_one)
{
  int values[1024];
  for (ymuint i = 0; i < 1024; ++ i) {
    values[i] = 1;
  }
  for (ymuint i = 0; i < 10; ++ i) {
    TvFunc f0 = TvFunc::const_one(i);
    ostringstream buf;
    buf << "TvFunc::const_one(" << i << ")";
    check_func(f0, values, buf.str());
  }
}

TEST(TvFuncTest, literal1)
{
  int values[1024];

  for (ymuint ni = 1; ni < 10; ++ ni) {
    ymuint ni_exp = 1U << ni;
    for (ymuint i = 0; i < ni; ++ i) {
      TvFunc f1 = TvFunc::literal(ni, VarId(i), false);
      for (ymuint p = 0; p < ni_exp; ++ p) {
	if ( p & (1U << i) ) {
	  values[p] = 1;
	}
	else {
	  values[p] = 0;
	}
      }
      ostringstream buf1;
      buf1 << "TvFunc::literal(" << ni << ", " << i << ", false)";
      check_func(f1, values, buf1.str());

      TvFunc f2 = TvFunc::literal(ni, VarId(i), true);
      for (ymuint p = 0; p < ni_exp; ++ p) {
	if ( p & (1U << i) ) {
	  values[p] = 0;
	}
	else {
	  values[p] = 1;
	}
      }
      ostringstream buf2;
      buf2 << "TvFunc::literal(" << ni << ", " << i << ", true)";
      check_func(f2, values, buf2.str());
    }
  }
}

TEST(TvFuncTest, literal2)
{
  int values[1024];

  for (ymuint ni = 1; ni < 10; ++ ni) {
    ymuint ni_exp = 1U << ni;
    for (ymuint i = 0; i < ni; ++ i) {
      Literal lit(VarId(i), false);
      TvFunc f1 = TvFunc::literal(ni, lit);
      for (ymuint p = 0; p < ni_exp; ++ p) {
	if ( p & (1U << i) ) {
	  values[p] = 1;
	}
	else {
	  values[p] = 0;
	}
      }
      ostringstream buf1;
      buf1 << "TvFunc::literal(" << ni << ", Literal(" << i << ", false))";
      check_func(f1, values, buf1.str());

      lit = Literal(VarId(i), true);
      TvFunc f2 = TvFunc::literal(ni, lit);
      for (ymuint p = 0; p < ni_exp; ++ p) {
	if ( p & (1U << i) ) {
	  values[p] = 0;
	}
	else {
	  values[p] = 1;
	}
      }
      ostringstream buf2;
      buf2 << "TvFunc::literal(" << ni << ", Literal(" << i << ", true))";
      check_func(f2, values, buf2.str());
    }
  }
}

TEST(TvFuncTest, posi_literal)
{
  int values[1024];

  for (ymuint ni = 1; ni < 10; ++ ni) {
    ymuint ni_exp = 1U << ni;
    for (ymuint i = 0; i < ni; ++ i) {
      TvFunc f1 = TvFunc::posi_literal(ni, VarId(i));
      for (ymuint p = 0; p < ni_exp; ++ p) {
	if ( p & (1U << i) ) {
	  values[p] = 1;
	}
	else {
	  values[p] = 0;
	}
      }
      ostringstream buf1;
      buf1 << "TvFunc::posi_literal(" << ni << ", " << i << ")";
      check_func(f1, values, buf1.str());
    }
  }
}

TEST(TvFuncTest, nega_literal)
{
  int values[1024];

  for (ymuint ni = 1; ni < 10; ++ ni) {
    ymuint ni_exp = 1U << ni;
    for (ymuint i = 0; i < ni; ++ i) {
      TvFunc f1 = TvFunc::nega_literal(ni, VarId(i));
      for (ymuint p = 0; p < ni_exp; ++ p) {
	if ( p & (1U << i) ) {
	  values[p] = 0;
	}
	else {
	  values[p] = 1;
	}
      }
      ostringstream buf1;
      buf1 << "TvFunc::posi_literal(" << ni << ", " << i << ")";
      check_func(f1, values, buf1.str());
    }
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

TEST(TvFuncTest, check_op2)
{
  RandGen rg;
  for (ymuint ni = 2; ni < 10; ++ ni) {
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
}

END_NAMESPACE_YM
