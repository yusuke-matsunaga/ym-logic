#ifndef ZDDDIFFOP_H
#define ZDDDIFFOP_H

/// @file ZddDiffOp.h
/// @brief ZddDiffOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ZddOpBase.h"
#include "DdEdge.h"
#include "Apply2Key.h"


BEGIN_NAMESPACE_YM_DD

//////////////////////////////////////////////////////////////////////
/// @class ZddDiffOp ZddDiffOp.h "ZddDiffOp.h"
/// @brief DIFF 演算を行うクラス
//////////////////////////////////////////////////////////////////////
class ZddDiffOp :
  public ZddOpBase
{
public:

  /// @brief コンストラクタ
  ZddDiffOp(
    ZddMgrImpl* mgr ///< [in] マネージャ
  ) : ZddOpBase{mgr}
  {
  }

  /// @brief デストラクタ
  ~ZddDiffOp() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief DIFF演算を行う処理
  DdEdge
  diff_step(
    DdEdge left,
    DdEdge right
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief left が 1 だった時の処理
  DdEdge
  diff_step2(
    DdEdge right
  );

  /// @brief right が 1 だった時の処理
  DdEdge
  diff_step3(
    DdEdge left
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 演算結果テーブル
  std::unordered_map<Apply2Key, DdEdge> mTable;

};

END_NAMESPACE_YM_DD

#endif // ZDDDIFFOP_H
