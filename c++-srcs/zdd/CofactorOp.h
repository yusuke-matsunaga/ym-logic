#ifndef COFACTOROP_H
#define COFACTOROP_H

/// @file CofactorOp.h
/// @brief CofactorOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "OpBase.h"
#include "ZddEdge.h"


BEGIN_NAMESPACE_YM_ZDD

//////////////////////////////////////////////////////////////////////
/// @class CofactorOp CofactorOp.h "CofactorOp.h"
/// @brief コファクターを計算するためのクラス
//////////////////////////////////////////////////////////////////////
class CofactorOp :
  public OpBase
{
public:

  /// @brief コンストラクタ
  CofactorOp(
    ZddMgrImpl* mgr, ///< [in] マネージャ
    SizeType var,    ///< [in] 変数
    bool phase       ///< [in] 極性
  ) : OpBase{mgr},
      mIndex{var},
      mPhase{phase}
  {
  }

  /// @brief デストラクタ
  ~CofactorOp() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief コファクターを計算する．
  ZddEdge
  op_step(
    ZddEdge edge ///< [in] 枝
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
  unordered_map<ZddEdge, ZddEdge> mTable;

};

END_NAMESPACE_YM_ZDD

#endif // COFACTOROP_H
