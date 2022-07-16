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


BEGIN_NAMESPACE_YM_BDD

class BddNode;

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
    BddMgrImpl* mgr, ///< [in] マネージャ
    SizeType index,  ///< [in] インデックス
    bool inv         ///< [in] 極性
  ) : OpBase{mgr},
      mIndex{index},
      mInv{inv}
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
    BddEdge edge ///< [in] 枝
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 演算結果テーブル
  unordered_map<BddNode*, BddEdge> mTable;

  // コファクター用のインデックス
  SizeType mIndex;

  // コファクターの極性
  bool mInv;

};

END_NAMESPACE_YM_BDD

#endif // COFACTOROP_H
