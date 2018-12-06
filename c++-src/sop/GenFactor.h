#ifndef GENFACTOR_H
#define GENFACTOR_H

/// @file GenFactor.h
/// @brief GenFactor のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.

#include "ym/SopCover.h"
#include "ym/SopCube.h"
#include "ym/Expr.h"
#include "OneLevel0Kernel.h"
#include "BestKernel.h"
#include "WeakDivision.h"


BEGIN_NAMESPACE_YM_LOGIC

//////////////////////////////////////////////////////////////////////
/// @class GenFactor GenFactor.h "GenFactor.h"
/// @brief 'GEN_FACTOR' アルゴリズムの実装クラス
//////////////////////////////////////////////////////////////////////
template<class Divisor, class Divide>
class GenFactor
{
public:

  /// @brief コンストラクタ
  GenFactor();

  /// @brief デストラクタ
  ~GenFactor();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ファクタリングを行う．
  Expr
  operator()(const SopCover& f);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 'LF' を行う．
  Expr
  literal_factor(const SopCover& f,
		 const vector<Literal>& lit_list);

  /// @brief SopCover をそのまま Expr に変換する．
  Expr
  cover_to_expr(const SopCover& f);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 除数を求めるクラス
  Divisor mDivisor;

  // 除算を行うクラス
  Divide mDivide;

};


//////////////////////////////////////////////////////////////////////
/// @class QuickFactor QuickFactor.h "GenFactor.h"
/// @brief 'QUICK_FACTOR' を実行するクラス
//////////////////////////////////////////////////////////////////////
using QuickFactor = GenFactor<OneLevel0Kernel, WeakDivision>;


//////////////////////////////////////////////////////////////////////
/// @class GoodFactor GoodFactor.h "GenFactor.h"
/// @brief 'GOOD_FACTOR' を実行するクラス
//////////////////////////////////////////////////////////////////////
using GoodFactor = GenFactor<BestKernel, WeakDivision>;


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////


// @brief コンストラクタ
template<class Divisor, class Divide>
inline
GenFactor<Divisor, Divide>::GenFactor()
{
}

// @brief デストラクタ
template<class Divisor, class Divide>
inline
GenFactor<Divisor, Divide>::~GenFactor()
{
}

// @brief ファクタリングを行う．
template<class Divisor, class Divide>
inline
Expr
GenFactor<Divisor, Divide>::operator()(const SopCover& f)
{
  SopCover d = mDivisor(f);
  if ( d.cube_num() == 0 ) {
    return cover_to_expr(f);
  }

  auto p = mDivide(f, d);
  const SopCover& q = p.first;
  const SopCover& r = p.second;
  if ( q.cube_num() == 1 ) {
    vector<vector<Literal>> cube_list;
    q.to_literal_list(cube_list);
    ASSERT_COND( cube_list.size() == 1 );
    return literal_factor(f, cube_list[0]);
  }
  else {
    SopCube cc = q.common_cube();
    SopCover q1 = q / cc;
    auto p = mDivide(f, q1);
    const SopCover& d1 = p.first;
    const SopCover& r1 = p.second;
    SopCube cc1 = d1.common_cube();
    if ( cc1.literal_num() == 0 ) {
      Expr q_expr = operator()(q1);
      Expr d_expr = operator()(d1);
      Expr r_expr = operator()(r1);
      return (q_expr & d_expr) | r_expr;
    }
    else {
      vector<Literal> lit_list;
      cc1.to_literal_list(lit_list);
      return literal_factor(f, lit_list);
    }
  }
}

template<class Divisor, class Divide>
inline
Expr
GenFactor<Divisor, Divide>::literal_factor(const SopCover& f,
					   const vector<Literal>& lit_list)
{
  // l <- lit_list に含まれるリテラルで最も出現回数の多いもの
  int max_n = 0;
  Literal l;
  for ( auto lit: lit_list ) {
    int n = f.literal_num(lit);
    if ( max_n < n ) {
      max_n = n;
      l = lit;
    }
  }
  SopCover q = f / l;
  SopCover r = f - q * l;
  Expr q_expr = operator()(q);
  Expr d_expr = Expr::literal(l);
  Expr r_expr = operator()(r);
  return (q_expr & d_expr) | r_expr;
}

// @brief SopCover をそのまま Expr に変換する．
template<class Divisor, class Divide>
inline
Expr
GenFactor<Divisor, Divide>::cover_to_expr(const SopCover& f)
{
  vector<vector<Literal>> cube_list;
  f.to_literal_list(cube_list);

  int nc = cube_list.size();
  if ( nc == 0 ) {
    return Expr::zero();
  }
  vector<Expr> and_list(nc);
  for ( int i = 0; i < nc; ++ i ) {
    const vector<Literal>& cube = cube_list[i];
    int nl = cube.size();
    vector<Expr> lit_list(nl);
    for ( int j = 0; j < nl; ++ j ) {
      lit_list[j] = Expr::literal(cube[j]);
    }
    and_list[i] = Expr::make_and(lit_list);
  }
  return Expr::make_or(and_list);
}


END_NAMESPACE_YM_LOGIC

#endif // GENFACTOR_H
