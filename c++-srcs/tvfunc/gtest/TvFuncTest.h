#ifndef TVFUNCTEST_H
#define TVFUNCTEST_H

/// @file TvFuncTest.h
/// @brief TvFuncTest のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/TvFunc.h"
#include <random>


BEGIN_NAMESPACE_YM

const bool debug = false;

//////////////////////////////////////////////////////////////////////
/// @class TvFuncTestWithParam TvFuncTestWithParam.h "TvFuncTestWithParam.h"
/// @brief 入力数をパラメータに持つ TvFunc 用のテストフィクスチャ
//////////////////////////////////////////////////////////////////////
class TvFuncTestWithParam :
public ::testing::TestWithParam<int>
{
public:

  // コンストラクタ
  TvFuncTestWithParam(
  ) : mRandDist(0, 1)
  {
  }

  void
  init_values()
  {
    auto ni = GetParam();
    auto ni_exp = 1 << ni;
    mValues.clear();
    mValues.resize(ni_exp, 0);
  }

  // 網羅的なテスト
  void
  check_func(
    const TvFunc& func,
    const vector<int>& exp_values,
    bool fast,
    const string& str = string()
  )
  {
    // value() のテスト
    // 同時に0と1の数も数えておく．
    auto ni = func.input_num();
    auto ni_exp = 1U << ni;
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

    // is_zero(), is_one() のテスト
    EXPECT_EQ( n1 == 0, func.is_zero() ) << str;
    EXPECT_EQ( n0 == 0, func.is_one() ) << str;

    // check_sup() のテスト
    for ( SizeType i = 0; i < ni; ++ i ) {
      int bit = 1 << i;
      bool sup = false;
      for ( int p: Range(ni_exp) ) {
	if ( p & bit ) {
	  if ( func.value(p) != func.value(p ^ bit) ) {
	    sup = true;
	    break;
	  }
	}
      }
      EXPECT_EQ( sup, func.check_sup(i) );
    }

    // check_unate() のテスト
    for ( SizeType i = 0; i < ni; ++ i ) {
      int bit = 1 << i;
      bool p_unate = true;
      bool n_unate = true;
      for ( int p: Range(ni_exp) ) {
	if ( p & bit ) {
	  auto p0 = func.value(p ^ bit);
	  auto p1 = func.value(p);
	  auto pc = p0 & p1;
	  if ( p0 < p1 ) {
	    n_unate = false;
	  }
	  if ( p0 > p1 ) {
	    p_unate = false;
	  }
	}
      }
      int unate = 0;
      if ( p_unate ) {
	unate |= 1;
      }
      if ( n_unate ) {
	unate |= 2;
      }
      if ( unate != func.check_unate(i) ) {
	cout << "VarId: " << i << endl;
	func.print(cout);
	cout << endl;
	cout << "F0: " << func.cofactor(i, true) << endl
	     << "F1: " << func.cofactor(i, false) << endl;
      }
      EXPECT_EQ( unate, func.check_unate(i) );
    }

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
      EXPECT_EQ( exp_w1, func.walsh_1(i) );
      EXPECT_EQ( exp_w1, vec[i] );
      if ( ni < 14 ) {
	EXPECT_EQ( exp_w1, vec1[i] );
      }
    }

    if ( ni < 14 ) {
      // walsh_2() のテスト
      for ( int i1: Range(ni) ) {
	int i1_bit = 1U << i1;
	for ( int i2: Range(i1 + 1, ni) ) {
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
	  EXPECT_EQ( exp_w2, func.walsh_2(i1, i2) );
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
	      EXPECT_EQ( ww1[j][w], func.walsh_w1(j, w, oinv, ibits) );
	    }
	  }
	  if ( debug ) {
	    cout << endl;
	  }
	}
      }
    }

    // dump/restore のテスト
    ostringstream obuf;
    BinEnc bos{obuf};
    func.dump(bos);

    istringstream ibuf{obuf.str()};
    BinDec bis{ibuf};
    TvFunc func2;
    func2.restore(bis);

    EXPECT_EQ( func2, func );
  }


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

END_NAMESPACE_YM

#endif // TVFUNCTEST_H
