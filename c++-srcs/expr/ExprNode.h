#ifndef EXPRNODE_H
#define EXPRNODE_H

/// @file ExprNode.h
/// @brief ExprNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Expr.h"
#include "ym/Array.h"


BEGIN_NAMESPACE_YM_LOGIC

class SopLit;

//////////////////////////////////////////////////////////////////////
/// @class ExprNode ExprNode.h "ExprNode.h"
/// @brief 論理式を形作るノードのクラス
//////////////////////////////////////////////////////////////////////
class ExprNode
{
public:

  using NodePtr = Expr::NodePtr;

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

  /// @brief コンストラクタ
  ExprNode() = default;

  /// @brief Const0/Const1 用のコンストラクタ
  ExprNode(
    Type type ///< [in] タイプ
  ) : mTypeVarId{static_cast<SizeType>(type)}
  {
    if ( type != Const0 && type != Const1 ) {
      throw std::invalid_argument{"type mismatch"};
    }
  }

  /// @brief PosiLiteral/NegaLiteral 用のコンストラクタ
  ExprNode(
    Type type,     ///< [in] タイプ
    SizeType varid ///< [in] 変数番号
  ) : mTypeVarId{static_cast<SizeType>(type) | (varid << 3)}
  {
    if ( type != PosiLiteral && type != NegaLiteral ) {
      throw std::invalid_argument{"type mismatch"};
    }
  }

  /// @brief And/Or/Xor 用のコンストラクタ
  ExprNode(
    Type type,                          ///< [in] タイプ
    const vector<NodePtr>& operand_list ///< [in] オペランドのリスト
  ) : mTypeVarId{static_cast<SizeType>(type)},
      mOperandList{operand_list}
  {
    if ( type != And && type != Or && type != Xor ) {
      throw std::invalid_argument{"type mismtach"};
    }
  }

  /// @brief デストラクタ
  ~ExprNode() = default;

  /// @brief 0 のノードを返す．
  static
  NodePtr
  new_zero();

  /// @brief 1 のノードを返す．
  static
  NodePtr
  new_one();

  /// @brief リテラルのノードを返す．
  static
  NodePtr
  new_literal(
    SizeType varid, ///< [in] 変数番号
    bool inv        ///< [in] 反転属性
  );

  /// @brief ANDノードを返す．
  static
  NodePtr
  new_and(
    const vector<NodePtr>& operand_list ///< [in] オペランドのリスト
  );

  /// @brief ORノードを返す．
  static
  NodePtr
  new_or(
    const vector<NodePtr>& operand_list ///< [in] オペランドのリスト
  );

  /// @brief XORノードを返す．
  static
  NodePtr
  new_xor(
    const vector<NodePtr>& operand_list ///< [in] オペランドのリスト
  );


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 内部の情報を読み出す関数
  /// @{

  /// @brief 型を返す
  Type
  type() const
  {
    return static_cast<Type>(mTypeVarId & 7U);
  }

  /// @brief 恒偽関数を表している時に真となる．
  bool
  is_zero() const
  {
    return type() == ExprNode::Const0;
  }

  /// @brief 恒真関数を表している時に真となる．
  bool
  is_one() const
  {
    return type() == ExprNode::Const1;
  }

  /// @brief 定数関数を表している時に真となる．
  bool
  is_constant() const
  {
    // ちょっときたないコード
    return static_cast<Type>(mTypeVarId & 6U) == ExprNode::Const0;
  }

  /// @brief 肯定のリテラルを表している時に真となる．
  bool
  is_posiliteral() const
  {
    return type() == ExprNode::PosiLiteral;
  }

  /// @brief 否定のリテラルを表している時に真となる．
  bool
  is_negaliteral() const
  {
    return type() == ExprNode::NegaLiteral;
  }

  /// @brief リテラルを表している時に真となる．
  bool
  is_literal() const
  {
    // ちょっとキタナイコード．
    return static_cast<Type>(mTypeVarId & 6U) == ExprNode::PosiLiteral;
  }

  /// @brief type がリテラルで極性が inv の時に真となる．
  bool
  is_literal(
    bool inv ///< [in] 極性
  ) const
  {
    return inv ? is_negaliteral() : is_posiliteral();
  }

  /// @brief リテラルノードの場合に変数番号を取り出す．
  SizeType
  varid() const
  {
    if ( is_literal() ) {
      return mTypeVarId >> 3;
    }
    return BAD_VARID;
  }

  /// @brief リテラルの取得
  /// @return 対象がリテラルの場合リテラルを返す．
  ///
  /// そうでなければリテラルとして不正な値を返す．
  Literal
  literal() const
  {
    if ( is_posiliteral() ) {
      return Literal{varid()};
    }
    if ( is_negaliteral() ) {
      return Literal{varid(), true};
    }
    return Literal::x();
  }

  /// @brief AND ノードの時に真となる．
  bool
  is_and() const
  {
    return type() == ExprNode::And;
  }

  /// @brief OR ノードの時に真となる．
  bool
  is_or() const
  {
    return type() == ExprNode::Or;
  }

  /// @brief XOR ノードの時に真となる．
  bool
  is_xor() const
  {
    return type() == ExprNode::Xor;
  }

  /// @brief 演算子ノードの時に真となる．
  ///
  /// enum の順番に依存するよくないコード
  bool
  is_op() const
  {
    // ちょっときたないコード
    return (mTypeVarId & 4U) == 4U;
  }

  /// @brief 演算子ノードの場合にオペランドの数を返す．
  SizeType
  operand_num() const
  {
    return mOperandList.size();
  }

  /// @brief 演算子ノードの場合にオペランドを返す．
  ///
  /// 演算子ノード以外の場合と pos が範囲外の場合には例外を送出する．
  const NodePtr&
  operand(
    SizeType pos ///< [in] 取り出す子供の位置
  ) const
  {
    if ( pos >= operand_num() ) {
      throw std::out_of_range{"pos is out of range"};
    }
    return mOperandList[pos];
  }

  /// @brief 演算子ノードの場合に子供のノードのリストを返す．
  const vector<NodePtr>&
  operand_list() const
  {
    return mOperandList;
  }

  /// @brief vals の値にしたがった評価を行う．
  BitVectType
  eval(
    const vector<BitVectType>& vals, ///< [in] 値のベクタ
    BitVectType mask                 ///< [in] マスク
  ) const;

  /// @brief 真理値表を作成する．
  /// @param[in] ni 入力数
  TvFunc
  make_tv(
    SizeType ni
  ) const;

  /// @brief 定数,リテラルもしくは子供がリテラルのノードの時に true を返す．
  bool
  is_simple() const
  {
    return !is_op() || is_simple_op();
  }

  /// @brief 子供がすべてリテラルの AND ノードの時に true を返す．
  bool
  is_simple_and() const
  {
    return is_and() && is_simple_op();
  }

  /// @brief 子供がすべてリテラルの OR ノードの時に true を返す．
  bool
  is_simple_or() const
  {
    return is_or() && is_simple_op();
  }

  /// @brief 子供がすべてリテラルの XOR ノードの時に true を返す．
  bool
  is_simple_xor() const
  {
    return is_xor() && is_simple_op();
  }

  /// @brief SOP 形式の時 true を返す．
  bool
  is_sop() const;

  /// @brief リテラル数を返す．
  SizeType
  litnum() const;

  /// @brief 特定の変数のリテラルの出現回数を返す．
  SizeType
  litnum(
    SizeType varid ///< [in] 計測対象の変数番号
  ) const;

  /// @brief 特定の変数の特定の極性のリテラルの出現回数を返す．
  SizeType
  litnum(
    SizeType varid, ///< [in] 計測対象の変数番号
    bool inv        ///< [in] 計測対象の極性
                    ///     - false: 反転なし (正極性)
                    ///     - true:  反転あり (負極性)
  ) const;

  /// @brief 使われている変数の最大の番号 + 1を得る．
  SizeType
  input_size() const;

  /// @brief SOP 形式に展開したときの積項数とリテラル数を見積もる．
  SopLit
  soplit(
    bool inverted ///< [in] true の時否定のSOPに対する見積を行う．
  ) const;

  /// @brief SOP 形式に展開したときの積項数とリテラル数を見積もる．
  SopLit
  soplit(
    bool inverted, ///< [in] true の時否定のSOPに対する見積を行う．
    SizeType varid ///< [in] 計測対象の変数番号
  ) const;

  /// @brief SOP 形式に展開したときの積項数とリテラル数を見積もる．
  SopLit
  soplit(
    bool inverted,  ///< [in] true の時否定のSOPに対する見積を行う．
    SizeType varid, ///< [in] 計測対象の変数番号
    bool inv        ///< [in] 計測対象の極性
  ) const;

  /// @brief 内容を表す文字列を返す．
  string
  rep_string() const;

  /// @brief 内容を出力する．
  void
  print(
    ostream& s ///< [in] 出力先のストリーム
  ) const;

  /// @brief node0 と node1 が式として等価のときに true を返す．
  static
  bool
  posi_equiv(
    const NodePtr& node0,
    const NodePtr& node1
  );

  /// @brief node0 と node1 が式として否定の関係にあるときに true を返す．
  static
  bool
  nega_equiv(
    const NodePtr& node0,
    const NodePtr& node1
  );

  /// @}
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // プライベートメンバ関数
  //////////////////////////////////////////////////////////////////////

  // 子供が全てリテラルの時に true を返す．
  bool
  is_simple_op() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノードタイプ(3ビット)と変数番号
  SizeType mTypeVarId;

  // オペランドのリスト
  vector<NodePtr> mOperandList;

};

END_NAMESPACE_YM_LOGIC

#endif // EXPRNODE_H
