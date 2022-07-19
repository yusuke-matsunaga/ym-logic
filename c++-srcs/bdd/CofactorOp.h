#ifndef COFACTOROP_H
#define COFACTOROP_H

/// @file CofactorOp.h
/// @brief CofactorOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bdd_nsdef.h"
#include "OpBase.h"
#include "BddEdge.h"
#include "Apply2Key.h"


BEGIN_NAMESPACE_YM_BDD

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
    BddMgrImpl* mgr ///< [in] マネージャ
  ) : OpBase{mgr}
  {
  }

  /// @brief デストラクタ
  ~CofactorOp() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief コファクターを計算する．
  BddEdge
  op_step(
    BddEdge edge,  ///< [in] 枝
    BddEdge cedge  ///< [in] 制約の枝
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 演算結果テーブル
  unordered_map<Apply2Key, BddEdge> mTable;

};

END_NAMESPACE_YM_BDD

#endif // COFACTOROP_H
