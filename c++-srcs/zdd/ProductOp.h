#ifndef PRODUCTOP_H
#define PRODUCTOP_H

/// @file ProductOp.h
/// @brief ProductOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "Apply2Key.h"
#include "OpBase.h"


BEGIN_NAMESPACE_YM_ZDD

//////////////////////////////////////////////////////////////////////
/// @class ProductOp ProductOp.h "ProductOp.h"
/// @brief PRODUCT 演算を行うクラス
//////////////////////////////////////////////////////////////////////
class ProductOp :
  public OpBase
{
public:

  /// @brief コンストラクタ
  ProductOp(
    ZddMgrImpl* mgr ///< [in] マネージャ
  ) : OpBase{mgr}
  {
  }

  /// @brief デストラクタ
  ~ProductOp() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief PRODUCT 演算を行う．
  ZddEdge
  op_step(
    ZddEdge left,
    ZddEdge right
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 演算結果テーブル
  unordered_map<Apply2Key, ZddEdge> mTable;

};

END_NAMESPACE_YM_ZDD

#endif // PRODUCTOP_H
