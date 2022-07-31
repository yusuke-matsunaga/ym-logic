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
#include "Apply3Key.h"


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

  /// @brief コピーする．
  BddEdge
  copy(
    const Bdd& src
  );

  /// @brief AND 演算を行う．
  BddEdge
  and_op(
    BddEdge left,
    BddEdge right
  )
  {
    return and_step(left, right);
  }

  /// @brief OR 演算を行う．
  BddEdge
  or_op(
    BddEdge left,
    BddEdge right
  )
  {
    // DeMorgan の法則
    return ~and_op(~left, ~right);
  }

  /// @brief XOR 演算を行う．
  BddEdge
  xor_op(
    BddEdge left,
    BddEdge right
  )
  {
    return xor_step(left, right);
  }

  /// @brief ITE 演算を行う．
  BddEdge
  ite_op(
    BddEdge e0,
    BddEdge e1,
    BddEdge e2
  )
  {
    return ite_step(e0, e1, e2);
  }

  /// @brief compose 演算を行う．
  BddEdge
  compose_op(
    BddEdge edge,
    SizeType index,
    BddEdge cedge
  );

  // @brief リテラル関数を作る．
  BddEdge
  make_literal(
    SizeType index ///< [in] インデックス
  )
  {
    return new_node(index, BddEdge::zero(), BddEdge::one());
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
  activate(
    BddEdge edge ///< [in] 対象の枝
  );

  /// @brief ノード(枝)の参照回数を減らす．
  void
  deactivate(
    BddEdge edge ///< [in] 対象の枝
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
    BddEdge edge ///< [in] 対象の枝
  );

  /// @brief ノード(枝)の参照回数を減らす．
  void
  dec_ref(
    BddEdge edge ///< [in] 対象の枝
  );

  /// @brief AND 演算を行う．
  BddEdge
  and_step(
    BddEdge left,
    BddEdge right
  );

  /// @brief OR 演算を行う．
  BddEdge
  or_step(
    BddEdge left,
    BddEdge right
  )
  {
    // DeMorgan の法則
    return ~and_step(~left, ~right);
  }

  /// @brief XOR 演算を行う．
  BddEdge
  xor_step(
    BddEdge left,
    BddEdge right
  );

  /// @brief ITE 演算を行う．
  BddEdge
  ite_step(
    BddEdge e0,
    BddEdge e1,
    BddEdge e2
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
  SizeType mGarbageNum{0};

  // GC を起こすしきい値
  SizeType mGcLimit{1024};

  // GC の許可フラグ
  bool mGcEnable;

  // ITE用のテーブル
  unordered_map<Apply3Key, BddEdge> mIteTable;

};

END_NAMESPACE_YM_BDD

#endif // BDDMGRIMPL_H
