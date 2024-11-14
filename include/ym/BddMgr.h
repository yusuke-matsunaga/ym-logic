#ifndef BDDMGR_H
#define BDDMGR_H

/// @file BddMgr.h
/// @brief BddMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "ym/BinDec.h"
#include "ym/BinEnc.h"


BEGIN_NAMESPACE_YM
class PyBddMgr;
END_NAMESPACE_YM

BEGIN_NAMESPACE_YM_DD

class BddMgrImpl;

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
  friend class nsYm::PyBddMgr;

public:

  /// @brief コンストラクタ
  BddMgr();

  /// @brief BddMgrImpl を指定したコンストラクタ
  BddMgr(
    BddMgrImpl* impl
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
  vector<BddVar>
  variable_list() const;

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
  ///
  /// 不正な形式の場合は std::invalid_argument 例外を送出する．
  Bdd
  from_truth(
    const vector<BddVar>& var_list, ///< [in] 変数のリスト
    const string& str               ///< [in] 01の文字列
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
  // BDD の内容を出力する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief BDDのノード数を数える．
  SizeType
  bdd_size(
    const Bdd& bdd ///< [in] BDDのリスト
  )
  {
    return bdd_size({bdd});
  }

  /// @brief 複数のBDDのノード数を数える．
  SizeType
  bdd_size(
    const vector<Bdd>& bdd_list ///< [in] BDDのリスト
  );

  /// @brief BDDの内容を出力する．
  void
  display(
    ostream& s,    ///< [in] 出力ストリーム
    const Bdd& bdd ///< [in] BDDのリスト
  )
  {
    display(s, {bdd});
  }

  /// @brief 複数のBDDの内容を出力する．
  void
  display(
    ostream& s,                 ///< [in] 出力ストリーム
    const vector<Bdd>& bdd_list ///< [in] BDDのリスト
  );

  /// @brief BDDを dot 形式で出力する．
  void
  gen_dot(
    ostream& s,                                    ///< [in] 出力ストリーム
    const Bdd& bdd,                                ///< [in] BDDのリスト
    const unordered_map<string, string>& attr_dict ///< [in] 属性値の辞書
    = {}
  )
  {
    gen_dot(s, {bdd}, attr_dict);
  }

  /// @brief 複数のBDDを dot 形式で出力する．
  void
  gen_dot(
    ostream& s,                                    ///< [in] 出力ストリーム
    const vector<Bdd>& bdd_list,                   ///< [in] BDDのリスト
    const unordered_map<string, string>& attr_dict ///< [in] 属性値の辞書
    = {}
  );

  /// @brief 構造を表す整数配列を作る．
  vector<SizeType>
  rep_data(
    const Bdd& bdd ///< [in] BDDのリスト
  )
  {
    return rep_data({bdd});
  }

  /// @brief 構造を表す整数配列を作る．
  vector<SizeType>
  rep_data(
    const vector<Bdd>& bdd_list ///< [in] BDDのリスト
  );

  /// @brief BDD の内容をバイナリダンプする．
  void
  dump(
    BinEnc& s,     ///< [in] 出力ストリーム
    const Bdd& bdd ///< [in] 対象の BDD
  )
  {
    dump(s, {bdd});
  }

  /// @brief BDD の内容をバイナリダンプする．
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

  /// @brief BddMgrImpl を取り出す．
  BddMgrImpl*
  impl() const
  {
    return mImpl;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 本体
  BddMgrImpl* mImpl;

};

END_NAMESPACE_YM_DD

#endif // BDDMGR_H
