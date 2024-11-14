#ifndef BDDMGRIMPL_H
#define BDDMGRIMPL_H

/// @file BddMgrImpl.h
/// @brief BddMgrImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022, 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "ym/Bdd.h"
#include "ym/BddVar.h"
#include "ym/BddLit.h"
#include "ym/BinDec.h"
#include "ym/BinEnc.h"
#include "dd/DdNode.h"
#include "dd/DdEdge.h"
#include "dd/DdNodeMgr.h"


BEGIN_NAMESPACE_YM_DD

class DdInfo;

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

  /// @brief 変数を返す．
  BddVar
  variable(
    SizeType varid ///< [in] 変数番号
  );

  /// @brief 変数のリストを返す．
  vector<BddVar>
  variable_list() const;

  /// @brief インデックス
  /// @brief 恒偽関数を作る．
  Bdd
  zero();

  /// @brief 恒真関数を作る．
  Bdd
  one();

  /// @brief BDD を作る．
  Bdd
  bdd(
    SizeType index,   ///< [in] インデックス
    const Bdd& edge0, ///< [in] 0枝
    const Bdd& edge1  ///< [in] 1枝
  );

  /// @brief BDD をコピーする．
  ///
  /// 通常は同じものを返すが，src のマネージャが異なる場合には
  /// 同じ構造のコピーを作る．
  Bdd
  copy(
    const Bdd& src
  );

  /// @brief リテラル関数を表すBDDを作る．
  BddLit
  literal(
    const BddVar& var, ///< [in] 変数
    bool inv = false   ///< [in] 反転フラグ
  )
  {
    _check_mgr(var);
    return BddLit{var, inv};
  }

  /// @brief 肯定のリテラル関数を作る．
  BddLit
  posi_literal(
    const BddVar& var ///< [in] 変数
  )
  {
    return literal(var, false);
  }

  /// @brief 否定のリテラル関数を作る．
  BddLit
  nega_literal(
    const BddVar& var ///< [in] 変数
  )
  {
    return literal(var, true);
  }

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
    const vector<BddVar>& var_list, ///< [in] 変数のリスト
    const string& str               ///< [in] 01の文字列
  );

  /// @brief AND 演算を行う．
  Bdd
  and_op(
    const Bdd& left,
    const Bdd& right
  );

  /// @brief OR 演算を行う．
  Bdd
  or_op(
    const Bdd& left,
    const Bdd& right
  )
  {
    // DeMorgan's law
    return ~and_op(~left, ~right);
  }

  /// @brief XOR 演算を行う．
  Bdd
  xor_op(
    const Bdd& left,
    const Bdd& right
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

  /// @brief コファクターを計算する．
  /// @return 結果を返す．
  Bdd
  cofactor(
    const Bdd& bdd,    ///< [in] 対象の BDD
    const BddVar& var, ///< [in] 変数
    bool inv           ///< [in] 反転フラグ
                       ///<  - false: 反転なし (正極性)
                       ///<  - true:  反転あり (負極性)
  );

  /// @brief コファクターを計算する．
  Bdd
  cofactor(
    const Bdd& bdd, ///< [in] 対象の BDD
    const Bdd& cube ///< [in] コファクターのキューブ
                    /// cube.is_cube() = true でなければならない．
  );

  /// @brief compose 演算を行う．
  Bdd
  compose(
    const Bdd& edge,
    const BddVar& var,
    const Bdd& cedge
  );

  /// @brief 複合compose演算
  Bdd
  multi_compose(
    const Bdd& bdd,                               ///< [in] 対象の BDD
    const unordered_map<BddVar, Bdd>& compose_map ///< [in] 変換マップ
  );

  /// @brief 変数順を入れ替える演算
  ///
  /// 極性も入れ替え可能
  Bdd
  remap_vars(
    const Bdd& bdd,                             ///< [in] 対象の BDD
    const unordered_map<BddVar, BddLit>& varmap ///< [in] 変数の対応表
  );

  /// @brief 変数リストをインデックスリストに変換する．
  vector<SizeType>
  index_list(
    const vector<BddVar>& var_list ///< [in] 変数リスト
  ) const;

  /// @brief インデックスを変数に変換する．
  BddVar
  index_to_var(
    SizeType index ///< [in] インデックス
  );

  /// @brief 変数順を設定する．
  void
  set_variable_order(
    const vector<BddVar>& order_list ///< [in] 変数順を表すリスト
  );


public:
  //////////////////////////////////////////////////////////////////////
  // 内容の出力を行う関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 複数のBDDのノード数を数える．
  SizeType
  bdd_size(
    const vector<Bdd>& bdd_list ///< [in] BDDのリスト
  );

  /// @brief 複数のBDDの内容を出力する．
  void
  display(
    ostream& s,                 ///< [in] 出力ストリーム
    const vector<Bdd>& bdd_list ///< [in] BDDのリスト
  );

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
    const vector<Bdd>& bdd_list ///< [in] BDDのリスト
  );

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

  /// @brief 複数のBDDのノードの情報を取り出す．
  vector<DdInfo>
  node_info(
    const vector<Bdd>& bdd_list,     ///< [in] BDDのリスト
    vector<SizeType>& root_edge_list ///< [out] 根の情報を格納するリスト
  );


public:
  //////////////////////////////////////////////////////////////////////
  // DdEdge を扱う関数
  //////////////////////////////////////////////////////////////////////

  /// @brief DdEdge を Bdd に変換する．
  Bdd
  _bdd(
    DdEdge edge
  );

  /// @brief Bdd から DdEdge を取り出す．
  DdEdge
  _edge(
    const Bdd& bdd
  );

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
    auto l_index = l_node->index();
    auto r_node = right.node();
    auto r_inv = right.inv();
    auto r_index = r_node->index();
    auto top = std::min(l_index, r_index);
    if ( l_index == top ) {
      left0 = l_node->edge0() ^ l_inv;
      left1 = l_node->edge1() ^ l_inv;
    }
    else {
      left0 = left;
      left1 = left;
    }
    if ( r_index == top ) {
      right0 = r_node->edge0() ^ r_inv;
      right1 = r_node->edge1() ^ r_inv;
    }
    else {
      right0 = right;
      right1 = right;
    }
    return top;
  }

  /// @brief BDDの根の枝のリストを作る．
  vector<DdEdge>
  root_list(
    const vector<Bdd>& bdd_list ///< [in] BDDのリスト
  );


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

  /// @brief 隣り合うインデックスを交換する．
  void
  swap_index(
    SizeType index ///< [in] 交換する上のインデックス
  );

  /// @brief このマネージャに属しているオブジェクトかチェックする．
  void
  _check_mgr(
    const Bdd& bdd ///< [in] 対象のオブジェクト
  ) const
  {
    if ( bdd.mMgr != this ) {
      throw std::invalid_argument{"BddMgr mismatch"};
    }
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数のリスト
  vector<BddVar> mVarList;

};

END_NAMESPACE_YM_DD

#endif // BDDMGRIMPL_H
