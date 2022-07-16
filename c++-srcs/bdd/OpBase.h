#ifndef OPBASE_H
#define OPBASE_H

/// @file OpBase.h
/// @brief OpBase のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bdd_nsdef.h"
#include "BddMgrImpl.h"


BEGIN_NAMESPACE_YM_BDD

class BddMgrImpl;

//////////////////////////////////////////////////////////////////////
/// @class OpBase OpBase.h "OpBase.h"
/// @brief BDD用の演算クラスの基底クラス
///
/// いわゆる BddMgrImpl の delegate クラス
//////////////////////////////////////////////////////////////////////
class OpBase
{
public:

  /// @brief コンストラクタ
  OpBase(
    BddMgrImpl* mgr ///< [in] マネージャ
  ) : mMgr{mgr}
  {
  }

  /// @brief デストラクタ
  ~OpBase() = default;


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードを作る．
  BddEdge
  new_node(
    SizeType index, ///< [in] インデックス
    BddEdge edge0,  ///< [in] 0枝
    BddEdge edge1   ///< [in] 1枝
  )
  {
    return mMgr->new_node(index, edge0, edge1);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // マネージャ
  BddMgrImpl* mMgr;

};

END_NAMESPACE_YM_BDD

#endif // OPBASE_H
