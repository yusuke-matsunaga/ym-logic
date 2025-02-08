
/// @file Tv2Sop.cc
/// @brief Tv2Sop の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Tv2Sop.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

const int debug_bcf = 1;
const int debug_mwc = 2;
const int debug_isop = 4;
const int debug = 0;

// キューブが R と交差する時 true を返す．
bool
check_intersect(
  const SopCube& cube,
  const TvFunc& R
)
{
  // cube を TvFunc に変換する．
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

// all_primes の下請け関数
//
// f に対する主項のリストを返す．
//
// f を var で分解し，3つの部分に分ける．
// f0: ~var のコファクター
// f1:  var のコファクター
// fc: f0 と f1 の共通部分
//
// f0, f1, fc それぞれに prime_sub() を適用して
// それぞれの結果を得る．c0, c1, cc とする．
// このうち，cc に含まれるキューブは明らかに
// f にとっても主項である．
// c0/c1 のキューブは fc に含まれない部分が
// ある場合のみ ~var/var を付加したものを加える．
vector<SopCube>
prime_sub(
  const TvFunc& f,
  SizeType var
)
{
  if ( f.is_zero() ) {
    if ( debug & debug_bcf ) {
      cout << "prime_sub(" << var << "): " << f << endl;
      cout << " ==> " << endl;
    }
    return vector<SopCube>{};
  }
  if ( f.is_one() ) {
    if ( debug & debug_bcf ) {
      cout << "prime_sub(" << var << "): " << f << endl;
      cout << " ==> {}" << endl;
    }
    auto ni = f.input_num();
    return vector<SopCube>{SopCube{ni}};
  }

  // 次に分解する変数を求める．
  while ( !f.check_sup(var) ) {
    ++ var;
  }
  auto lit0 = Literal{var, true};
  auto lit1 = Literal{var, false};

  // コファクターに対して再帰する．
  // ただし，共通部分を求めて3分割する．
  auto f0 = f.cofactor(lit0);
  auto f1 = f.cofactor(lit1);
  auto fc = f0 & f1;
  if ( debug & debug_bcf ) {
    cout << "prime_sub(" << var << "): " << f << endl;
    cout << "  fc: " << fc << endl;
    cout << "  f0: " << f0 << endl;
    cout << "  f1: " << f1 << endl;
  }
  auto cube_list = prime_sub(fc, var + 1);
  auto cube0_list = prime_sub(f0, var + 1);
  auto cube1_list = prime_sub(f1, var + 1);
  if ( debug & debug_bcf ) {
    cout << "*prime_sub(" << var << "): " << f << endl;
    cout << "  cc: ";
    for ( auto& cube: cube_list ) {
      cout << " " << cube;
    }
    cout << endl;
    cout << "  c0: ";
    for ( auto& cube: cube0_list ) {
      cout << " " << cube;
    }
    cout << endl;
    cout << "  c1: ";
    for ( auto& cube: cube1_list ) {
      cout << " " << cube;
    }
    cout << endl;
  }

  // 結果をマージする．
  // cube_list はそのまま
  // cube0_list と cube1_list は ~fc と交差している
  // キューブのみを追加する．
  auto R = ~fc;
  for ( auto& cube: cube0_list ) {
    if ( check_intersect(cube, R) ) {
      cube_list.push_back(cube & lit0);
    }
  }
  for ( auto& cube: cube1_list ) {
    if ( check_intersect(cube, R) ) {
      cube_list.push_back(cube & lit1);
    }
  }

  if ( debug & debug_bcf ) {
    cout << "**prime_sub(" << var << "): " << f << endl;
    cout << " ==> ";
    const char* spc = "";
    for ( auto& cube: cube_list ) {
      cout << spc << cube;
      spc = ", ";
    }
    cout << endl;
  }

  return cube_list;
}

END_NONAMESPACE

// @brief 主項を求める．
vector<SopCube>
Tv2Sop::all_primes(
  const TvFunc& f
)
{
  auto cube_list = prime_sub(f, 0);
  return cube_list;
}

// @brief Blake's Cannonical Form を求める．
SopCover
Tv2Sop::BCF(
  const TvFunc& f
)
{
  auto cube_list = all_primes(f);
  return SopCover{f.input_num(), cube_list};
}

BEGIN_NONAMESPACE

// cube が f に包含されている時 true を返す．
bool
check_containment(
  const SopCube& cube,
  const TvFunc& f
)
{
  auto f1 = cube.tvfunc();
  return !( f1 && ~f );
}

// MWC(Merge With Containment) の下請け関数
vector<SopCube>
mwc_sub(
  const TvFunc& f,
  SizeType var
)
{
  if ( f.is_zero() ) {
    if ( debug & debug_mwc ) {
      cout << "mwc_sub(" << var << "): " << f << endl;
      cout << " ==> 0" << endl;
    }
    return vector<SopCube>{};
  }
  if ( f.is_one() ) {
    if ( debug & debug_mwc ) {
      cout << "mwc_sub(" << var << "): " << f << endl;
      cout << " ==> 1" << endl;
    }
    auto ni = f.input_num();
    return vector<SopCube>{SopCube{ni}};
  }

  // 次に分解する変数を求める．
  while ( !f.check_sup(var) ) {
    ++ var;
  }
  auto lit0 = Literal{var, true};
  auto lit1 = Literal{var, false};

  // コファクターを求める．
  auto f0 = f.cofactor(lit0);
  auto f1 = f.cofactor(lit1);

  if ( debug & debug_mwc ) {
    cout << "mwc_sub(" << var << "): " << f << endl;
    cout << "  f0: " << f0 << endl;
    cout << "  f1: " << f1 << endl;
  }

  // コファクターに対して再帰する．
  auto cube0_list = mwc_sub(f0, var + 1);
  auto cube1_list = mwc_sub(f1, var + 1);
  if ( debug & debug_mwc ) {
    cout << "mwc_sub(" << var << "): " << f << endl;
    cout << " cube0_list:";
    for ( auto& cube: cube0_list ) {
      cout << " " << cube;
    }
    cout << endl;
    cout << " cube1_list:";
    for ( auto& cube: cube1_list ) {
      cout << " " << cube;
    }
    cout << endl;
  }

  // cube0_list, cube1_list で同一のものを探す．
  // 同時に他方に包含されているキューブを削除する．
  vector<SopCube> ans_list;
  auto n0 = cube0_list.size();
  auto n1 = cube1_list.size();
  auto r0 = ~f0;
  auto r1 = ~f1;
  auto cube0_iter = cube0_list.begin();
  auto cube1_iter = cube1_list.begin();
  auto cube0_end = cube0_list.end();
  auto cube1_end = cube1_list.end();
  while ( cube0_iter != cube0_end &&
	  cube1_iter != cube1_end ) {
    auto& cube0 = *cube0_iter;
    auto& cube1 = *cube1_iter;
    int stat = compare(cube0, cube1);
    if ( stat < 0 ) {
      ++ cube0_iter;
      if ( check_containment(cube0, f1) ) {
	ans_list.push_back(std::move(cube0));
      }
      else {
	ans_list.push_back(cube0 & lit0);
      }
    }
    else if ( stat > 0 ) {
      ++ cube1_iter;
      if ( check_containment(cube1, f0) ) {
	ans_list.push_back(std::move(cube1));
      }
      else {
	ans_list.push_back(cube1 & lit1);
      }
    }
    else {
      ++ cube0_iter;
      ++ cube1_iter;
      ans_list.push_back(std::move(cube0));
    }
  }
  while ( cube0_iter != cube0_end ) {
    auto& cube0 = *cube0_iter;
    if ( check_containment(cube0, f1) ) {
      ans_list.push_back(std::move(cube0));
    }
    else {
      ans_list.push_back(cube0 & lit0);
    }
    ++ cube0_iter;
  }
  while ( cube1_iter != cube1_end ) {
    auto& cube1 = *cube1_iter;
    if ( check_containment(cube1, f0) ) {
      ans_list.push_back(std::move(cube1));
    }
    else {
      ans_list.push_back(cube1 & lit1);
    }
    ++ cube1_iter;
  }

  if ( debug & debug_mwc ) {
    cout << "**mwc_sub(" << var << "): " << f << endl;
    cout << " ==> ";
    const char* spc = "";
    for ( auto& cube: ans_list ) {
      cout << spc << cube;
      spc = ", ";
    }
    cout << endl;
  }

  return ans_list;
}

END_NONAMESPACE

// @brief Merge With Containment を行って積和系論理式を得る．
SopCover
Tv2Sop::MWC(
  const TvFunc& f
)
{
  auto cube_list = mwc_sub(f, 0);
  return SopCover{f.input_num(), cube_list};
}

BEGIN_NONAMESPACE

// ISOP() の下請け関数
vector<SopCube>
isop_sub(
  const TvFunc& f,
  const TvFunc& dc,
  SizeType var
)
{
  if ( f.check_containment(dc) ) {
    // f が dc に含まれている．
    // 結果は 0
    if ( debug & debug_isop ) {
      cout << "isop_sub(" << var << "): " << f
	   << ", " << dc << endl;
      cout << " ==> 0" << endl;
    }
    return {};
  }
  if ( (~f).check_containment(dc) ) {
    // ~f が dc に含まれている．
    // 結果は 1
    if ( debug & debug_isop ) {
      cout << "isop_sub(" << var << "): " << f
	   << ", " << dc << endl;
      cout << " ==> {}" << endl;
    }
    auto ni = f.input_num();
    return {SopCube{ni}};
  }

  // 次に分解する変数を求める．
  while ( !f.check_sup(var) &&
	  !dc.check_sup(var) ) {
    ++ var;
  }
  auto lit0 = Literal{var, true};
  auto lit1 = Literal{var, false};

  // f と dc を var で分解する．
  auto f0 = f.cofactor(lit0);
  auto dc0 = dc.cofactor(lit0);

  auto f1 = f.cofactor(lit1);
  auto dc1 = dc.cofactor(lit1);

  // var と無関係な部分を求める．
  auto f2 = f0 & f1;
  auto dc2 = dc0 & dc1;

  if ( debug & debug_isop ) {
    cout << "isop_sub(" << var << "): " << f
	 << ", " << dc << endl;
    cout << "  f0: " << f0 << ", " << dc0 << endl;
    cout << "  f1: " << f1 << ", " << dc1 << endl;
    cout << "  f2: " << f2 << ", " << dc2 << endl;
  }

  auto cube_list = isop_sub(f2, dc2, var + 1);

  // cube_list を dc0 と dc1 に加える．
  if ( !cube_list.empty() ) {
    auto dc_new = SopCube::tvfunc(cube_list);
    dc0 |= dc_new;
    dc1 |= dc_new;
  }

  if ( debug & debug_isop ) {
    cout << "*isop_sub(" << var << "): " << f
	 << ", " << dc << endl;
    cout << "  cube_list: ";
    for ( auto& cube: cube_list ) {
      cout << " " << cube;
    }
    cout << endl;
    cout << " dc0: " << dc0 << endl
	 << " dc1: " << dc1 << endl;
  }

  // 再帰する．
  auto cube0_list = isop_sub(f0, dc0, var + 1);
  auto cube1_list = isop_sub(f1, dc1, var + 1);

  if ( debug & debug_isop ) {
    cout << "*isop_sub(" << var << "): " << f
	 << ", " << dc << endl;
    cout << "  cube_list: ";
    for ( auto& cube: cube_list ) {
      cout << " " << cube;
    }
    cout << endl;
    cout << "  cube0_list: ";
    for ( auto& cube: cube0_list ) {
      cout << " " << cube;
    }
    cout << endl;
    cout << "  cube1_list: ";
    for ( auto& cube: cube1_list ) {
      cout << " " << cube;
    }
    cout << endl;
  }

  // マージする．
  auto size = cube_list.size() + cube0_list.size() + cube1_list.size();
  cube_list.reserve(size);
  for ( auto& cube: cube0_list ) {
    cube_list.push_back(cube & lit0);
  }
  for ( auto& cube: cube1_list ) {
    cube_list.push_back(cube & lit1);
  }

  if ( debug & debug_isop ) {
    cout << "**isop_sub(" << var << "): " << f
	 << ", " << dc << endl;
    cout << " ==> ";
    const char* spc = "";
    for ( auto& cube: cube_list ) {
      cout << spc << cube;
      spc = ", ";
    }
    cout << endl;
  }

  return cube_list;
}

END_NONAMESPACE

// @brief 単純なシャノン展開を行ってで非冗長積和形を求める．
SopCover
Tv2Sop::ISOP(
  const TvFunc& f ///< [in] 対象の関数
)
{
  auto dc = TvFunc::zero(f.input_num());
  return ISOP(f, dc);
}

// @brief 単純なシャノン展開で非冗長積和形を求める．
SopCover
Tv2Sop::ISOP(
  const TvFunc& f,
  const TvFunc& dc
)
{
  SizeType ni = f.input_num();
  if ( ni != dc.input_num() ) {
    throw std::invalid_argument{"input_num() mismatch"};
  }
  auto cube_list = isop_sub(f, dc, 0);
  return SopCover{ni, cube_list};
}

END_NAMESPACE_YM
