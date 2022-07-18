#ifndef BDDMGRIMPL_H
#define BDDMGRIMPL_H

/// @file BddMgrImpl.h
/// @brief BddMgrImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bdd_nsdef.h"
#include "BddNode.h"
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

  /// @brief ノードを作る．
  BddEdge
  new_node(
    SizeType index, ///< [in] インデックス
    BddEdge edge0,  ///< [in] 0枝
    BddEdge edge1   ///< [in] 1枝
  );

  /// @brief ノード(枝)の参照回数を増やす．
  void
  activate(
    BddEdge edge ///< [in] 対象の枝
  );

  /// @brief ノード(枝)の参照回数を減らす．
  void
  deactivate(
    BddEdge edge ///< [in] 対象の枝
  );

  /// @brief ノード数を返す．
  SizeType
  node_num()
  {
    return mNodeNum;
  }

  /// @brief GC を起動するしきい値を返す．
  SizeType
  gc_limit()
  {
    return mGcLimit;
  }

  /// @brief GC を起動するしきい値を設定する．
  void
  set_gc_limit(
    SizeType limit
  )
  {
    mGcLimit = limit;
  }


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

  // 表のサイズ
  SizeType mSize{0};

  // ハッシュ用のモジュロサイズ
  // 素数になるように調整されている．
  SizeType mHashSize{0};

  // 表の本体
  BddNode** mTable{nullptr};

  // 格納されているノード数
  SizeType mNodeNum{0};

  // テーブルを拡張する目安
  SizeType mNextLimit;

  // ガーベージノード数
  SizeType mGarbageNum;

  // GC を起こすしきい値
  SizeType mGcLimit;

};

END_NAMESPACE_YM_BDD

#endif // BDDMGRIMPL_H
