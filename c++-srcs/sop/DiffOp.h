#ifndef DIFFOP_H
#define DIFFOP_H

/// @file DiffOp.h
/// @brief DiffOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "SopMgr.h"
#include "SopBlock.h"


BEGIN_NAMESPACE_YM_SOP

//////////////////////////////////////////////////////////////////////
/// @class DiffOp DiffOp.h "ym/DiffOp.h"
/// @brief カバーの演算を行う実装クラス
//////////////////////////////////////////////////////////////////////
class DiffOp :
  public SopMgr
{
public:

  /// @brief コンストラクタ
  DiffOp(
    SizeType variable_num ///< [in] 変数の数
  ) : SopMgr{variable_num}
  {
  }

  /// @brief デストラクタ
  ~DiffOp() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 2つのカバーの差分を計算する．
  ///
  /// カバーをキューブの集合とみなして集合差を計算する．
  SopBlock
  operator()(
    const SopBlockRef& block1, ///< [in] 1つめのカバーのビットベクタ
    const SopBlockRef& block2  ///< [in] 2つめのカバーのビットベクタ
  );

};

END_NAMESPACE_YM_SOP

#endif // DIFFOP_H
