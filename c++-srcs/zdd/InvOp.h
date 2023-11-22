#ifndef INVOP_H
#define INVOP_H

/// @file InvOp.h
/// @brief InvOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "OpBase.h"


BEGIN_NAMESPACE_YM_ZDD

//////////////////////////////////////////////////////////////////////
/// @class InvOp InvOp.h "InvOp.h"
/// @brief ZDD の反転(補集合演算)を行う演算
//////////////////////////////////////////////////////////////////////
class InvOp :
  public OpBase
{
public:

  /// @brief コンストラクタ
  InvOp(
    ZddMgrImpl* mgr ///< [in] マネージャ
  ) : OpBase{mgr}
  {
  }

  /// @brief デストラクタ
  ~InvOp() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 反転する．
  ZddEdge
  inv_step(
    ZddEdge edge ///< [in] 枝
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 演算結果テーブル
  unordered_map<ZddNode*, ZddNode*> mTable;

};

END_NAMESPACE_YM_ZDD

#endif // INVOP_H
