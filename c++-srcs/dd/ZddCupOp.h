#ifndef ZDDCUPOP_H
#define ZDDCUPOP_H

/// @file ZddCupOp.h
/// @brief ZddCupOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ZddOpBase.h"
#include "DdEdge.h"
#include "Apply2Key.h"


BEGIN_NAMESPACE_YM_DD

//////////////////////////////////////////////////////////////////////
/// @class ZddCupOp ZddCupOp.h "ZddCupOp.h"
/// @brief CUP 演算を行うクラス
//////////////////////////////////////////////////////////////////////
class ZddCupOp :
  public ZddOpBase
{
public:

  /// @brief コンストラクタ
  ZddCupOp(
    ZddMgrImpl* mgr ///< [in] マネージャ
  ) : ZddOpBase{mgr}
  {
  }

  /// @brief デストラクタ
  ~ZddCupOp() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief CUP演算を行う処理
  DdEdge
  cup_step(
    DdEdge left,
    DdEdge right
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 一方の枝が 1 だった時の処理
  DdEdge
  cup_step2(
    DdEdge left
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 演算結果テーブル
  unordered_map<Apply2Key, DdEdge> mTable;

};

END_NAMESPACE_YM_DD

#endif // ZDDCUPOP_H
