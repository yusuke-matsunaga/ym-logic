#ifndef EXPRNODE_H
#define EXPRNODE_H

/// @file ExprNode.h
/// @brief ExprNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "ym/Expr.h"
#include "ym/Array.h"


BEGIN_NAMESPACE_YM_LOGIC

class SopLit;

//////////////////////////////////////////////////////////////////////
/// @class ExprNode ExprNode.h "ExprNode.h"
/// @brief 論理式を形作るノードのクラス
/// 効率化のためにコピーはポインタのコピーを用いる．
/// そのために参照回数を持つ．
//////////////////////////////////////////////////////////////////////
class ExprNode
{
  friend class Expr;
  friend class ExprNodePtr;
  friend class ExprMgr;

public:

  using BitVectType = Expr::BitVectType;

  /// @brief 演算子の種類を表す列挙型
  ///
  /// 定数やリテラルも含む．
  /// 型判定の dirty trick のために値の指定が必要．
  enum Type {
    Const0      = 0,
    Const1      = 1,
    PosiLiteral = 2,
    NegaLiteral = 3,
    And         = 4,
    Or          = 5,
    Xor         = 6
  };

public:

  //////////////////////////////////////////////////////////////////////
  /// @name 内部の情報を読み出す関数
  /// @{

  /// @brief 型を返す
  Type
  type() const;

  /// @brief 恒偽関数を表している時に真となる．
  bool
  is_zero() const;

  /// @brief 恒真関数を表している時に真となる．
  bool
  is_one() const;

  /// @brief 定数関数を表している時に真となる．
  bool
  is_constant() const;

  /// @brief 肯定のリテラルを表している時に真となる．
  bool
  is_posiliteral() const;

  /// @brief 否定のリテラルを表している時に真となる．
  bool
  is_negaliteral() const;

  /// @brief リテラルを表している時に真となる．
  bool
  is_literal() const;

  /// @brief type がリテラルで極性が inv の時に真となる．
  /// @param[in] inv 極性
  bool
  is_literal(bool inv) const;

  /// @brief リテラルノードの場合に変数番号を取り出す．
  VarId
  varid() const;

  /// @brief リテラルの取得
  /// @return 対象がリテラルの場合リテラルを返す．
  ///
  /// そうでなければリテラルとして不正な値を返す．
  Literal
  literal() const;

  /// @brief AND ノードの時に真となる．
  bool
  is_and() const;

  /// @brief OR ノードの時に真となる．
  bool
  is_or() const;

  /// @brief XOR ノードの時に真となる．
  bool
  is_xor() const;

  /// @brief 演算子ノードの時に真となる．
  ///
  /// enum の順番に依存するよくないコード
  bool
  is_op() const;

  /// @brief 演算子ノードの場合に項の数を返す．
  SizeType
  child_num() const;

  /// @brief 演算子ノードの場合に子供のノードを返す．
  /// @param[in] pos 取り出す子供の位置
  /// @note 演算子ノード以外の場合と pos が範囲外の場合には定数0を返す．
  /// 通常は定数ノードが子供に含まれることはないのでエラーとわかる．
  const ExprNode*
  child(int pos) const;

  /// @brief 演算子ノードの場合に子供のノードのリストを返す．
  Array<const ExprNode*>
  child_list() const;

  /// @brief vals の値にしたがった評価を行う．
  BitVectType
  eval(const vector<BitVectType>& vals,
       BitVectType mask) const;

  /// @brief 真理値表を作成する．
  /// @param[in] ni 入力数
  TvFunc
  make_tv(SizeType ni) const;

  /// @brief 定数,リテラルもしくは子供がリテラルのノードの時に true を返す．
  bool
  is_simple() const;

  /// @brief 子供がすべてリテラルの AND ノードの時に true を返す．
  bool
  is_simple_and() const;

  /// @brief 子供がすべてリテラルの OR ノードの時に true を返す．
  bool
  is_simple_or() const;

  /// @brief 子供がすべてリテラルの XOR ノードの時に true を返す．
  bool
  is_simple_xor() const;

  /// @brief SOP 形式の時 true を返す．
  bool
  is_sop() const;

  /// @brief リテラル数を返す．
  SizeType
  litnum() const;

  /// @brief 特定の変数のリテラルの出現回数を返す．
  /// @param[in] varid 計測対象の変数番号
  SizeType
  litnum(VarId varid) const;

  /// @brief 特定の変数の特定の極性のリテラルの出現回数を返す．
  /// @param[in] varid 計測対象の変数番号
  /// @param[in] inv 計測対象の極性
  ///                - false: 反転なし (正極性)
  ///                - true:  反転あり (負極性)
  SizeType
  litnum(VarId varid,
	 bool inv) const;

  /// @brief 使われている変数の最大の番号 + 1を得る．
  SizeType
  input_size() const;

  /// @brief SOP 形式に展開したときの積項数とリテラル数を見積もる．
  /// @param[in] inverted true の時否定のSOPに対する見積を行う．
  SopLit
  soplit(bool inverted) const;

  /// @brief SOP 形式に展開したときの積項数とリテラル数を見積もる．
  /// @param[in] inverted true の時否定のSOPに対する見積を行う．
  /// @param[in] varid 計測対象の変数番号
  SopLit
  soplit(bool inverted,
	 VarId varid) const;

  /// @brief SOP 形式に展開したときの積項数とリテラル数を見積もる．
  /// @param[in] inverted true の時否定のSOPに対する見積を行う．
  /// @param[in] varid 計測対象の変数番号
  /// @param[in] inv 計測対象の極性
  SopLit
  soplit(bool inverted,
	 VarId varid,
	 bool inv) const;

  /// @brief 内容を出力する．
  /// @param[in] s 出力先のストリーム
  void
  print(ostream& s) const;

  /// @}
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  /// @name 参照回数の制御
  /// @{

  // 参照回数を1つ増やす．
  void
  inc_ref() const;

  // 参照回数を1つ減らす．
  // 結果0になれば自分自身を削除する．
  void
  dec_ref() const;

  /// @}
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // プライベートメンバ関数
  //////////////////////////////////////////////////////////////////////

  // コンストラクタ/デストラクタはむやみに使ってはだめなので
  // private にしている．
public:

  // コンストラクタ
  ExprNode();

  // デストラクタ
  ~ExprNode();


private:

  // 参照回数を得る．
  SizeType
  ref() const;

  // 参照回数をセットする．
  void
  ref(SizeType ref) const;

  // 自殺する．
  void
  suicide();

  // 子供が全てリテラルの時に true を返す．
  bool
  is_simple_op() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 参照回数＋ノードタイプ(3ビット)
  SizeType mRefType;

  // 子供の数 もしくは 変数番号
  SizeType mNc;

  // 子を指すポインタの配列
  const ExprNode* mChildArray[1];

  // 参照回数の最大値
  static const SizeType kRefMax{1UL << (sizeof(SizeType) * 8 - 4)};

};

/// @relates ExprNode
/// @brief node0 と node1 が式として等価のときに true を返す．
bool
posi_equiv(const ExprNode* node0,
	   const ExprNode* node1);


/// @relates ExprNode
/// @brief node0 と node1 が式として否定の関係にあるときに true を返す．
bool
nega_equiv(const ExprNode* node0,
	   const ExprNode* node1);



//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// コンストラクタ
inline
ExprNode::ExprNode()
{
}

// デストラクタ
inline
ExprNode::~ExprNode()
{
}

inline
ExprNode::Type
ExprNode::type() const
{
  return static_cast<Type>(mRefType & 7U);
}

inline
bool
ExprNode::is_zero() const
{
  return type() == ExprNode::Const0;
}

inline
bool
ExprNode::is_one() const
{
  return type() == ExprNode::Const1;
}

inline
bool
ExprNode::is_constant() const
{
  // ちょっときたないコード
  return static_cast<Type>(mRefType & 6U) == ExprNode::Const0;
}

inline
bool
ExprNode::is_posiliteral() const
{
  return type() == ExprNode::PosiLiteral;
}

inline
bool
ExprNode::is_negaliteral() const
{
  return type() == ExprNode::NegaLiteral;
}

inline
bool
ExprNode::is_literal() const
{
  // ちょっとキタナイコード．
  return static_cast<Type>(mRefType & 6U) == ExprNode::PosiLiteral;
}

inline
bool
ExprNode::is_literal(bool inv) const
{
  return inv ? is_negaliteral() : is_posiliteral();
}

inline
VarId
ExprNode::varid() const
{
  return is_literal() ? VarId(mNc) : VarId();
}

// @brief リテラルの取得
// @return 対象がリテラルの場合リテラルを返す．
//
// そうでなければリテラルとして不正な値を返す．
inline
Literal
ExprNode::literal() const
{
  for ( bool inv: {false, true} ) {
    if ( is_literal(inv) ) {
      return Literal(VarId(mNc), inv);
    }
  }
  return Literal();
}

inline
bool
ExprNode::is_and() const
{
  return type() == ExprNode::And;
}

inline
bool
ExprNode::is_or() const
{
  return type() == ExprNode::Or;
}

inline
bool
ExprNode::is_xor() const
{
  return type() == ExprNode::Xor;
}

inline
bool
ExprNode::is_op() const
{
  // ちょっときたないコード
  return (mRefType & 4U) == 4U;
}

// 定数,リテラルもしくは子供がリテラルのノードの時に true を返す．
inline
bool
ExprNode::is_simple() const
{
  return !is_op() || is_simple_op();
}

// 子供がすべてリテラルの AND ノードの時に true を返す．
inline
bool
ExprNode::is_simple_and() const
{
  return is_and() && is_simple_op();
}

// 子供がすべてリテラルの OR ノードの時に true を返す．
inline
bool
ExprNode::is_simple_or() const
{
  return is_or() && is_simple_op();
}

// 子供がすべてリテラルの XOR ノードの時に true を返す．
inline
bool
ExprNode::is_simple_xor() const
{
  return is_xor() && is_simple_op();
}

inline
SizeType
ExprNode::child_num() const
{
  return is_op() ? mNc : 0;
}

inline
const ExprNode*
ExprNode::child(int pos) const
{
  ASSERT_COND( pos >= 0 && pos < child_num() );

  return mChildArray[pos];
}

// @brief 演算子ノードの場合に子供のノードのリストを返す．
inline
Array<const ExprNode*>
ExprNode::child_list() const
{
  if ( is_op() ) {
    auto arrayptr{const_cast<const ExprNode**>(mChildArray)};
    return Array<const ExprNode*>(arrayptr, 0, mNc);
  }
  else {
    return Array<const ExprNode*>(nullptr, 0, 0);
  }
}

inline
SizeType
ExprNode::ref() const
{
  return static_cast<int>(mRefType >> 3);
}

inline
void
ExprNode::ref(SizeType ref) const
{
  auto node{const_cast<ExprNode*>(this)};
  // 昔の参照回数を落とす．
  node->mRefType &= 7;
  // ref をセットする．
  node->mRefType |= (ref << 3);
}

inline
void
ExprNode::inc_ref() const
{
  // MAX の時は増やさない．
  if ( ref() < kRefMax ) {
    auto node{const_cast<ExprNode*>(this)};
    node->mRefType += 8;
  }
}

inline
void
ExprNode::dec_ref() const
{
  SizeType r = ref();
  auto node{const_cast<ExprNode*>(this)};
  if ( r == 1 ) {
    // これが最後の参照だった．
    node->suicide();
  }
  else if ( r < kRefMax ) {
    node->mRefType -= 8;
  }
  // MAX の時は減らさない．
}

END_NAMESPACE_YM_LOGIC

#endif // EXPRNODE_H
