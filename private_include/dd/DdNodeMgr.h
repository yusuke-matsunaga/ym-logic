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
#include "dd/DdNodeTable.h"


BEGIN_NAMESPACE_YM_DD

//////////////////////////////////////////////////////////////////////
/// @class DdNodeMgr DdNodeMgr.h "DdNodeMgr.h"
/// @brief DdNode の管理を行うクラス
///
/// BDD と ZDD のノードの縮約ルールが異なるのでここでは縮約は行わない．
/// level, edge0, edge1 が等しいノードを共有する処理だけを行う．
/// ただし実際の共有処理は DdNodeTable が行う．
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

  /// @brief 変数番号からレベルを返す．
  SizeType
  varid_to_level(
    SizeType varid
  ) const
  {
    if ( varid >= mLevelArray.size() ) {
      throw std::out_of_range{"varid is out of range"};
    }
    return mLevelArray[varid];
  }

  /// @brief レベルから変数番号を返す．
  SizeType
  level_to_varid(
    SizeType level ///< [in] レベル
  ) const
  {
    _check_level(level);
    return mTableArray[level]->varid();
  }

  /// @brief ノードを作る．
  const DdNode*
  new_node(
    SizeType level, ///< [in] レベル
    DdEdge edge0,   ///< [in] 0枝
    DdEdge edge1    ///< [in] 1枝
  );

  /// @brief ノードを登録する．
  void
  reg_node(
    DdNode* node
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

  /// @brief 隣り合うレベルの変数を交換する．
  ///
  /// 具体的には level と (level + 1) を交換する．
  void
  swap_level(
    SizeType level
  )
  {
    auto level2 = level + 1;
    std::swap(mTableArray[level], mTableArray[level2]);
    auto table1 = mTableArray[level];
    table1->chg_level(level);
    auto table2 = mTableArray[level2];
    table2->chg_level(level2);

    auto varid1 = level_to_varid(level);
    auto varid2 = level_to_varid(level2);
    std::swap(mLevelArray[varid1], mLevelArray[varid2]);
  }

  /// @brief 保持しているノードに対して処理を行う．
  void
  scan(
    SizeType level,
    std::function<bool(DdNode*)> func
  )
  {
    _check_level(level);
    mTableArray[level]->scan(func);
  }

  /// @brief ガーベージコレクションを行う．
  void
  garbage_collection();

  /// @brief ガーベージコレクションを行う．
  void
  garbage_collection(
    SizeType level ///< [in] レベル
  )
  {
    _check_level(level);
    auto n = mTableArray[level]->garbage_collection();
    mNodeNum -= n;
    mGarbageNum -= n;
  }

  /// @brief ノード数を返す．
  SizeType
  node_num() const
  {
    return mNodeNum;
  }

  /// @brief 特定のレベルのノード数を返す．
  SizeType
  node_num(
    SizeType level ///< [in] レベル
  ) const
  {
    _check_level(level);
    return mTableArray[level]->node_num();
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

  /// @brief レベルが適正か調べる．
  void
  _check_level(
    SizeType level
  ) const
  {
    if ( level >= mTableArray.size() ) {
      abort();
      throw std::out_of_range{"level is out of range"};
    }
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 参照回数
  SizeType mRefCount{0};

  // 変数番号をキーにしてレベルを格納する配列
  vector<SizeType> mLevelArray;

  // レベルごとのテーブル配列
  vector<DdNodeTable*> mTableArray;

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
