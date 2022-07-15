#ifndef BDDMGRIMPL_H
#define BDDMGRIMPL_H

/// @file BddMgrImpl.h
/// @brief BddMgrImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "BddNode.h"
#include "BddNodeTable.h"
#include "BddApplyTable.h"
#include "BddEdge.h"


BEGIN_NAMESPACE_YM

class BddVar;

//////////////////////////////////////////////////////////////////////
/// @class BddMgrImpl BddMgrImpl.h "BddMgrImpl.h"
/// @brief BddMgr の実際の処理を行うクラス
//////////////////////////////////////////////////////////////////////
class BddMgrImpl
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

  /// @brief 変数を割り当てる．
  /// @return 変数を返す．
  BddVar
  new_variable(
    const string& name = string{} ///< [in] 変数名(オプショナル)
  );

  /// @brief 論理積を計算する．
  /// @return 結果の枝を返す．
  BddEdge
  and_op(
    BddEdge left, ///< [in] オペランド1
    BddEdge right ///< [in] オペランド2
  );

  /// @brief 論理和を計算する．
  /// @return 結果の枝を返す．
  BddEdge
  or_op(
    BddEdge left, ///< [in] オペランド1
    BddEdge right ///< [in] オペランド2
  );

  /// @brief 排他的論理和を計算する．
  /// @return 結果の枝を返す．
  BddEdge
  xor_op(
    BddEdge left, ///< [in] オペランド1
    BddEdge right ///< [in] オペランド2
  );

  /// @brief コファクターを計算する．
  /// @return 結果の枝を返す．
  BddEdge
  cofactor_op(
    BddEdge edge,   ///< [in] 対象の根の枝
    SizeType index, ///< [in] 変数インデックス
    bool inv        ///< [in] 反転フラグ
  );

  /// @brief サポートチェックを行う．
  /// @retval true サポートだった
  /// @retval false サポートではなかった．
  bool
  check_sup(
    BddEdge edge,  ///< [in] 対象の根の枝
    SizeType index ///< [in] 変数インデックス
  );

  /// @brief 対称変数のチェックを行う．
  /// @retval true 対称だった
  /// @retval false 対称ではなかった．
  bool
  check_sym(
    BddEdge edge,    ///< [in] 対象の根の枝
    SizeType index1, ///< [in] 変数インデックス1
    SizeType index2, ///< [in] 変数インデックス1
    bool inv         ///< [in] 反転フラグ
  );

  /// @brief ノード数を数える．
  SizeType
  count_size(
    BddEdge edge    ///< [in] 対象の根の枝
  );

  /// @brief ノード数を数える．
  SizeType
  count_size(
    const vector<BddEdge>& edge_list ///< [in] 対象の根の枝のリスト
  );

  /// @brief 真理値表形式の文字列からBDDを作る．
  ///
  /// str の形式は以下の通り
  /// - big endian: 0文字目が(1, 1, 1, 1)に対応する
  /// - 最初の変数が LSB に対応する．
  BddEdge
  from_truth(
    const string& str,             ///< [in] 01の文字列
    const vector<BddVar>& var_list ///< [in] 変数のリスト
  );

  /// @brief インデックスから変数を返す．
  const BddVar&
  var(
    SizeType index ///< [in] 変数インデックス
  ) const
  {
    ASSERT_COND( 0 <= index && index < mVarList.size() );
    return mVarList[index];
  }

  /// @brief ノードを作る．
  BddEdge
  new_node(
    SizeType index, ///< [in] インデックス
    BddEdge edge0,  ///< [in] 0枝
    BddEdge edge1   ///< [in] 1枝
  );

  /// @brief ノード(枝)の参照回数を増やす．
  void
  inc_ref(
    BddEdge edge ///< [in] 対象の枝
  );

  /// @brief ノード(枝)の参照回数を減らす．
  void
  dec_ref(
    BddEdge edge ///< [in] 対象の枝
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief コファクターを計算する．
  /// @return 結果の枝を返す．
  BddEdge
  cofactor_step(
    BddEdge edge ///< [in] 対象の根の枝
  );

  /// @brief サポートチェックを行う．
  bool
  check_sup_step(
    BddEdge edge
  );

  /// @brief from_truth の下請け関数
  BddEdge
  truth_step(
    const string& str,
    SizeType pos
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数のリスト
  vector<BddVar> mVarList;

  // ノードテーブル
  BddNodeTable mNodeTable;

  // 演算結果テーブル
  BddApplyTable mApplyTable;

  // コファクター用の演算結果テーブル
  unordered_map<BddNode*, BddEdge> mCofacTable;

  // コファクター用のインデックス
  SizeType mCofacIndex;

  // コファクター用の極性
  bool mCofacInv;

  // truth 用の演算結果テーブル
  unordered_map<string, BddEdge> mTruthTable;

  // truth 用の変数リスト
  vector<BddVar> mTruthVarList;

};

END_NAMESPACE_YM

#endif // BDDMGR_H
