#ifndef DIVISOR_H
#define DIVISOR_H

/// @file Divisor.h
/// @brief Divisor のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"


BEGIN_NAMESPACE_YM_ALG

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
  AlgCover
  operator()(
    const AlgCover& f ///< [in] 対象の論理式
  ) const = 0;

};

END_NAMESPACE_YM_ALG

#endif // DIVISOR_H
