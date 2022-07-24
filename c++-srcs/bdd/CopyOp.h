#ifndef COPYOP_H
#define COPYOP_H

/// @file CopyOp.h
/// @brief CopyOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "OpBase.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
/// @class CopyOp CopyOp.h "CopyOp.h"
/// @brief BDD のコピーを行う演算
//////////////////////////////////////////////////////////////////////
class CopyOp :
  public OpBase
{
public:

  /// @brief コンストラクタ
  CopyOp(
    BddMgrImpl* mgr ///< [in] マネージャ
  ) : OpBase{mgr}
  {
  }

  /// @brief デストラクタ
  ~CopyOp() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief コピーする．
  BddEdge
  copy_step(
    BddEdge edge ///< [in] 枝
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 演算結果テーブル
  unordered_map<BddNode*, BddNode*> mTable;

};

END_NAMESPACE_YM_BDD

#endif // COPYOP_H
