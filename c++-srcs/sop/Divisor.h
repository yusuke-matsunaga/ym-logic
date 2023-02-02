#ifndef DIVISOR_H
#define DIVISOR_H

/// @file Divisor.h
/// @brief Divisor のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/sop_nsdef.h"


BEGIN_NAMESPACE_YM_SOP

//////////////////////////////////////////////////////////////////////
/// @class Divisor Divisor.h "Divisor.h"
/// @brief 除数を求める純粋仮想基底クラス
//////////////////////////////////////////////////////////////////////
class Divisor
{
public:

  /// @brief デストラクタ
  virtual
  ~Divisor() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 除数を求める．
  /// @return 除数を表す論理式を返す．
  virtual
  SopCover
  operator()(
    const SopCover& f ///< [in] 対象の論理式
  ) const = 0;

};

END_NAMESPACE_YM_SOP

#endif // DIVISOR_H
