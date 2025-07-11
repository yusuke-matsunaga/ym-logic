
/// @file Expr.cc
/// @brief Expr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Expr.h"

#include "ExprNode.h"
#include "ExprMgr.h"
#include "ExprParser.h"
#include "SopLit.h"
#include "ym/TvFunc.h"
#include "RepStringParser.h"


BEGIN_NAMESPACE_YM_LOGIC

// 定数 0 の論理式を作る
Expr
Expr::zero()
{
  return Expr{ExprNode::new_zero()};
}

// 定数 1 の論理式を作る
Expr
Expr::one()
{
  return Expr{ExprNode::new_one()};
}

// 肯定のリテラルを作る．
Expr
Expr::posi_literal(
  SizeType varid
)
{
  return Expr{ExprNode::new_literal(varid, false)};
}

// 否定のリテラルを作る．
Expr
Expr::nega_literal(
  SizeType varid
)
{
  return Expr{ExprNode::new_literal(varid, true)};
}

// 与えられた論理式を部分論理式に持つ 2 入力ANDの論理式を作るクラス・メソッド
Expr
Expr::and_op(
  const Expr& chd1,
  const Expr& chd2
)
{
  ExprMgr mgr;
  auto begin = mgr.nodestack_top();
  mgr.nodestack_push(chd1.root());
  mgr.nodestack_push(chd2.root());
  return Expr{mgr.and_op(begin)};
}

// 与えられた論理式を部分論理式に持つ n 入力ANDの論理式を作るクラス・メソッド
Expr
Expr::and_op(
  const std::vector<Expr>& chd_list
)
{
  auto n = chd_list.size();
  if ( n == 0 ) {
    return one();
  }
  if ( n == 1 ) {
    return chd_list.front();
  }
  ExprMgr mgr;
  auto begin = mgr.nodestack_top();
  for ( auto expr: chd_list ) {
    mgr.nodestack_push(expr.root());
  }
  return Expr{mgr.and_op(begin)};
}

// 与えられた論理式を部分論理式に持つ 2 入力ORの論理式を作るクラス・メソッド
Expr
Expr::or_op(
  const Expr& chd1,
  const Expr& chd2
)
{
  ExprMgr mgr;
  auto begin = mgr.nodestack_top();
  mgr.nodestack_push(chd1.root());
  mgr.nodestack_push(chd2.root());
  return Expr{mgr.or_op(begin)};
}

// 与えられた論理式を部分論理式に持つ n 入力ORの論理式を作るクラス・メソッド
Expr
Expr::or_op(
  const std::vector<Expr>& chd_list
)
{
  auto n = chd_list.size();
  if ( n == 0 ) {
    return zero();
  }
  if ( n == 1 ) {
    return chd_list.front();
  }
  ExprMgr mgr;
  auto begin = mgr.nodestack_top();
  for ( auto expr: chd_list ) {
    mgr.nodestack_push(expr.root());
  }
  return Expr{mgr.or_op(begin)};
}

// 与えられた論理式を部分論理式に持つ 2 入力ANDの論理式を作るクラス・メソッド
Expr
Expr::xor_op(
  const Expr& chd1,
  const Expr& chd2
)
{
  ExprMgr mgr;
  auto begin = mgr.nodestack_top();
  mgr.nodestack_push(chd1.root());
  mgr.nodestack_push(chd2.root());
  return Expr{mgr.xor_op(begin)};
}

// 与えられた論理式を部分論理式に持つ n 入力XORの論理式を作るクラス・メソッド
Expr
Expr::xor_op(
  const std::vector<Expr>& chd_list
)
{
  auto n = chd_list.size();
  if ( n == 0 ) {
    return zero();
  }
  if ( n == 1 ) {
    return chd_list.front();
  }
  ExprMgr mgr;
  auto begin = mgr.nodestack_top();
  for ( auto expr: chd_list ) {
    mgr.nodestack_push(expr.root());
  }
  return Expr{mgr.xor_op(begin)};
}

// 論理式をパーズしてファクタードフォームを作る．
Expr
Expr::from_string(
  const std::string& expr_str
)
{
  try {
    ExprParser p{expr_str};
    return p.get_expr(ExprToken::END);
  }
  catch ( SyntaxError e ) {
    auto err_msg = e.mMsg;
    throw std::invalid_argument{err_msg};
  }
}

// rep_string() 形式の文字列から変換する．
Expr
Expr::from_rep_string(
  const std::string& rep_str
)
{
  if ( rep_str == std::string{} ) {
    return Expr{};
  }
  ExprMgr mgr;
  RepStringParser parser{rep_str};
  return Expr{mgr.from_rep_string(parser)};
}

// @brief operator~() の別名
Expr
Expr::invert() const
{
  ExprMgr mgr;
  return Expr{mgr.complement(root())};
}

// 自分の論理式と src の論理式の論理積を計算し自分に代入する．
Expr&
Expr::operator&=(
  const Expr& src
)
{
  ExprMgr mgr;
  auto begin = mgr.nodestack_top();
  mgr.nodestack_push(root());
  mgr.nodestack_push(src.root());
  mRoot = mgr.and_op(begin);
  return *this;
}

// 自分の論理式と src の論理式の論理和を計算し自分に代入する．
Expr&
Expr::operator|=(
  const Expr& src
)
{
  ExprMgr mgr;
  auto begin = mgr.nodestack_top();
  mgr.nodestack_push(root());
  mgr.nodestack_push(src.root());
  mRoot = mgr.or_op(begin);
  return *this;
}

// 自分の論理式と src の論理式の排他的論理和を計算し自分に代入する．
Expr&
Expr::operator^=(
  const Expr& src
)
{
  ExprMgr mgr;
  auto begin = mgr.nodestack_top();
  mgr.nodestack_push(root());
  mgr.nodestack_push(src.root());
  mRoot = mgr.xor_op(begin);
  return *this;
}

// pos 番目のリテラルを src の論理式に置き換える．
Expr
Expr::compose(
  SizeType varid,
  const Expr& src
) const
{
  ExprMgr mgr;
  return Expr{mgr.compose(root(), varid, src.root())};
}

// comp_map にしたがって複数のリテラルの置き換えを行う．
Expr
Expr::compose(
  const std::unordered_map<SizeType, Expr>& comp_map
) const
{
  ExprMgr mgr;
  return Expr{mgr.compose(root(), comp_map)};
}

// @brief 複数変数の compose 演算
// @param[in] comp_list 置き換える変数と置き換える先の
// 論理式をペアとしてリスト
// @return comp_list にしたがって置き換えを行った論理式
//
// - 一度に複数の置き換えを行う
// - comp_list 中に変数の重複が有った場合の動作は不定となる．
Expr
Expr::compose(
  const std::vector<std::pair<SizeType, Expr>>& comp_list
) const
{
  std::unordered_map<SizeType, Expr> comp_map;
  for ( auto p: comp_list ) {
    comp_map.emplace(p.first, p.second);
  }
  ExprMgr mgr;
  return Expr{mgr.compose(root(), comp_map)};
}

// 与えられた論理式のリテラル番号を再マップする．
Expr
Expr::remap_var(
  const std::unordered_map<SizeType, SizeType>& varmap
) const
{
  ExprMgr mgr;
  return Expr{mgr.remap_var(root(), varmap)};
}

// 与えられた論理式のリテラル番号を再マップする．
Expr
Expr::remap_var(
  const std::vector<std::pair<SizeType, SizeType>>& varlist
) const
{
  std::unordered_map<SizeType, SizeType> varmap;
  for ( auto p: varlist ) {
    varmap.emplace(p.first, p.second);
  }
  ExprMgr mgr;
  return Expr{mgr.remap_var(root(), varmap)};
}

// 重複した式を取り除く
Expr&
Expr::simplify()
{
  ExprMgr mgr;
  mRoot = mgr.simplify(root());
  return *this;
}

// @brief 値の評価
Expr::BitVectType
Expr::eval(
  const std::vector<BitVectType>& vals,
  BitVectType mask
) const
{
  return root()->eval(vals, mask);
}

// @brief 真理値表の作成
TvFunc
Expr::tvfunc(
  SizeType ni
) const
{
  if ( is_invalid() ) {
    return TvFunc::invalid();
  }
  auto ni2 = input_size();
  if ( ni < ni2 ) {
    ni = ni2;
  }
  return root()->make_tv(ni);
}

// 恒偽関数を表している時に真となる．
bool
Expr::is_zero() const
{
  if ( is_invalid() ) {
    return false;
  }
  return root()->is_zero();
}

// 恒真関数を表している時に真となる．
bool
Expr::is_one() const
{
  if ( is_invalid() ) {
    return false;
  }
  return root()->is_one();
}

// 定数関数を表している時に真となる．
bool
Expr::is_constant() const
{
  if ( is_invalid() ) {
    return false;
  }
  return root()->is_constant();
}

// 肯定のリテラルを表している時に真となる．
bool
Expr::is_positive_literal() const
{
  if ( is_invalid() ) {
    return false;
  }
  return root()->is_posiliteral();
}

// 否定のリテラルを表している時に真となる．
bool
Expr::is_negative_literal() const
{
  if ( is_invalid() ) {
    return false;
  }
  return root()->is_negaliteral();
}

// リテラルを表している時に真となる．
bool
Expr::is_literal() const
{
  if ( is_invalid() ) {
    return false;
  }
  return root()->is_literal();
}

// リテラルの時に変数番号を返す．
SizeType
Expr::varid() const
{
  if ( is_invalid() ) {
    return BAD_VARID;
  }
  return root()->varid();
}

// @brief リテラルの取得
Literal
Expr::literal() const
{
  if ( is_invalid() ) {
    return Literal::x();
  }
  return root()->literal();
}

// トップがANDの時に真となる．
bool
Expr::is_and() const
{
  if ( is_invalid() ) {
    return false;
  }
  return root()->is_and();
}

// トップがORの時に真となる．
bool
Expr::is_or() const
{
  if ( is_invalid() ) {
    return false;
  }
  return root()->is_or();
}

// トップがXORの時に真となる．
bool
Expr::is_xor() const
{
  if ( is_invalid() ) {
    return false;
  }
  return root()->is_xor();
}

// トップが演算子の時に真となる．
bool
Expr::is_op() const
{
  if ( is_invalid() ) {
    return false;
  }
  return root()->is_op();
}

bool
Expr::operator==(
  const Expr& right
) const
{
  if ( is_invalid() && right.is_invalid() ) {
    return true;
  }
  if ( is_invalid() || right.is_invalid() ) {
    return false;
  }
  return ExprNode::posi_equiv(root(), right.root());
}

// src1 と src2 の根のタイプが同じとき true を返す．
bool
compare_type(
  const Expr& src1,
  const Expr& src2
)
{
  if ( src1.is_invalid() || src2.is_invalid() ) {
    return false;
  }
  return src1.root()->type() == src2.root()->type();
}

// AND/OR/XOR の時にオペランドの項の数を返す．
// それ以外のノードの時には 0 を返す．
SizeType
Expr::operand_num() const
{
  if ( is_invalid() ) {
    return 0;
  }
  return root()->operand_num();
}

// pos 番目のオペランドを返す．
Expr
Expr::operand(
  SizeType pos
) const
{
  if ( pos >= operand_num() ) {
    throw std::out_of_range{"pos is out of range"};
  }
  return Expr{root()->operand(pos)};
}

// @brief オペランドのリストの取得
std::vector<Expr>
Expr::operand_list() const
{
  if ( is_invalid() ) {
    return {};
  }
  auto n = operand_num();
  std::vector<Expr> ans_list;
  ans_list.reserve(n);
  for ( auto& opr: root()->operand_list() ) {
    auto expr1 = Expr{opr};
    ans_list.push_back(expr1);
  }
  return ans_list;
}

// 一種類の演算子のみからなる式のとき true を返す．
// より具体的には zero, one, literal とすべての子供が
// リテラルの and, or, xor
bool
Expr::is_simple() const
{
  if ( is_invalid() ) {
    return false;
  }
  return root()->is_simple();
}

// is_simple() && is_and()
bool
Expr::is_simple_and() const
{
  if ( is_invalid() ) {
    return false;
  }
  return root()->is_simple_and();
}

// is_simple() && is_or()
bool
Expr::is_simple_or() const
{
  if ( is_invalid() ) {
    return false;
  }
  return root()->is_simple_or();
}

// is_simple() && is_xor()
bool
Expr::is_simple_xor() const
{
  if ( is_invalid() ) {
    return false;
  }
  return root()->is_simple_xor();
}

// SOP形式の時 true を返す．
bool
Expr::is_sop() const
{
  if ( is_invalid() ) {
    return false;
  }
  return root()->is_sop();
}

// リテラル数を得る．
SizeType
Expr::literal_num() const
{
  if ( is_invalid() ) {
    return 0;
  }
  return root()->litnum();
}

// 特定の変数のリテラル数を得る．
SizeType
Expr::literal_num(
  SizeType varid
) const
{
  if ( is_invalid() ) {
    return 0;
  }
  return root()->litnum(varid);
}

// 特定の変数の特定の極性のリテラル数を得る．
SizeType
Expr::literal_num(
  SizeType varid,
  bool inv
) const
{
  if ( is_invalid() ) {
    return 0;
  }
  return root()->litnum(varid, inv);
}

// @brief 使われている変数の最大の番号を得る．
SizeType
Expr::input_size() const
{
  if ( is_invalid() ) {
    return 0;
  }
  return root()->input_size();
}

// SOP形式に展開したときのキューブ数を得る．
SizeType
Expr::sop_cube_num() const
{
  if ( is_invalid() ) {
    return 0;
  }
  auto l = root()->soplit(false);
  return l.np();
}

// SOP形式に展開した時のリテラル数を見積もる．
SizeType
Expr::sop_literal_num() const
{
  if ( is_invalid() ) {
    return 0;
  }
  auto l = root()->soplit(false);
  return l.nl();
}

// SOP形式に展開した時の varid 番めの変数のリテラルの出現回数を得る．
SizeType
Expr::sop_literal_num(
  SizeType varid
) const
{
  if ( is_invalid() ) {
    return 0;
  }
  auto l = root()->soplit(false, varid);
  return l.nl();
}

// SOP形式に展開した時の varid 番めの変数の極性が pol のリテラル
// の出現回数を得る．
SizeType
Expr::sop_literal_num(
  SizeType varid,
  bool inv
) const
{
  if ( is_invalid() ) {
    return 0;
  }
  auto l = root()->soplit(false, varid, inv);
  return l.nl();
}


BEGIN_NONAMESPACE

std::vector<Expr>
restore_operand_list(
  BinDec& s
)
{
  SizeType nc = s.read_64();
  std::vector<Expr> opr_list;
  opr_list.reserve(nc);
  for ( SizeType i = 0; i < nc; ++ i ) {
    opr_list.push_back(Expr::restore(s));
  }
  return opr_list;
}

END_NONAMESPACE

// ストリームから論理式を作る．
Expr
Expr::restore(
  BinDec& s
)
{
  auto type = s.read_8();
  switch ( type ) {
  case 255:
    return Expr::invalid();

  case 0:
    return Expr::zero();

  case 1:
    return Expr::one();

  case 2:
    {
      SizeType var = s.read_64();
      return Expr::posi_literal(var);
    }

  case 3:
    {
      SizeType var = s.read_64();
      return Expr::nega_literal(var);
    }

  case 4:
    return Expr::and_op(restore_operand_list(s));

  case 5:
    return Expr::or_op(restore_operand_list(s));

  case 6:
    return Expr::xor_op(restore_operand_list(s));

  default:
    ASSERT_NOT_REACHED;
  }

  // ダミー
  return Expr::invalid();
}

std::string
Expr::to_string() const
{
  std::ostringstream os;
  os << *this;
  return os.str();
}

// @brief 圧縮形式の文字列を出力する．
std::string
Expr::rep_string() const
{
  if ( is_valid() ) {
    return root()->rep_string();
  }
  return {};
}

// @brief バイナリストリームに出力する．
void
Expr::dump(
  BinEnc& s
) const
{
  if ( is_invalid() ) {
    s.write_8(255);
    return;
  }
  if ( is_zero() ) {
    s.write_8(0);
    return;
  }
  if ( is_one() ) {
    s.write_8(1);
    return;
  }
  if ( is_positive_literal() ) {
    s.write_8(2);
    s.write_64(varid());
    return;
  }
  if ( is_negative_literal() ) {
    s.write_8(3);
    s.write_64(varid());
    return;
  }

  // 残りは論理演算ノード
  std::uint8_t type = 0;
  if ( is_and() ) {
    type = 4;
  }
  else if ( is_or() ) {
    type = 5;
  }
  else if ( is_xor() ) {
    type = 6;
  }
  else {
    ASSERT_NOT_REACHED;
  }
  s.write_8(type);

  s.write_64(operand_num());
  for ( auto& opr: operand_list() ) {
    opr.dump(s);
  }
}

END_NAMESPACE_YM_LOGIC
