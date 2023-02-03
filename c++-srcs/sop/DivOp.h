#ifndef DIVOP_H
#define DIVOP_H

/// @file DivOp.h
/// @brief DivOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
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
  void
  operator()(
    SopBlock& dst,        ///< [in] 結果を格納するブロック
    const SopBlock& cov1, ///< [in] 被除数
    const SopBlock& cov2  ///< [in] 除数
  );

  /// @brief カバーをキューブで割る．
  void
  operator()(
    SopBlock& dst,         ///< [in] 1つめのカバー
    const SopBlock& cov1,  ///< [in] 1つめのカバー
    const SopBitVect* cube ///< [in] 除数のキューブ
  );

  /// @brief カバーをリテラルで割る．
  void
  operator()(
    SopBlock& dst,        ///< [in] 結果を格納するブロック
    const SopBlock& cov1, ///< [in] 1つめのカバー
    Literal lit           ///< [in] 除数のリテラル
  );

};

END_NAMESPACE_YM_SOP

#endif // DIVOP_H
