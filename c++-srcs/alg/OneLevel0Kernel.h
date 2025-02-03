#ifndef ONELEVEL0KERNEL_H
#define ONELEVEL0KERNEL_H

/// @file OneLevel0Kernel.h
/// @brief OneLevel0Kernel のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.

#include "Divisor.h"


BEGIN_NAMESPACE_YM_SOP

//////////////////////////////////////////////////////////////////////
/// @class OneLevel0Kernel OneLevel0Kernel.h "OneLevel0Kernel.h"
/// @brief 'one_level0_kernel' を求める Divisor の派生クラス
//////////////////////////////////////////////////////////////////////
class OneLevel0Kernel :
  public Divisor
{
public:

  /// @brief コンストラクタ
  OneLevel0Kernel();

  /// @brief デストラクタ
  ~OneLevel0Kernel();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 除数を求める．
  /// @return 除数を表す論理式を返す．
  SopCover
  operator()(
    const SopCover& f ///< [in] 対象の論理式
  ) const override;

};

END_NAMESPACE_YM_SOP

#endif // ONELEVEL0KERNEL_H
