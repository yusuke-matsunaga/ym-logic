#ifndef BESTKERNEL_H
#define BESTKERNEL_H

/// @file BestKernel.h
/// @brief BestKernel のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/SopCover.h"


BEGIN_NAMESPACE_YM_FACTOR

//////////////////////////////////////////////////////////////////////
/// @class BestKernel BestKernel.h "BestKernel.h"
/// @brief 'best_kernel' を求める Divisor 型のクラス
//////////////////////////////////////////////////////////////////////
class BestKernel
{
public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 除数を求める．
  /// @return 除数を表す論理式を返す．
  static
  SopCover
  divisor(
    const SopCover& f ///< [in] 対象の論理式
  );

};

END_NAMESPACE_YM_FACTOR

#endif // BESTKERNEL_H
