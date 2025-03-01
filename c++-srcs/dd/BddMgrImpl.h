#ifndef BDDMGRIMPL_H
#define BDDMGRIMPL_H

/// @file BddMgrImpl.h
/// @brief BddMgrImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022, 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "ym/JsonValue.h"
#include "ym/BinDec.h"
#include "ym/BinEnc.h"
#include "DdNode.h"
#include "DdEdge.h"
#include "DdNodeMgr.h"


BEGIN_NAMESPACE_YM_DD

class DdInfoMgr;

//////////////////////////////////////////////////////////////////////
/// @class BddMgrImpl BddMgrImpl.h "BddMgrImpl.h"
/// @brief BddMgr の実際の処理を行うクラス
//////////////////////////////////////////////////////////////////////
class BddMgrImpl :
  public DdNodeMgr
{
public:

  /// @brief コンストラクタ
  BddMgrImpl();

  /// @brief デストラクタ
  ~BddMgrImpl();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @breif 変数の数を返す．
  SizeType
  variable_num() const;

  /// @brief 変数(を表す枝)を返す．
  DdEdge
  variable(
    SizeType varid ///< [in] 変数番号
  );

  /// @brief 変数(を表す枝)のリストを返す．
  const vector<DdEdge>&
  variable_list()
  {
    return mVarList;
  }

  /// @brief 変数順を表す変数のリストを返す．
  ///
  /// レベルの昇順に並んでいる
  vector<DdEdge>
  variable_order();

  /// @brief 変数順を設定する．
  void
  set_variable_order(
    const vector<DdEdge>& order_list ///< [in] 変数順を表すリスト
  );

  /// @brief sifting を用いて変数順の最適化を行う．
  void
  dvo_sift();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容の出力を行う関数
  //////////////////////////////////////////////////////////////////////

  /// @brief BDD の内容をバイナリダンプする．
  void
  dump(
    BinEnc& s,                ///< [in] 出力ストリーム
    const DdInfoMgr& info_mgr ///< [in] ノード情報
  );

  /// @brief バイナリダンプから復元する．
  /// @return 生成されたBDDのリストを返す．
  ///
  /// 不正な形式の場合は std::invalid_argument 例外を送出する．
  vector<DdEdge>
  restore(
    BinDec& s ///< [in] 入力ストリーム
  );


public:
  //////////////////////////////////////////////////////////////////////
  // DdEdge を扱う関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードを作る．
  DdEdge
  new_node(
    SizeType index, ///< [in] インデックス
    DdEdge edge0,  ///< [in] 0枝
    DdEdge edge1   ///< [in] 1枝
  );

  /// @brief トップの変数で分解する．
  static
  SizeType
  decomp(
    DdEdge left,
    DdEdge right,
    DdEdge& left0,
    DdEdge& left1,
    DdEdge& right0,
    DdEdge& right1
  )
  {
    auto l_node = left.node();
    auto l_inv = left.inv();
    auto l_level = l_node->level();
    auto r_node = right.node();
    auto r_inv = right.inv();
    auto r_level = r_node->level();
    auto top = std::min(l_level, r_level);
    if ( l_level == top ) {
      left0 = l_node->edge0() ^ l_inv;
      left1 = l_node->edge1() ^ l_inv;
    }
    else {
      left0 = left;
      left1 = left;
    }
    if ( r_level == top ) {
      right0 = r_node->edge0() ^ r_inv;
      right1 = r_node->edge1() ^ r_inv;
    }
    else {
      right0 = right;
      right1 = right;
    }
    return top;
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 演算を行う関数
  //////////////////////////////////////////////////////////////////////

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
  and_oop(
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

  /// @brief 変数順を表す変数のリストを返す．
  ///
  /// レベルの昇順に並んでいる
  vector<BddVar>
  variable_order() const;


private:
  //////////////////////////////////////////////////////////////////////
  // DdNodeMgr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief garbage_collection() が呼ばれた後に呼び出される関数
  void
  after_gc() override;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief dvo_sift() の下請け関数
  ///
  /// level の移動先として最適な位置を求める．
  void
  dvo_sub(
    SizeType level
  );

  /// @brief 隣り合うレベルを交換する．
  void
  swap_level(
    SizeType level ///< [in] 交換する上のレベル
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数(の枝)のリスト
  vector<DdEdge> mVarList;

};

END_NAMESPACE_YM_DD

#endif // BDDMGRIMPL_H
