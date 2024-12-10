#ifndef BDDMGR_H
#define BDDMGR_H

/// @file BddMgr.h
/// @brief BddMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "ym/Bdd.h"
#include "ym/BddVar.h"
#include "ym/BddMgrPtr.h"
#include "ym/JsonValue.h"
#include "ym/BinDec.h"
#include "ym/BinEnc.h"


BEGIN_NAMESPACE_YM_DD

//////////////////////////////////////////////////////////////////////
/// @class BddMgr BddMgr.h "ym/BddMgr.h"
/// @brief BDD を管理するためのクラス
///
/// - Bdd の演算は同一の BddMgr に属している Bdd の間でのみ有効
/// - 異なる BddMgr に属する Bdd の演算は std::invalid_argument
///   例外を送出する．
/// - BddMgr は変数順を保持しており，異なる BddMgr 間では変数順が
///   異なる可能性がある．
///
/// - 実体は BddMgrImpl でこのクラスはただの shared pointer となっている．
///   コピーしても同一の BddMgrImpl を持つインスタンスが生成される．
///
/// - BddMgrImpl は内部で参照回数を持っており，参照回数がゼロになると
///   自動的に開放される．
//////////////////////////////////////////////////////////////////////
class BddMgr
{
public:

  /// @brief コンストラクタ
  BddMgr();

  /// @brief BddMgrPtr を指定したコンストラクタ
  BddMgr(
    const BddMgrPtr& impl
  );

  /// @brief コピーコンストラクタ
  BddMgr(
    const BddMgr& src
  );

  /// @brief デストラクタ
  ~BddMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 変数に関する関数
  //////////////////////////////////////////////////////////////////////

  /// @breif 変数の数を返す．
  SizeType
  variable_num() const;

  /// @brief 変数を返す．
  ///
  /// - varid >= variable_num() の場合には新しい変数が確保される．
  /// - auto var = variable(id); var.id() == id; が成り立つ．
  BddVar
  variable(
    SizeType varid ///< [in] 変数番号
  );

  /// @brief 変数のリストを返す．
  ///
  /// 変数番号の昇順に並んでいる
  vector<BddVar>
  variable_list() const;

  /// @brief 変数順を表す変数のリストを返す．
  ///
  /// インデックスの昇順に並んでいる
  vector<BddVar>
  variable_order() const;

  /// @brief 変数順を設定する．
  void
  set_variable_order(
    const vector<BddVar>& order_list ///< [in] 変数順を表すリスト
  );


public:
  //////////////////////////////////////////////////////////////////////
  // BDD を生成する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief BDD をコピーする．
  ///
  /// 通常は同じものを返すが，src のマネージャが異なる場合には
  /// 同じ構造のコピーを作る．
  /// ただし，変数順が異なる場合には効率の悪い処理となる．
  Bdd
  copy(
    const Bdd& src
  );

  /// @brief 恒偽関数を作る．
  Bdd
  zero();

  /// @brief 恒真関数を作る．
  Bdd
  one();

  /// @brief 真理値表形式の文字列からBDDを作る．
  ///
  /// - str は '0' か '1' の文字列．
  /// - ただし，長さはvar_listのサイズのべき乗である必要がある．
  /// - for some reason, この文字列は big endian となっている．
  /// - 0文字目が(1, 1, 1, 1)に対応する
  /// - var_list が省略された場合は自動的に適切な変数リストを用いる．
  /// - 不正な形式の場合は std::invalid_argument 例外を送出する．
  Bdd
  from_truth(
    const string& str,             ///< [in] 01の文字列
    const vector<BddVar>& var_list ///< [in] 変数のリスト
    = {}
  );

  /// @brief ITE 演算を行う．
  Bdd
  ite(
    const Bdd& e0,
    const Bdd& e1,
    const Bdd& e2
  );

  /// @brief ドントケアを利用した簡単化を行う．
  Bdd
  simplify(
    const Bdd& on,  ///< [in] オンセット
    const Bdd& dc   ///< [in] ドントケアセット
  );


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
    const vector<Bdd>& bdd_list ///< [in] BDDのリスト
  );

  /// @brief 複数のBDDの内容を出力する．
  ///
  /// bdd_list 中の BDD は同一のマネージャに属していなければならない．
  static
  void
  display(
    ostream& s,                 ///< [in] 出力ストリーム
    const vector<Bdd>& bdd_list ///< [in] BDDのリスト
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
    ostream& s,                   ///< [in] 出力ストリーム
    const vector<Bdd>& bdd_list,  ///< [in] BDDのリスト
    const JsonValue& option       ///< [in] オプションを表す JSON オブジェクト
    = JsonValue{}
  );

  /// @brief 構造を表す整数配列を作る．
  ///
  /// bdd_list 中の BDD は同一のマネージャに属していなければならない．
  static
  vector<SizeType>
  rep_data(
    const vector<Bdd>& bdd_list ///< [in] BDDのリスト
  );

  /// @brief BDD の内容をバイナリダンプする．
  ///
  /// bdd_list 中の BDD は同一のマネージャに属していなければならない．
  static
  void
  dump(
    BinEnc& s,                  ///< [in] 出力ストリーム
    const vector<Bdd>& bdd_list ///< [in] 対象の BDDのリスト
  );

  /// @brief バイナリダンプから復元する．
  /// @return 生成されたBDDのリストを返す．
  ///
  /// 不正な形式の場合は std::invalid_argument 例外を送出する．
  vector<Bdd>
  restore(
    BinDec& s ///< [in] 入力ストリーム
  );


public:
  //////////////////////////////////////////////////////////////////////
  // マネージャの諸元に関する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ガーベージコレクションを行う．
  void
  garbage_collection();

  /// @brief ノード数を返す．
  SizeType
  node_num() const;

  /// @brief GC を起動するしきい値を返す．
  SizeType
  gc_limit() const;

  /// @brief GC を起動するしきい値を設定する．
  void
  set_gc_limit(
    SizeType limit
  );

  /// @brief GC を許可する．
  ///
  /// ただちに GC が起動するわけではない．
  void
  enable_gc();

  /// @brief GC を禁止する．
  void
  disable_gc();

  /// @brief 等価比較演算子
  bool
  operator==(
    const BddMgr& right
  ) const
  {
    return mImpl == right.mImpl;
  }

  /// @brief 非等価比較演算子
  bool
  operator!=(
    const BddMgr& right
  ) const
  {
    return !operator==(right);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 同じマネージャの要素かチェックする．
  ///
  /// 異なる場合には std::invalid_argument 例外を送出する．
  void
  _check_mgr(
    const Bdd& bdd
  ) const
  {
    bdd._check_mgr(mImpl);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 本体
  BddMgrPtr mImpl;

};

END_NAMESPACE_YM_DD

#endif // BDDMGR_H
