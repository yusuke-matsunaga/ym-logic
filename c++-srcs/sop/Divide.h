#ifndef DIVIDE_H
#define DIVIDE_H

/// @file Divide.h
/// @brief Divide のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"


BEGIN_NAMESPACE_YM_SOP

//////////////////////////////////////////////////////////////////////
/// @class Divide Divide.h "Divide.h"
/// @brief 除算を行う純粋仮想基底クラス
//////////////////////////////////////////////////////////////////////
class Divide
{
public:

  /// @brief デストラクタ
  virtual
  ~Divide() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 除算を行う．
  /// @return 商q と余りr の pair を返す．
  virtual
  pair<SopCover, SopCover>
  operator()(
    const SopCover& f, ///< [in] 被除数
    const SopCover& d  ///< [in] 除数
  ) const = 0;

};

END_NAMESPACE_YM_SOP

#endif // DIVIDE_H
