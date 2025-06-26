
/// @file ExprWriter.cc
/// @brief ExprWriter の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/ExprWriter.h"
#include "ym/Expr.h"


BEGIN_NAMESPACE_YM_LOGIC

// ostream に対する書出し
ostream&
ExprWriter::dump(
  ostream& s,
  const Expr& expr,
  const unordered_map<SizeType, string>& var_names
) const
{
  if ( expr.is_invalid() ) {
    s << "---";
  }
  else {
    dump_sub(s, expr, var_names);
  }
  return s;
}

// ostream に対する書出し
ostream&
ExprWriter::dump(
  ostream& s,
  const Expr& expr
) const
{
  dump(s, expr, unordered_map<SizeType, string>());
  return s;
}

// @brief 内容を文字列にする．
string
ExprWriter::dump_to_string(
  const Expr& expr
) const
{
  ostringstream buf;
  dump(buf, expr);
  return string(buf.str());
}

// @brief 内容を文字列にする．
string
ExprWriter::dump_to_string(
  const Expr& expr,
  const unordered_map<SizeType, string>& var_names
) const
{
  ostringstream buf;
  dump(buf, expr, var_names);
  return string(buf.str());
}

// 演算子文字列を設定する．
void
ExprWriter::set_opstr(
  const vector<string>& op
)
{
  for ( auto i: { 0, 1, 2, 3 } ) {
    mOpStrArray[i] = op[i];
  }
}

// 演算子文字列を取得する．
vector<string>
ExprWriter::opstr() const
{
  vector<string> op(4);
  for ( auto i: { 0, 1, 2, 3 } ) {
    op[i] = mOpStrArray[i];
  }
  return op;
}

// NOT演算子の文字列を得る．
const string&
ExprWriter::not_str() const
{
  return mOpStrArray[0];
}

// AND演算子の文字列を得る．
const string&
ExprWriter::and_str() const
{
  return mOpStrArray[1];
}

// OR演算子の文字列を得る．
const string&
ExprWriter::or_str() const
{
  return mOpStrArray[2];
}

// XOR演算子の文字列を得る．
const string&
ExprWriter::xor_str() const
{
  return mOpStrArray[3];
}

// 論理式の内容を ostream に書出す．
ostream&
operator<<(
  ostream& s,
  const Expr& expr
)
{
  ExprWriter writer;
  // 空の map を渡す
  return writer.dump(s, expr, unordered_map<SizeType, string>());
}


// dump() のサブルーティン
void
ExprWriter::dump_sub(
  ostream& s,
  const Expr& expr,
  const unordered_map<SizeType, string>& var_names
) const
{
  if ( expr.is_zero() ) {
    s << "0";
  }
  else if ( expr.is_one() ) {
    s << "1";
  }
  else if ( expr.is_literal() ) {
    if ( expr.is_negative_literal() ) {
      s << not_str();
    }
    auto id = expr.varid();
    if ( var_names.count(id) > 0 ) {
      auto ans{var_names.at(id)};
      s << ans;
    }
    else {
      s << id;
    }
  }
  else { // AND/OR/XOR
    string delim;
    if ( expr.is_and() ) {
      delim = and_str();
    }
    else if ( expr.is_or() ) {
      delim = or_str();
    }
    else if ( expr.is_xor() ) {
      delim = xor_str();
    }
    else {
      ASSERT_NOT_REACHED;
    }
    s << "( ";
    string delim1 = "";
    for ( auto& opr: expr.operand_list() ) {
      s << delim1;
      dump_sub(s, opr, var_names);
      delim1 = " " + delim + " ";
    }
    s << " )";
  }
}

END_NAMESPACE_YM_LOGIC
