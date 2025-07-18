﻿
/// @file ExprNode.cc
/// @brief ExprNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ExprNode.h"
#include "SopLit.h"
#include "ym/TvFunc.h"


BEGIN_NAMESPACE_YM_LOGIC

//////////////////////////////////////////////////////////////////////
// クラス ExprNode
//////////////////////////////////////////////////////////////////////

// 0 のノードを返す．
Expr::NodePtr
ExprNode::new_zero()
{
  auto node = new ExprNode{ExprNode::Const0};
  return NodePtr{node};
}

// 1 のノードを返す．
Expr::NodePtr
ExprNode::new_one()
{
  auto node = new ExprNode{ExprNode::Const1};
  return NodePtr{node};
}

// リテラルのノードを返す．
Expr::NodePtr
ExprNode::new_literal(
  SizeType varid,
  bool inv
)
{
  auto type = inv ? NegaLiteral : PosiLiteral;
  auto node = new ExprNode{type, varid};
  return NodePtr{node};
}

// @brief ANDノードを返す．
Expr::NodePtr
ExprNode::new_and(
  const std::vector<NodePtr>& operand_list
)
{
  auto node = new ExprNode{And, operand_list};
  return NodePtr{node};
}

// @brief ORノードを返す．
Expr::NodePtr
ExprNode::new_or(
  const std::vector<NodePtr>& operand_list
)
{
  auto node = new ExprNode{Or, operand_list};
  return NodePtr{node};
}

// @brief XORノードを返す．
Expr::NodePtr
ExprNode::new_xor(
  const std::vector<NodePtr>& operand_list
)
{
  auto node = new ExprNode{Xor, operand_list};
  return NodePtr{node};
}

// 同一の式を表していたら true を返す．
bool
ExprNode::posi_equiv(
  const NodePtr& node0,
  const NodePtr& node1
)
{
  if ( node0->type() != node1->type() ||
       node0->operand_num() != node1->operand_num() ) {
    return false;
  }

  auto n = node0->operand_num();
  for ( SizeType i = 0; i < n; ++ i ) {
    auto& op0 = node0->operand(i);
    auto& op1 = node1->operand(i);
    if ( !posi_equiv(op0, op1) ) {
      return false;
    }
  }

  if ( node0->is_literal() ) {
    if ( node0->varid() != node1->varid() ) {
      return false;
    }
  }
  return true;
}

// 互いに否定の関係にある式を表していたら true を返す．
bool
ExprNode::nega_equiv(
  const NodePtr& node0,
  const NodePtr& node1
)
{
  if ( node0->is_one() ) {
    return node0->is_zero();
  }
  if ( node0->is_zero() ) {
    return node1->is_one();
  }
  if ( node0->is_posiliteral() ) {
    return node1->is_negaliteral() && node0->varid() == node1->varid();
  }
  if ( node0->is_negaliteral() ) {
    return node1->is_posiliteral() && node0->varid() == node1->varid();
  }

  auto n = node0->operand_num();
  if ( node1->operand_num() != n ) {
    return false;
  }

  if ( node0->is_and() ) {
    if ( !node1->is_or() ) {
      return false;
    }
    for ( SizeType i = 0; i < n; ++ i ) {
      auto& op0 = node0->operand(i);
      auto& op1 = node1->operand(i);
      if ( !nega_equiv(op0, op1) ) {
	return false;
      }
    }
    return true;
  }
  if ( node0->is_or() ) {
    if ( !node1->is_and() ) {
      return false;
    }
    for ( SizeType i = 0; i < n; ++ i ) {
      auto& op0 = node0->operand(i);
      auto& op1 = node1->operand(i);
      if ( !nega_equiv(op0, op1) ) {
	return false;
      }
    }
  }
  else if ( node0->is_xor() ) {
    if ( !node1->is_xor() ) {
      return false;
    }
    bool inv = false;
    for ( SizeType i = 0; i < n; ++ i ) {
      auto& op0 = node0->operand(i);
      auto& op1 = node1->operand(i);
      if ( !nega_equiv(op0, op1) ) {
	inv = !inv;
      }
      else if ( !posi_equiv(op0, op1) ) {
	return false;
      }
    }
    return inv;
  }

  ASSERT_NOT_REACHED;

  return false;
}

// @brief vals の値にしたがった評価を行う．
ExprNode::BitVectType
ExprNode::eval(
  const std::vector<BitVectType>& vals,
  BitVectType mask
) const
{
  if ( is_zero() ) {
    return 0UL;
  }
  if ( is_one() ) {
    return ~0UL & mask;
  }
  if ( is_posiliteral() ) {
    return vals[varid()];
  }
  if ( is_negaliteral() ) {
    return ~vals[varid()] & mask;
  }

  auto nop = operand_num();
  ASSERT_COND( nop > 0 );

  auto ans = operand(0)->eval(vals, mask);
  if ( is_and() ) {
    for ( SizeType i = 1; i < nop; ++ i ) {
      ans &= operand(i)->eval(vals, mask);
    }
  }
  else if ( is_or() ) {
    for ( SizeType i = 1; i < nop; ++ i ) {
      ans |= operand(i)->eval(vals, mask);
    }
  }
  else if ( is_xor() ) {
    for ( SizeType i = 1; i < nop; ++ i ) {
      ans ^= operand(i)->eval(vals, mask);
    }
  }
  else {
    ASSERT_NOT_REACHED;
  }
  return ans;
}

// @brief 真理値表を作成する．
TvFunc
ExprNode::make_tv(
  SizeType ni
) const
{
  if ( is_zero() ) {
    return TvFunc::zero(ni);
  }
  if ( is_one() ) {
    return TvFunc::one(ni);
  }
  if ( is_posiliteral() ) {
    return TvFunc::posi_literal(ni, varid());
  }
  if ( is_negaliteral() ) {
    return TvFunc::nega_literal(ni, varid());
  }

  // あとは AND/OR/XOR のみ
  auto nop = operand_num();
  ASSERT_COND( nop > 0 );

  auto ans = operand(0)->make_tv(ni);
  if ( is_and() ) {
    for ( SizeType i = 1; i < nop; ++ i ) {
      ans &= operand(i)->make_tv(ni);
    }
  }
  else if ( is_or() ) {
    for ( SizeType i = 1; i < nop; ++ i ) {
      ans |= operand(i)->make_tv(ni);
    }
  }
  else if ( is_xor() ) {
    for ( SizeType i = 1; i < nop; ++ i ) {
      ans ^= operand(i)->make_tv(ni);
    }
  }
  else {
    ASSERT_NOT_REACHED;
  }
  return ans;
}

// 子供が全てリテラルの時に true を返す．
bool
ExprNode::is_simple_op() const
{
  for ( auto& opr: operand_list() ) {
    if ( !opr->is_literal() ) {
      return false;
    }
  }
  return true;
}

// SOP 形式の時 true を返す．
bool
ExprNode::is_sop() const
{
  if ( is_xor() ) {
    return false;
  }
  if ( is_simple() ) {
    return true;
  }
  if ( !is_or() ) {
    return false;
  }

  for ( auto& opr: operand_list() ){
    if ( !opr->is_literal() && !opr->is_simple_and() ) {
      return false;
    }
  }
  return true;
}

// リテラル数を返す．
SizeType
ExprNode::litnum() const
{
  if ( is_literal() ) {
    // リテラルならリテラル数は1
    return 1;
  }

  SizeType num = 0;
  if ( is_op() ) {
    // AND/OR/XOR ノードなら子供のリテラル数の和を返す．
    for ( auto& opr: operand_list() ) {
      num += opr->litnum();
    }
  }
  return num;
}

// 特定の変数のリテラル数を返す．
SizeType
ExprNode::litnum(
  SizeType id
) const
{
  if ( is_literal() && varid() == id ) {
    // リテラルならリテラル数は1
    return 1;
  }

  SizeType num = 0;
  if ( is_op() ) {
    // AND/OR/XOR ノードなら子供のリテラル数の和を返す．
    for ( auto& opr: operand_list() ) {
      num += opr->litnum(id);
    }
  }
  return num;
}

// 特定の変数の特定の極性のリテラル数を返す．
SizeType
ExprNode::litnum(
  SizeType id,
  bool inv
) const
{
  if ( is_literal(inv) && varid() == id ) {
    // リテラルならリテラル数は1
    return 1;
  }

  SizeType num = 0;
  if ( is_op() ) {
    // AND/OR/XOR ノードなら子供のリテラル数の和を返す．
    for ( auto& opr: operand_list() ) {
      num += opr->litnum(id, inv);
    }
  }
  return num;
}

// @brief 使われている変数の最大の番号 + 1を得る．
SizeType
ExprNode::input_size() const
{
  if ( is_literal() ) {
    return varid() + 1;
  }

  SizeType ans = 0;
  if ( is_op() ) {
    for ( auto& opr: operand_list() ) {
      auto ans1 = opr->input_size();
      ans = std::max(ans, ans1);
    }
  }
  return ans;
}

// SOP形式の積項数とリテラル数を計算する．
SopLit
ExprNode::soplit(
  bool inverted
) const
{
  if ( is_literal() ) {
    return SopLit{1, 1};
  }

  if ( (type() == ExprNode::And && !inverted) ||
       (type() == ExprNode::Or && inverted) ) {
    auto l = SopLit{1, 0};
    for ( auto& opr: operand_list() ) {
      auto l1 = opr->soplit(inverted);
      l *= l1;
    }
    return l;
  }

  if ( (type() == ExprNode::Or && !inverted) ||
       (type() == ExprNode::And && inverted) ) {
    auto l = SopLit{0, 0};
    for ( auto& opr: operand_list() ) {
      auto l1 = opr->soplit(inverted);
      l += l1;
    }
    return l;
  }

  if ( type() == ExprNode::Xor ) {
    auto& opr = operand(0);
    auto lp = opr->soplit(inverted);
    auto ln = opr->soplit(!inverted);
    auto nop = operand_num();
    for ( SizeType i = 1; i < nop; ++ i ) {
      auto& opr = operand(i);
      auto l1p = lp;
      auto l1n = ln;
      auto l2p = opr->soplit(false);
      auto l2n = opr->soplit(true);
      lp = l1p * l2n + l1n * l2p;
      ln = l1p * l2p + l1n * l2n;
    }
    return lp;
  }

  return SopLit::zero();
}

// SOP形式の積項数とリテラル数を計算する．
SopLit
ExprNode::soplit(
  bool inverted,
  SizeType id
) const
{
  if ( is_literal() ) {
    if ( varid() == id ) {
      return SopLit{1, 1};
    }
    else {
      return SopLit{1, 0};
    }
  }

  if ( (type() == ExprNode::And && !inverted) ||
       (type() == ExprNode::Or  && inverted) ) {
    auto l = SopLit{1, 0};
    for ( auto& opr: operand_list() ) {
      auto l1 = opr->soplit(inverted, id);
      l *= l1;
    }
    return l;
  }

  if ( (type() == ExprNode::Or && !inverted) ||
       (type() == ExprNode::And && inverted) ) {
    auto l = SopLit{0, 0};
    for ( auto& opr: operand_list() ) {
      auto l1 = opr->soplit(inverted, id);
      l += l1;
    }
    return l;
  }

  if ( type() == ExprNode::Xor ) {
    auto& opr = operand(0);
    auto lp = opr->soplit(inverted);
    auto ln = opr->soplit(inverted);
    auto nop = operand_num();
    for ( SizeType i = 1; i < nop; ++ i ) {
      auto& opr = operand(i);
      auto l1p = lp;
      auto l1n = ln;
      auto l2p = opr->soplit(false, id);
      auto l2n = opr->soplit(true, id);
      lp = l1p * l2n + l1n * l2p;
      ln = l1p * l2p + l1n * l2n;
    }
    return lp;
  }

  return SopLit::zero();
}

// SOP形式の積項数とリテラル数を計算する．
SopLit
ExprNode::soplit(
  bool inverted,
  SizeType id,
  bool inv
) const
{
  if ( is_literal() ) {
    if ( varid() == id && is_literal(inv) ) {
      return SopLit{1, 1};
    }
    else {
      return SopLit{1, 0};
    }
  }

  if ( (type() == ExprNode::And && !inverted) ||
       (type() == ExprNode::Or && inverted) ) {
    auto l = SopLit{1, 0};
    for ( auto& opr: operand_list() ) {
      auto l1 = opr->soplit(inverted, id, inv);
      l *= l1;
    }
    return l;
  }

  if ( (type() == ExprNode::Or && !inverted) ||
       (type() == ExprNode::And && inverted) ) {
    auto l = SopLit{0, 0};
    for ( auto& opr: operand_list() ) {
      auto l1 = opr->soplit(inverted, id, inv);
      l += l1;
    }
    return l;
  }

  if ( type() == ExprNode::Xor ) {
    auto nop = operand_num();
    auto& opr = operand(0);
    auto lp = opr->soplit(inverted);
    auto ln = opr->soplit(!inverted);
    for ( SizeType i = 1; i < nop; ++ i ) {
      auto& opr = operand(i);
      auto l1p = lp;
      auto l1n = ln;
      auto l2p = opr->soplit(false, id);
      auto l2n = opr->soplit(true, id);
      lp = l1p * l2n + l1n * l2p;
      ln = l1p * l2p + l1n * l2n;
    }
    return lp;
  }

  return SopLit::zero();
}

// @brief 内容を表す文字列を返す．
std::string
ExprNode::rep_string() const
{
  // 逆ポーランド記法を用いる．
  std::ostringstream buf;
  switch ( type() ) {
  case ExprNode::Const0:      buf << "C0"; break;
  case ExprNode::Const1:      buf << "C1"; break;
  case ExprNode::PosiLiteral: buf << "P" << varid(); break;
  case ExprNode::NegaLiteral: buf << "N" << varid(); break;
  case ExprNode::And:         buf << "A" << operand_num(); break;
  case ExprNode::Or:          buf << "O" << operand_num(); break;
  case ExprNode::Xor:         buf << "X" << operand_num(); break;
  }
  for ( auto& opr: operand_list() ) {
    buf << opr->rep_string();
  }
  return buf.str();
}

// @brief 内容を出力する．
void
ExprNode::print(
  std::ostream& s
) const
{
  switch ( type() ) {
  case ExprNode::Const0:      s << "0"; return;
  case ExprNode::Const1:      s << "1"; return;
  case ExprNode::PosiLiteral: s << "P" << varid(); return;
  case ExprNode::NegaLiteral: s << "N" << varid(); return;
  default: break;
  }
  const char* op_str = "";
  const char* op1_str = "";
  s << "(";
  switch ( type() ) {
  case ExprNode::And: op1_str = " & "; break;
  case ExprNode::Or:  op1_str = " | "; break;
  case ExprNode::Xor: op1_str = " ^ "; break;
  default: break;
  }
  for ( auto& opr: operand_list() ) {
    s << op_str;
    opr->print(s);
    op_str = op1_str;
  }
  s << ")";
}

END_NAMESPACE_YM_LOGIC
