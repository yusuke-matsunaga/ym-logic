#ifndef BDDONEOP_H
#define BDDONEOP_H

/// @file BddOneOp.h
/// @brief Bdd_OneOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "BddOpBase.h"


BEGIN_NAMESPACE_YM_DD

//////////////////////////////////////////////////////////////////////
/// @class BddOneOp BddOneOp.h "BddOneOp.h"
/// @brief 1に至るパスを求める演算
//////////////////////////////////////////////////////////////////////
class BddOneOp :
  public BddOpBase
{
public:

  /// @brief コンストラクタ
  BddOneOp(
    BddMgrImpl* mgr ///< [in] マネージャ
  ) : BddOpBase{mgr}
  {
  }

  /// @brief デストラクタ
  ~BddOneOp() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 1に至るパスを求める．
  DdEdge
  op_step(
    DdEdge edge ///< [in] 枝
  );

};

END_NAMESPACE_YM_DD

#endif // BDD_ONEOP_H
