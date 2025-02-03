#ifndef MULOP_H
#define MULOP_H

/// @file MulOp.h
/// @brief MulOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "SopMgr.h"
#include "SopBlock.h"


BEGIN_NAMESPACE_YM_SOP

//////////////////////////////////////////////////////////////////////
/// @class MulOp MulOp.h "ym/MulOp.h"
/// @brief カバーの演算を行う実装クラス
//////////////////////////////////////////////////////////////////////
class MulOp :
  public SopMgr
{
public:

  /// @brief コンストラクタ
  MulOp(
    SizeType variable_num ///< [in] 変数の数
  ) : SopMgr{variable_num}
  {
  }

  /// @brief デストラクタ
  ~MulOp() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 2つのカバーの論理積を計算する．
  SopBlock
  operator()(
    const SopBlockRef& block1, ///< [in] 被乗数のビットベクタ
    const SopBlockRef& block2  ///< [in] 乗数のビットベクタ
  );

  /// @brief カバーとキューブの論理積を計算する．
  SopBlock
  operator()(
    const SopBlockRef& block1, ///< [in] 被乗数のビットベクタ
    const SopBitVect& bv2      ///< [in] 乗数のキューブのビットベクタ
  )
  {
    return operator()(block1, SopBlockRef{1, bv2});
  }

  /// @brief カバーとリテラルとの論理積を計算する．
  SopBlock
  operator()(
    const SopBlockRef& block1, ///< [in] 被乗数のビットベクタ
    Literal lit                ///< [in] 乗数のリテラル
  );

};

END_NAMESPACE_YM_SOP

#endif // MULOP_H
