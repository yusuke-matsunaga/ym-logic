#ifndef YM_EXPRWRITER_H
#define YM_EXPRWRITER_H

/// @file ym/ExprWriter.h
/// @brief ExprWriter のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"


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

  /// @breif 変数名の辞書
  using VarNameDict = std::unordered_map<SizeType, std::string>;

public:

  /// @brief コンストラクタ
  ExprWriter() = default;

  /// @brief デストラクタ
  ~ExprWriter() = default;


public:

  /// @brief 内容の出力
  /// @return s
  std::ostream&
  dump(
    std::ostream& s, ///< [in] 出力ストリーム
    const Expr& expr ///< [in] 式
  ) const;

  /// @brief 内容の出力
  /// @return s
  std::ostream&
  dump(
    std::ostream& s,             ///< [in] 出力ストリーム
    const Expr& expr,            ///< [in] 式
    const VarNameDict& var_names ///< [in] 各変数から変数名への写像
                                 ///<      var の変数名として var_names[var] を用いる．
                                 ///<      登録されていなければデフォルトの表記を用いる．
  ) const;

  /// @brief 内容を文字列にする．
  /// @return 式の内容を表す文字列
  std::string
  dump_to_string(
    const Expr& expr ///< [in] 式
  ) const;

  /// @brief 内容を文字列にする．
  /// @return 式の内容を表す文字列
  std::string
  dump_to_string(
    const Expr& expr,            ///< [in] 出力ストリーム
    const VarNameDict& var_names ///< [in] 各変数から変数名への写像
                                 ///<      var の変数名として var_names[var] を用いる．
                                 ///<      登録されていなければデフォルトの表記を用いる．
  ) const;


public:

  /// @brief 演算子を表す文字列の設定
  /// @sa opstr
  void
  set_opstr(
    const std::vector<std::string>& op ///< [in] 演算子を表す文字列を入れる．
                                       ///< - op[0]: 否定 (~)
                                       ///< - op[1]: AND  (&)
                                       ///< - op[2]: OR   (|)
                                       ///< - op[3]: XOR  (^)
  );

  /// @brief 演算子を表す文字列の取得
  /// @return 演算子を表す文字列を格納するベクタ(サイズは4)
  /// @sa set_opstr
  std::vector<std::string>
  opstr() const;

  /// @brief 否定演算子を表す文字列の取得
  const std::string&
  not_str() const;

  /// @brief AND演算子を表す文字列の取得
  const std::string&
  and_str() const;

  /// @brief OR演算子を表す文字列の取得
  const std::string&
  or_str() const;

  /// @brief XOR演算子を表す文字列の取得
  const std::string&
  xor_str() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  // dump() のサブルーティン
  void
  dump_sub(
    std::ostream& s,
    const Expr& expr,
    const VarNameDict& var_names
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // dump() 時の演算子を表す文字列の配列．
  std::string mOpStrArray[4]{"~", "&", "|", "^"};

};

END_NAMESPACE_YM_LOGIC

#endif // YM_LOGEXPRWRITER_H
