#ifndef MULOP_H
#define MULOP_H

/// @file MulOp.h
/// @brief MulOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
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
  void
  operator()(
    SopBlock& dst,        ///< [in] 結果を格納するブロック
    const SopBlock& src1, ///< [in] 1つめのカバー
    const SopBlock& src2  ///< [in] 2つめのカバー
  );

  /// @brief カバーとキューブの論理積を計算する．
  /// @return 結果のカバーを返す．
  void
  operator()(
    SopBlock& dst,         ///< [in] 結果を格納するブロック
    const SopBlock& src1,  ///< [in] 1つめのカバー
    const SopBitVect* cube ///< [in] 対象のキューブ
  );

  /// @brief カバーとリテラルとの論理積を計算する．
  void
  operator()(
    SopBlock& dst,        ///< [in] 結果を格納するブロック
    const SopBlock& src1, ///< [in] 1つめのカバー
    Literal lit           ///< [in] 対象のリテラル
  );

};

END_NAMESPACE_YM_SOP

#endif // MULOP_H
