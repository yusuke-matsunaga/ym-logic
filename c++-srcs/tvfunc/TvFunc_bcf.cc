
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

// キューブの表している論理関数を作る．
TvFunc
make_func(
  SizeType ni,
  const SopCube& cube
)
{
  TvFunc f_cube = TvFunc::make_one(ni);
  for ( SizeType var = 0; var < ni; ++ var ) {
    auto pat = cube.get_pat(var);
    if ( pat == SopPat::_1 ) {
      f_cube &= TvFunc::make_posi_literal(ni, var);
    }
    else if ( pat == SopPat::_0 ) {
      f_cube &= TvFunc::make_nega_literal(ni, var);
    }
  }
  return f_cube;
}

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

  // コファクターに対して再帰する．
  // ただし，共通部分を求めて3分割する．
  auto f0 = f.cofactor(var, true);
  auto f1 = f.cofactor(var, false);
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
  Literal lit0{var, true};
  for ( auto& cube: c0 ) {
    auto f_cube = make_func(ni, cube);
    if ( f_cube && R ) {
      cc.push_back(cube * lit0);
    }
  }
  Literal lit1{var, false};
  for ( auto& cube: c1 ) {
    auto f_cube = make_func(ni, cube);
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
  return cov.expr();
}

BEGIN_NONAMESPACE

bool
check_containment(
  const SopCube& cube,
  const vector<SopCube>& cover
)
{
  for ( auto cube1: cover ) {
    if ( cube.check_containment(cube1) ) {
      return true;
    }
  }
  return false;
}

vector<SopCube>
mwc_sub(
  SizeType ni,
  const TvFunc& f,
  SizeType var
)
{

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

  // コファクターに対して再帰する．
  auto f0 = f.cofactor(var, true);
  auto f1 = f.cofactor(var, false);

  auto cov0 = mwc_sub(ni, f0, var + 1);
  auto cov1 = mwc_sub(ni, f1, var + 1);

  // cov0, cov1 で同一のものを探す．
  // 同時に他方に包含されているキューブを削除する．
  vector<SopCube> ans_list;
  SizeType n0 = cov0.size();
  SizeType n1 = cov1.size();
  auto r0 = ~f0;
  auto r1 = ~f1;
  Literal lit0{var, true};
  Literal lit1{var, false};
  SizeType i0 = 0;
  SizeType i1 = 0;
  while ( i0 < n0 && i1 < n1 ) {
    const auto& cube0 = cov0[i0];
    const auto& cube1 = cov1[i1];
    int stat = compare(cube0, cube1);
    if ( stat < 0 ) {
      ++ i0;
      if ( check_containment(cube0, cov1) ) {
	ans_list.push_back(std::move(cube0));
      }
      else {
	ans_list.push_back(cube0 * lit0);
      }
    }
    else if ( stat > 0 ) {
      ++ i1;
      if ( check_containment(cube1, cov0) ) {
	ans_list.push_back(std::move(cube1));
      }
      else {
	ans_list.push_back(cube1 * lit1);
      }
    }
    else {
      ++ i0;
      ++ i1;
      ans_list.push_back(std::move(cube0));
    }
  }
  while ( i0 < n0 ) {
    const auto& cube0 = cov0[i0];
    if ( check_containment(cube0, cov1) ) {
      ans_list.push_back(std::move(cube0));
    }
    else {
      ans_list.push_back(cube0 * lit0);
    }
    ++ i0;
  }
  while ( i1 < n1 ) {
    const auto& cube1 = cov1[i1];
    if ( check_containment(cube1, cov0) ) {
      ans_list.push_back(std::move(cube1));
    }
    else {
      ans_list.push_back(cube1 * lit1);
    }
    ++ i1;
  }
  sort(ans_list.begin(), ans_list.end());
  return ans_list;
}

END_NONAMESPACE

// @brief Merge With Containment を行って積和系論理式を得る．
SopCover
TvFunc::MWC() const
{
  SizeType ni = input_num();
  auto cov = mwc_sub(ni, *this, 0);
  return SopCover{ni, cov};
}

// @brief Merge With Containment を行って Expr を得る．
Expr
TvFunc::MWC_expr() const
{
  auto cov = MWC();
  auto expr = cov.expr();
  return expr;
}

END_NAMESPACE_YM
