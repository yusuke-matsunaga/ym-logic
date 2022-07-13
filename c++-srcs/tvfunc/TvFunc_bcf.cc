
/// @file TvFunc_bcf.cc
/// @brief TvFunc_bcf の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/TvFunc.h"
#include "ym/SopCover.h"
#include "ym/SopCube.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

bool debug = false;

vector<SopCube>
bcf_sub(
  SizeType ni,
  const TvFunc& f,
  SizeType var
)
{
  if ( debug ) {
    cout << "bcf_sub: " << f << ": " << var << endl;
  }

  if ( f.is_zero() ) {
    if ( debug ) {
      cout << " ==> 0" << endl;
    }
    return vector<SopCube>{};
  }
  if ( f.is_one() ) {
    if ( debug ) {
      cout << " ==> 1" << endl;
    }
    return vector<SopCube>{SopCube{ni}};
  }

  ASSERT_COND( var < ni );

  VarId varid{var};

  // コファクターに対して再帰する．
  // ただし，共通部分を求めて3分割する．
  auto f0 = f.cofactor(varid, true);
  auto f1 = f.cofactor(varid, false);
  auto fc = f0 & f1;
  if ( debug ) {
    cout << "  fc: " << fc << endl;
    cout << "  f0: " << f0 << endl;
    cout << "  f1: " << f1 << endl;
  }
  auto cc = bcf_sub(ni, fc, var + 1);
  auto c0 = bcf_sub(ni, f0, var + 1);
  auto c1 = bcf_sub(ni, f1, var + 1);
  if ( debug ) {
    cout << "  cc: ";
    for ( auto& cube: cc ) {
      cout << " " << cube;
    }
    cout << endl;
    cout << "  c0: ";
    for ( auto& cube: c0 ) {
      cout << " " << cube;
    }
    cout << endl;
    cout << "  c1: ";
    for ( auto& cube: c1 ) {
      cout << " " << cube;
    }
    cout << endl;
  }
  // 結果をマージする．
  auto R = ~fc;
  Literal lit0{varid, true};
  for ( auto& cube: c0 ) {
    TvFunc f_cube = TvFunc::make_one(ni);
    for ( SizeType i = 0; i < ni; ++ i ) {
      VarId var{i};
      auto pat = cube.get_pat(var);
      if ( pat == SopPat::_1 ) {
	f_cube &= TvFunc::make_posi_literal(ni, var);
      }
      else if ( pat == SopPat::_0 ) {
	f_cube &= TvFunc::make_nega_literal(ni, var);
      }
    }
    if ( f_cube && R ) {
      cc.push_back(cube * lit0);
    }
  }
  Literal lit1{varid, false};
  for ( auto& cube: c1 ) {
    TvFunc f_cube = TvFunc::make_one(ni);
    for ( SizeType i = 0; i < ni; ++ i ) {
      VarId var{i};
      auto pat = cube.get_pat(var);
      if ( pat == SopPat::_1 ) {
	f_cube &= TvFunc::make_posi_literal(ni, var);
      }
      else if ( pat == SopPat::_0 ) {
	f_cube &= TvFunc::make_nega_literal(ni, var);
      }
    }
    if ( f_cube && R ) {
      cc.push_back(cube * lit1);
    }
  }

  if ( debug ) {
    cout << "bcf_sub: " << f << ": " << var << endl;
    cout << " ==> ";
    for ( auto& cube: cc ) {
      cout << " " << cube;
    }
    cout << endl;
  }

  return cc;
}

END_NONAMESPACE

// @brief Blake's Cannonical Form を求める．
SopCover
TvFunc::BCF() const
{
  SizeType ni = input_num();
  auto cov = bcf_sub(ni, *this, 0);
  sort(cov.begin(), cov.end());
  return SopCover{ni, cov};
}

// @brief Blake's Cannonical Form を表す Expr を求める．
Expr
TvFunc::BCF_expr() const
{
  auto cov = BCF();
  return cov.to_expr();
}

END_NAMESPACE_YM
