#ifndef DIVOP_H
#define DIVOP_H

/// @file DivOp.h
/// @brief DivOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "SopMgr.h"
#include "SopBlock.h"


BEGIN_NAMESPACE_YM_SOP

//////////////////////////////////////////////////////////////////////
/// @class DivOp DivOp.h "ym/DivOp.h"
/// @brief 除算を行うクラス
//////////////////////////////////////////////////////////////////////
class DivOp :
  public SopMgr
{
public:

  /// @brief コンストラクタ
  DivOp(
    SizeType variable_num ///< [in] 変数の数
  ) : SopMgr{variable_num}
  {
  }

  /// @brief デストラクタ
  ~DivOp() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief カバーの代数的除算を行う．
  SopBlock
  operator()(
    const SopBlockRef& block1, ///< [in] 被除数のビットベクタ
    const SopBlockRef& block2  ///< [in] 除数のビットベクタ
  );

  /// @brief カバーをキューブで割る．
  ///
  /// 上の block2 のキューブ数を1にしても同じことが行えるが，
  /// こちらのほうが効率がよい．
  SopBlock
  operator()(
    const SopBlockRef& block1, ///< [in] 被除数のビットベクタ
    const SopBitVect& bv2      ///< [in] 除数のキューブのビットベクタ
  );

  /// @brief カバーをリテラルで割る．
  SopBlock
  operator()(
    const SopBlockRef& block1, ///< [in] 被除数のビットベクタ
    Literal lit                ///< [in] 除数のリテラル
  );

};

END_NAMESPACE_YM_SOP

#endif // DIVOP_H
