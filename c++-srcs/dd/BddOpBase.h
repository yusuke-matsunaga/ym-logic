#ifndef BDDOPBASE_H
#define BDDOPBASE_H

/// @file BddOpBase.h
/// @brief BddOpBase のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "BddMgrImpl.h"


BEGIN_NAMESPACE_YM_DD

class BddMgrImpl;

//////////////////////////////////////////////////////////////////////
/// @class BddOpBase BddOpBase.h "BddOpBase.h"
/// @brief BDD用の演算クラスの基底クラス
///
/// いわゆる BddMgrImpl の delegate クラス
//////////////////////////////////////////////////////////////////////
class BddOpBase
{
public:

  /// @brief コンストラクタ
  BddOpBase(
    BddMgrImpl& mgr ///< [in] マネージャ
  ) : mMgr{mgr}
  {
  }

  /// @brief デストラクタ
  ~BddOpBase() = default;


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief マネージャを返す．
  BddMgrImpl&
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

  /// @brief トップの変数で分解する．
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
    return BddMgrImpl::decomp(left, right, left0, left1, right0, right1);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // マネージャ
  BddMgrImpl& mMgr;

};

END_NAMESPACE_YM_DD

#endif // BDDOPBASE_H
