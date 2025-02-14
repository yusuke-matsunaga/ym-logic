
/// @file isop_test.cc
/// @brief Tv2Sop::isop() のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "gtest/gtest.h"
#include "ym/Tv2Sop.h"
#include "ym/SopCover.h"
#include "ym/SopCube.h"
#include "ym/Range.h"
#include "TvFuncTest.h"


BEGIN_NAMESPACE_YM

class IsopTest:
  public TvFuncTestWithParam
{
public:

  // Tv2Sop::isop() のチェック
  void
  check(
    const TvFunc& f,
    const TvFunc& d
  )
  {
    auto cube_list = Tv2Sop::isop(f, d);
    auto cov = SopCover{f.input_num(), cube_list};

    {
      cout << "F: " << f << endl;
      cout << "D: " << d << endl;
      cout << "isop: " << cov << endl;
    }
    // cof_f が [f: f + d] の範囲に入っているかテスト
    auto cov_f = cov.tvfunc();
    ostringstream buf;
    buf << f << "|" << d << endl
	<< cov_f;
    auto f1 = f & ~d;
    EXPECT_TRUE( f1.check_containment(cov_f) ) << buf.str();
    EXPECT_TRUE( cov_f.check_containment(f | d) ) << buf.str();

    // cube_list の各キューブが prime かどうかのテスト
    auto r = ~(f | d);
    for ( auto& cube: cube_list ) {
      check_prime(cube, r);
    }

    // cube_list の各キューブが f1 と交差しているかのテスト
    for ( auto& cube: cube_list ) {
      auto cube_f = cube.tvfunc();
      EXPECT_TRUE( cube_f && f1 );
    }

    // cube_list の各キューブが非冗長かどうかのテスト
    auto nc = cube_list.size();
    vector<TvFunc> fl_list(nc);
    vector<TvFunc> fu_list(nc);
    auto fl = TvFunc::zero(f.input_num());
    for ( SizeType i = 0; i < nc; ++ i ) {
      fl_list[i] = fl;
      fl |= cube_list[i].tvfunc();
    }
    auto fu = TvFunc::zero(f.input_num());
    for ( SizeType i = 0; i < nc; ++ i ) {
      auto j = nc - i - 1;
      fu_list[j] = fu;
      fu |= cube_list[j].tvfunc();
    }
    for ( SizeType i = 0; i < nc; ++ i ) {
      auto f1 = fl_list[i] | fu_list[i];
      EXPECT_NE( f1, f );
    }
  }


private:

  void
  check_prime(
    const SopCube& cube,
    const TvFunc& r
  )
  {
    SizeType ni = cube.variable_num();
    auto lit_list = cube.literal_list();
    auto nl = lit_list.size();
    ostringstream buf;
    cube.print(buf);
    buf << ", " << r;
    for ( SizeType i = 0; i < nl; ++ i ) {
      vector<Literal> lit_list1;
      lit_list1.reserve(nl - 1);
      for ( SizeType j = 0; j < nl; ++ j ) {
	if ( i == j ) {
	  continue;
	}
	auto lit = lit_list[j];
	lit_list1.push_back(lit);
      }
      auto cube_f = TvFunc::cube(ni, lit_list1);
      EXPECT_TRUE( cube_f && r ) << buf.str();
    }
  }
};

TEST_F(IsopTest, isop0)
{
  SizeType ni = 3;
  auto var0 = TvFunc::literal(ni, 0);
  auto var1 = TvFunc::literal(ni, 1);
  auto var2 = TvFunc::literal(ni, 2);
  auto f = var0 & var1 | var0 & var2;
  auto d = ~(var1 | var2);

  check(f, d);
}

TEST_F(IsopTest, isop1)
{
  SizeType ni = 3;
  auto var0 = TvFunc::literal(ni, 0);
  auto var1 = TvFunc::literal(ni, 1);
  auto var2 = TvFunc::literal(ni, 2);
  auto f = var1 & var2 | var0 & var2;
  auto d = ~(var0 | var1);

  check(f, d);
}

TEST_F(IsopTest, isop2)
{
  SizeType ni = 4;
  auto var0 = TvFunc::literal(ni, 0);
  auto var1 = TvFunc::literal(ni, 1);
  auto var2 = TvFunc::literal(ni, 2);
  auto var3 = TvFunc::literal(ni, 3);
  auto f = var0 & var2 | var0 & var3 | var1 & var2 | var1 & var3;
  auto d = ~(var2 | var3);

  check(f, d);
}

TEST_F(IsopTest, isop3)
{
  SizeType ni = 4;
  auto var0 = TvFunc::literal(ni, 0);
  auto var1 = TvFunc::literal(ni, 1);
  auto var2 = TvFunc::literal(ni, 2);
  auto var3 = TvFunc::literal(ni, 3);
  auto f = var0 & var2 | var0 & var3 | var1 & var2 | var1 & var3;
  auto d = ~(var0 | var1);

  check(f, d);
}

TEST_F(IsopTest, isop4)
{
  SizeType ni = 4;
  auto var0 = TvFunc::literal(ni, 0);
  auto var1 = TvFunc::literal(ni, 1);
  auto var2 = TvFunc::literal(ni, 2);
  auto var3 = TvFunc::literal(ni, 3);
  auto f = var0 & var3;
  auto d = ~var0 & ~var1 & ~var2;

  check(f, d);
}

TEST_P(IsopTest, isop_p)
{
  SizeType ni = GetParam();
  auto ni_exp = 1 << ni;

  // テストのサンプル数
  SizeType n;
  if ( ni < 1 ) {
    return;
  }
  else if ( ni <= 3 ) {
    n = (1 << ni_exp) * (1 << ni_exp);
  }
  else if ( ni < 7 ) {
    n = 4000;
  }
  else {
    n = 400;
  }

  auto f_values = vector<int>(ni_exp, 0);
  auto d_values = vector<int>(ni_exp, 0);
  for ( SizeType c: Range(n) ) {
    for ( auto p: Range(ni_exp) ) {
      if ( ni <= 3 ) {
	if ( c & (1 << p) ) {
	  f_values[p] = 1;
	}
	else {
	  f_values[p] = 0;
	}
	if ( (c >> ni_exp) & (1 << p) ) {
	  d_values[p] = 1;
	}
	else {
	  d_values[p] = 0;
	}
      }
      else {
	if ( mRandDist(mRandGen) ) {
	  d_values[p] = 1;
	  f_values[p] = 1;
	}
	else {
	  d_values[p] = 0;
	  if ( mRandDist(mRandGen) ) {
	    f_values[p] = 1;
	  }
	  else {
	    f_values[p] = 0;
	  }
	}
      }
    }
    auto f = TvFunc{ni, f_values};
    auto d = TvFunc{ni, d_values};
    check(f, d);
  }
}

INSTANTIATE_TEST_SUITE_P(Isop0to10,
			 IsopTest,
			 ::testing::Range(0, 11));

END_NAMESPACE_YM
