#ifndef ONEOP_H
#define ONEOP_H

/// @file OneOp.h
/// @brief OneOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "OpBase.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
/// @class OneOp OneOp.h "OneOp.h"
/// @brief 1に至るパスを求める演算
//////////////////////////////////////////////////////////////////////
class OneOp :
  public OpBase
{
public:

  /// @brief コンストラクタ
  OneOp(
    BddMgrImpl* mgr ///< [in] マネージャ
  ) : OpBase{mgr}
  {
  }

  /// @brief デストラクタ
  ~OneOp() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 1に至るパスを求める．
  BddEdge
  op_step(
    BddEdge edge ///< [in] 枝
  );

};

END_NAMESPACE_YM_BDD

#endif // ONEOP_H
