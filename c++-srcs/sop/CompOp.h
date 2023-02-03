#ifndef COMPOP_H
#define COMPOP_H

/// @file CompOp.h
/// @brief CompOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
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
    const SopBlock& src1, ///< [in] オペランド1
    const SopBlock& src2  ///< [in] オペランド2
  );

};

END_NAMESPACE_YM_SOP

#endif // COMPOP_H
