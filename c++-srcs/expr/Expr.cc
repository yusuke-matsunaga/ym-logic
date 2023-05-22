﻿
/// @file Expr.cc
/// @brief Expr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2016, 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Expr.h"

#include "ExprMgr.h"
#include "ExprNode.h"
#include "ExprParser.h"
#include "SopLit.h"
#include "ym/TvFunc.h"
#include "RepStringParser.h"


BEGIN_NAMESPACE_YM_LOGIC

// 根のノードを指定したコンストラクタ
Expr::Expr(
  const ExprNode* node
)
{
  set_root(node);
}

// デフォルトコンストラクタ
Expr::Expr()
{
  // 不正値
  set_root(nullptr);
}

// コピーコンストラクタ
Expr::Expr(
  const Expr& src
)
{
  set_root(src.root());
}

// @brief 根のノードを得る．
const ExprNode*
Expr::root() const
{
  return mRootPtr;
}

// 代入演算子
Expr&
Expr::operator=(
  const Expr& src
)
{
  set_root(src.root());
  return *this;
}

// デストラクタ
Expr::~Expr()
{
  set_root(nullptr);
}

// 根のノードをセットする．
void
Expr::set_root(
  const ExprNode* node
)
{
  if ( node ) {
    node->inc_ref();
  }
  if ( mRootPtr ) {
    mRootPtr->dec_ref();
  }
  mRootPtr = node;
}

// @brief エラーオブジェクトの生成
// @return 不適正なオブジェクトを返す．
//
// 返されたオブジェクトは is_valid() == false となる．
Expr
Expr::make_invalid()
{
  return Expr{nullptr};
}

// 定数 0 の論理式を作る
Expr
Expr::make_zero()
{
  return Expr{ExprMgr::the_obj().make_zero()};
}

// 定数 1 の論理式を作る
Expr
Expr::make_one()
{
  return Expr{ExprMgr::the_obj().make_one()};
}

// 肯定のリテラルを作る．
Expr
Expr::make_posi_literal(
  SizeType varid
)
{
  return Expr{ExprMgr::the_obj().make_posi_literal(varid)};
}

// 否定のリテラルを作る．
Expr
Expr::make_nega_literal(
  SizeType varid
)
{
  return Expr{ExprMgr::the_obj().make_nega_literal(varid)};
}

// 与えられた論理式を部分論理式に持つ 2 入力ANDの論理式を作るクラス・メソッド
Expr
Expr::and_op(
  const Expr& chd1,
  const Expr& chd2
)
{
  auto& mgr = ExprMgr::the_obj();
  SizeType begin{mgr.nodestack_top()};
  mgr.nodestack_push(chd1.root());
  mgr.nodestack_push(chd2.root());
  return Expr{mgr.make_and(begin)};
}

// 与えられた論理式を部分論理式に持つ n 入力ANDの論理式を作るクラス・メソッド
Expr
Expr::make_and(
  const vector<Expr>& chd_list
)
{
  if ( chd_list.size() == 0 ) {
    return make_one();
  }

  auto& mgr = ExprMgr::the_obj();
  SizeType begin{mgr.nodestack_top()};
  for ( auto expr: chd_list ) {
    mgr.nodestack_push(expr.root());
  }
  return Expr{mgr.make_and(begin)};
}

// 与えられた論理式を部分論理式に持つ 2 入力ORの論理式を作るクラス・メソッド
Expr
Expr::or_op(
  const Expr& chd1,
  const Expr& chd2
)
{
  auto& mgr = ExprMgr::the_obj();
  SizeType begin{mgr.nodestack_top()};
  mgr.nodestack_push(chd1.root());
  mgr.nodestack_push(chd2.root());
  return Expr{mgr.make_or(begin)};
}

// 与えられた論理式を部分論理式に持つ n 入力ORの論理式を作るクラス・メソッド
Expr
Expr::make_or(
  const vector<Expr>& chd_list
)
{
  if ( chd_list.size() == 0 ) {
    return make_zero();
  }

  auto& mgr = ExprMgr::the_obj();
  SizeType begin{mgr.nodestack_top()};
  for ( auto expr: chd_list ) {
    mgr.nodestack_push(expr.root());
  }
  return Expr{mgr.make_or(begin)};
}

// 与えられた論理式を部分論理式に持つ 2 入力ANDの論理式を作るクラス・メソッド
Expr
Expr::xor_op(
  const Expr& chd1,
  const Expr& chd2
)
{
  auto& mgr = ExprMgr::the_obj();
  SizeType begin{mgr.nodestack_top()};
  mgr.nodestack_push(chd1.root());
  mgr.nodestack_push(chd2.root());
  return Expr{mgr.make_xor(begin)};
}

// 与えられた論理式を部分論理式に持つ n 入力XORの論理式を作るクラス・メソッド
Expr
Expr::make_xor(
  const vector<Expr>& chd_list
)
{
  ASSERT_COND( chd_list.size() > 0 );

  auto& mgr = ExprMgr::the_obj();
  SizeType begin{mgr.nodestack_top()};
  for ( auto expr: chd_list ) {
    mgr.nodestack_push(expr.root());
  }
  return Expr{mgr.make_xor(begin)};
}

// 論理式をパーズしてファクタードフォームを作る．
Expr
Expr::from_string(
  const string& expr_str
)
{
  string err_msg;
  try {
    ExprParser p(expr_str);
    return p.get_expr(ExprToken::END);
  }
  catch ( SyntaxError e ) {
    err_msg = e.mMsg;
    throw std::invalid_argument{err_msg};
  }
}

// rep_string() 形式の文字列から変換する．
Expr
Expr::from_rep_string(
  const string& rep_str
)
{
  if ( rep_str == string{} ) {
    return Expr{};
  }
  RepStringParser parser{rep_str};
  return Expr{ExprMgr::the_obj().from_rep_string(parser)};
}

// @brief 確保していたメモリを開放する．
// @note メモリリークチェックのための関数なので通常は使用しない．
void
Expr::__clear_memory()
{
  ExprMgr::clear_memory();
}

// @brief operator~() の別名
Expr
Expr::invert() const
{
  auto& mgr = ExprMgr::the_obj();
  return Expr{mgr.complement(root())};
}

// 自分の論理式と src の論理式の論理積を計算し自分に代入する．
Expr&
Expr::operator&=(
  const Expr& src
)
{
  auto& mgr = ExprMgr::the_obj();
  SizeType begin{mgr.nodestack_top()};
  mgr.nodestack_push(root());
  mgr.nodestack_push(src.root());
  set_root(mgr.make_and(begin));
  return *this;
}

// 自分の論理式と src の論理式の論理和を計算し自分に代入する．
Expr&
Expr::operator|=(
  const Expr& src
)
{
  auto& mgr = ExprMgr::the_obj();
  SizeType begin{mgr.nodestack_top()};
  mgr.nodestack_push(root());
  mgr.nodestack_push(src.root());
  set_root(mgr.make_or(begin));
  return *this;
}

// 自分の論理式と src の論理式の排他的論理和を計算し自分に代入する．
Expr&
Expr::operator^=(
  const Expr& src
)
{
  auto& mgr = ExprMgr::the_obj();
  SizeType begin{mgr.nodestack_top()};
  mgr.nodestack_push(root());
  mgr.nodestack_push(src.root());
  set_root(mgr.make_xor(begin));
  return *this;
}

// pos 番目のリテラルを src の論理式に置き換える．
Expr
Expr::compose(
  SizeType varid,
  const Expr& src
) const
{
  auto& mgr = ExprMgr::the_obj();
  return Expr{mgr.compose(root(), varid, src.root())};
}

// comp_map にしたがって複数のリテラルの置き換えを行う．
Expr
Expr::compose(
  const unordered_map<SizeType, Expr>& comp_map
) const
{
  auto& mgr = ExprMgr::the_obj();
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
  const vector<pair<SizeType, Expr>>& comp_list
) const
{
  unordered_map<SizeType, Expr> comp_map;
  for ( auto p: comp_list ) {
    comp_map.emplace(p.first, p.second);
  }
  auto& mgr = ExprMgr::the_obj();
  return Expr{mgr.compose(root(), comp_map)};
}

// 与えられた論理式のリテラル番号を再マップする．
Expr
Expr::remap_var(
  const unordered_map<SizeType, SizeType>& varmap
) const
{
  auto& mgr = ExprMgr::the_obj();
  return Expr{mgr.remap_var(root(), varmap)};
}

// 与えられた論理式のリテラル番号を再マップする．
Expr
Expr::remap_var(
  const vector<pair<SizeType, SizeType>>& varlist
) const
{
  unordered_map<SizeType, SizeType> varmap;
  for ( auto p: varlist ) {
    varmap.emplace(p.first, p.second);
  }
  auto& mgr = ExprMgr::the_obj();
  return Expr{mgr.remap_var(root(), varmap)};
}

// 重複した式を取り除く
Expr&
Expr::simplify()
{
  auto& mgr = ExprMgr::the_obj();
  set_root(mgr.simplify(root()));
  return *this;
}

// @brief 値の評価
// @param[in] vals 変数の値割り当て
// @return 評価値
Expr::BitVectType
Expr::eval(
  const vector<BitVectType>& vals,
  BitVectType mask
) const
{
  return root()->eval(vals, mask);
}

// @brief 真理値表の作成
TvFunc
Expr::make_tv(
  SizeType ni
) const
{
  if ( is_invalid() ) {
    return TvFunc::make_invalid();
  }
  SizeType ni2{input_size()};
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
Expr::is_posi_literal() const
{
  if ( is_invalid() ) {
    return false;
  }
  return root()->is_posiliteral();
}

// 否定のリテラルを表している時に真となる．
bool
Expr::is_nega_literal() const
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
  return posi_equiv(root(), right.root());
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
  return root()->child_num();
}

// pos 番目のオペランドを返す．
Expr
Expr::operand(
  SizeType pos
) const
{
  ASSERT_COND( 0 <= pos && pos < operand_num() );
  return Expr{root()->child(pos)};
}

// @brief オペランドのリストの取得
vector<Expr>
Expr::operand_list() const
{
  SizeType n = operand_num();
  vector<Expr> ans_list(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    ans_list[i] = Expr{root()->child(i)};
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
  SopLit l = root()->soplit(false);
  return l.np();
}

// SOP形式に展開した時のリテラル数を見積もる．
SizeType
Expr::sop_literal_num() const
{
  if ( is_invalid() ) {
    return 0;
  }
  SopLit l = root()->soplit(false);
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
  SopLit l = root()->soplit(false, varid);
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

vector<Expr>
restore_operand_list(
  BinDec& s
)
{
  SizeType nc;
  s >> nc;
  vector<Expr> opr_list(nc);
  for ( SizeType i = 0; i < nc; ++ i ) {
    opr_list[i] = Expr::restore(s);
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
  std::uint8_t type;
  s >> type;
  switch ( type ) {
  case 255:
    return Expr::make_invalid();

  case 0:
    return Expr::make_zero();

  case 1:
    return Expr::make_one();

  case 2:
    {
      SizeType var;
      s >> var;
      return Expr::make_posi_literal(var);
    }

  case 3:
    {
      SizeType var;
      s >> var;
      return Expr::make_nega_literal(var);
    }

  case 4:
    return Expr::make_and(restore_operand_list(s));

  case 5:
    return Expr::make_or(restore_operand_list(s));

  case 6:
    return Expr::make_xor(restore_operand_list(s));

  default:
    ASSERT_NOT_REACHED;
  }

  // ダミー
  return Expr::make_invalid();
}

string
Expr::to_string() const
{
  ostringstream os;
  os << *this;
  return os.str();
}

// @brief 圧縮形式の文字列を出力する．
string
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
    s << static_cast<std::uint8_t>(255);
    return;
  }
  if ( is_zero() ) {
    s << static_cast<std::uint8_t>(0);
    return;
  }
  if ( is_one() ) {
    s << static_cast<std::uint8_t>(1);
    return;
  }
  if ( is_posi_literal() ) {
    s << static_cast<std::uint8_t>(2)
      << varid();
    return;
  }
  if ( is_nega_literal() ) {
    s << static_cast<std::uint8_t>(3)
      << varid();
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

  SizeType no = operand_num();
  s << type
    << no;
  for ( auto& opr: operand_list() ) {
    opr.dump(s);
  }
}

END_NAMESPACE_YM_LOGIC
