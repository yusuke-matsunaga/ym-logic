#ifndef ROOTHOLDER_H
#define ROOTHOLDER_H

/// @file RootHolder.h
/// @brief RootHolder のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ZddMgrImpl.h"
#include "ZddEdge.h"


BEGIN_NAMESPACE_YM_ZDD

//////////////////////////////////////////////////////////////////////
/// @class RootHolder RootHolder.h "RootHolder.h"
/// @brief 再帰呼び出しの内部で参照回数を管理するためのクラス
///
// コンストラクタとデストラクタのみのクラス
//////////////////////////////////////////////////////////////////////
class RootHolder
{
public:

  /// @brief コンストラクタ
  RootHolder(
    ZddMgrImpl* mMgr, ///< [in] マネージャ
    ZddEdge edge      ///< [in] 枝
  );

  /// @brief デストラクタ
  ~RootHolder();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // マネージャ
  ZddMgrImpl* mMgr;

  // 枝
  ZddEdge mEdge;

};

END_NAMESPACE_YM_ZDD

#endif // ROOTHOLDER_H
