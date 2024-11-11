#ifndef ZDDCOPYOP_H
#define ZDDCOPYOP_H

/// @file ZddCopyOp.h
/// @brief ZddCopyOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "ym/Zdd.h"
#include "ym/ZddItem.h"


BEGIN_NAMESPACE_YM_DD

class ZddMgrImpl;

//////////////////////////////////////////////////////////////////////
/// @class ZddCopyOp ZddCopyOp.h "ZddCopyOp.h"
/// @brief ZDD のコピーを行う演算
///
/// 対象と自分のマネージャが異なるので変数順も異なる可能性がある．
//////////////////////////////////////////////////////////////////////
class ZddCopyOp
{
public:

  /// @brief コンストラクタ
  ZddCopyOp(
    ZddMgrImpl& mgr,                   ///< [in] マネージャ
    const vector<ZddItem>& item_list,  ///< [in] 変数リスト
    const vector<SizeType>& index_list ///< [in] インデックスのリスト
  ) : mMgr{mgr},
      mItemList{item_list},
      mIndexList{index_list}
  {
  }

  /// @brief デストラクタ
  ~ZddCopyOp() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief コピーする．
  DdEdge
  copy_step(
    const Zdd& zdd,
    SizeType pos
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // Zddマネージャ
  ZddMgrImpl& mMgr;

  // 要素リスト
  vector<ZddItem> mItemList;

  // インデックスのリスト
  vector<SizeType> mIndexList;

  // 演算結果テーブル
  unordered_map<Zdd, DdEdge> mTable;

};

END_NAMESPACE_YM_DD

#endif // ZDDCOPYOP_H
