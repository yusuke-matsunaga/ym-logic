#ifndef COMPOP_H
#define COMPOP_H

/// @file CompOp.h
/// @brief CompOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "SopMgr.h"


BEGIN_NAMESPACE_YM_SOP

//////////////////////////////////////////////////////////////////////
/// @class CompOp CompOp.h "CompOp.h"
/// @brief カバーの比較を行うクラス
//////////////////////////////////////////////////////////////////////
class CompOp :
  public SopMgr
{
public:

  /// @brief コンストラクタ
  CompOp(
    SizeType variable_num ///< [in] 変数の数
  ) : SopMgr{variable_num}
  {
  }

  /// @brief デストラクタ
  ~CompOp() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief カバーの比較を行う．
  int
  operator()(
    const SopBlockRef& block1, ///< [in] 第1オペランドのビットベクタ
    const SopBlockRef& block2  ///< [in] 第2オペランドのビットベクタ
  );

};

END_NAMESPACE_YM_SOP

#endif // COMPOP_H
