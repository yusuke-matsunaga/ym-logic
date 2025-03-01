#ifndef BDDMGR_H
#define BDDMGR_H

/// @file BddMgr.h
/// @brief BddMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "ym/Bdd.h"
#include "ym/BddVar.h"
#include "ym/BddMgrHolder.h"
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
class BddMgr :
  public BddMgrHolder
{
public:

  /// @brief コンストラクタ
  BddMgr();

  /// @brief BddMgrHolder を指定したコンストラクタ
  explicit
  BddMgr(
    const BddMgrHolder& holder
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

  /// @brief 論理式から BDD を作る．
  ///
  /// - var_list が省略された場合は自動的に適切な変数リストを用いる．
  Bdd
  from_expr(
    const Expr& expr,              ///< [in] 論理式
    const vector<BddVar>& var_list ///< [in] 変数のリスト
    = {}
  );


public:
  //////////////////////////////////////////////////////////////////////
  // 複数の BDD の情報を取得する関数
  //////////////////////////////////////////////////////////////////////

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
    return get() == right.get();
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

  /// @brief 枝のリストをBddのリストに変換する．
  vector<Bdd>
  conv_to_bddlist(
    const vector<DdEdge>& edge_list ///< [in] 枝のリスト
  ) const;

  /// @brief 枝のリストを変数のリストに変換する．
  vector<BddVar>
  conv_to_varlist(
    const vector<DdEdge>& edge_list ///< [in] 枝のリスト
  ) const;

};

END_NAMESPACE_YM_DD

#endif // BDDMGR_H
