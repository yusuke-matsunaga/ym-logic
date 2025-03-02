
/// @file Tv2Sop.cc
/// @brief Tv2Sop の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Tv2Sop.h"
#include "ym/SopCover.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

const int debug_bcf = 1;
const int debug_mwc = 2;
const int debug_isop = 4;
const int debug = 0;

void
print_func(
  ostream& s,
  const TvFunc& f,
  const TvFunc& d
)
{
  SizeType ni = f.input_num();
  SizeType ni_exp = 1 << ni;
  for ( SizeType p = 0; p < ni_exp; ++ p ) {
    auto v1 = f.value(p);
    auto v2 = d.value(p);
    if ( v2 ) {
      s << "*";
    }
    else if ( v1 ) {
      s << "1";
    }
    else {
      s << "0";
    }
  }
}

void
print_func(
  ostream& s,
  const TvFunc& f
)
{
  SizeType ni = f.input_num();
  SizeType ni_exp = 1 << ni;
  for ( SizeType p = 0; p < ni_exp; ++ p ) {
    auto v1 = f.value(p);
    if ( v1 ) {
      s << "1";
    }
    else {
      s << "0";
    }
  }
}

void
print_cover(
  ostream& s,
  const vector<vector<Literal>>& cover
)
{
  const char* delim = "";
  for ( auto& cube: cover ) {
    s << delim;
    delim = " + ";
    const char* spc = "";
    for ( auto lit: cube ) {
      s << spc << lit;
      spc = " ";
    }
  }
  cout << endl;
}

// キューブが R と交差する時 true を返す．
inline
bool
check_intersect(
  const vector<Literal>& cube,
  const TvFunc& R
)
{
  // cube を TvFunc に変換する．
  auto f_cube = TvFunc::cube(R.input_num(), cube);
  return f_cube && R;
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
vector<vector<Literal>>
prime_sub(
  const TvFunc& f
)
{
  if ( f.is_zero() ) {
    if ( debug & debug_bcf ) {
      cout << "prime_sub: " << f << endl;
      cout << " ==> " << endl;
    }
    return vector<vector<Literal>>{};
  }
  if ( f.is_one() ) {
    if ( debug & debug_bcf ) {
      cout << "prime_sub: " << f << endl;
      cout << " ==> {}" << endl;
    }
    return vector<vector<Literal>>{{}};
  }

  // 分解する．
  TvFunc f0;
  TvFunc f1;
  f.decompose(f0, f1);

  if ( f0 == f1 ) {
    return prime_sub(f0);
  }
  SizeType var = f0.input_num();
  auto lit0 = Literal(var, true);
  auto lit1 = Literal(var, false);

  // コファクターに対して再帰する．
  // ただし，共通部分を求めて3分割する．
  auto fc = f0 & f1;
  if ( debug & debug_bcf ) {
    cout << "prime_sub: " << f << endl;
    cout << "  fc: " << fc << endl;
    cout << "  f0: " << f0 << endl;
    cout << "  f1: " << f1 << endl;
  }
  auto cube_list = prime_sub(fc);
  auto cube0_list = prime_sub(f0);
  auto cube1_list = prime_sub(f1);
  if ( debug & debug_bcf ) {
    cout << "*prime_sub: " << f << endl;
    cout << "  cc: ";
    print_cover(cout, cube_list);
    cout << "  c0: ";
    print_cover(cout, cube0_list);
    cout << "  c1: ";
    print_cover(cout, cube1_list);
  }

  // 結果をマージする．
  // cube_list はそのまま
  // cube0_list と cube1_list は ~fc と交差している
  // キューブのみを追加する．
  auto R = ~fc;
  for ( auto& cube: cube0_list ) {
    if ( check_intersect(cube, R) ) {
      auto new_cube = cube;
      new_cube.push_back(lit0);
      cube_list.push_back(new_cube);
    }
  }
  for ( auto& cube: cube1_list ) {
    if ( check_intersect(cube, R) ) {
      auto new_cube = cube;
      new_cube.push_back(lit1);
      cube_list.push_back(new_cube);
    }
  }

  if ( debug & debug_bcf ) {
    cout << "**prime_sub: " << f << endl;
    cout << " ==> ";
    print_cover(cout, cube_list);
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
  auto cube_list = prime_sub(f);
  vector<SopCube> ans_list;
  SizeType ni = f.input_num();
  for ( auto& lits: cube_list ) {
    auto cube = SopCube(ni, lits);
    ans_list.push_back(cube);
  }
  return ans_list;
}

#if 0
BEGIN_NONAMESPACE

// cube が f に包含されている時 true を返す．
bool
check_containment(
  const Cube& cube,
  const TvFunc& f
)
{
  auto f1 = cube.tvfunc();
  return f1.check_containment(f);
}

// MWC(Merge With Containment) の下請け関数
vector<vector<Literal>>
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
    return vector<vector<Literal>>{};
  }
  if ( f.is_one() ) {
    if ( debug & debug_mwc ) {
      cout << "mwc_sub(" << var << "): " << f << endl;
      cout << " ==> 1" << endl;
    }
    auto ni = f.input_num();
    return vector<vector<Literal>>{{}};
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
    print_cover(cout, cube0_list);
    cout << " cube1_list:";
    print_cover(cout, cube1_list);
  }

  // cube0_list, cube1_list で同一のものを探す．
  // 同時に他方に包含されているキューブを削除する．
  vector<vector<Literal>> ans_list;
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
	auto tmp_cube = cube0;
	tmp_cube.push_back(lit0);
	ans_list.push_back(tmp_cube);
      }
    }
    else if ( stat > 0 ) {
      ++ cube1_iter;
      if ( check_containment(cube1, f0) ) {
	ans_list.push_back(std::move(cube1));
      }
      else {
	auto tmp_cube = cube1;
	tmp_cube.push_back(lit1);
	ans_list.push_back(tmp_cube);
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
      auto tmp_cube = cube0;
      tmp_cube.push_back(lit0);
      ans_list.push_back(tmp_cube);
    }
    ++ cube0_iter;
  }
  while ( cube1_iter != cube1_end ) {
    auto& cube1 = *cube1_iter;
    if ( check_containment(cube1, f0) ) {
      ans_list.push_back(std::move(cube1));
    }
    else {
      auto tmp_cube = cube1;
      tmp_cube.push_back(lit1);
      ans_list.push_back(tmp_cube);
    }
    ++ cube1_iter;
  }

  if ( debug & debug_mwc ) {
    cout << "**mwc_sub(" << var << "): " << f << endl;
    cout << " ==> ";
    print_cover(cout, ans_list);
  }

  return ans_list;
}

END_NONAMESPACE

// @brief Merge With Containment を行って積和系論理式を得る．
vector<SopCube>
Tv2Sop::MWC(
  const TvFunc& f
)
{
  auto cube_list = mwc_sub(f, 0);
  return cube_list;
}
#endif

BEGIN_NONAMESPACE

// isop() の下請け関数
vector<vector<Literal>>
isop_sub(
  const TvFunc& f,
  const TvFunc& d
)
{
  if ( f.check_containment(d) ) {
    // f が 0
    // 結果は 0
    if ( debug & debug_isop ) {
      cout << "isop_sub: ";
      print_func(cout, f, d);
      cout << endl;
      cout << " ==> 0" << endl;
    }
    return vector<vector<Literal>>{};
  }
  if ( (~f).check_containment(d) ) {
    // r が 0
    // 結果は 1
    if ( debug & debug_isop ) {
      cout << "isop_sub: ";
      print_func(cout, f, d);
      cout << endl;
      cout << " ==> {}" << endl;
    }
    return vector<vector<Literal>>{{}};
  }

  // f と r を分解する．
  TvFunc f0;
  TvFunc f1;
  f.decompose(f0, f1);
  TvFunc d0;
  TvFunc d1;
  d.decompose(d0, d1);

  // special cases
  if ( f0 == f1 && d0 == d1 ) {
    return isop_sub(f0, d0);
  }
  if ( d0.is_one() ) {
    return isop_sub(f1, d1);
  }
  if ( d1.is_one() ) {
    return isop_sub(f0, d0);
  }

  // 再帰する．
  auto fc = (f0 | d0) & (f1 | d1);
  auto cube0_list = isop_sub(f0, d0 | fc);
  auto cube1_list = isop_sub(f1, d1 | fc);
  auto ni = f.input_num() - 1;
  auto f0_new = TvFunc::cover(ni, cube0_list);
  auto f1_new = TvFunc::cover(ni, cube1_list);
  auto dc = (d0 | f0_new) & (d1 | f1_new);
  auto cube_list = isop_sub(fc, dc);
  if ( debug & debug_isop ) {
    cout << "*isop_sub: ";
    print_func(cout, f, d);
    cout << endl;
    cout << "  cube0_list: ";
    print_cover(cout, cube0_list);
    cout << "  cube1_list: ";
    print_cover(cout, cube1_list);
    cout << "fc: ";
    print_func(cout, fc, dc);
    cout << endl;
    cout << "  cube_list: ";
    print_cover(cout, cube_list);
  }

  // マージする．
  auto size = cube_list.size() + cube0_list.size() + cube1_list.size();
  cube_list.reserve(size);
  auto lit1 = Literal(ni, false);
  auto lit0 = ~lit1;
  for ( auto cube: cube0_list ) {
    cube.push_back(lit0);
    cube_list.push_back(cube);
  }
  for ( auto cube: cube1_list ) {
    cube.push_back(lit1);
    cube_list.push_back(cube);
  }

  if ( debug & debug_isop ) {
    cout << "**isop_sub: ";
    print_func(cout, f, d);
    cout << endl;
    cout << " ==> ";
    print_cover(cout, cube_list);
  }
  return cube_list;
}

END_NONAMESPACE

// @brief 単純なシャノン展開を行って非冗長積和形を求める．
vector<SopCube>
Tv2Sop::isop(
  const TvFunc& f ///< [in] 対象の関数
)
{
  SizeType ni = f.input_num();
  auto d = TvFunc::zero(ni);
  return isop(f, d);
}

// @brief 単純なシャノン展開で非冗長積和形を求める．
vector<SopCube>
Tv2Sop::isop(
  const TvFunc& f,
  const TvFunc& d
)
{
  SizeType ni = f.input_num();
  if ( ni != d.input_num() ) {
    throw std::invalid_argument{"input_num() mismatch"};
  }
  auto cube_list = isop_sub(f, d);
  vector<SopCube> ans_list;
  for ( auto& lits: cube_list ) {
    auto cube = SopCube(ni, lits);
    ans_list.push_back(cube);
  }
  return ans_list;
}

END_NAMESPACE_YM
