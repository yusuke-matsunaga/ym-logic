
/// @file bcf_test.cc
/// @brief bcf_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include <libgen.h>
#include "ym/TvFunc.h"
#include "ym/Range.h"
#include <random>


BEGIN_NAMESPACE_YM

int
bcf_test(
  int argc,
  char** argv
)
{
  if ( argc != 2 ) {
    cerr << basename(argv[0]) << " <# of inputs>" << endl;
    return 1;
  }

  SizeType ni = atoi(argv[1]);
  auto ni_exp = 1 << ni;

  // テストのサンプル数
  SizeType n;
  if ( ni < 1 ) {
    return 0;
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
    return 0;
  }

  // 乱数生成器
  std::mt19937 rand_gen;

  // 0, 1 の一様分布生成器
  std::uniform_int_distribution<int> rand_dist;

  vector<int> values(ni_exp, 0);
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
	if ( rand_dist(rand_gen) ) {
	  values[p] = 1;
	}
	else {
	  values[p] = 0;
	}
      }
    }
    auto func = TvFunc{ni, values};
    auto cov = func.BCF();
    cout << "BCF = ";
    cov.print(cout);
    cout << endl;
  }

  return 0;
}

END_NAMESPACE_YM

int
main(
  int argc,
  char** argv
)
{
  return YM_NAMESPACE::bcf_test(argc, argv);
}
