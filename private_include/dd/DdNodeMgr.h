#ifndef DDNODEMGR_H
#define DDNODEMGR_H

/// @file DdNodeMgr.h
/// @brief DdNodeMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "dd/DdNode.h"
#include "dd/DdEdge.h"


BEGIN_NAMESPACE_YM_DD

class DdNodeTable;

//////////////////////////////////////////////////////////////////////
/// @class DdNodeMgr DdNodeMgr.h "DdNodeMgr.h"
/// @brief DdNode の管理を行うクラス
///
/// BDD と ZDD のノードの縮約ルールが異なるのでここでは縮約は行わない．
/// index, edge0, edge1 が等しいノードを共有する処理だけを行う．
/// また，各ノードは参照回数を持っており，参照回数が 0 のノードは
/// 必要に応じて再利用される(GC: ガーベージコレクション)．
/// GC は明示的に行うこともできるが deactivate() をトリガーとして
/// 自動的に行われることもある．
//////////////////////////////////////////////////////////////////////
class DdNodeMgr
{
public:

  /// @brief コンストラクタ
  DdNodeMgr();

  /// @brief デストラクタ
  virtual
  ~DdNodeMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 変数の数を返す．
  SizeType
  variable_num() const;

  /// @brief 新しい変数テーブルを追加する．
  /// @return 変数番号を返す．
  SizeType
  new_variable();

  /// @brief 変数番号からインデックスを返す．
  SizeType
  varid_to_index(
    SizeType varid
  ) const;

  /// @brief インデックスから変数番号を返す．
  SizeType
  index_to_varid(
    SizeType index ///< [in] インデックス
  ) const;

  /// @brief ノードを作る．
  const DdNode*
  new_node(
    SizeType index, ///< [in] インデックス
    DdEdge edge0,   ///< [in] 0枝
    DdEdge edge1    ///< [in] 1枝
  );

  /// @brief ノード(枝)の参照回数を増やす．
  void
  activate(
    DdEdge edge ///< [in] 対象の枝
  );

  /// @brief ノード(枝)の参照回数を減らす．
  void
  deactivate(
    DdEdge edge ///< [in] 対象の枝
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


public:
  //////////////////////////////////////////////////////////////////////
  // 自身の参照回数管理用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 自身の参照回数を増やす．
  void
  inc()
  {
    ++ mRefCount;
  }

  /// @brief 自身の参照回数を減らす．
  ///
  /// 参照回数がゼロになったら破壊される．
  void
  dec()
  {
    -- mRefCount;
    if ( mRefCount == 0 ) {
      this->~DdNodeMgr();
    }
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノード(枝)の参照回数を増やす．
  void
  inc_ref(
    DdEdge edge ///< [in] 対象の枝
  );

  /// @brief ノード(枝)の参照回数を減らす．
  void
  dec_ref(
    DdEdge edge ///< [in] 対象の枝
  );

  /// @brief garbage_collection() が呼ばれた後に呼び出される関数
  virtual
  void
  after_gc();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 参照回数
  SizeType mRefCount{0};

#if 0
  // 表のサイズ
  SizeType mSize{0};

  // ハッシュ用のモジュロサイズ
  // 素数になるように調整されている．
  SizeType mHashSize{0};

  // 表の本体
  DdNode** mTable{nullptr};

  // 格納されているノード数
  SizeType mNodeNum{0};

  // テーブルを拡張する目安
  SizeType mNextLimit;
#else

  // 変数番号をキーにしてインデックスを格納する配列
  vector<SizeType> mIndexArray;

  // インデックスごとのテーブル配列
  vector<DdNodeTable*> mTableArray;
#endif

  // 格納されているノード数
  SizeType mNodeNum{0};

  // ガーベージノード数
  SizeType mGarbageNum{0};

  // GC を起こすしきい値
  SizeType mGcLimit{1024};

  // GC の許可フラグ
  bool mGcEnable;

};

END_NAMESPACE_YM_DD

#endif // BDDNODEMGR_H
