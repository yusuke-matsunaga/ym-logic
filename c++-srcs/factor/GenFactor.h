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


BEGIN_NAMESPACE_YM_SOP

const int debug = 0;

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
    if ( debug ) {
      cout << "factor(f" << endl
	   << "f[" << f.cube_num() << "] = " << f << endl;
    }
    if ( f.cube_num() == 0 ) {
      if ( debug ) {
	cout << " => 0" << endl;
      }
      return Expr::zero();
    }
    auto d = Divisor::divisor(f);
    if ( d.cube_num() == 0 ) {
      auto expr = cov_to_expr(f);
      if ( debug ) {
	cout << " => " << expr << endl;
      }
      return expr;
    }
    auto p = Divide::divide(f, d);
    auto& q = p.first;
    auto& r = p.second;
    if ( debug ) {
      cout << "--------------------------" << endl;
      cout << "d[" << d.cube_num() << "] = " << d << endl
	   << "q[" << q.cube_num() << "] = " << q << endl
	   << "r[" << r.cube_num() << "] = " << r << endl;
    }
    if ( q.literal_num() == 0 ) {
      // special case
      if ( debug ) {
	cout << "factor(d)" << endl;
      }
      auto d_expr = factor(d);
      if ( debug ) {
	cout << "factor(r)" << endl;
      }
      auto r_expr = factor(r);
      auto expr = d_expr | r_expr;
      if ( debug ) {
	cout << "*factor(" << f << ")" << endl
	     << " => " << expr << endl;
      }
      return expr;
    }
    if ( q.cube_num() == 1 ) {
      auto cube_list = q.literal_list();
      if ( cube_list.size() != 1 ) {
	throw std::logic_error{"cube_list.size() != 1"};
      }
      if ( debug ) {
	cout << "literal_factor(f)" << endl;
      }
      auto expr = literal_factor(f, cube_list.front());
      if ( debug ) {
	cout << "*factor(" << f << ")" << endl
	     << " => " << expr << endl;
      }
      return expr;
    }
    else {
      auto cc = q.common_cube();
      auto q1 = q.algdiv(cc);
      auto p = Divide::divide(f, q1);
      auto& d1 = p.first;
      auto& r1 = p.second;
      if ( debug ) {
	cout << "--------------------------" << endl;
	cout << "cc = " << cc << endl;
	cout << "d1[" << d1.cube_num() << "] = " << d1 << endl
	     << "q1[" << q1.cube_num() << "] = " << q1 << endl
	     << "r1[" << r1.cube_num() << "] = " << r1 << endl;
      }
      if ( d1.literal_num() == 0 ) {
	// special case
	if ( debug ) {
	  cout << "factor(q1)" << endl;
	}
	auto q_expr = factor(q1);
	if ( debug ) {
	  cout << "factor(r1)" << endl;
	}
	auto r_expr = factor(r1);
	auto expr = q_expr | r_expr;
	if ( debug ) {
	  cout << "*factor(" << f << ")" << endl
	       << " => " << expr << endl;
	}
	return expr;
      }
      auto cc1 = d1.common_cube();
      if ( cc1.literal_num() == 0 ) {
	if ( debug ) {
	  cout << "factor(q1)" << endl;
	}
	auto q_expr = factor(q1);
	if ( debug ) {
	  cout << "factor(d1)" << endl;
	}
	auto d_expr = factor(d1);
	if ( debug ) {
	  cout << "factor(r1)" << endl;
	}
	auto r_expr = factor(r1);
	auto expr = (q_expr & d_expr) | r_expr;
	if ( debug ) {
	  cout << "*factor(" << f << ")" << endl
	       << " => " << expr << endl;
	}
	return expr;
      }
      else {
	auto lit_list = cc1.literal_list();
	if ( debug ) {
	  cout << "literal_factor(f)" << endl;
	}
	auto expr = literal_factor(f, lit_list);
	if ( debug ) {
	  cout << "*factor(" << f << ")" << endl
	       << " => " << expr << endl;
	}
	return expr;
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
    if ( lit_list.empty() ) {
      throw std::logic_error{"lit_list is empty()"};
    }
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
    auto r = f - (q & l);
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

END_NAMESPACE_YM_SOP

#endif // GENFACTOR_H
