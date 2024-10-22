
/// @file ExprNode.cc
/// @brief ExprNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ExprNode.h"
#include "ExprMgr.h"
#include "SopLit.h"
#include "ym/TvFunc.h"


BEGIN_NAMESPACE_YM_LOGIC

//////////////////////////////////////////////////////////////////////
// クラス ExprNode
//////////////////////////////////////////////////////////////////////

// 同一の式を表していたら true を返す．
bool
posi_equiv(
  const ExprNode* node0,
  const ExprNode* node1
)
{
  if ( node0->type() != node1->type() ||
       node0->child_num() != node1->child_num() ) {
    return false;
  }

  SizeType n{node0->child_num()};
  for ( SizeType i = 0; i < n; ++ i ) {
    auto chd0 = node0->child(i);
    auto chd1 = node1->child(i);
    if ( !posi_equiv(chd0, chd1) ) {
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
nega_equiv(
  const ExprNode* node0,
  const ExprNode* node1
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

  SizeType n{node0->child_num()};
  if ( node1->child_num() != n ) {
    return false;
  }

  if ( node0->is_and() ) {
    if ( !node1->is_or() ) {
      return false;
    }
    for ( SizeType i = 0; i < n; ++ i ) {
      auto chd0 = node0->child(i);
      auto chd1 = node1->child(i);
      if ( !nega_equiv(chd0, chd1) ) {
	return false;
      }
    }
    return true;
  }
  else if ( node0->is_or() ) {
    if ( !node1->is_and() ) {
      return false;
    }
    for ( SizeType i = 0; i < n; ++ i ) {
      auto chd0 = node0->child(i);
      auto chd1 = node1->child(i);
      if ( !nega_equiv(chd0, chd1) ) {
	return false;
      }
    }
    return true;
  }
  else if ( node0->is_xor() ) {
    if ( !node1->is_xor() ) {
      return false;
    }
    bool inv = false;
    for ( SizeType i = 0; i < n; ++ i ) {
      auto chd0 = node0->child(i);
      auto chd1 = node1->child(i);
      if ( !nega_equiv(chd0, chd1) ) {
	inv = !inv;
      }
      else if ( !posi_equiv(chd0, chd1) ) {
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
  const vector<BitVectType>& vals,
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

  SizeType nc = child_num();
  ASSERT_COND( nc > 0 );

  BitVectType ans{child(0)->eval(vals, mask)};
  if ( is_and() ) {
    for ( SizeType i = 1; i < nc; ++ i ) {
      ans &= child(i)->eval(vals, mask);
    }
  }
  else if ( is_or() ) {
    for ( SizeType i = 1; i < nc; ++ i ) {
      ans |= child(i)->eval(vals, mask);
    }
  }
  else if ( is_xor() ) {
    for ( SizeType i = 1; i < nc; ++ i ) {
      ans ^= child(i)->eval(vals, mask);
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
  SizeType nc = child_num();
  ASSERT_COND( nc > 0 );

  TvFunc ans{child(0)->make_tv(ni)};
  if ( is_and() ) {
    for ( SizeType i = 1; i < nc; ++ i ) {
      ans &= child(i)->make_tv(ni);
    }
  }
  else if ( is_or() ) {
    for ( SizeType i = 1; i < nc; ++ i ) {
      ans |= child(i)->make_tv(ni);
    }
  }
  else if ( is_xor() ) {
    for ( SizeType i = 1; i < nc; ++ i ) {
      ans ^= child(i)->make_tv(ni);
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
  for ( auto child: child_list() ) {
    if ( !child->is_literal() ) {
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

  for ( auto child: child_list() ){
    if ( !child->is_literal() && !child->is_simple_and() ) {
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
    for ( auto child: child_list() ) {
      num += child->litnum();
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
    for ( auto child: child_list() ) {
      num += child->litnum(id);
    }
    return num;
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
    for ( auto child: child_list() ) {
      num += child->litnum(id, inv);
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
    for ( auto child: child_list() ) {
      SizeType ans1{child->input_size()};
      if ( ans < ans1 ) {
	ans = ans1;
      }
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
    return SopLit(1, 1);
  }

  if ( (type() == ExprNode::And && !inverted) ||
       (type() == ExprNode::Or && inverted) ) {
    SopLit l{1, 0};
    for ( auto child: child_list() ) {
      auto l1{child->soplit(inverted)};
      l *= l1;
    }
    return l;
  }

  if ( (type() == ExprNode::Or && !inverted) ||
       (type() == ExprNode::And && inverted) ) {
    SopLit l{0, 0};
    for ( auto child: child_list() ) {
      SopLit l1 = child->soplit(inverted);
      l += l1;
    }
    return l;
  }

  if ( type() == ExprNode::Xor ) {
    auto chd = child(0);
    auto lp = chd->soplit(inverted);
    auto ln = chd->soplit(inverted);
    SizeType nc = child_num();
    for ( SizeType i = 1; i < nc; ++ i ) {
      auto chd = child(i);
      auto l1p = lp;
      auto l1n = ln;
      auto l2p = chd->soplit(false);
      auto l2n = chd->soplit(true);
      lp = l1p * l2n + l1n * l2p;
      ln = l1p * l2p + l1n * l2n;
    }
    return lp;
  }

  return SopLit{0, 0};
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
      return SopLit(1, 1);
    }
    else {
      return SopLit(1, 0);
    }
  }

  if ( (type() == ExprNode::And && !inverted) ||
       (type() == ExprNode::Or  && inverted) ) {
    SopLit l{1, 0};
    for ( auto child: child_list() ) {
      auto l1 = child->soplit(inverted, id);
      l *= l1;
    }
    return l;
  }

  if ( (type() == ExprNode::Or && !inverted) ||
       (type() == ExprNode::And && inverted) ) {
    SopLit l{0, 0};
    for ( auto child: child_list() ) {
      auto l1 = child->soplit(inverted, id);
      l += l1;
    }
    return l;
  }

  if ( type() == ExprNode::Xor ) {
    auto chd = child(0);
    auto lp = chd->soplit(inverted);
    auto ln = chd->soplit(inverted);
    SizeType nc = child_num();
    for ( SizeType i = 1; i < nc; ++ i ) {
      auto chd = child(i);
      auto l1p = lp;
      auto l1n = ln;
      auto l2p = chd->soplit(false, id);
      auto l2n = chd->soplit(true, id);
      lp = l1p * l2n + l1n * l2p;
      ln = l1p * l2p + l1n * l2n;
    }
    return lp;
  }

  return SopLit(0, 0);
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
    SopLit l{1, 0};
    for ( auto child: child_list() ) {
      auto l1 = child->soplit(inverted, id, inv);
      l *= l1;
    }
    return l;
  }

  if ( (type() == ExprNode::Or && !inverted) ||
       (type() == ExprNode::And && inverted) ) {
    SopLit l{0, 0};
    for ( auto child: child_list() ) {
      auto l1 = child->soplit(inverted, id, inv);
      l += l1;
    }
    return l;
  }

  if ( type() == ExprNode::Xor ) {
    SizeType n = child_num();
    auto chd = child(0);
    auto lp = chd->soplit(inverted);
    auto ln = chd->soplit(inverted);
    for ( SizeType i = 1; i < n; ++ i ) {
      auto chd = child(i);
      auto l1p = lp;
      auto l1n = ln;
      auto l2p = chd->soplit(false, id);
      auto l2n = chd->soplit(true, id);
      lp = l1p * l2n + l1n * l2p;
      ln = l1p * l2p + l1n * l2n;
    }
    return lp;
  }

  return SopLit{0, 0};
}

// @brief 内容を表す文字列を返す．
string
ExprNode::rep_string() const
{
  // 逆ポーランド記法を用いる．
  ostringstream buf;
  switch ( type() ) {
  case ExprNode::Const0:      buf << "C0"; break;
  case ExprNode::Const1:      buf << "C1"; break;
  case ExprNode::PosiLiteral: buf << "P" << varid(); break;
  case ExprNode::NegaLiteral: buf << "N" << varid(); break;
  case ExprNode::And:         buf << "A" << child_num(); break;
  case ExprNode::Or:          buf << "O" << child_num(); break;
  case ExprNode::Xor:         buf << "X" << child_num(); break;
  }
  for ( auto child: child_list() ) {
    buf << child->rep_string();
  }
  return buf.str();
}

// @brief 内容を出力する．
void
ExprNode::print(
  ostream& s
) const
{
  switch ( type() ) {
  case ExprNode::Const0:      s << "0"; return;
  case ExprNode::Const1:      s << "1"; return;
  case ExprNode::PosiLiteral: s << "P" << varid(); return;
  case ExprNode::NegaLiteral: s << "N" << varid(); return;
  default: break;
  }
  const char* op = "";
  const char* op1 = "";
  s << "(";
  switch ( type() ) {
  case ExprNode::And: op1 = " & "; break;
  case ExprNode::Or:  op1 = " | "; break;
  case ExprNode::Xor: op1 = " ^ "; break;
  default: break;
  }
  for ( auto child: child_list() ) {
    s << op;
    child->print(s);
    op = op1;
  }
  s << ")";
}

// 自殺する．
void
ExprNode::suicide()
{
  // なんでこれだけのコードを別の関数にするかというと，
  // これを ExprNode.h に書くと ExprMgr.h をインクルードしなければ
  // ならなくなるので．
  ExprMgr::the_obj().free_node(this);
}

END_NAMESPACE_YM_LOGIC
