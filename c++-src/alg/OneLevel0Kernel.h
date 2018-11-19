#ifndef ONELEVEL0KERNEL_H
#define ONELEVEL0KERNEL_H

/// @file OneLevel0Kernel.h
/// @brief OneLevel0Kernel のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.

#include "Divisor.h"


BEGIN_NAMESPACE_YM_LOGIC

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
  /// @param[in] f 対象の論理式
  /// @return 除数を表す論理式を返す．
  AlgCover
  operator()(const AlgCover& f) const override;

};

END_NAMESPACE_YM_LOGIC

#endif // ONELEVEL0KERNEL_H
