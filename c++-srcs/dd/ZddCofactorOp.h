#ifndef ZDDCOFACTOROP_H
#define ZDDCOFACTOROP_H

/// @file ZddCofactorOp.h
/// @brief ZddCofactorOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "ZddOpBase.h"
#include "dd/DdEdge.h"


BEGIN_NAMESPACE_YM_DD

//////////////////////////////////////////////////////////////////////
/// @class ZddCofactorOp ZddCofactorOp.h "ZddCofactorOp.h"
/// @brief コファクターを計算するためのクラス
//////////////////////////////////////////////////////////////////////
class ZddCofactorOp :
  public ZddOpBase
{
public:

  /// @brief コンストラクタ
  ZddCofactorOp(
    ZddMgrImpl& mgr, ///< [in] マネージャ
    SizeType var,    ///< [in] 変数
    bool phase       ///< [in] 極性
  ) : ZddOpBase{mgr},
      mIndex{var},
      mPhase{phase}
  {
  }

  /// @brief デストラクタ
  ~ZddCofactorOp() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief コファクターを計算する．
  DdEdge
  cofactor_step(
    DdEdge edge ///< [in] 枝
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数
  SizeType mIndex;

  // 極性
  bool mPhase;

  // 演算結果テーブル
  unordered_map<DdEdge, DdEdge> mTable;

};

END_NAMESPACE_YM_DD

#endif // ZDD_COFACTOROP_H
