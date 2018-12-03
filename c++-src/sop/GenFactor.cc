
/// @file GenFactor.cc
/// @brief GenFactor の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.


#include "GenFactor.h"
#include "ym/SopCover.h"
#include "ym/SopCube.h"
#include "Divisor.h"
#include "Divide.h"

#include "OneLevel0Kernel.h"
#include "BestKernel.h"
#include "WeakDivision.h"


BEGIN_NAMESPACE_YM_LOGIC

// @brief コンストラクタ
// @param[in] divisor 除数を求める関数オブジェクト
// @param[in] divide 除算を行う関数オブジェクト
GenFactor::GenFactor(const Divisor& divisor,
		     const Divide& divide) :
  mDivisor(divisor),
  mDivide(divide)
{
}

// @brief デストラクタ
GenFactor::~GenFactor()
{
}

// @brief ファクタリングを行う．
Expr
GenFactor::operator()(const SopCover& f)
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

Expr
GenFactor::literal_factor(const SopCover& f,
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
Expr
GenFactor::cover_to_expr(const SopCover& f)
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


//////////////////////////////////////////////////////////////////////
// クラス QuickFactor
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
QuickFactor::QuickFactor() :
  GenFactor(OneLevel0Kernel(), WeakDivision())
{
}

// @brief デストラクタ
QuickFactor::~QuickFactor()
{
}


//////////////////////////////////////////////////////////////////////
// クラス GoodFactor
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
GoodFactor::GoodFactor() :
  GenFactor(BestKernel(), WeakDivision())
{
}

// @brief デストラクタ
GoodFactor::~GoodFactor()
{
}

END_NAMESPACE_YM_LOGIC
