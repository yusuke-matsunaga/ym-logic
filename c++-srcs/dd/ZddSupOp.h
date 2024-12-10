#ifndef ZDDSUPOP_H
#define ZDDSUPOP_H

/// @file ZddSupOp.h
/// @brief ZddSupOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "ZddOpBase.h"


BEGIN_NAMESPACE_YM_DD

//////////////////////////////////////////////////////////////////////
/// @class ZddSupOp ZddSupOp.h "ZddSupOp.h"
/// @brief サポートを表すZDDを求める演算
//////////////////////////////////////////////////////////////////////
class ZddSupOp :
  public ZddOpBase
{
public:

  /// @brief コンストラクタ
  ZddSupOp(
    ZddMgrImpl* mgr ///< [in] マネージャ
  ) : ZddOpBase{mgr}
  {
  }

  /// @brief デストラクタ
  ~ZddSupOp() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief サポートを表すZDDを返す．
  DdEdge
  get_step(
    DdEdge edge ///< [in] 枝
  );

  /// @brief サポートのユニオンを求める．
  DdEdge
  cup_step(
    DdEdge edge0,
    DdEdge edge1
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 演算結果テーブル
  unordered_map<const DdNode*, DdEdge> mTable;

};

END_NAMESPACE_YM_DD

#endif // ZDDSUPOP_H
