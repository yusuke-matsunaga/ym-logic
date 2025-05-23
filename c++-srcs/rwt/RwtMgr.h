#ifndef RWTMGR_H
#define RWTMGR_H

/// @file RwtMgr.h
/// @brief RwtMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/aig.h"


BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
/// @class RwtMgr RwtMgr.h "RwtMgr.h"
/// @brief
//////////////////////////////////////////////////////////////////////
class RwtMgr
{
public:

  /// @brief コンストラクタ
  RwtMgr();

  /// @brief デストラクタ
  ~RwtMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief パタンの置き換えを行う．
  void
  rewrite(
    AigMgrImpl* mgr ///< [in] AIGマネージャ
  );

private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////


};

END_NAMESPACE_YM_AIG

#endif // RWTMGR_H
