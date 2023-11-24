#ifndef ZDDINVOP_H
#define ZDDINVOP_H

/// @file ZddInvOp.h
/// @brief ZddInvOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ZddOpBase.h"


BEGIN_NAMESPACE_YM_DD

//////////////////////////////////////////////////////////////////////
/// @class ZddInvOp ZddInvOp.h "ZddInvOp.h"
/// @brief ZDD の反転(補集合演算)を行う演算
//////////////////////////////////////////////////////////////////////
class ZddInvOp :
  public ZddOpBase
{
public:

  /// @brief コンストラクタ
  ZddInvOp(
    ZddMgrImpl& mgr ///< [in] マネージャ
  ) : ZddOpBase{mgr}
  {
  }

  /// @brief デストラクタ
  ~ZddInvOp() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 反転する．
  DdEdge
  inv_step(
    DdEdge edge ///< [in] 枝
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 演算結果テーブル
  unordered_map<const DdNode*, const DdNode*> mTable;

};

END_NAMESPACE_YM_DD

#endif // INVOP_H
