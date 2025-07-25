#ifndef BDD_H
#define BDD_H

/// @file Bdd.h
/// @brief Bdd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "ym/BddMgrHolder.h"
#include "ym/BinEnc.h"
#include "ym/JsonValue.h"


BEGIN_NAMESPACE_YM_DD

class DdEdge;

//////////////////////////////////////////////////////////////////////
/// @class Bdd Bdd.h "ym/Bdd.h"
/// @brief BDD を表すクラス
///
/// - 基本的に個々の Bdd は一つの BddMgr に属す．
/// - 例外は invalid な Bdd で関連する BddMgr を持たない．
/// - Bdd 間の演算は同じ BddMgr に属するもののみ可とする．
/// - 異なる BddMgr に属する Bdd の演算は std::invalid_argument 例外を
///   送出する．
//////////////////////////////////////////////////////////////////////
class Bdd :
  public BddMgrHolder
{
  friend class BddMgrHolder;

public:

  /// @brief compose 用の変換マップの型
  using ComposeMap = std::unordered_map<BddVar, Bdd>;

  /// @brief remap_vars 用の変換マップの型
  using VarMap = std::unordered_map<BddVar, BddLit>;

public:

  /// @brief 空のコンストラクタ
  ///
  /// 不正な値となる．
  Bdd() = default;

  /// @brief コピーコンストラクタ
  Bdd(
    const Bdd& src ///< [in] コピー元のBDD
  );

  /// @brief コピー代入演算子
  Bdd&
  operator=(
    const Bdd& src ///< [in] コピー元のBDD
  );

  /// @brief デストラクタ
  ~Bdd();


public:
  //////////////////////////////////////////////////////////////////////
  /// @name オブジェクト生成用のクラスメソッド
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief 不正な値を作る．
  static
  Bdd
  invalid()
  {
    return Bdd{};
  }

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 論理演算
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief 否定した関数を返す．
  /// @return 結果を返す．
  Bdd
  invert() const;

  /// @brief invert の別名
  /// @return 結果を返す．
  Bdd
  operator~() const
  {
    return invert();
  }

  /// @brief 極性をかけ合わせる．
  Bdd
  operator^(
    bool inv ///< [in] 反転フラグ
  ) const;

  /// @brief 論理積を返す．
  /// @return 結果を返す．
  Bdd
  and_op(
    const Bdd& right ///< [in] オペランド
  ) const;

  /// @brief and_op の別名
  /// @return 結果を返す．
  Bdd
  operator&(
    const Bdd& right ///< [in] オペランド
  ) const
  {
    return and_op(right);
  }

  /// @brief 論理和を返す．
  /// @return 結果を返す．
  Bdd
  or_op(
    const Bdd& right ///< [in] オペランド
  ) const;

  /// @brief or_op の別名
  /// @return 結果を返す．
  Bdd
  operator|(
    const Bdd& right ///< [in] オペランド
  ) const
  {
    return or_op(right);
  }

  /// @brief 排他的論理和を返す．
  /// @return 結果を返す．
  Bdd
  xor_op(
    const Bdd& right ///< [in] オペランド
  ) const;

  /// @brief xor_op の別名
  /// @return 結果を返す．
  Bdd
  operator^(
    const Bdd& right ///< [in] オペランド
  ) const
  {
    return xor_op(right);
  }

  /// @brief コファクターを計算する．
  /// @return 結果を返す．
  Bdd
  cofactor(
    const BddVar& var, ///< [in] 変数
    bool inv           ///< [in] 反転フラグ
                       ///<      - false: 反転なし (正極性)
                       ///<      - true:  反転あり (負極性)
  ) const;

  /// @brief コファクターを計算する．
  /// @return 結果を返す．
  Bdd
  cofactor(
    const BddLit& lit ///< [in] リテラル
  ) const;

  /// @brief cofactor の別名
  Bdd
  operator/(
    const BddLit& lit ///< [in] リテラル
  ) const
  {
    return cofactor(lit);
  }

  /// @brief コファクターを計算する．
  Bdd
  cofactor(
    const Bdd& cube ///< [in] コファクターのキューブ
                    /// cube.is_cube() = true でなければならない．
  ) const;

  /// @brief cofactor の別名
  Bdd
  operator/(
    const Bdd& cube ///< [in] コファクターのキューブ
                    /// cube.is_cube() = true でなければならない．
  ) const
  {
    return cofactor(cube);
  }

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 論理演算を伴った代入
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief 自分自身を否定する．
  /// @return 自分自身への参照を返す．
  Bdd&
  invert_int();

  /// @brief 極性をかけ合わせて代入する．
  /// @return 自分自身への参照を返す．
  Bdd&
  operator^=(
    bool inv ///< [in] 反転フラグ
  );

  /// @brief 論理積を計算して代入する．
  /// @return 自分自身への参照を返す．
  Bdd&
  and_int(
    const Bdd& right ///< [in] オペランド
  );

  /// @brief and_int の別名
  /// @return 自分自身への参照を返す．
  Bdd&
  operator&=(
    const Bdd& right ///< [in] オペランド
  )
  {
    return and_int(right);
  }

  /// @brief 論理和を計算して代入する．
  /// @return 自分自身への参照を返す．
  Bdd&
  or_int(
    const Bdd& right ///< [in] オペランド
  );

  /// @brief or_int の別名
  /// @return 自分自身への参照を返す．
  Bdd&
  operator|=(
    const Bdd& right ///< [in] オペランド
  )
  {
    return or_int(right);
  }

  /// @brief 排他的論理和を計算して代入する．
  /// @return 自分自身への参照を返す．
  Bdd&
  xor_int(
    const Bdd& right ///< [in] オペランド
  );

  /// @brief xor_int の別名
  /// @return 自分自身への参照を返す．
  Bdd&
  operator^=(
    const Bdd& right ///< [in] オペランド
  )
  {
    return xor_int(right);
  }

  /// @brief コファクターを計算して代入する．
  /// @return 自分自身への参照を返す．
  Bdd&
  cofactor_int(
    const BddVar& var, ///< [in] 変数
    bool inv           ///< [in] 反転フラグ
                       ///<      - false: 反転なし (正極性)
                       ///<      - true:  反転あり (負極性)
  );

  /// @brief コファクターを計算して代入する．
  /// @return 自分自身への参照を返す．
  Bdd&
  cofactor_int(
    const BddLit& lit ///< [in] リテラル
  );

  /// @brief cofactor_int の別名
  /// @return 自分自身への参照を返す．
  Bdd&
  operator/=(
    const BddLit& lit ///< [in] リテラル
  )
  {
    return cofactor_int(lit);
  }

  /// @brief コファクターを計算して代入する．
  /// @return 自分自身への参照を返す．
  Bdd&
  cofactor_int(
    const Bdd& cube ///< [in] コファクターのキューブ
                    /// cube.is_cube() = true でなければならない．
                    /// そうでない場合は std::invalid_argument 例外が
                    /// 送出される．
  );

  /// @brief cofactor_int の別名
  /// @return 自分自身への参照を返す．
  Bdd&
  operator/=(
    const Bdd& cube ///< [in] コファクターのキューブ
                    /// cube.is_cube() = true でなければならない．
                    /// そうでない場合は std::invalid_argument 例外が
                    /// 送出される．
  )
  {
    return cofactor_int(cube);
  }

  /// @brief If-Then-Else 演算
  static
  Bdd
  ite(
    const Bdd& cond,   ///< [in] 条件
    const Bdd& then_f, ///< [in] 条件が成り立ったとき選ばれる関数
    const Bdd& else_f  ///< [in] 条件が成り立たなかった時選ばれる関数
  );

  /// @brief ドントケアを利用した簡単化を行う．
  static
  Bdd
  simplify(
    const Bdd& on,  ///< [in] オンセット
    const Bdd& dc   ///< [in] ドントケアセット
  );

  /// @brief (単一)compose演算
  Bdd
  compose(
    const BddVar& var, ///< [in] 対象の変数
    const Bdd& cfunc   ///< [in] 置き換える関数
  ) const;

  /// @brief (単一)compose演算を行って代入する．
  Bdd&
  compose_int(
    const BddVar& var, ///< [in] 対象の変数
    const Bdd& cfunc   ///< [in] 置き換える関数
  );

  /// @brief 複合compose演算
  Bdd
  multi_compose(
    const ComposeMap& compose_map ///< [in] 変換マップ
  ) const;

  /// @brief 複合compose演算を行って代入する．
  Bdd&
  multi_compose_int(
    const ComposeMap& compose_map ///< [in] 変換マップ
  );

  /// @brief 変数順を入れ替える演算
  ///
  /// 極性も入れ替え可能
  Bdd
  remap_vars(
    const VarMap& var_map ///< [in] 変数の変換マップ
  ) const;

  /// @}
  //////////////////////////////////////////////////////////////////////


protected:
  //////////////////////////////////////////////////////////////////////
  /// @name サポート集合を表すBDDに関する演算
  //////////////////////////////////////////////////////////////////////

  /// @brief サポート集合のユニオンを計算して代入する．
  DdEdge
  support_cup(
    const Bdd& right ///< [in] 第2オペランド
  ) const;

  /// @brief サポート集合のインターセクションを計算して代入する．
  DdEdge
  support_cap(
    const Bdd& right ///< [in] 第2オペランド
  ) const;

  /// @brief サポート集合の差を計算して代入する．
  DdEdge
  support_diff(
    const Bdd& right ///< [in] 第2オペランド
  ) const;

  /// @brief サポート集合が共通部分を持つか調べる．
  bool
  support_check_intersect(
    const Bdd& right ///< [in] 第2オペランド
  ) const;

  /// @brief 変数のリストに変換する．
  ///
  /// - is_posicube() == true が成り立つと仮定している．
  /// - そうでない時は std::invalid_argument 例外を送出する．
  std::vector<BddVar>
  to_varlist() const;


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 内容を取得する関数
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief 親のマネージャを返す．
  BddMgr
  mgr() const;

  /// @brief 定数0の時 true を返す．
  bool
  is_zero() const;

  /// @brief 定数1の時 true を返す．
  bool
  is_one() const;

  /// @brief 定数の時 true を返す．
  bool
  is_const() const;

  /// @brief 変数の時 true を返す．
  bool
  is_variable() const;

  /// @brief リテラルの時 true を返す．
  bool
  is_literal() const;

  /// @brief 肯定のリテラルの時 true を返す．
  ///
  /// - is_variable() と等価
  bool
  is_positive_literal() const;

  /// @brief 否定のリテラルの時 true を返す．
  bool
  is_negative_literal() const;

  /// @brief 積項の時 true を返す．
  bool
  is_cube() const;

  /// @brief 正リテラルの積項の時 true を返す．
  bool
  is_posicube() const;

  /// @brief サポート変数のサイズを返す．
  SizeType
  support_size() const;

  /// @brief 与えられた変数がサポートの時 true を返す．
  bool
  check_sup(
    const BddVar& var ///< [in] 変数
  ) const;

  /// @brief 与えられた変数に対して対称の時 true を返す．
  bool
  check_sym(
    const BddVar& var1, ///< [in] 変数1
    const BddVar& var2, ///< [in] 変数2
    bool inv = false    ///< [in] 反転フラグ
  ) const;

  /// @brief サポート変数を表すBDD(BddVarSet)を返す．
  BddVarSet
  get_support() const;

  /// @brief サポート変数のリスト(vector)を得る．
  std::vector<BddVar>
  get_support_list() const;

  /// @brief 1となるパスを求める．
  /// @return 論理積を表すBDDを返す．
  ///
  /// 結果のBDDは is_cube() = true となっている．
  Bdd
  get_onepath() const;

  /// @brief 0となるパスを求める．
  /// @return 論理積を表すBDDを返す．
  ///
  /// 結果のBDDは is_cube() = true となっている．
  Bdd
  get_zeropath() const;

  /// @brief 根の変数とコファクターを求める．
  ///
  /// 自身が葉のノードの場合，invalid な BddVar を返す．
  /// f0, f1 には自分自身が入る．
  BddVar
  root_decomp(
    Bdd& f0, ///< [out] 負のコファクター
    Bdd& f1  ///< [out] 正のコファクター
  ) const;

  /// @brief 根の変数を得る．
  ///
  /// 自身が葉のノードの場合，invalid な BddVar を返す．
  BddVar
  root_var() const;

  /// @brief 負のコファクターを返す．
  ///
  /// 自身が葉のノードの場合，自分自身を返す．
  Bdd
  root_cofactor0() const;

  /// @brief 正のコファクターを返す．
  ///
  ///
  /// 自身が葉のノードの場合，自分自身を返す．
  Bdd
  root_cofactor1() const;

  /// @brief 根が否定されている時 true を返す．
  bool
  root_inv() const;

  /// @brief 評価を行う．
  bool
  eval(
    const std::vector<bool>& inputs ///< [in] 入力値ベクタ
  ) const;

  /// @brief 等価比較演算
  ///
  /// 構造が同じで異なるBDDとの等価比較は
  /// is_identical() で行う．
  bool
  operator==(
    const Bdd& right ///< [in] オペランド
  ) const;

  /// @brief 非等価比較演算
  bool
  operator!=(
    const Bdd& right ///< [in] オペランド2
  ) const
  {
    return !operator==(right);
  }

  /// @brief 同じ構造を持つか調べる．
  ///
  /// 同じマネージャに属するBDDなら同じノードだが
  /// マネージャが異なる場合には構造を調べる必要がある．
  /// その場合，変数番号ではなくレベルを参照する．
  bool
  is_identical(
    const Bdd& right ///< [in] 比較対象のZDD
  ) const;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name その他の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノード数を返す．
  SizeType
  size() const;

  /// @brief リテラルのリストの変換する．
  ///
  /// - is_cube() == true が成り立っていると仮定している．
  /// - そうでない時は std::invalid_argument 例外を送出する．
  std::vector<BddLit>
  to_litlist() const;

  /// @brief 内容を真理値表の文字列に変換する．
  ///
  /// - var_list の先頭がMSBとなる．
  /// - var_list に含まれていない変数があった場合には
  ///   std::invalid_argument 例外を送出する．
  std::string
  to_truth(
    const std::vector<BddVar> var_list ///< [in] 入力変数のリスト
  ) const;

  /// @brief 内容を出力する．
  void
  display(
    std::ostream& s ///< [in] 出力ストリーム
  ) const;

  /// @brief dot 形式で出力する．
  ///
  /// - option は以下のようなキーを持った JSON オブジェクト
  ///   * attr: dot の各種属性値を持った辞書
  ///     属性値は <グループ名> ':' <属性名> で表す．
  ///     グループ名は以下の通り
  ///     - graph:     グラフ全体
  ///     - root:      根のノード
  ///     - node:      通常のノード
  ///     - terminal:  終端ノード
  ///     - terminal0: 定数0の終端ノード
  ///     - terminal1: 定数1の終端ノード
  ///     - edge:      枝
  ///     - edge0:     0-枝
  ///     - edge1:     1-枝
  ///     グループ名と ':' がない場合には全てのグループに対して同一の属性値
  ///     を適用する．
  ///     具体的な属性名と属性値については graphviz の仕様を参照すること．
  ///   * var_label: 変数ラベルを表す配列．配列のキーは変数番号
  ///   * var_texlbl: TeX用の変数ラベルを表す配列．配列のキーは変数番号
  ///   * var_label と var_texlbl は排他的となる．var_texlbl がある時，
  ///     var_label は無視される．
  void
  gen_dot(
    std::ostream& s,        ///< [in] 出力ストリーム
    const JsonValue& option ///< [in] オプションを表す JSON オブジェクト
    = JsonValue{}
  ) const;

  /// @brief 構造を表す整数配列を作る．
  std::vector<SizeType>
  rep_data() const;

  /// @brief 独自形式でバイナリダンプする．
  ///
  /// 復元には BddMgr::restore() を用いる．
  void
  dump(
    BinEnc& s ///< [in] 出力ストリーム
  ) const;

  /// @brief ハッシュ値を返す．
  SizeType
  hash() const;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  // 複数の BDD の情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 複数のBDDのノード数を数える．
  ///
  /// bdd_list 中の BDD は同一のマネージャに属していなければならない．
  static
  SizeType
  bdd_size(
    const std::vector<Bdd>& bdd_list ///< [in] BDDのリスト
  );

  /// @brief 複数のBDDの内容を出力する．
  ///
  /// bdd_list 中の BDD は同一のマネージャに属していなければならない．
  static
  void
  display(
    std::ostream& s,                 ///< [in] 出力ストリーム
    const std::vector<Bdd>& bdd_list ///< [in] BDDのリスト
  );

  /// @brief 複数のBDDを dot 形式で出力する．
  ///
  /// - bdd_list 中の BDD は同一のマネージャに属していなければならない．
  /// - option は以下のようなキーを持った JSON オブジェクト
  ///   * attr: dot の各種属性値を持った辞書
  ///     属性値は <グループ名> ':' <属性名> で表す．
  ///     グループ名は以下の通り
  ///     - graph:     グラフ全体
  ///     - root:      根のノード
  ///     - node:      通常のノード
  ///     - terminal:  終端ノード
  ///     - terminal0: 定数0の終端ノード
  ///     - terminal1: 定数1の終端ノード
  ///     - edge:      枝
  ///     - edge0:     0-枝
  ///     - edge1:     1-枝
  ///     グループ名と ':' がない場合には全てのグループに対して同一の属性値
  ///     を適用する．
  ///     具体的な属性名と属性値については graphviz の仕様を参照すること．
  ///   * var_label: 変数ラベルを表す配列．配列のキーは変数番号
  ///   * var_texlbl: TeX用の変数ラベルを表す配列．配列のキーは変数番号
  ///   * var_label と var_texlbl は排他的となる．var_texlbl がある時，
  ///     var_label は無視される．
  static
  void
  gen_dot(
    std::ostream& s,                  ///< [in] 出力ストリーム
    const std::vector<Bdd>& bdd_list, ///< [in] BDDのリスト
    const JsonValue& option           ///< [in] オプションを表す JSON オブジェクト
    = JsonValue{}
  );

  /// @brief 構造を表す整数配列を作る．
  ///
  /// bdd_list 中の BDD は同一のマネージャに属していなければならない．
  static
  std::vector<SizeType>
  rep_data(
    const std::vector<Bdd>& bdd_list ///< [in] BDDのリスト
  );

  /// @brief BDD の内容をバイナリダンプする．
  ///
  /// bdd_list 中の BDD は同一のマネージャに属していなければならない．
  static
  void
  dump(
    BinEnc& s,                       ///< [in] 出力ストリーム
    const std::vector<Bdd>& bdd_list ///< [in] 対象の BDDのリスト
  );


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 正常検査用の関数
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief 適正な状態か調べる．
  ///
  /// is_valid() でない時は std::invalid_argument 例外を送出する．
  void
  _check_valid() const
  {
    if ( is_invalid() ) {
      throw std::invalid_argument{"invalid BDD"};
    }
  }

  /// @brief リテラルリストを表すBDDか調べる．
  ///
  /// is_cube() でない時は std::invalid_argument 例外を送出する．
  void
  _check_cube() const
  {
    if ( !is_cube() ) {
      throw std::invalid_argument{"not a cube"};
    }
  }

  /// @brief 変数リストを表すBDDか調べる．
  ///
  /// is_posicube() でない時は std::invalid_argument 例外を送出する．
  void
  _check_posicube() const
  {
    if ( !is_posicube() ) {
      throw std::invalid_argument{"not a positive cube"};
    }
  }

  /// @}
  //////////////////////////////////////////////////////////////////////


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスで用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を指定したコンストラクタ
  Bdd(
    const BddMgrHolder& holder,
    DdEdge root
  );

  /// @brief 根の枝を返す．
  DdEdge
  root() const;

  /// @brief 根の枝を変更する．
  void
  _change_root(
    DdEdge new_root ///< [in] 変更する枝
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief AND 演算の下請け関数
  DdEdge
  _and_op(
    const Bdd& right
  ) const;

  /// @brief OR 演算の下請け関数
  DdEdge
  _or_op(
    const Bdd& right
  ) const;

  /// @brief XOR 演算の下請け関数
  DdEdge
  _xor_op(
    const Bdd& right
  ) const;

  /// @brief コファクターを計算する．
  DdEdge
  _cofactor(
    const BddVar& var, ///< [in] 変数
    bool inv           ///< [in] 反転フラグ
                       ///<      - false: 反転なし (正極性)
                       ///<      - true:  反転あり (負極性)
  ) const;

  /// @brief コファクターを計算する．
  DdEdge
  _cofactor(
    const Bdd& cube ///< [in] コファクターのキューブ
                    /// cube.is_cube() = true でなければならない．
  ) const;

  /// @brief 複合compose演算
  DdEdge
  _multi_compose(
    const ComposeMap& compose_map ///< [in] 変換マップ
  ) const;

  /// @brief 変数順を入れ替える演算
  ///
  /// 極性も入れ替え可能
  DdEdge
  _remap_vars(
    const VarMap& var_map ///< [in] 変換マップ
  ) const;

  /// @brief レベルを変数に変換する．
  BddVar
  _level_to_var(
    SizeType level ///< [in] レベル
  ) const;

  /// @brief BDDのリストからマネージャを取り出す．
  ///
  /// 異なるマネージャを持つBDDが混在している場合，
  /// 例外を送出する．
  /// 空リストの場合は nullptr を返す．
  static
  BddMgrImpl*
  _mgr(
    const std::vector<Bdd>& bdd_list ///< [in] BDDのリスト
  );

  /// @brief BDDのリストから枝のリストに変換する．
  static
  std::vector<DdEdge>
  _conv_to_edgelist(
    const std::vector<Bdd>& bdd_list ///< [in] BDDのリスト
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 根の枝(ポインタ+反転属性)
  PtrIntType mRoot{0};

};

END_NAMESPACE_YM_DD

BEGIN_NAMESPACE_STD

// Bdd をキーにしたハッシュ関数クラスの定義
template <>
struct hash<YM_NAMESPACE::Bdd>
{
  SizeType
  operator()(
    const YM_NAMESPACE::Bdd& bdd
  ) const
  {
    return bdd.hash();
  }
};

END_NAMESPACE_STD

#endif // BDD_H
