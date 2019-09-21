﻿#ifndef YM_EXPRWRITER_H
#define YM_EXPRWRITER_H

/// @file ym/ExprWriter.h
/// @brief ExprWriter のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2017 Yusuke Matsunaga
/// All rights reserved.


#include "ym/logic.h"
#include "ym/VarId.h"


BEGIN_NAMESPACE_YM_LOGIC

//////////////////////////////////////////////////////////////////////
/// @class ExprWriter ExprWriter.h "ym/ExprWriter.h"
/// @ingroup LogicGroup
/// @brief Expr を出力するためのクラス
/// @sa Expr
//////////////////////////////////////////////////////////////////////
class ExprWriter
{
public:

  /// @brief コンストラクタ
  ExprWriter();

  /// @brief デストラクタ
  ~ExprWriter();


public:

  /// @brief 内容の出力
  /// @param[in] s 出力ストリーム
  /// @param[in] expr 文字列
  /// @return s
  ostream&
  dump(ostream& s,
       const Expr& expr) const;

  /// @brief 内容の出力
  /// @param[in] s 出力ストリーム
  /// @param[in] expr 文字列
  /// @param[in] var_names 各変数番号から変数名への写像
  /// @note varid 番目の変数名として var_names[varid] を用いる．
  /// 登録されていなければデフォルトの表記を用いる．
  /// @return s
  ostream&
  dump(ostream& s,
       const Expr& expr,
       const unordered_map<VarId, string>& var_names) const;

  /// @brief 内容を文字列にする．
  /// @param[in] expr 文字列
  string
  dump_to_string(const Expr& expr) const;

  /// @brief 内容を文字列にする．
  /// @param[in] expr 文字列
  /// @param[in] var_names 各変数番号から変数名への写像
  /// @note varid 番目の変数名として var_names[varid] を用いる．
  /// 登録されていなければデフォルトの表記を用いる．
  string
  dump_to_string(const Expr& expr,
		 const unordered_map<VarId, string>& var_names) const;


public:

  /// @brief 演算子を表す文字列の設定
  /// @param[in] op 演算子を表す文字列を入れる．
  /// - op[0]: 否定 (~)
  /// - op[1]: AND  (&)
  /// - op[2]: OR   (|)
  /// - op[3]: XOR  (^)
  /// @sa opstr
  void
  set_opstr(const vector<string>& op);

  /// @brief 演算子を表す文字列の取得
  /// @param[out] op 演算子を表す文字列を格納するベクタ
  /// @sa set_opstr
  void
  opstr(vector<string>& op) const;

  /// @brief 否定演算子を表す文字列の取得
  const string&
  not_str() const;

  /// @brief AND演算子を表す文字列の取得
  const string&
  and_str() const;

  /// @brief OR演算子を表す文字列の取得
  const string&
  or_str() const;

  /// @brief XOR演算子を表す文字列の取得
  const string&
  xor_str() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  // dump() のサブルーティン
  void
  dump_sub(ostream& s,
	   const Expr& expr,
	   const unordered_map<VarId, string>& var_names) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // dump() 時の演算子を表す文字列の配列．
  string mOpStrArray[4];

};

END_NAMESPACE_YM_LOGIC

#endif // YM_LOGEXPRWRITER_H
