#ifndef BDDCOFACTOROP_H
#define BDDCOFACTOROP_H

/// @file BddCofactorOp.h
/// @brief BddCofactorOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "BddOpBase.h"
#include "DdEdge.h"
#include "Apply2Key.h"


BEGIN_NAMESPACE_YM_DD

//////////////////////////////////////////////////////////////////////
/// @class BddCofactorOp BddCofactorOp.h "BddCofactorOp.h"
/// @brief コファクターを計算するためのクラス
//////////////////////////////////////////////////////////////////////
class BddCofactorOp :
  public BddOpBase
{
public:

  /// @brief コンストラクタ
  BddCofactorOp(
    const BddMgrPtr& mgr ///< [in] マネージャ
  ) : BddOpBase{mgr}
  {
  }

  /// @brief デストラクタ
  ~BddCofactorOp() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief コファクターを計算する．
  DdEdge
  op_step(
    DdEdge edge,  ///< [in] 枝
    DdEdge cedge  ///< [in] 制約の枝
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 演算結果テーブル
  unordered_map<Apply2Key, DdEdge> mTable;

};

END_NAMESPACE_YM_DD

#endif // BDDCOFACTOROP_H
