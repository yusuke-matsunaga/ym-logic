#ifndef GENFACTOR_H
#define GENFACTOR_H

/// @file GenFactor.h
/// @brief GenFactor のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/SopCover.h"
#include "ym/SopCube.h"
#include "ym/Expr.h"


BEGIN_NAMESPACE_YM_FACTOR

//////////////////////////////////////////////////////////////////////
/// @class GenFactor GenFactor.h "GenFactor.h"
/// @brief 'GEN_FACTOR' アルゴリズムの実装クラス
//////////////////////////////////////////////////////////////////////
template<class Divisor, class Divide>
class GenFactor
{
public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ファクタリングを行う．
  Expr
  operator()(
    const SopCover& f
  )
  {
    return factor(f);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファクタリングを行う．
  Expr
  factor(
    const SopCover& f
  )
  {
    auto d = Divisor::divisor(f);
    if ( d.cube_num() == 0 ) {
      return cov_to_expr(f);
    }
    auto p = Divide::divide(f, d);
    auto& q = p.first;
    auto& r = p.second;
    if ( q.cube_num() == 1 ) {
      auto cube_list = q.literal_list();
      ASSERT_COND( cube_list.size() == 1 );
      return literal_factor(f, cube_list[0]);
    }
    else {
      auto cc = q.common_cube();
      auto q1 = q.algdiv(cc);
      auto p = Divide::divide(f, q1);
      auto& d1 = p.first;
      auto& r1 = p.second;
      auto cc1 = d1.common_cube();
      if ( cc1.literal_num() == 0 ) {
	auto q_expr = factor(q1);
	auto d_expr = factor(d1);
	auto r_expr = factor(r1);
	return (q_expr & d_expr) | r_expr;
      }
      else {
	auto lit_list = cc1.literal_list();
	return literal_factor(f, lit_list);
      }
    }
  }

  /// @brief 'LF' を行う．
  Expr
  literal_factor(
    const SopCover& f,
    const vector<Literal>& lit_list
  )
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
    auto q = f.algdiv(l);
    auto r = f - q & l;
    auto q_expr = factor(q);
    auto d_expr = Expr::literal(l);
    auto r_expr = factor(r);
    return (q_expr & d_expr) | r_expr;
  }

  /// @brief SopCover をそのまま Expr に変換する．
  Expr
  cov_to_expr(
    const SopCover& f
  )
  {
    auto cube_list = f.literal_list();

    SizeType nc = cube_list.size();
    if ( nc == 0 ) {
      return Expr::zero();
    }
    vector<Expr> and_list;
    and_list.reserve(nc);
    for ( auto cube: cube_list ) {
      SizeType nl = cube.size();
      vector<Expr> lit_list;
      lit_list.reserve(nl);
      for ( auto lit: cube ) {
	auto lit_expr = Expr::literal(lit);
	lit_list.push_back(lit_expr);
      }
      auto and_expr = Expr::and_op(lit_list);
      and_list.push_back(and_expr);
    }
    return Expr::or_op(and_list);
  }

};

END_NAMESPACE_YM_FACTOR

#endif // GENFACTOR_H
