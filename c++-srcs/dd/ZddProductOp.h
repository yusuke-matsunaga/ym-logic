#ifndef ZDDPRODUCTOP_H
#define ZDDPRODUCTOP_H

/// @file ZddProductOp.h
/// @brief ZddProductOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "Apply2Key.h"
#include "ZddOpBase.h"
#include "ZddCupOp.h"


BEGIN_NAMESPACE_YM_DD

//////////////////////////////////////////////////////////////////////
/// @class ZddProductOp ZddProductOp.h "ZddProductOp.h"
/// @brief PRODUCT 演算を行うクラス
//////////////////////////////////////////////////////////////////////
class ZddProductOp :
  public ZddOpBase
{
public:

  /// @brief コンストラクタ
  ZddProductOp(
    ZddMgrImpl& mgr ///< [in] マネージャ
  ) : ZddOpBase{mgr},
      mCupOp{mgr}
  {
  }

  /// @brief デストラクタ
  ~ZddProductOp() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief PRODUCT 演算を行う．
  DdEdge
  prod_step(
    DdEdge left,
    DdEdge right
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // CUP 演算
  ZddCupOp mCupOp;

  // 演算結果テーブル
  unordered_map<Apply2Key, DdEdge> mTable;

};

END_NAMESPACE_YM_DD

#endif // PRODUCTOP_H
