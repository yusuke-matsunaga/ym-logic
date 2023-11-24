#ifndef DDNODEMGR_H
#define DDNODEMGR_H

/// @file DdNodeMgr.h
/// @brief DdNodeMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "DdNode.h"
#include "DdEdge.h"


BEGIN_NAMESPACE_YM_DD

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

  /// @brief ノードを作る．
  const DdNode*
  new_node(
    SizeType index, ///< [in] インデックス
    DdEdge edge0,   ///< [in] 0枝
    DdEdge edge1    ///< [in] 1枝
  );

  /// @brief DD の構造をコピーする．
  ///
  /// edge は他のマネージャーに属すると仮定する．
  DdEdge
  copy(
    DdEdge edge ///< [in] 根の枝
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

  /// @brief 表を拡張する．
  void
  extend(
    SizeType req_size ///< [in] 要求サイズ
  );

  /// @brief garbage_collection() が呼ばれた後に呼び出される関数
  virtual
  void
  after_gc();


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
  DdNode** mTable{nullptr};

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

};

END_NAMESPACE_YM_DD

#endif // BDDNODEMGR_H
