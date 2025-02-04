#ifndef BESTKERNEL_H
#define BESTKERNEL_H

/// @file BestKernel.h
/// @brief BestKernel のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.

#include "Divisor.h"


BEGIN_NAMESPACE_YM_ALG

//////////////////////////////////////////////////////////////////////
/// @class BestKernel BestKernel.h "BestKernel.h"
/// @brief 'best_kernel' を求める Divisor の派生クラス
//////////////////////////////////////////////////////////////////////
class BestKernel :
  public Divisor
{
public:

  /// @brief コンストラクタ
  BestKernel();

  /// @brief デストラクタ
  ~BestKernel();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 除数を求める．
  /// @return 除数を表す論理式を返す．
  AlgCover
  operator()(
    const AlgCover& f ///< [in] 対象の論理式
  ) const override;

};

END_NAMESPACE_YM_ALG

#endif // BESTKERNEL_H
