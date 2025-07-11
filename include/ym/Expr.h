﻿#ifndef YM_EXPR_H
#define YM_EXPR_H

/// @file ym/Expr.h
/// @brief Expr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "ym/Literal.h"
#include "ym/BinDec.h"
#include "ym/BinEnc.h"


BEGIN_NAMESPACE_YM_LOGIC

// クラス名の先行宣言
class ExprNode;

//////////////////////////////////////////////////////////////////////
/// @class Expr Expr.h "ym/Expr.h"
/// @ingroup LogicGroup
/// @brief 論理式を表すクラス
///
/// 論理式は
/// - 定数0, 定数1
/// - リテラル (変数番号 + 極性)
///
/// および, 複数の論理式を AND/OR/XOR でまとめたものから構成される
///
/// このクラスでは表現としての論理式を扱っているので論理関数としての
/// 処理は行っていない．
///
/// 実装は ExprNode の shared_ptr を持っているのでコピーのコストは
/// 高くない．
/// そのためムーブ演算は対応していない．
/// @sa Literal
//////////////////////////////////////////////////////////////////////
class Expr
{
  friend class ExprMgr;

public:
  //////////////////////////////////////////////////////////////////////
  // このクラスで用いられる型の定義
  //////////////////////////////////////////////////////////////////////

  // ExprNode の shared_ptr
  using NodePtr = std::shared_ptr<ExprNode>;

  /// @brief eval() で用いられるビットベクタの型
  using BitVectType = std::uint64_t;

  /// @brief compose() で用いる辞書の型
  using ComposeMap = std::unordered_map<SizeType, Expr>;

  /// @brief compose() で用いるリストの型
  using ComposeList = std::vector<std::pair<SizeType, Expr>>;

  /// @brief remap_var() で用いる辞書の型
  using VarMap = std::unordered_map<SizeType, SizeType>;

  /// @brief remap_var() で用いるリストの型
  using VarList = std::vector<std::pair<SizeType, SizeType>>;


public:
  //////////////////////////////////////////////////////////////////////
  /// @name コンストラクタ/デストラクタ/代入演算子/生成用クラスメソッド
  /// @{

  /// @brief 空のデフォルトコンストラクタ
  ///
  /// - 不正な値になる．
  Expr() = default;

  /// @brief コピーコンストラクタ
  Expr(
    const Expr& src ///< [in] コピー元のオブジェクト
  ) = default;

  /// @brief 代入演算子
  /// @return 自分自身
  Expr&
  operator=(
    const Expr& src ///< [in] コピー元のオブジェクト
  ) = default;

  /// @brief デストラクタ
  ~Expr() = default;

  /// @brief エラーオブジェクトの生成
  /// @return 不適正なオブジェクトを返す．
  ///
  /// 返されたオブジェクトは is_valid() == false となる．
  static
  Expr
  invalid()
  {
    return Expr{};
  }

  /// @brief 恒偽関数の生成
  /// @return 生成したオブジェクト
  static
  Expr
  zero();

  /// @brief 恒真関数の生成
  /// @return 生成したオブジェクト
  static
  Expr
  one();

  /// @brief リテラル式の生成
  /// @return 生成したオブジェクト
  static
  Expr
  literal(
    SizeType varid,  ///< [in] 変数番号
    bool inv = false ///< [in] 極性
                     ///<      - false: 反転なし (正極性)
                     ///<      - true:  反転あり (負極性)
  )
  {
    return inv ? nega_literal(varid) : posi_literal(varid);
  }

  /// @brief リテラル式の生成
  /// @return 生成したオブジェクト
  static
  Expr
  literal(
    Literal lit ///< [in] リテラル
  )
  {
    return literal(lit.varid(), lit.is_negative());
  }

  /// @brief 正(肯定)リテラル式の生成
  /// @return 生成したオブジェクト
  static
  Expr
  posi_literal(
    SizeType varid ///< [in] 変数番号
  );

  /// @brief 負(否定)リテラル式の生成
  /// @return 生成したオブジェクト
  static
  Expr
  nega_literal(
    SizeType varid ///< [in] 変数番号
  );

  /// @brief AND 式の生成
  /// @return chd1, chd2 を部分論理式に持つ AND 式を生成し，返す．
  static
  Expr
  and_op(
    const Expr& chd1, ///< [in] 第1オペランド
    const Expr& chd2  ///< [in] 第2オペランド
  );

  /// @brief AND 式の生成
  /// @return chd_list を部分論理式に持つ AND 式を生成し，返す．
  static
  Expr
  and_op(
    const std::vector<Expr>& chd_list ///< [in] オペランドのベクタ
  );

  /// @brief OR 式の生成
  /// @return chd1, chd2 を部分論理式に持つ OR 式を生成し，返す．
  static
  Expr
  or_op(
    const Expr& chd1, ///< [in] 第1オペランド
    const Expr& chd2  ///< [in] 第2オペランド
  );

  /// @brief OR 式の生成
  /// @return chd_list を部分論理式に持つ OR 式を生成し，返す．
  static
  Expr
  or_op(
    const std::vector<Expr>& chd_list ///< [in] オペランドのベクタ
  );

  /// @brief XOR 式の生成
  /// @return chd1, chd2 を部分論理式に持つ XOR 式を生成し，返す．
  static
  Expr
  xor_op(
    const Expr& chd1, ///< [in] 第1オペランド
    const Expr& chd2  ///< [in] 第2オペランド
  );

  /// @brief XOR 式の生成
  /// @return chd_list を部分論理式に持つ XOR 式を生成し，返す．
  static
  Expr
  xor_op(
    const std::vector<Expr>& chd_list ///< [in] オペランドのベクタ
  );

  /// @brief 論理式をパーズして Expr オブジェクトを作る．
  /// @return 変換された Expr オブジェクト
  ///
  /// - エラーが起きたら std::invalid_argument 例外を送出する．
  static
  Expr
  from_string(
    const std::string& expr_str ///< [in] 論理式を表す文字列
  );

  /// @brief rep_string() 形式の文字列から変換する．
  /// @return 変換された Expr オブジェクト
  ///
  /// - エラーが起きたら std::invalid_argument 例外を送出する．
  static
  Expr
  from_rep_string(
    const std::string& rep_str ///< [in] 論理式を表す文字列
  );

  /// @}
  //////////////////////////////////////////////////////////////////////


  //////////////////////////////////////////////////////////////////////
  /// @name 論理演算および論理演算＋代入演算
  /// @{

  /// @brief 論理否定
  /// @return 自分自身を否定した形の論理式を返す．
  Expr
  operator~() const
  {
    return invert();
  }

  /// @brief operator~() の別名
  Expr
  invert() const;

  /// @brief AND 演算
  /// @return src2 の論理式との AND を返す．
  Expr
  operator&(
    const Expr& src2  ///< [in] 第2オペランド
  ) const
  {
    return Expr::and_op(*this, src2);
  }

  /// @brief AND つき代入
  /// @return 自分自身
  ///
  /// 自分の論理式と src の論理式の AND を計算し自分に代入する．
  Expr&
  operator&=(
    const Expr& src ///< [in] オペランド
  );

  /// @brief OR 演算
  /// @return src2 の論理式との OR を返す．
  Expr
  operator|(
    const Expr& src2  ///< [in] 第2オペランド
  ) const
  {
    return Expr::or_op(*this, src2);
  }

  /// @brief OR つき代入
  /// @return 自分自身
  ///
  /// 自分の論理式と src の論理式の OR を計算し自分に代入する．
  Expr&
  operator|=(
    const Expr& src ///< [in] オペランド
  );

  /// @brief XOR 演算
  /// @return src2 の論理式との XOR を返す．
  Expr
  operator^(
    const Expr& src2  ///< [in] 第2オペランド
  ) const
  {
    return Expr::xor_op(*this, src2);
  }

  /// @brief XOR つき代入
  /// @return 自分自身
  ///
  /// 自分の論理式と src の論理式の XOR を計算し自分に代入する．
  Expr&
  operator^=(
    const Expr& src ///< [in] オペランド
  );

  /// @brief compose 演算
  /// @return varid 番目の終端ノードを sub に置き換えたものを返す．
  ///
  /// - sub の論理式の中に varid 番目のリテラルがあっても
  /// 展開はされない．
  /// -もしも自分自身の論理式の中に varid 番目のリテラル
  /// が含まれない場合にはなにも変わらない．
  Expr
  compose(
    SizeType varid, ///< [in] 置き換え対象の変数番号
    const Expr& sub ///< [in] varid を置き換える先の論理式
  ) const;

  /// @brief 複数変数の compose 演算
  /// @return comp_map にしたがって置き換えを行った論理式
  ///
  /// 一度に複数の置き換えを行う
  Expr
  compose(
    const ComposeMap& comp_map ///< [in] 置き換える変数をキーにして置き換える先の
                               ///< 論理式を値とした連想配列
  ) const;

  /// @brief 複数変数の compose 演算
  /// @return comp_list にしたがって置き換えを行った論理式
  ///
  /// - 一度に複数の置き換えを行う
  /// - comp_list 中に変数の重複が有った場合の動作は不定となる．
  Expr
  compose(
    const ComposeList& comp_list ///< [in] 置き換える変数と置き換える先の
                                 ///<  論理式をペアとしてリスト
  ) const;

  /// @brief 変数番号を再マップする．
  /// @return 置き換えた論理式
  ///
  /// varmap に登録されていない場合には不変とする．
  Expr
  remap_var(
    const VarMap& varmap ///< [in] 置き換え元の変数番号をキーとして
                         ///< 置き換え先の変数番号を値とした連想配列
  ) const;

  /// @brief 変数番号を再マップする．
  /// @return 置き換えた論理式
  ///
  /// - varlist に現れない場合には不変とする．
  /// - varlist 中に変数の重複が有った場合の動作は不定となる．
  Expr
  remap_var(
    const VarList& varlist ///< [in] 置き換え元の変数番号と置き換え先の変数番号
                           ///<      をペアとしたリスト
  ) const;

  /// @brief 簡単化
  /// - expr + expr = expr
  /// - expr + ~expr = 1
  /// のような簡単なルールで簡単かを行う
  /// 自分自身が簡単化された式に置き換わる．
  /// @return 自分自身
  Expr&
  simplify();

  /// @brief 値の評価
  /// @return 評価値
  ///
  /// 演算はビット毎に独立に行われる．
  BitVectType
  eval(
    const std::vector<BitVectType>& vals, ///< [in] 変数の値割り当て
    BitVectType mask = ~0UL               ///< [in] 使用するビットのためのマスク
  ) const;

  /// @brief 真理値表の作成
  ///
  /// ni が省略された場合には input_size() が用いられる．
  TvFunc
  tvfunc(
    SizeType ni = 0 ///< [in] 入力数
  ) const;

  /// @}
  //////////////////////////////////////////////////////////////////////


  //////////////////////////////////////////////////////////////////////
  /// @name 根本の演算子の情報を得る．
  /// @{

  /// @brief 適正な値を持っているかチェックする．
  bool
  is_valid() const
  {
    return !is_invalid();
  }

  /// @brief 適正な値を持っていない時 true を返す．
  bool
  is_invalid() const
  {
    return mRoot.get() == nullptr;
  }

  /// @brief 恒偽関数のチェック
  /// @return 恒偽関数を表している時に true を返す．
  bool
  is_zero() const;

  /// @brief 恒真関数のチェック
  /// @return 恒真関数を表している時に true を返す．
  bool
  is_one() const;

  /// @brief 定数関数のチェック
  /// @return 定数関数)(恒偽/恒真)を表している時に true を返す．
  bool
  is_constant() const;

  /// @brief 肯定のリテラルのチェック
  /// @return 肯定のリテラルを表している時に true を返す．
  bool
  is_positive_literal() const;

  /// @brief 否定のリテラルのチェック
  /// @return 否定のリテラルを表している時に true を返す．
  bool
  is_negative_literal() const;

  /// @brief リテラルのチェック
  /// @return リテラルを表している時に true を返す．
  bool
  is_literal() const;

  /// @brief リテラルの変数番号の取得
  /// @retval 変数番号 リテラルの場合
  /// @retval BAD_VARID リテラル以外
  SizeType
  varid() const;

  /// @brief リテラルの取得
  /// @return 対象がリテラルの場合リテラルを返す．
  ///
  /// そうでなければリテラルとして不正な値を返す．
  Literal
  literal() const;

  /// @brief AND 式のチェック
  /// @return トップが AND の時に true を返す．
  bool
  is_and() const;

  /// @brief OR 式のチェック
  /// @return トップが OR の時に true を返す．
  bool
  is_or() const;

  /// @brief XOR 式のチェック
  /// @return トップが XOR の時に true を返す．
  bool
  is_xor() const;

  /// @brief 演算子のチェック
  /// @return トップが演算子の時に true を返す．
  bool
  is_op() const;

  /// @brief オペランドの個数の取得
  /// @retval オペランドの個数 AND/OR/XOR の場合
  /// @retval 0 上記以外の場合
  SizeType
  operand_num() const;

  /// @brief オペランドの取得
  /// @return pos 番目のオペランドを返す．
  ///
  /// ただし pos が範囲外の場合と演算子ノードでなかった場合には
  /// 0を表す式を返す．
  Expr
  operand(
    SizeType pos ///< [in] 取り出すオペランドの位置 (0 <= pos < operand_num())
  ) const;

  /// @brief オペランドのリストの取得
  std::vector<Expr>
  operand_list() const;

  /// @brief "シンプル"な論理式のチェック
  /// @return シンプルな論理式のときに true を返す．
  ///
  /// より具体的には 0, 1, リテラルとすべての子供が
  /// リテラルであるようなの AND/OR/XOR 式を"シンプル"と
  /// 定義している．
  bool
  is_simple() const;

  /// @brief "シンプル"な AND 式のチェック
  /// @return すべての子供がリテラルであるような AND 式のときに
  /// true を返す．
  bool
  is_simple_and() const;

  /// @brief "シンプル"な OR 式のチェック
  /// @return すべての子供がリテラルであるような OR 式のときに
  /// true を返す．
  bool
  is_simple_or() const;

  /// @brief "シンプル"な XOR 式のチェック
  /// @return すべての子供がリテラルであるような XOR 式のときに
  /// true を返す．
  bool
  is_simple_xor() const;

  /// @brief 積和形論理式のチェック
  /// @return 1段目が OR, 2段目が AND の論理式のときに true を返す．
  bool
  is_sop() const;

  /// @brief 組み込み型のチェック
  ///
  /// 組み込み型出ない場合は PrimType::None を返す．
  PrimType
  analyze() const;

  /// @}
  //////////////////////////////////////////////////////////////////////


  //////////////////////////////////////////////////////////////////////
  /// @name リテラル数の計算
  /// @{

  /// @brief リテラル数の取得
  /// @return リテラル数
  SizeType
  literal_num() const;

  /// @brief 変数の出現回数の取得
  /// @return varid 番めの変数のリテラルの出現回数を得る．
  SizeType
  literal_num(
    SizeType varid ///< [in] 変数番号
  ) const;

  /// @brief リテラルの出現回数の取得
  /// @return varid 番めの変数の極性が inv のリテラルの出現回数を得る．
  SizeType
  literal_num(
    SizeType varid, ///< [in] 変数番号
    bool inv        ///< [in] 極性
                    ///<      - false: 反転なし (正極性)
                    ///<      - true:  反転あり (負極性)
  ) const;

  /// @brief リテラルの出現回数の取得
  /// @return lit のリテラルの出現回数を得る．
  SizeType
  literal_num(
    Literal lit ///< [in] リテラル
  ) const
  {
    return literal_num(lit.varid(), lit.is_negative());
  }

  /// @brief 使われている変数の最大の番号 + 1を得る．
  SizeType
  input_size() const;

  /// @brief SOP形式に展開したときのキューブ数の見積もり
  /// @return SOP形式のキューブ (積項) 数
  SizeType
  sop_cube_num() const;

  /// @brief SOP形式に展開した時のリテラル数の見積もり
  /// @return SOP形式のリテラル数
  SizeType
  sop_literal_num() const;

  /// @brief SOP形式に展開した時の変数の出現回数の見積もり
  /// @return SOP形式の varid 番めの変数のリテラルの出現回数
  SizeType
  sop_literal_num(
    SizeType varid ///< [in] 変数番号
  ) const;

  /// @brief SOP形式に展開した時のテラルの出現回数の見積もり
  /// @return SOP形式に展開した時の varid 番めの変数の極性が
  /// inv のリテラルの出現回数
  SizeType
  sop_literal_num(
    SizeType varid, ///< [in] 変数番号
    bool inv        ///< [in] 極性
                    ///<      - false: 反転なし (正極性)
                    ///<      - true:  反転あり (負極性)
  ) const;

  /// @brief SOP形式に展開したときのリテラルの出現回数の見積もり
  /// @return SOP形式に展開したときの lit のリテラルの出現回数
  SizeType
  sop_literal_num(
    Literal lit ///< [in] リテラル
  ) const
  {
    return sop_literal_num(lit.varid(), lit.is_negative());
  }

  /// @}
  //////////////////////////////////////////////////////////////////////


  //////////////////////////////////////////////////////////////////////
  /// @name 比較演算
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief 等価な論理式かどうかチェックする．
  bool
  operator==(
    const Expr& right ///< [in] 第2オペランド
  ) const;

  /// @brief 非等価な論理式かどうかチェックする．
  bool
  operator!=(
    const Expr& right ///< [in] 第2オペランド
  ) const
  {
    return !operator==(right);
  }

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 文字列出力
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief 論理式の内容を文字列にする．
  std::string
  to_string() const;

  /// @brief 圧縮形式の文字列を出力する．
  std::string
  rep_string() const;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name バイナリダンプ/リストア関数
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief バイナリストリームに出力する．
  void
  dump(
    BinEnc& s ///< [in] 出力ストリーム
  ) const;

  /// @brief バイナリストリームから読み込む．
  static
  Expr
  restore(
    BinDec& s ///< [in] 入力ストリーム
  );

  /// @}
  //////////////////////////////////////////////////////////////////////


public:

  //////////////////////////////////////////////////////////////////////
  // friend 関数の宣言
  //////////////////////////////////////////////////////////////////////

  /// @brief 根の演算タイプの比較
  /// @return src1 と src2 の根のタイプが同じとき true を返す．
  friend
  bool
  compare_type(
    const Expr& src1, ///< [in] 第1オペランド
    const Expr& src2  ///< [in] 第2オペランド
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  // @brief 内部で用いるコンストラクタ
  explicit
  Expr(
    NodePtr node ///< [in] 根のノード
  ) : mRoot{node}
  {
  }

  /// @brief 根のノートを得る．
  NodePtr
  root() const
  {
    return mRoot;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 根のノード
  NodePtr mRoot{nullptr};

};


//////////////////////////////////////////////////////////////////////
// クラスに属さない関数の宣言
//////////////////////////////////////////////////////////////////////

/// @name 論理演算
/// @{

/// @relates Expr
bool
compare_type(
  const Expr& src1, ///< [in] 第1オペランド
  const Expr& src2  ///< [in] 第2オペランド
);

/// @}

/// @relates Expr
/// @brief 論理式の内容のストリーム出力
/// @return s
std::ostream&
operator<<(
  std::ostream& s,      ///< [in] 出力ストリーム
  const Expr& expr ///< [in] 論理式
);

/// @relates Expr
/// @brief 論理式の内容のバイナリ出力
/// @return s
inline
BinEnc&
operator<<(
  BinEnc& s,      ///< [in] 出力ストリーム
  const Expr& expr ///< [in] 論理式
)
{
  expr.dump(s);
  return s;
}

/// @relates Expr
/// @brief 論理式の内容のバイナリ入力
/// @return s
inline
BinDec&
operator>>(
  BinDec& s, ///< [in] 入力ストリーム
  Expr& expr ///< [out] 論理式
)
{
  expr = Expr::restore(s);
  return s;
}

END_NAMESPACE_YM_LOGIC

#endif // YM_EXPR_H
