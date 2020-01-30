
/// @file ExprParser.cc
/// @brief ExprParser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ExprParser.h"


BEGIN_NAMESPACE_YM_LOGIC

// 字句解析ルール
// "(double-quote) で囲まれた文字列はいかなるコードであろうと
// string と見なす．
// また，直前に\(backslash)がついているコードも string と見なされる．


// @brief コンストラクタ
// @param[in] expr_str 論理式を表す文字列
ExprParser::ExprParser(const string& expr_str) :
  mExprStr(expr_str),
  mInput(mExprStr)
{
}

// @brief デストラクタ
ExprParser::~ExprParser()
{
}

// str からリテラル番号を得る．
VarId
ExprParser::str_to_literal(const string& str)
{
  int id = 0;
  for ( char c: str ) {
    id *= 10;
    id += static_cast<ymuint>(c - '0');
  }
  return VarId(id);
}

// トークンを一つ読み出す．
ExprToken
ExprParser::get_token(VarId& lit_id)
{
  string str;
  char c;

  // 汚いのは承知で goto 文で状態遷移を書きます．
 state0:
  if ( !mInput.get(c) ) return ExprToken::END;
  switch (c) {
  case '\0': return ExprToken::END;
  case '*':  // 次のケースとおなじ
  case '&':  return ExprToken::AND;
  case '+':  // 次のケースとおなじ
  case '|':  return ExprToken::OR;
  case '^':  return ExprToken::XOR;
  case '(':  return ExprToken::LP;
  case ')':  return ExprToken::RP;
  case '~':  // 次のケースとおなじ
  case '!':  return ExprToken::NOT;
  case ' ':  // 次のケースとおなじ
  case '\t': // 次のケースとおなじ
  case '\n': goto state0;
  case '0':  // 次のケースとおなじ
  case '1':  // 次のケースとおなじ
  case '2':  // 次のケースとおなじ
  case '3':  // 次のケースとおなじ
  case '4':  // 次のケースとおなじ
  case '5':  // 次のケースとおなじ
  case '6':  // 次のケースとおなじ
  case '7':  // 次のケースとおなじ
  case '8':  // 次のケースとおなじ
  case '9':  str.append(1, c); goto state1;
  case 'z':  // 次のケースと同じ
  case 'Z':  goto stateZ;
  case 'o':  // 次のケースと同じ
  case 'O':  goto stateO;
  default:
    throw SyntaxError("syntax error");
  }

 state1:
  if ( !mInput.get(c) ) {
    lit_id = str_to_literal(str);
    return ExprToken::NUM;
  }
  switch (c) {
  case '\0': // 次のケースとおなじ
  case '*':  // 次のケースとおなじ
  case '&':  // 次のケースとおなじ
  case '+':  // 次のケースとおなじ
  case '|':  // 次のケースとおなじ
  case '^':  // 次のケースとおなじ
  case '(':  // 次のケースとおなじ
  case ')':  // 次のケースとおなじ
  case '~':  // 次のケースとおなじ
  case '!':  // 次のケースとおなじ
  case ' ':  // 次のケースとおなじ
  case '\t': // 次のケースとおなじ
  case '\n': // 次のケースとおなじ
    mInput.putback(c);
    lit_id = str_to_literal(str);
    return ExprToken::NUM;
  case '0':  // 次のケースとおなじ
  case '1':  // 次のケースとおなじ
  case '2':  // 次のケースとおなじ
  case '3':  // 次のケースとおなじ
  case '4':  // 次のケースとおなじ
  case '5':  // 次のケースとおなじ
  case '6':  // 次のケースとおなじ
  case '7':  // 次のケースとおなじ
  case '8':  // 次のケースとおなじ
  case '9':  str.append(1, c); goto state1;
  default:
    throw SyntaxError("syntax error");
  }

 stateZ:
  if ( !mInput.get(c) || (c != 'e' && c != 'E') ) {
    throw SyntaxError("syntax error");
  }
  if ( !mInput.get(c) || (c != 'r' && c != 'R') ) {
    throw SyntaxError("syntax error");
  }
  if ( !mInput.get(c) || (c != 'o' && c != 'O') ) {
    throw SyntaxError("syntax error");
  }
  return ExprToken::ZERO;

 stateO:
  if ( !mInput.get(c) || (c != 'n' && c != 'N') ) {
    throw SyntaxError("syntax error");
  }
  if ( !mInput.get(c) || (c != 'e' && c != 'E') ) {
    throw SyntaxError("syntax error");
  }
  return ExprToken::ONE;
}

// 次のトークンが AND ならそれを読み出し true を返す．
// そうでなければなにもしないで false を返す．
bool
ExprParser::get_and_token()
{
  for ( ; ; ) {
    char c;
    if ( !mInput.get(c) )       return false;
    if ( c == '*' || c == '&' ) return true;
    if ( c != ' ' && c != '\t' && c != '\n' ) {
      mInput.putback(c);
      return false;
    }
  }
}

// リテラルをとって来る．
// ただし，LP expr RP もリテラルと見なす．
Expr
ExprParser::get_literal()
{
  // ここに来る可能性のあるのは NUM, NOT, LP のみ
  // それ以外はエラー
  VarId id;
  ExprToken token = get_token(id);

  if ( token == ExprToken::ZERO ) {
    return Expr::make_zero();
  }

  if ( token == ExprToken::ONE ) {
    return Expr::make_one();
  }

  if ( token == ExprToken::NUM ) {
    // id 番目の肯定のリテラルを作る．
    return Expr::make_posi_literal(id);
  }

  if ( token == ExprToken::NOT ) {
    // 次のトークンをとって来る．
    token = get_token(id);
    if ( token != ExprToken::NUM ) {
      // これは文字列でなければならない．
      throw SyntaxError("NUMBER is expected after NOT");
    }
    // id 番目の否定のリテラルを作る．
    return Expr::make_nega_literal(id);
  }

  if ( token == ExprToken::LP ) {
    // RP で終る論理式をとって来る．
    return get_expr(ExprToken::RP);
  }

  // それ以外はエラー
  throw SyntaxError("syntax error");

  // ダミー
  return Expr::make_zero();
}

// AND でつながった積項をとって来る．
Expr
ExprParser::get_product()
{
  // まず第一項めを取り出す．
  Expr expr = get_literal();

  for ( ; ; ) {
    // 次のトークンが AND かどうかを調べる．
    if ( ! get_and_token() ) {
      // AND 以外なら終り．
      return expr;
    }
    // 次のリテラルを取り出す．
    Expr expr1 = get_literal();

    // 積項を作る．
    expr &= expr1;
  }
  // ここにはこない．
}

// OR もしくは XOR でつながった積項をとって来る．
// 最後は end_token で終らなければ false を返す．
Expr
ExprParser::get_expr(ExprToken end_token)
{
  // まず第一項めを取り出す．
  Expr expr = get_product();

  for ( ; ; ) {
    // 次のトークンを調べる．
    VarId dummy;
    ExprToken token = get_token(dummy);
    if ( token == end_token ) {
      return expr;
    }
    if ( token != ExprToken::OR && token != ExprToken::XOR ) {
      // OR か XOR で無ければならない．
      cerr << "token = " << token << endl;
      throw SyntaxError("syntax error");
    }
    // 次の積項を取り出す．
    Expr expr1 = get_product();

    if ( token == ExprToken::OR ) {
      // 和項を作る．
      expr |= expr1;
    }
    else {
      // 排他的論理和項を作る．
      expr ^= expr1;
    }
  }
  // ここにはこない．
}

// @brief トークンを出力する．主にデバッグ用
ostream&
operator<<(ostream& s,
	   ExprToken token)
{
  switch (token) {
  case ExprToken::END:  s << "TokenEND"; break;
  case ExprToken::ZERO: s << "TokenZERO"; break;
  case ExprToken::ONE:  s << "TokenONE"; break;
  case ExprToken::NUM:  s << "TokenNUM"; break;
  case ExprToken::LP:   s << "TokenLP "; break;
  case ExprToken::RP:   s << "TokenRP "; break;
  case ExprToken::AND:  s << "TokenAND"; break;
  case ExprToken::OR:   s << "TokenOR "; break;
  case ExprToken::XOR:  s << "TokenXOR"; break;
  case ExprToken::NOT:  s << "TokenNOT"; break;
  case ExprToken::ERR:  s << "TokenERR"; break;
  }
  return s;
}

END_NAMESPACE_YM_LOGIC
