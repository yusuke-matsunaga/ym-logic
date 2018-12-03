#ifndef DIVISOR_H
#define DIVISOR_H

/// @file Divisor.h
/// @brief Divisor のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Sop.h"


BEGIN_NAMESPACE_YM_LOGIC

//////////////////////////////////////////////////////////////////////
/// @class Divisor Divisor.h "Divisor.h"
/// @brief 除数を求める純粋仮想基底クラス
//////////////////////////////////////////////////////////////////////
class Divisor
{
public:

  /// @brief デストラクタ
  virtual
  ~Divisor() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 除数を求める．
  /// @param[in] f 対象の論理式
  /// @return 除数を表す論理式を返す．
  virtual
  SopCover
  operator()(const SopCover& f) const = 0;

};

END_NAMESPACE_YM_LOGIC

#endif // DIVISOR_H
