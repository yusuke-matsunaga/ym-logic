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

  /// @brief リテラル関数を作る．
  Bdd
  literal(
    SizeType var,    ///< [in] 変数
    bool inv = false ///< [in] 反転フラグ
  );

  /// @brief リテラル関数を作る．
  Bdd
  literal(
    Literal lit ///< [in] リテラル
  )
  {
    return literal(lit.varid(), lit.is_negative());
  }

  /// @brief 肯定のリテラル関数を作る．
  Bdd
  posi_literal(
    SizeType var ///< [in] 変数
  )
  {
    return literal(var, false);
  }

  /// @brief 否定のリテラル関数を作る．
  Bdd
  nega_literal(
    SizeType var ///< [in] 変数
  )
  {
    return literal(var, true);
  }

  /// @brief 真理値表形式の文字列からBDDを作る．
  ///
  /// str は '0' か '1' の文字列．
  /// ただし，長さは2のべき乗である必要がある．
  /// for some reason, この文字列は big endian となっている．
  /// 0文字目が(1, 1, 1, 1)に対応する
  ///
  /// 不正な形式の場合は std::invalid_argument 例外を送出する．
  Bdd
  from_truth(
    const string& str ///< [in] 01の文字列
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
    const Bdd& bdd, ///< [in] 対象の BDD
    SizeType var,   ///< [in] 変数
    bool inv        ///< [in] 反転フラグ
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
    SizeType index,
    const Bdd& cedge
  );

  /// @brief 複合compose演算
  Bdd
  multi_compose(
    const Bdd& bdd,                                 ///< [in] 対象の BDD
    const unordered_map<SizeType, Bdd>& compose_map ///< [in] 変換マップ
  );

  /// @brief 変数順を入れ替える演算
  ///
  /// 極性も入れ替え可能
  Bdd
  remap_vars(
    const Bdd& bdd,                                ///< [in] 対象の BDD
    const unordered_map<SizeType, Literal>& varmap ///< [in] 変数の対応表
  );

  /// @brief 変数シフト演算
  ///
  /// var をひとつ下にずらす．
  Bdd
  shift_var(
    const Bdd& bdd, ///< [in] 対象の BDD
    SizeType var    ///< [in] 移動元の変数
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

};

END_NAMESPACE_YM_DD

#endif // BDDMGRIMPL_H
