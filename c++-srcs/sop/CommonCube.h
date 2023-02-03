#ifndef COMMONCUBEOP_H
#define COMMONCUBEOP_H

/// @file CommonCube.h
/// @brief CommonCube のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "SopMgr.h"
#include "SopBlock.h"


BEGIN_NAMESPACE_YM_SOP

//////////////////////////////////////////////////////////////////////
/// @class CommonCube CommonCube.h "ym/CommonCube.h"
/// @brief カバーの演算を行う実装クラス
//////////////////////////////////////////////////////////////////////
class CommonCube :
  public SopMgr
{
public:

  /// @brief コンストラクタ
  CommonCube(
    SizeType variable_num ///< [in] 変数の数
  ) : SopMgr{variable_num}
  {
  }

  /// @brief デストラクタ
  ~CommonCube() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 共通なキューブを求める．
  SopBitVect*
  operator()(
    const SopBlock& cov1 ///< [in] 被除数
  );

};

END_NAMESPACE_YM_SOP

#endif // COMMONCUBEOP_H
