
/// @file TvFuncTest.cc
/// @brief TvFuncTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "gtest/gtest.h"
#include "ym/TvFunc.h"
#include "ym/NpnMap.h"
#include "ym/RandPermGen.h"
#include "ym/Range.h"
#include "TvFuncTest.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class TvFuncTest_base TvFuncTest_base.h "TvFuncTest_base.h"
/// @brief TvFunc の基本的なテスト
//////////////////////////////////////////////////////////////////////
class TvFuncTest_base:
  public TvFuncTestWithParam
{
public:

  /// @brief コンストラクタ
  TvFuncTest_base() = default;

  /// @brief デストラクタ
  ~TvFuncTest_base() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  // 網羅的なテスト
  void
  check_func(
    const TvFunc& func,
    const vector<int>& exp_values,
    bool fast,
    const string& str = string{}
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
    auto fn = func;
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

};

void
check_op(
  const TvFunc& func1,
  const TvFunc& func2
)
{
  auto ni = func1.input_num();
  auto ni_exp = 1U << ni;
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
  auto f_and = func1 & func2;
  auto f_or  = func1 | func2;
  auto f_xor = func1 ^ func2;

  auto exp_f_and = TvFunc(ni, exp_and);
  EXPECT_EQ( exp_f_and, f_and );

  auto exp_f_or = TvFunc(ni, exp_or);
  EXPECT_EQ( exp_f_or, f_or );

  auto exp_f_xor = TvFunc(ni, exp_xor);
  EXPECT_EQ( exp_f_xor, f_xor );

  auto f_and2 = func1;
  f_and2 &= func2;
  EXPECT_EQ( exp_f_and, f_and2 );

  auto f_or2 = func1;
  f_or2 |= func2;
  EXPECT_EQ( exp_f_or, f_or2 );

  auto f_xor2 = func1;
  f_xor2 ^= func2;
  EXPECT_EQ( exp_f_xor, f_xor2 );
}

TEST_P(TvFuncTest_base, input_num)
{
  SizeType ni = GetParam();
  auto f = TvFunc(ni);
  EXPECT_EQ( ni, f.input_num() );
}

TEST_P(TvFuncTest_base, empty_constr)
{
  init_values();
  SizeType ni = GetParam();
  auto f0 = TvFunc(ni);
  ostringstream buf;
  check_func(f0, mValues, false, buf.str());
}

TEST_P(TvFuncTest_base, const_zero)
{
  init_values();
  SizeType ni = GetParam();
  auto f0 = TvFunc::zero(ni);
  ostringstream buf;
  check_func(f0, mValues, false, buf.str());
}

TEST_P(TvFuncTest_base, const_one)
{
  init_values();
  SizeType ni = GetParam();
  auto ni_exp = 1U << ni;
  for ( int p: Range(ni_exp) ) {
    mValues[p] = 1;
  }
  auto f0 = TvFunc::one(ni);
  ostringstream buf;
  check_func(f0, mValues, false, buf.str());
}

TEST_P(TvFuncTest_base, literal1)
{
  init_values();
  SizeType ni = GetParam();
  auto ni_exp = 1U << ni;
  for ( int i: Range(ni) ) {
    auto f1 = TvFunc::literal(ni, i, false);
    for ( int p: Range(ni_exp) ) {
      if ( p & (1U << i) ) {
	mValues[p] = 1;
      }
      else {
	mValues[p] = 0;
      }
    }
    ostringstream buf1;
    buf1 << "TvFunc::literal(" << ni << ", " << i << ", false)";
    check_func(f1, mValues, false, buf1.str());

    auto f2 = TvFunc::literal(ni, i, true);
    for ( int p: Range(ni_exp) ) {
      if ( p & (1U << i) ) {
	mValues[p] = 0;
      }
      else {
	mValues[p] = 1;
      }
    }
    ostringstream buf2;
    buf2 << "TvFunc::literal(" << ni << ", " << i << ", true)";
    check_func(f2, mValues, false, buf2.str());
  }
}

TEST_P(TvFuncTest_base, literal2)
{
  init_values();
  SizeType ni = GetParam();
  auto ni_exp = 1U << ni;
  for ( int i: Range(ni) ) {
    auto f1 = TvFunc::literal(ni, i, false);
    for ( int p: Range(ni_exp) ) {
      if ( p & (1U << i) ) {
	mValues[p] = 1;
      }
      else {
	mValues[p] = 0;
      }
    }
    ostringstream buf1;
    buf1 << "TvFunc::literal(" << ni << ", Literal(" << i << ", false))";
    check_func(f1, mValues, true, buf1.str());

    auto f2 = TvFunc::literal(ni, i, true);
    for ( int p: Range(ni_exp) ) {
      if ( p & (1U << i) ) {
	mValues[p] = 0;
      }
      else {
	mValues[p] = 1;
      }
    }
    ostringstream buf2;
    buf2 << "TvFunc::literal(" << ni << ", Literal(" << i << ", true))";
    check_func(f2, mValues, true, buf2.str());
  }
}

TEST_P(TvFuncTest_base, posi_literal)
{
  init_values();
  SizeType ni = GetParam();
  auto ni_exp = 1U << ni;
  for ( int i: Range(ni) ) {
    auto f1 = TvFunc::posi_literal(ni, i);
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

TEST_P(TvFuncTest_base, nega_literal)
{
  init_values();
  SizeType ni = GetParam();
  auto ni_exp = 1U << ni;
  for ( int i: Range(ni) ) {
    auto f1 = TvFunc::nega_literal(ni, i);
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

TEST_P(TvFuncTest_base, cube1)
{
  init_values();
  SizeType ni = GetParam();
  auto ni_exp = 1U << ni;
  for ( SizeType i: Range(ni) ) {
    for ( auto lit: {Literal{i, false}, Literal{i, true}} ) {
      auto lit_list = {lit};
      auto f1 = TvFunc::cube(ni, lit_list);
      for ( int p: Range(ni_exp) ) {
	auto pval = lit.is_positive() ? 1 : 0;
	auto nval = lit.is_positive() ? 0 : 1;
	if ( p & (1U << i) ) {
	  mValues[p] = pval;
	}
	else {
	  mValues[p] = nval;
	}
      }
      ostringstream buf1;
      buf1 << "TvFunc::cube(" << ni << ", {" << lit << "})";
      check_func(f1, mValues, false, buf1.str());
    }
  }
}

TEST_P(TvFuncTest_base, cube2)
{
  init_values();
  SizeType ni = GetParam();
  auto ni_exp = 1U << ni;
  SizeType nsample = ni < 13 ? 100 : ni < 14 ? 20 : 5;
  std::uniform_int_distribution<SizeType> rd(0, ni - 1);
  for ( SizeType i: Range(ni) ) {
    for ( SizeType c: Range(nsample) ) {
      // リテラル数
      SizeType nlit = rd(mRandGen) + 1;
      vector<Literal> lit_list(nlit);
      for ( SizeType i = 0; i < nlit; ++ i ) {
	auto varid = rd(mRandGen);
	auto inv = mRandDist(mRandGen) ? true : false;
	auto lit = Literal{varid, inv};
	lit_list[i] = lit;
      }
      auto f1 = TvFunc::cube(ni, lit_list);
      // 期待値は TvFunc::Literal() から作る．
      auto exp_f = TvFunc::one(ni);
      for ( auto lit: lit_list ) {
	auto lit_f = TvFunc::literal(ni, lit);
	exp_f &= lit_f;
      }
      ostringstream buf1;
      buf1 << "TvFunc::cube(" << ni << ", {";
      const char* comma = "";
      for ( auto lit: lit_list ) {
	buf1 << comma << lit;
	comma = ", ";
      }
      buf1 << "})";
      EXPECT_EQ( exp_f, f1 ) << buf1.str();
    }
  }
}

TEST_P(TvFuncTest_base, cover1)
{
  init_values();
  SizeType ni = GetParam();
  auto ni_exp = 1U << ni;
  SizeType nsample = ni < 13 ? 100 : ni < 14 ? 20 : 5;
  std::uniform_int_distribution<SizeType> rd(0, ni - 1);
  for ( SizeType i: Range(ni) ) {
    for ( SizeType c: Range(nsample) ) {
      // キューブ数は２
      SizeType nlit1 = rd(mRandGen) + 1;
      vector<Literal> lit_list1(nlit1);
      for ( SizeType i = 0; i < nlit1; ++ i ) {
	auto varid = rd(mRandGen);
	auto inv = mRandDist(mRandGen) ? true : false;
	auto lit = Literal{varid, inv};
	lit_list1[i] = lit;
      }
      SizeType nlit2 = rd(mRandGen) + 1;
      vector<Literal> lit_list2(nlit2);
      for ( SizeType i = 0; i < nlit2; ++ i ) {
	auto varid = rd(mRandGen);
	auto inv = mRandDist(mRandGen) ? true : false;
	auto lit = Literal{varid, inv};
	lit_list2[i] = lit;
      }
      auto f1 = TvFunc::cover(ni, {lit_list1, lit_list2});
      // 期待値
      auto cube1 = TvFunc::cube(ni, lit_list1);
      auto cube2 = TvFunc::cube(ni, lit_list2);
      auto exp_f = cube1 | cube2;
      EXPECT_EQ( exp_f, f1 );
    }
  }
}

TEST_P(TvFuncTest_base, random_func)
{
  init_values();
  SizeType ni = GetParam();
  auto ni_exp = 1 << ni;
  SizeType n = ni < 13 ? 100 : ni < 14 ? 20 : 5;
  for ( auto c: Range(n) ) {
    for ( auto p: Range(ni_exp) ) {
      if ( mRandDist(mRandGen) ) {
	mValues[p] = 1;
      }
    }
    auto func = TvFunc(ni, mValues);
    bool fast = (ni > 14);
    check_func(func, mValues, fast);
  }
}

TEST(TvFuncTest, constructor_bad1)
{
  // ベクタの長さが 2^3 ではない．
  ASSERT_THROW((TvFunc{3, vector<int>{0, 1, 1, 0}}),
	       std::invalid_argument);
}

TEST(TvFuncTest, constructor_bad2)
{
  // '0', '1' 以外の文字を含んでいる．
  ASSERT_THROW((TvFunc{"01--"}),
	       std::invalid_argument);
}

TEST(TvFuncTest, constructor_bad3)
{
  // ベクタの長さが2のべき乗になっていない．
  ASSERT_THROW((TvFunc{"01000"}),
	       std::invalid_argument);
}

TEST(TvFuncTest, literal_bad1)
{
  ASSERT_THROW( TvFunc::literal(2, 3, false),
		std::out_of_range );
}

TEST(TvFuncTest, literal_bad2)
{
  auto lit = Literal(3, true);
  ASSERT_THROW( TvFunc::literal(2, lit),
		std::out_of_range );
}

TEST(TvFuncTest, posi_literal_bad)
{
  ASSERT_THROW( TvFunc::posi_literal(2, 3),
		std::out_of_range );
}

TEST(TvFuncTest, nega_literal_bad)
{
  ASSERT_THROW( TvFunc::nega_literal(2, 3),
		std::out_of_range );
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
    auto func1 = TvFunc(2, values1);
    for ( int v2: Range(14) ) {
      for ( int p: {0, 1, 2, 3} ) {
	if ( v2 & (1 << p) ) {
	  values2[p] = 1;
	}
	else {
	  values2[p] = 0;
	}
      }
      auto func2 = TvFunc(2, values2);
      check_op(func1, func2);
    }
  }
}

TEST(TvFuncTest, and_int_bad)
{
  auto func1 = TvFunc("0110");
  auto func2 = TvFunc("01");

  ASSERT_THROW( func1.and_int(func2),
		std::invalid_argument );
}

TEST(TvFuncTest, or_int_bad)
{
  auto func1 = TvFunc("0110");
  auto func2 = TvFunc("01");

  ASSERT_THROW( func1.or_int(func2),
		std::invalid_argument );
}

TEST(TvFuncTest, xor_int_bad)
{
  auto func1 = TvFunc("0110");
  auto func2 = TvFunc("01");

  ASSERT_THROW( func1.xor_int(func2),
		std::invalid_argument );
}

TEST(TvFuncTest, cofactor_int_bad1)
{
  auto func1 = TvFunc("0110");

  ASSERT_THROW( func1.cofactor_int(3),
		std::out_of_range );
}

TEST(TvFuncTest, cofactor_int_bad2)
{
  auto func1 = TvFunc("0110");
  auto lit = Literal(3, true);

  ASSERT_THROW( func1.cofactor_int(lit),
		std::out_of_range );
}

TEST(TvFuncTest, value_bad)
{
  auto func1 = TvFunc("0110");

  ASSERT_THROW( func1.value(10),
		std::out_of_range );
}

TEST(TvFuncTest, walsh_1_bad)
{
  auto func1 = TvFunc("0110");

  ASSERT_THROW( func1.walsh_1(2),
		std::out_of_range );
}

TEST(TvFuncTest, walsh_2_bad1)
{
  auto func1 = TvFunc("0110");

  ASSERT_THROW( func1.walsh_2(2, 1),
		std::out_of_range );
}

TEST(TvFuncTest, walsh_2_bad2)
{
  auto func1 = TvFunc("0110");

  ASSERT_THROW( func1.walsh_2(1, 2),
		std::out_of_range );
}

TEST(TvFuncTest, check_sup_bad)
{
  auto func1 = TvFunc("0110");

  ASSERT_THROW( func1.check_sup(2),
		std::out_of_range );
}

TEST(TvFuncTest, check_unate_bad)
{
  auto func1 = TvFunc("0110");

  ASSERT_THROW( func1.check_unate(2),
		std::out_of_range );
}

TEST(TvFuncTest, check_sym_bad1)
{
  auto func1 = TvFunc("0110");

  ASSERT_THROW( func1.check_sym(2, 1),
		std::out_of_range );
}

TEST(TvFuncTest, check_sym_bad2)
{
  auto func1 = TvFunc("0110");

  ASSERT_THROW( func1.check_sym(1, 2),
		std::out_of_range );
}

TEST_P(TvFuncTest_base, check_op2)
{
  SizeType ni = GetParam();
  auto ni_exp = 1 << ni;
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
    auto func1 = TvFunc(ni, values1);
    auto func2 = TvFunc(ni, values2);
    check_op(func1, func2);
  }
}

TEST_P(TvFuncTest_base, check_sup1)
{
  SizeType ni = GetParam();
  auto ni_exp = 1 << ni;
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
      auto func = TvFunc(ni, values);
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
	EXPECT_EQ( exp_ans, func.check_sup(j) );
      }
    }
  }
}

TEST_P(TvFuncTest_base, check_sup2)
{
  SizeType ni = GetParam();
  auto ni_exp = 1 << ni;
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
    auto func = TvFunc(ni, values);
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
      EXPECT_EQ( exp_ans, func.check_sup(j) );
    }
  }
}

int
check_sym(
  const vector<int>& values,
  int i1,
  int i2
)
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

TEST_P(TvFuncTest_base, check_sym)
{
  SizeType ni = GetParam();
  if ( ni > 15 ) {
    return;
  }
  std::uniform_int_distribution<int> rd(0, ni - 1);
  auto ni_exp = 1 << ni;
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
	  auto func = TvFunc(ni, values);
	  int exp_sym = check_sym(values, i1, i2);
	  bool exp_sym0 = exp_sym & 1;
	  bool exp_sym1 = (exp_sym >> 1) & 1;
	  EXPECT_EQ( exp_sym0, func.check_sym(i1, i2, false) );
	  EXPECT_EQ( exp_sym0, func.check_sym(i2, i1, false) );
	  EXPECT_EQ( exp_sym1, func.check_sym(i1, i2, true) );
	  EXPECT_EQ( exp_sym1, func.check_sym(i2, i1, true) );
	  for ( int c2: Range(n2) ) {
	    int j1 = rd(mRandGen);
	    int j2 = rd(mRandGen);
	    if ( j1 == j2 ) {
	      continue;
	    }
	    int exp_sym = check_sym(values, j1, j2);
	    bool exp_sym0 = exp_sym & 1;
	    bool exp_sym1 = (exp_sym >> 1) & 1;
	    EXPECT_EQ( exp_sym0, func.check_sym(j1, j2, false) );
	    EXPECT_EQ( exp_sym1, func.check_sym(j1, j2, true) );
	  }
	}
      }
    }
  }
}

TEST_P(TvFuncTest_base, cofactor)
{
  SizeType ni = GetParam();
  auto ni_exp = 1 << ni;
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
    auto func = TvFunc(ni, values);
    for ( int var: Range(ni) ) {
      auto func0 = func.cofactor(var, true);
      auto func1 = func.cofactor(var, false);
      vector<int> values0(ni_exp, 0);
      vector<int> values1(ni_exp, 0);
      int i_bit = 1U << var;
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
      auto exp_func0 = TvFunc(ni, values0);
      auto exp_func1 = TvFunc(ni, values1);
      EXPECT_EQ( exp_func0, func0 );
      EXPECT_EQ( exp_func1, func1 );

      auto cfunc0 = func;
      cfunc0.cofactor_int(var, true);
      EXPECT_EQ( exp_func0, cfunc0 );

      auto cfunc1 = func;
      cfunc1.cofactor_int(var, false);
      EXPECT_EQ( exp_func1, cfunc1 );
    }
  }
}

TEST_P(TvFuncTest_base, decompose)
{
  SizeType ni = GetParam();
  if ( ni < 2 ) {
    return;
  }
  auto ni_exp = 1 << ni;
  vector<int> values(ni_exp, 0);
  SizeType ni1 = ni - 1;
  auto ni1_exp = 1 << ni1;
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
    auto func = TvFunc(ni, values);
    TvFunc func0;
    TvFunc func1;
    func.decompose(func0, func1);
    EXPECT_EQ( ni1, func0.input_num() );
    EXPECT_EQ( ni1, func1.input_num() );
    vector<int> values0(ni1_exp, 0);
    vector<int> values1(ni1_exp, 0);
    for ( SizeType i = 0; i < ni1_exp; ++ i ) {
      values0[i] = values[i];
      values1[i] = values[i + ni1_exp];
    }
    auto exp_func0 = TvFunc(ni1, values0);
    auto exp_func1 = TvFunc(ni1, values1);
    EXPECT_EQ( exp_func0, func0 );
    EXPECT_EQ( exp_func1, func1 );
  }
}

TEST_P(TvFuncTest_base, xform)
{
  SizeType ni = GetParam();
  auto ni_exp = 1 << ni;
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
    auto func = TvFunc(ni, values);
    RandPermGen rpg(ni);
    for ( int c2: Range(n2) ) {
      bool oinv = mRandDist(mRandGen) ? true : false;
      NpnMap map(ni);
      map.set_oinv(oinv);
      rpg.generate(mRandGen);
      for ( int i: Range(ni) ) {
	bool iinv = mRandDist(mRandGen) ? true : false;
	map.set(i, rpg.elem(i), iinv);
      }
      auto xfunc = func.xform(map);

      vector<int> exp_values(ni_exp);
      for ( int p: Range(ni_exp) ) {
	int q = 0;
	for ( int i: Range(ni) ) {
	  auto vmap = map.imap(i);
	  int j = vmap.var();
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
      auto exp_xfunc = TvFunc(ni, exp_values);
      EXPECT_EQ( exp_xfunc, xfunc );
    }
  }
}

TEST(TvFuncTest, shrink_0)
{
  SizeType ni = 10;
  auto func0 = TvFunc::zero(ni);

  auto map = func0.shrink_map();
  auto func1 = func0.xform(map);

  EXPECT_EQ( TvFunc::zero(0), func1 );

  EXPECT_EQ( ni, map.input_num() );
  for ( int i: Range(ni) ) {
    auto imap = map.imap(i);
    EXPECT_TRUE( imap.is_invalid() );
  }
}

TEST(TvFuncTest, shrink_1)
{
  SizeType ni = 10;
  auto func0 = TvFunc::one(ni);

  auto map = func0.shrink_map();
  auto func1 = func0.xform(map);

  EXPECT_EQ( TvFunc::one(0), func1 );

  EXPECT_EQ( ni, map.input_num() );
  for ( int i: Range(ni) ) {
    auto imap = map.imap(i);
    EXPECT_TRUE( imap.is_invalid() );
  }
}

TEST(TvFuncTest, shrink_lit1)
{
  SizeType ni = 10;
  auto func0 = TvFunc::literal(ni, 0, false);

  auto map = func0.shrink_map();;
  auto func1 = func0.xform(map);

  EXPECT_EQ( TvFunc::literal(1, 0, false), func1);

  EXPECT_EQ( ni, map.input_num() );

  {
    auto imap = map.imap(0);
    EXPECT_FALSE( imap.is_invalid() );
    EXPECT_EQ( 0, imap.var() );
    EXPECT_FALSE( imap.inv() );
  }

  for ( int i: Range(1, ni) ) {
    auto imap = map.imap(i);
    EXPECT_TRUE( imap.is_invalid() );
  }
}

TEST(TvFuncTest, shrink_lit2)
{
  SizeType ni = 10;
  auto func0 = TvFunc::literal(ni, 2, false);

  auto map = func0.shrink_map();
  auto func1 = func0.xform(map);

  EXPECT_EQ( TvFunc::literal(1, 0, false), func1);

  EXPECT_EQ( ni, map.input_num() );

  {
    auto imap = map.imap(2);
    EXPECT_FALSE( imap.is_invalid() );
    EXPECT_EQ( 0, imap.var() );
    EXPECT_FALSE( imap.inv() );
  }

  for ( int i: Range(ni) ) {
    if ( i == 2 ) {
      continue;
    }
    auto imap = map.imap(i);
    EXPECT_TRUE( imap.is_invalid() );
  }
}

TEST(TvFuncTest, shrink_lit3)
{
  SizeType ni = 10;
  auto lit1 = TvFunc::literal(ni, 2, false);
  auto lit2 = TvFunc::literal(ni, 5, false);
  auto func0 = lit1 & ~lit2;

  auto map = func0.shrink_map();
  auto func1 = func0.xform(map);

  {
    auto lit1 = TvFunc::literal(2, 0, false);
    auto lit2 = TvFunc::literal(2, 1, false);
    auto func2 = lit1 & ~lit2;
    EXPECT_EQ( func2, func1);
  }

  EXPECT_EQ( ni, map.input_num() );

  {
    auto imap = map.imap(2);
    EXPECT_FALSE( imap.is_invalid() );
    EXPECT_EQ( 0, imap.var() );
    EXPECT_FALSE( imap.inv() );
  }

  {
    auto imap = map.imap(5);
    EXPECT_FALSE( imap.is_invalid() );
    EXPECT_EQ( 1, imap.var() );
    EXPECT_FALSE( imap.inv() );
  }

  for ( int i: Range(ni) ) {
    if ( i == 2 || i == 5 ) {
      continue;
    }
    auto imap = map.imap(i);
    EXPECT_TRUE( imap.is_invalid() );
  }
}

TEST(TvFuncTest, expand_0)
{
  auto func0 = TvFunc::zero(0);

  auto map = NpnMap(0, 10);
  auto func1 = func0.xform(map);

  EXPECT_EQ( TvFunc::zero(10), func1 );
}

TEST(TvFuncTest, expand_1)
{
  auto func0 = TvFunc::one(0);

  auto map = NpnMap(0, 10);
  auto func1 = func0.xform(map);

  EXPECT_EQ( TvFunc::one(10), func1 );
}

TEST(TvFuncTest, expand_lit1)
{
  auto func0 = TvFunc::literal(1, 0, false);

  auto map = NpnMap(1, 10);
  map.set(0, 0, false);
  auto func1 = func0.xform(map);

  EXPECT_EQ( TvFunc::literal(10, 0, false), func1 );
}

TEST(TvFuncTest, expand_lit2)
{
  auto func0 = TvFunc::literal(2, 1, false);

  auto map = NpnMap(2, 10);
  map.set(1, 2, false);
  auto func1 = func0.xform(map);

  EXPECT_EQ( TvFunc::literal(10, 2, false), func1 );
}

TEST(TvFuncTest, invalid_func)
{
  auto f1 = TvFunc(); // デフォルトで不正値となる．

  ostringstream strbuf1;
  BinEnc enc{strbuf1};
  f1.dump(enc);

  istringstream strbuf2{strbuf1.str()};
  BinDec dec{strbuf2};
  auto f2 = TvFunc::zero(10);
  f2.restore(dec);

  EXPECT_EQ( f2, f1 );
}

TEST(TvFuncTest, from_str)
{
  const char* fstr = "0111";
  auto f = TvFunc(fstr);

  EXPECT_EQ( fstr, f.str() );
}

TEST(TvFuncTest, hex_str)
{
  const char* fstr = "0111";
  auto f = TvFunc(fstr);

  ostringstream buf;
  f.print(buf, 16);
  auto xstr = buf.str();
  EXPECT_EQ( "7", xstr );
}

TEST(TvFuncTest, from_vect)
{
  vector<int> vect{0, 1, 1, 1};
  auto f = TvFunc(2, vect);

  EXPECT_EQ( "1110", f.str() );
}

INSTANTIATE_TEST_SUITE_P(Test0to20,
			 TvFuncTest_base,
			 ::testing::Range(0, 21));

END_NAMESPACE_YM
