#ifndef BDDMGRPTR_H
#define BDDMGRPTR_H

/// @file BddMgrPtr.h
/// @brief BddMgrPtr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "ym/BinDec.h"
#include "ym/BinEnc.h"
#include "ym/JsonValue.h"


BEGIN_NAMESPACE_YM_DD

class BddMgrImpl;
class DdEdge;
class DdInfoMgr;

//////////////////////////////////////////////////////////////////////
/// @class BddMgrPtr BddMgrPtr.h "ym/BddMgrPtr.h"
/// @brief BddMgrImpl のスマートポインタ
///
/// 中身は std::shared_ptr<BddMgrImpl> だが意味的には BddMgrImpl
/// の delegate クラスとなっている．
//////////////////////////////////////////////////////////////////////
class BddMgrPtr
{
public:

  /// @brief 生のポインタを指定したコンストラクタ
  explicit
  BddMgrPtr(
    BddMgrImpl* ptr = nullptr ///< [in] マネージャのポインタ
  );

  /// @brief コピーコンストラクタ
  BddMgrPtr(
    const BddMgrPtr& src ///< [in] コピー元のオブジェクト
  );

  /// @brief コピー代入演算子
  BddMgrPtr&
  operator=(
    const BddMgrPtr& src ///< [in] コピー元のオブジェクト
  );

  /// @brief デストラクタ
  ~BddMgrPtr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 変数を返す．
  BddVar
  variable(
    SizeType varid ///< [in] 変数番号
  ) const;

  /// @brief 変数のリストを返す．
  vector<BddVar>
  variable_list() const;

  /// @brief 恒偽関数を作る．
  Bdd
  zero() const;

  /// @brief 恒真関数を作る．
  Bdd
  one() const;

  /// @brief BDD を作る．
  Bdd
  bdd(
    SizeType index,   ///< [in] インデックス
    const Bdd& edge0, ///< [in] 0枝
    const Bdd& edge1  ///< [in] 1枝
  ) const;

  /// @brief BDD をコピーする．
  ///
  /// 通常は同じものを返すが，src のマネージャが異なる場合には
  /// 同じ構造のコピーを作る．
  Bdd
  copy(
    const Bdd& src
  ) const;

  /// @brief リテラル関数を表すBDDを作る．
  BddLit
  literal(
    const BddVar& var, ///< [in] 変数
    bool inv = false   ///< [in] 反転フラグ
  ) const;

  /// @brief 肯定のリテラル関数を作る．
  BddLit
  posi_literal(
    const BddVar& var ///< [in] 変数
  ) const;

  /// @brief 否定のリテラル関数を作る．
  BddLit
  nega_literal(
    const BddVar& var ///< [in] 変数
  ) const;

  /// @brief AND 演算を行う．
  Bdd
  and_op(
    const Bdd& left,
    const Bdd& right
  ) const;

  /// @brief OR 演算を行う．
  Bdd
  or_op(
    const Bdd& left,
    const Bdd& right
  ) const;

  /// @brief XOR 演算を行う．
  Bdd
  xor_op(
    const Bdd& left,
    const Bdd& right
  ) const;

  /// @brief ITE 演算を行う．
  Bdd
  ite(
    const Bdd& e0,
    const Bdd& e1,
    const Bdd& e2
  ) const;

  /// @brief 真理値表形式の文字列からBDDを作る．
  ///
  /// str は '0' か '1' の文字列．
  /// ただし，長さは var_list の長さのべき乗である必要がある．
  /// for some reason, この文字列は big endian となっている．
  /// 0文字目が(1, 1, 1, 1)に対応する
  ///
  /// 不正な形式の場合は std::invalid_argument 例外を送出する．
  Bdd
  from_truth(
    const string& str,             ///< [in] 01の文字列
    const vector<BddVar>& var_list ///< [in] 変数のリスト
  ) const;

  /// @brief 論理式から BDD を作る．
  ///
  /// - var_list が省略された場合は自動的に適切な変数リストを用いる．
  Bdd
  from_expr(
    const Expr& expr,              ///< [in] 論理式
    const vector<BddVar>& var_list ///< [in] 変数のリスト
  );

  /// @brief ドントケアを利用した簡単化を行う．
  Bdd
  simplify(
    const Bdd& on,  ///< [in] オンセット
    const Bdd& dc   ///< [in] ドントケアセット
  ) const;

  /// @brief コファクターを計算する．
  /// @return 結果を返す．
  Bdd
  cofactor(
    const Bdd& bdd,    ///< [in] 対象の BDD
    const BddVar& var, ///< [in] 変数
    bool inv           ///< [in] 反転フラグ
                       ///<  - false: 反転なし (正極性)
                       ///<  - true:  反転あり (負極性)
  ) const;

  /// @brief コファクターを計算する．
  Bdd
  cofactor(
    const Bdd& bdd, ///< [in] 対象の BDD
    const Bdd& cube ///< [in] コファクターのキューブ
                    /// cube.is_cube() = true でなければならない．
  ) const;

  /// @brief compose 演算を行う．
  Bdd
  compose(
    const Bdd& edge,
    const BddVar& var,
    const Bdd& cedge
  ) const;

  /// @brief 複合compose演算
  Bdd
  multi_compose(
    const Bdd& bdd,                               ///< [in] 対象の BDD
    const unordered_map<BddVar, Bdd>& compose_map ///< [in] 変換マップ
  ) const;

  /// @brief 変数順を入れ替える演算
  ///
  /// 極性も入れ替え可能
  Bdd
  remap_vars(
    const Bdd& bdd,                             ///< [in] 対象の BDD
    const unordered_map<BddVar, BddLit>& varmap ///< [in] 変数の対応表
  ) const;

  /// @brief 変数のリストをレベルのリストに変換する．
  vector<SizeType>
  level_list(
    const vector<BddVar>& var_list ///< [in] 変数リスト
  ) const;

  /// @brief レベルを変数に変換する．
  BddVar
  level_to_var(
    SizeType level ///< [in] レベル
  ) const;

  /// @brief 変数順を表す変数のリストを返す．
  ///
  /// レベルの昇順に並んでいる
  vector<BddVar>
  variable_order() const;

  /// @brief 変数順を設定する．
  void
  set_variable_order(
    const vector<BddVar>& order_list ///< [in] 変数順を表すリスト
  ) const;

  /// @brief 複数のBDDのノード数を数える．
  SizeType
  bdd_size(
    const vector<Bdd>& bdd_list ///< [in] BDDのリスト
  ) const;

  /// @brief 複数のBDDの内容を出力する．
  void
  display(
    ostream& s,                 ///< [in] 出力ストリーム
    const vector<Bdd>& bdd_list ///< [in] BDDのリスト
  ) const;

  /// @brief 複数のBDDを dot 形式で出力する．
  void
  gen_dot(
    ostream& s,                  ///< [in] 出力ストリーム
    const vector<Bdd>& bdd_list, ///< [in] BDDのリスト
    const JsonValue& option      ///< [in] オプションを表す JSON オブジェクト
  ) const;

  /// @brief 構造を表す整数配列を作る．
  vector<SizeType>
  rep_data(
    const vector<Bdd>& bdd_list ///< [in] BDDのリスト
  ) const;

  /// @brief BDD の内容をバイナリダンプする．
  void
  dump(
    BinEnc& s,                  ///< [in] 出力ストリーム
    const vector<Bdd>& bdd_list ///< [in] 対象の BDDのリスト
  ) const;

  /// @brief バイナリダンプから復元する．
  /// @return 生成されたBDDのリストを返す．
  ///
  /// 不正な形式の場合は std::invalid_argument 例外を送出する．
  vector<Bdd>
  restore(
    BinDec& s ///< [in] 入力ストリーム
  ) const;

  /// @brief 複数のBDDのノードの情報を取り出す．
  DdInfoMgr
  node_info(
    const vector<Bdd>& bdd_list ///< [in] BDDのリスト
  ) const;

  /// @brief 親のマネージャを返す．
  BddMgr
  mgr() const;

  /// @brief DdEdge を Bdd に変換する．
  Bdd
  _bdd(
    DdEdge edge
  ) const;

  /// @brief DdEdge を BddVar に変換する．
  BddVar
  _var(
    DdEdge edge
  ) const;

  /// @brief BddMgrImpl のポインタを取り出す．
  BddMgrImpl*
  get() const
  {
    return mPtr.get();
  }

  /// @brief get() の別名
  BddMgrImpl*
  operator->() const
  {
    return get();
  }

  /// @brief 適正な値を持っている時に true を返す．
  bool
  is_valid() const
  {
    return mPtr.get() != nullptr;
  }

  /// @brief 不正値の時に true を返す．
  bool
  is_invalid() const
  {
    return !is_valid();
  }

  /// @brief 等価比較演算子
  bool
  operator==(
    const BddMgrPtr& right
  ) const
  {
    return mPtr == right.mPtr;
  }

  /// @brief 非等価比較演算子
  bool
  operator!=(
    const BddMgrPtr& right
  ) const
  {
    return !operator==(right);
  }

  /// @brief 同じマネージャの要素かチェックする．
  ///
  /// 異なる場合には std::invalid_argument 例外を送出する．
  void
  _check_mgr(
    const BddMgrPtr& other
  ) const
  {
    if ( mPtr != other.mPtr ) {
      throw std::invalid_argument{"BddMgr mismatch"};
    }
  }

  /// @brief 同じマネージャの要素かチェックする．
  ///
  /// 異なる場合には std::invalid_argument 例外を送出する．
  void
  _check_mgr(
    const Bdd& bdd
  ) const;


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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // マネージャの本体
  std::shared_ptr<BddMgrImpl> mPtr;

};

END_NAMESPACE_YM_DD

#endif // BDDMGRPTR_H
