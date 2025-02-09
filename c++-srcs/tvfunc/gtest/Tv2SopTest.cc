
/// @file Tv2SopTest.cc
/// @brief Tv2Sop のテストプログラム
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

SopCover
BCF(
  const TvFunc& func
)
{
  return SopCover{func.input_num(), Tv2Sop::all_primes(func)};
}

// 検証用に主項のリストを作る．
SopCover
prime_cover(
  const TvFunc& func
)
{
  auto ni = func.input_num();
  auto ni_exp = 1 << ni;
  // オンセットのリストを作る．
  vector<SopCube> cube_list;
  for ( SizeType p = 0; p < ni_exp; ++ p ) {
    if ( func.value(p) ) {
      vector<Literal> lit_list;
      for ( SizeType i = 0; i < ni; ++ i ) {
	auto lit = Literal{i, false};
	if ( p & (1 << i) ) {
	  lit_list.push_back(lit);
	}
	else {
	  lit_list.push_back(~lit);
	}
      }
      cube_list.push_back(SopCube{ni, lit_list});
    }
  }
  // 教科書的に１ビット違いのキューブをマージしていき，
  // マージされなかったキューブを集める．
  vector<vector<SopCube>> cube_list_list(ni + 1);
  cube_list_list[0] = cube_list;
  unordered_set<SopCube> cube_mark;
  unordered_set<SopCube> cube_check;
  for ( SizeType d = 0; d < ni; ++ d ) {
    const auto& cube_list = cube_list_list[d];
    // 1ビット違いのキューブをマージする．
    SizeType nc = cube_list.size();
    for ( SizeType i0 = 0; i0 < nc; ++ i0 ) {
      const auto& cube0 = cube_list[i0];
      for ( SizeType i1 = i0 + 1; i1 < nc; ++ i1 ) {
	const auto& cube1 = cube_list[i1];
	SizeType dpos = -1;
	vector<Literal> lit_list;
	for ( SizeType i = 0; i < ni; ++ i ) {
	  auto pat0 = cube0.get_pat(i);
	  auto pat1 = cube1.get_pat(i);
	  if ( pat0 == pat1 ) {
	    if ( pat0 == SopPat::_1 ) {
	      lit_list.push_back(Literal{i, false});
	    }
	    else if ( pat0 == SopPat::_0 ) {
	      lit_list.push_back(Literal{i, true});
	    }
	    continue;
	  }
	  if ( pat0 == SopPat::_X || pat1 == SopPat::_X ) {
	    dpos = -1;
	    break;
	  }
	  if ( dpos != -1 ) {
	    dpos = -1;
	    break;
	  }
	  dpos = i;
	}
	if ( dpos != -1 ) {
	  SopCube new_cube{ni, lit_list};
	  if ( cube_check.count(new_cube) == 0 ) {
	    cube_list_list[d + 1].push_back(new_cube);
	    cube_check.emplace(new_cube);
	  }
	  cube_mark.emplace(cube0);
	  cube_mark.emplace(cube1);
	}
      }
    }
    if ( cube_list_list[d + 1].empty() ) {
      break;
    }
  }
  vector<SopCube> ans_list;
  for ( const auto& cube_list: cube_list_list ) {
    for ( const auto& cube: cube_list ) {
      if ( cube_mark.count(cube) == 0 ) {
	ans_list.push_back(cube);
      }
    }
  }
  std::sort(ans_list.begin(), ans_list.end(), std::greater<>());
  return SopCover{ni, ans_list};
}

TEST(TvFuncTest, bcf1)
{
  auto vect = vector<int>{0, 1, 1, 1};
  auto f = TvFunc{2, vect};
  auto cov = BCF(f);
  auto exp_cov = prime_cover(f);
  EXPECT_EQ( exp_cov, cov );

  ostringstream buf;
  cov.print(buf);
  EXPECT_EQ( "v0 + v1", buf.str() );
}

TEST(TvFuncTest, bcf2)
{
  auto vect = vector<int>{0, 0, 0, 1, 0, 1, 1, 1};
  auto f = TvFunc{3, vect};
  auto cov = BCF(f);
  auto exp_cov = prime_cover(f);
  EXPECT_EQ( exp_cov, cov );

  ostringstream buf;
  cov.print(buf);
  EXPECT_EQ( "v0 v1 + v0 v2 + v1 v2", buf.str() );
}

TEST(TvFuncTest, bcf3)
{
  auto vect = vector<int>{1, 1, 1, 1, 0, 1, 0, 0};
  auto f = TvFunc{3, vect};
  auto cov = BCF(f);
  auto exp_cov = prime_cover(f);
  EXPECT_EQ( exp_cov, cov );
}

TEST_P(TvFuncTestWithParam, bcf)
{
  SizeType ni = GetParam();
  auto ni_exp = 1 << ni;

  // テストのサンプル数
  SizeType n;
  if ( ni < 1 ) {
    return;
  }
  else if ( ni <= 4 ) {
    n = (1 << ni_exp);
  }
  else if ( ni < 7 ) {
    n = 1000;
  }
  else if ( ni < 10 ) {
    n =100;
  }
  else {
    return;
  }

  auto values = vector<int>(ni_exp, 0);
  for ( SizeType c: Range(n) ) {
    for ( auto p: Range(ni_exp) ) {
      if ( ni <= 4 ) {
	if ( c & (1 << p) ) {
	  values[p] = 1;
	}
	else {
	  values[p] = 0;
	}
      }
      else {
	if ( mRandDist(mRandGen) ) {
	  values[p] = 1;
	}
	else {
	  values[p] = 0;
	}
      }
    }
    auto func = TvFunc{ni, values};
    auto cov = BCF(func);
    // 正解を計算する．
    auto exp_cov = prime_cover(func);
    EXPECT_EQ( exp_cov, cov );
  }
}

TEST_P(TvFuncTestWithParam, mwc)
{
  SizeType ni = GetParam();
  auto ni_exp = 1 << ni;

  // テストのサンプル数
  SizeType n;
  if ( ni < 1 ) {
    return;
  }
  else if ( ni <= 4 ) {
    n = (1 << ni_exp);
  }
  else if ( ni < 7 ) {
    n = 1000;
  }
  else if ( ni < 10 ) {
    n =100;
  }
  else {
    return;
  }

  auto values = vector<int>(ni_exp, 0);
  for ( SizeType c: Range(n) ) {
    for ( auto p: Range(ni_exp) ) {
      if ( ni <= 4 ) {
	if ( c & (1 << p) ) {
	  values[p] = 1;
	}
	else {
	  values[p] = 0;
	}
      }
      else {
	if ( mRandDist(mRandGen) ) {
	  values[p] = 1;
	}
	else {
	  values[p] = 0;
	}
      }
    }
    auto func = TvFunc{ni, values};
    auto cov = Tv2Sop::MWC(func);
    // TvFunc に再変換して func と等しいか調べる．
    // ヒューリスティックの簡単化なので正解はない．
    auto exp_func = TvFunc::cover(ni, cov);
    EXPECT_EQ( exp_func, func );
  }
}

TEST_P(TvFuncTestWithParam, isop)
{
  SizeType ni = GetParam();
  auto ni_exp = 1 << ni;

  // テストのサンプル数
  SizeType n;
  if ( ni < 1 ) {
    return;
  }
  else if ( ni <= 4 ) {
    n = (1 << ni_exp);
  }
  else if ( ni < 7 ) {
    n = 1000;
  }
  else if ( ni < 10 ) {
    n =100;
  }
  else {
    return;
  }

  auto values = vector<int>(ni_exp, 0);
  for ( SizeType c: Range(n) ) {
    for ( auto p: Range(ni_exp) ) {
      if ( ni <= 4 ) {
	if ( c & (1 << p) ) {
	  values[p] = 1;
	}
	else {
	  values[p] = 0;
	}
      }
      else {
	if ( mRandDist(mRandGen) ) {
	  values[p] = 1;
	}
	else {
	  values[p] = 0;
	}
      }
    }
    auto func = TvFunc{ni, values};
    auto cov = Tv2Sop::ISOP(func);
    // TvFunc に再変換して func と等しいか調べる．
    // ヒューリスティックの簡単化なので正解はない．
    auto exp_func = TvFunc::cover(ni, cov);
    EXPECT_EQ( exp_func, func );
  }
}

INSTANTIATE_TEST_SUITE_P(Test0to20,
			 TvFuncTestWithParam,
			 ::testing::Range(0, 11));

END_NAMESPACE_YM
