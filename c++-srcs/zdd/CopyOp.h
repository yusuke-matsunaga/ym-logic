#ifndef COPYOP_H
#define COPYOP_H

/// @file CopyOp.h
/// @brief CopyOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "OpBase.h"


BEGIN_NAMESPACE_YM_ZDD

//////////////////////////////////////////////////////////////////////
/// @class CopyOp CopyOp.h "CopyOp.h"
/// @brief ZDD のコピーを行う演算
//////////////////////////////////////////////////////////////////////
class CopyOp :
  public OpBase
{
public:

  /// @brief コンストラクタ
  CopyOp(
    ZddMgrImpl* mgr ///< [in] マネージャ
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
  ZddEdge
  copy_step(
    ZddEdge edge ///< [in] 枝
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 演算結果テーブル
  unordered_map<ZddNode*, ZddNode*> mTable;

};

END_NAMESPACE_YM_ZDD

#endif // COPYOP_H
