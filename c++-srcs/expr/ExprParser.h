#ifndef EXPRPARSER_H
#define EXPRPARSER_H

/// @file ExprParser.h
/// @brief ExprParser のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Expr.h"


BEGIN_NAMESPACE_YM_LOGIC

//////////////////////////////////////////////////////////////////////
/// @brief トークンの種類を表す列挙型．
//////////////////////////////////////////////////////////////////////
enum class ExprToken {
  /// 終了
  END,
  /// 0
  ZERO,
  /// 1
  ONE,
  /// 数字(変数番号)
  NUM,
  /// 左かっこ
  LP,
  /// 右かっこ
  RP,
  /// 論理積
  AND,
  /// 論理和
  OR,
  /// 排他的論理和
  XOR,
  /// 否定
  NOT,
  /// エラー
  ERR
};

/// @brief トークンを出力する．主にデバッグ用
ostream& operator<<(
  ostream& s,
  ExprToken token
);


//////////////////////////////////////////////////////////////////////
/// @class SyntaxError ExprParser.h "ExprParser.h"
/// @brief パーズ中のシンタックスエラーを表すクラス
//////////////////////////////////////////////////////////////////////
class SyntaxError :
  public exception
{
public:

  SyntaxError(const char* msg) : mMsg{msg} { }

  /// @brief エラーメッセージ
  const char* mMsg;
};


//////////////////////////////////////////////////////////////////////
/// @class ExprParser ExprParser.h "ExprParser.h"
/// @brief 論理式をパーズするためのクラス
//////////////////////////////////////////////////////////////////////
class ExprParser
{
public:

  /// @brief コンストラクタ
  ExprParser(
    const string& expr_str ///< [in] 論理式を表す文字列
  );

  /// @brief デストラクタ
  ~ExprParser();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief リテラル文字列からリテラル番号を得る．
  SizeType
  str_to_literal(
    const string& str
  );

  /// @brief トークンを一つ読み出す．
  /// @return 読み出されたトークン値
  ExprToken
  get_token(
    SizeType& lit_id ///< [out] トークンが ExprToken::NUM の時にはリテラル番号を入れる．
  );

  /// @brief 次のトークンが AND ならそれを読み出し true を返す．
  ///
  /// そうでなければなにもしないで false を返す．
  bool
  get_and_token();

  /// @brief リテラル(もしくは括弧で囲まれた単位)を読み出し Expr に変換する
  Expr
  get_literal();

  /// @brief AND でつながった単位を読み出し Expr に変換する．
  Expr
  get_product();

  /// @brief end_token で終わる論理式を読み出し Expr に変換する．
  Expr
  get_expr(
    ExprToken end_token
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 元の文字列
  string mExprStr;

  // 入力用の stream ポインタ
  istringstream mInput;

};

END_NAMESPACE_YM_LOGIC

#endif // EXPRPARSER_H
