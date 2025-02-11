#ifndef ONELEVEL0KERNEL_H
#define ONELEVEL0KERNEL_H

/// @file OneLevel0Kernel.h
/// @brief OneLevel0Kernel のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/SopCover.h"


BEGIN_NAMESPACE_YM_SOP

//////////////////////////////////////////////////////////////////////
/// @class OneLevel0Kernel OneLevel0Kernel.h "OneLevel0Kernel.h"
/// @brief 'one_level0_kernel' を求める Divisor 型のクラス
//////////////////////////////////////////////////////////////////////
class OneLevel0Kernel
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

END_NAMESPACE_YM_SOP

#endif // ONELEVEL0KERNEL_H
