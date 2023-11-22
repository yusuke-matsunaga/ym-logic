#ifndef ZDDMGRIMPL_H
#define ZDDMGRIMPL_H

/// @file ZddMgrImpl.h
/// @brief ZddMgrImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "ZddNode.h"
#include "ZddEdge.h"
#include "Apply2Key.h"


BEGIN_NAMESPACE_YM_ZDD

//////////////////////////////////////////////////////////////////////
/// @class ZddMgrImpl ZddMgrImpl.h "ZddMgrImpl.h"
/// @brief ZddMgr の実際の処理を行うクラス
//////////////////////////////////////////////////////////////////////
class ZddMgrImpl
{
public:

  /// @brief コンストラクタ
  ZddMgrImpl();

  /// @brief デストラクタ
  ~ZddMgrImpl();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief コピーする．
  ZddEdge
  copy(
    const Zdd& src
  );

  /// @brief CAP 演算を行う．
  ZddEdge
  cap_op(
    ZddEdge left,
    ZddEdge right
  )
  {
    return cap_step(left, right);
  }

  /// @brief CUP 演算を行う．
  ZddEdge
  cup_op(
    ZddEdge left,
    ZddEdge right
  )
  {
    return cup_step(left, right);
  }

  /// @brief DIFF 演算を行う．
  ZddEdge
  diff_op(
    ZddEdge left,
    ZddEdge right
  )
  {
    return diff_step(left, right);
  }

  /// @brief PRODUCT 演算を行う．
  ZddEdge
  product_op(
    ZddEdge left,
    ZddEdge right
  )
  {
    return product_step(left, right);
  }

  /// @brief 部分集合を作る．
  ZddEdge
  make_set(
    const vector<SizeType>& elem_list
  );

  /// @brief ノードを作る．
  ZddEdge
  new_node(
    SizeType index, ///< [in] インデックス
    ZddEdge edge0,  ///< [in] 0枝
    ZddEdge edge1   ///< [in] 1枝
  );

  /// @brief ノード(枝)の参照回数を増やす．
  void
  activate(
    ZddEdge edge ///< [in] 対象の枝
  );

  /// @brief ノード(枝)の参照回数を減らす．
  void
  deactivate(
    ZddEdge edge ///< [in] 対象の枝
  );

  /// @brief ガーベージコレクションを行う．
  void
  garbage_collection();

  /// @brief ノード数を返す．
  SizeType
  node_num()
  {
    return mNodeNum;
  }

  /// @brief ガーベージノード数を返す．
  SizeType
  garbage_num()
  {
    return mGarbageNum;
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

  /// @brief GC を許可する．
  void
  enable_gc()
  {
    mGcEnable = true;
  }

  /// @brief GC を禁止する．
  void
  disable_gc()
  {
    mGcEnable = false;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノード(枝)の参照回数を増やす．
  void
  inc_ref(
    ZddEdge edge ///< [in] 対象の枝
  );

  /// @brief ノード(枝)の参照回数を減らす．
  void
  dec_ref(
    ZddEdge edge ///< [in] 対象の枝
  );

  /// @brief CAP 演算を行う．
  ZddEdge
  cap_step(
    ZddEdge left,
    ZddEdge right
  );

  /// @brief CUP 演算を行う．
  ZddEdge
  cup_step(
    ZddEdge left,
    ZddEdge right
  );

  /// @brief DIFF 演算を行う．
  ZddEdge
  diff_step(
    ZddEdge left,
    ZddEdge right
  );

  /// @brief PRODUCT 演算を行う．
  ZddEdge
  product_step(
    ZddEdge left,
    ZddEdge right
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
  ZddNode** mTable{nullptr};

  // 格納されているノード数
  SizeType mNodeNum{0};

  // テーブルを拡張する目安
  SizeType mNextLimit;

  // ガーベージノード数
  SizeType mGarbageNum{0};

  // GC を起こすしきい値
  SizeType mGcLimit{1024};

  // GC の許可フラグ
  bool mGcEnable;

  // CAP用のテーブル
  unordered_map<Apply2Key, ZddEdge> mCapTable;

  // CUP用のテーブル
  unordered_map<Apply2Key, ZddEdge> mCupTable;

  // DIFF用のテーブル
  unordered_map<Apply2Key, ZddEdge> mDiffTable;

  // PRODUCT用のテーブル
  unordered_map<Apply2Key, ZddEdge> mProductTable;

};

END_NAMESPACE_YM_ZDD

#endif // ZDDMGRIMPL_H
