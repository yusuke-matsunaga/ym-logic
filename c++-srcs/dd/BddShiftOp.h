#ifndef BDDSHIFTOP_H
#define BDDSHIFTOP_H

/// @file BddShiftOp.h
/// @brief BddShiftOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "BddOpBase.h"


BEGIN_NAMESPACE_YM_DD

//////////////////////////////////////////////////////////////////////
/// @class BddShiftOp BddShiftOp.h "BddShiftOp.h"
/// @brief 変数シフト演算を行うクラス
//////////////////////////////////////////////////////////////////////
class BddShiftOp :
  public BddOpBase
{
public:

  /// @brief コンストラクタ
  BddShiftOp(
    BddMgrImpl& mgr, ///< [in] マネージャ
    SizeType var     ///< [in] 移動元の変数
  ) : BddOpBase{mgr},
      mVar{var}
  {
  }

  /// @brief デストラクタ
  ~BddShiftOp() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief シフト演算の結果表すBDDを返す．
  DdEdge
  shift_step(
    DdEdge edge ///< [in] 枝
  );

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象の変数
  SizeType mVar;

  // 演算結果テーブル
  unordered_map<const DdNode*, DdEdge> mTable;

};

END_NAMESPACE_YM_DD

#endif // BDDSHIFTOP_H
