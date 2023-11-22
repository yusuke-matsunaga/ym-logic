#ifndef OPBASE_H
#define OPBASE_H

/// @file OpBase.h
/// @brief OpBase のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "ZddMgrImpl.h"


BEGIN_NAMESPACE_YM_ZDD

class ZddMgrImpl;

//////////////////////////////////////////////////////////////////////
/// @class OpBase OpBase.h "OpBase.h"
/// @brief ZDD用の演算クラスの基底クラス
///
/// いわゆる ZddMgrImpl の delegate クラス
//////////////////////////////////////////////////////////////////////
class OpBase
{
public:

  /// @brief コンストラクタ
  OpBase(
    ZddMgrImpl* mgr ///< [in] マネージャ
  ) : mMgr{mgr}
  {
  }

  /// @brief デストラクタ
  ~OpBase() = default;


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief マネージャを返す．
  ZddMgrImpl*
  mgr()
  {
    return mMgr;
  }

  /// @brief ノードを作る．
  ZddEdge
  new_node(
    SizeType index, ///< [in] インデックス
    ZddEdge edge0,  ///< [in] 0枝
    ZddEdge edge1   ///< [in] 1枝
  )
  {
    return mMgr->new_node(index, edge0, edge1);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // マネージャ
  ZddMgrImpl* mMgr;

};

END_NAMESPACE_YM_ZDD

#endif // OPBASE_H
