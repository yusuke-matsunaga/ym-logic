#ifndef FACTOR_H
#define FACTOR_H

/// @file Factor.h
/// @brief Factor のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Alg.h"
#include "ym/Expr.h"


BEGIN_NAMESPACE_YM_LOGIC

class Divisor;
class Divide;

//////////////////////////////////////////////////////////////////////
/// @class GenFactor GenFactor.h "GenFactor.h"
/// @brief 'GEN_FACTOR' アルゴリズムの実装クラス
//////////////////////////////////////////////////////////////////////
class GenFactor
{
public:

  /// @brief コンストラクタ
  /// @param[in] divisor 除数を求める関数オブジェクト
  /// @param[in] divide 除算を行う関数オブジェクト
  GenFactor(const Divisor& divisor,
	    const Divide& divide);

  /// @brief デストラクタ
  ~GenFactor();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ファクタリングを行う．
  Expr
  operator()(const AlgCover& f);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 'LF' を行う．
  Expr
  literal_factor(const AlgCover& f,
		 const vector<Literal>& lit_list);

  /// @brief AlgCover をそのまま Expr に変換する．
  Expr
  cover_to_expr(const AlgCover& f);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 除数を求める関数オブジェクト
  const Divisor& mDivisor;

  // 除算を行う関数オブジェクト
  const Divide& mDivide;

};


//////////////////////////////////////////////////////////////////////
/// @class QuickFactor QuickFactor.h "GenFactor.h"
/// @brief 'QUICK_FACTOR' を実行するクラス
//////////////////////////////////////////////////////////////////////
class QuickFactor :
  public GenFactor
{
public:

  /// @brief コンストラクタ
  QuickFactor();

  /// @brief デストラクタ
  ~QuickFactor();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////


};


//////////////////////////////////////////////////////////////////////
/// @class GoodFactor GoodFactor.h "GenFactor.h"
/// @brief 'GOOD_FACTOR' を実行するクラス
//////////////////////////////////////////////////////////////////////
class GoodFactor :
  public GenFactor
{
public:

  /// @brief コンストラクタ
  GoodFactor();

  /// @brief デストラクタ
  ~GoodFactor();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////


};

END_NAMESPACE_YM_LOGIC

#endif // FACTOR_H