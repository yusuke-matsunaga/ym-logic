
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

const bool debug = false;

// キューブが R と交差する時 true を返す．
bool
check_cube(
  const SopCube& cube,
  const TvFunc& R
)
{
  // これ実装はもうちょっと効率化できるはず．
  auto ni = cube.variable_num();
  auto f_cube = TvFunc::one(ni);
  for ( SizeType var = 0; var < ni; ++ var ) {
    auto pat = cube.get_pat(var);
    if ( pat == SopPat::_1 ) {
      f_cube &= TvFunc::posi_literal(ni, var);
    }
    else if ( pat == SopPat::_0 ) {
      f_cube &= TvFunc::nega_literal(ni, var);
    }
  }
  if ( f_cube && R ) {
    return true;
  }
  return false;
}

// BCF の下請け関数
//
// f に対する主項のリストを返す．
//
// f を var で分解し，3つの部分に分ける．
// f0: ~var のコファクター
// f1:  var のコファクター
// fc: f0 と f1 の共通部分
//
// f0, f1, fc それぞれに bcf_sub() を適用して
// それぞれの結果を得る．c0, c1, cc とする．
// このうち，cc に含まれるキューブは明らかに
// f にとっても主項である．
// c0/c1 のキューブは fc に含まれない部分が
// ある場合のみ ~var/var を付加したものを加える．
vector<SopCube>
bcf_sub(
  const TvFunc& f,
  SizeType var
)
{
  auto ni = f.input_num();

  if ( f.is_zero() ) {
    if ( debug ) {
      cout << "bcf_sub(" << var << "): " << f << endl;
      cout << " ==> " << endl;
    }
    return vector<SopCube>{};
  }
  if ( f.is_one() ) {
    if ( debug ) {
      cout << "bcf_sub(" << var << "): " << f << endl;
      cout << " ==> {}" << endl;
    }
    return vector<SopCube>{SopCube{ni}};
  }

  // コファクターに対して再帰する．
  // ただし，共通部分を求めて3分割する．
  ASSERT_COND( var < ni );
  auto f0 = f.cofactor(var, true);
  auto f1 = f.cofactor(var, false);
  auto fc = f0 & f1;
  if ( debug ) {
    cout << "bcf_sub(" << var << "): " << f << endl;
    cout << "  fc: " << fc << endl;
    cout << "  f0: " << f0 << endl;
    cout << "  f1: " << f1 << endl;
  }
  auto cc = bcf_sub(fc, var + 1);
  auto c0 = bcf_sub(f0, var + 1);
  auto c1 = bcf_sub(f1, var + 1);
  if ( debug ) {
    cout << "*bcf_sub(" << var << "): " << f << endl;
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
  auto lit0 = Literal{var, true};
  for ( auto& cube: c0 ) {
    if ( check_cube(cube, R) ) {
      cc.push_back(cube & lit0);
    }
  }
  auto lit1 = Literal{var, false};
  for ( auto& cube: c1 ) {
    if ( check_cube(cube, R) ) {
      cc.push_back(cube & lit1);
    }
  }

  if ( debug ) {
    cout << "**bcf_sub(" << var << "): " << f << endl;
    cout << " ==> ";
    const char* spc = "";
    for ( auto& cube: cc ) {
      cout << spc << cube;
      spc = ", ";
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
  auto cov = bcf_sub(*this, 0);
  return SopCover{input_num(), cov};
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
  const TvFunc& f,
  SizeType var
)
{
  auto ni = f.input_num();

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

  auto cov0 = mwc_sub(f0, var + 1);
  auto cov1 = mwc_sub(f1, var + 1);

  // cov0, cov1 で同一のものを探す．
  // 同時に他方に包含されているキューブを削除する．
  vector<SopCube> ans_list;
  auto n0 = cov0.size();
  auto n1 = cov1.size();
  auto r0 = ~f0;
  auto r1 = ~f1;
  auto lit0 = Literal{var, true};
  auto lit1 = Literal{var, false};
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
	ans_list.push_back(cube0 & lit0);
      }
    }
    else if ( stat > 0 ) {
      ++ i1;
      if ( check_containment(cube1, cov0) ) {
	ans_list.push_back(std::move(cube1));
      }
      else {
	ans_list.push_back(cube1 & lit1);
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
      ans_list.push_back(cube0 & lit0);
    }
    ++ i0;
  }
  while ( i1 < n1 ) {
    const auto& cube1 = cov1[i1];
    if ( check_containment(cube1, cov0) ) {
      ans_list.push_back(std::move(cube1));
    }
    else {
      ans_list.push_back(cube1 & lit1);
    }
    ++ i1;
  }
  std::sort(ans_list.begin(), ans_list.end());
  return ans_list;
}

END_NONAMESPACE

// @brief Merge With Containment を行って積和系論理式を得る．
SopCover
TvFunc::MWC() const
{
  auto cov = mwc_sub(*this, 0);
  return SopCover{input_num(), cov};
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
