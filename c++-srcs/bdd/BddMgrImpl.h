#ifndef BDDMGRIMPL_H
#define BDDMGRIMPL_H

/// @file BddMgrImpl.h
/// @brief BddMgrImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bdd_nsdef.h"
#include "ym/BddVar.h"
#include "BddNode.h"
#include "BddNodeTable.h"
#include "BddEdge.h"


BEGIN_NAMESPACE_YM_BDD

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

  /// @brief 該当するノードを探す．
  ///
  /// なければ nullptr を返す．
  BddNode*
  find(
    SizeType index, ///< [in] インデックス
    BddEdge edge0,  ///< [in] 0枝
    BddEdge edge1   ///< [in] 1枝
  ) const;

  /// @brief ノードを登録する．
  void
  put(
    BddNode* node ///< [in] 結果のノード
  );

  /// @brief 表を拡張する．
  void
  extend(
    SizeType req_size ///< [in] 要求サイズ
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数のリスト
  vector<BddVar> mVarList;

  // 表のサイズ
  SizeType mSize{0};

  // ハッシュ用のモジュロサイズ
  SizeType mHashSize{0};

  // 表の本体
  BddNode** mTable{nullptr};

  // 格納されているノード数
  SizeType mCount{0};

  // テーブルを拡張する目安
  SizeType mNextLimit;

};

END_NAMESPACE_YM_BDD

#endif // BDDMGRIMPL_H
