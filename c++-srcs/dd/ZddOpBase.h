#ifndef ZDDOPBASE_H
#define ZDDOPBASE_H

/// @file ZddOpBase.h
/// @brief ZddOpBase のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "ZddMgrImpl.h"


BEGIN_NAMESPACE_YM_DD

class ZddMgrImpl;

//////////////////////////////////////////////////////////////////////
/// @class ZddOpBase ZddOpBase.h "ZddOpBase.h"
/// @brief BDD用の演算クラスの基底クラス
///
/// いわゆる ZddMgrImpl の delegate クラス
//////////////////////////////////////////////////////////////////////
class ZddOpBase
{
public:

  /// @brief コンストラクタ
  ZddOpBase(
    ZddMgrImpl& mgr ///< [in] マネージャ
  ) : mMgr{mgr}
  {
  }

  /// @brief デストラクタ
  ~ZddOpBase() = default;


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief マネージャを返す．
  ZddMgrImpl&
  mgr()
  {
    return mMgr;
  }

  /// @brief ノードを作る．
  DdEdge
  new_node(
    SizeType index, ///< [in] インデックス
    DdEdge edge0,  ///< [in] 0枝
    DdEdge edge1   ///< [in] 1枝
  )
  {
    return mMgr.new_node(index, edge0, edge1);
  }

  /// @brief 先頭の変数で分解する．
  static
  SizeType
  decomp(
    DdEdge left,
    DdEdge right,
    DdEdge& left0,
    DdEdge& left1,
    DdEdge& right0,
    DdEdge& right1
  )
  {
    return ZddMgrImpl::decomp(left, right, left0, left1, right0, right1);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // マネージャ
  ZddMgrImpl& mMgr;

};

END_NAMESPACE_YM_DD

#endif // ZDDOPBASE_H
