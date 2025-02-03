#ifndef CHECKCONTAINMENT_H
#define CHECKCONTAINMENT_H

/// @file CheckContainment.h
/// @brief CheckContainment のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "SopMgr.h"


BEGIN_NAMESPACE_YM_SOP

//////////////////////////////////////////////////////////////////////
/// @class CheckContainment CheckContainment.h "ym/CheckContainment.h"
/// @brief キューブの包含関係のチェックを行う．
//////////////////////////////////////////////////////////////////////
class CheckContainment :
  public SopMgr
{
public:

  /// @brief コンストラクタ
  CheckContainment(
    SizeType variable_num ///< [in] 変数の数
  ) : SopMgr{variable_num}
  {
  }

  /// @brief デストラクタ
  ~CheckContainment() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 一方のキューブが他方のキューブに含まれているか調べる．
  /// @return 1つめのキューブが2つめのキューブ に含まれていたら true を返す．
  bool
  operator()(
    SopBitVectConstIter src1_iter, ///< [in] 1つめのキューブを表すビットベクタ
    SopBitVectConstIter src2_iter  ///< [in] 2つめのキューブを表すビットベクタ
  );

};

END_NAMESPACE_YM_SOP

#endif // CHECKCONTAINMENT_H
