#ifndef WEAKDIVISION_H
#define WEAKDIVISION_H

/// @file WeakDivision.h
/// @brief WeakDivision のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/AlgCover.h"


BEGIN_NAMESPACE_YM_ALG

//////////////////////////////////////////////////////////////////////
/// @class WeakDivision WeakDivision.h "WeakDivision.h"
/// @brief 'weak_division' を行う Divide 型のクラス
//////////////////////////////////////////////////////////////////////
class WeakDivision
{
public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 除算を行う．
  /// @return 商q と余りr の pair を返す．
  static
  pair<AlgCover, AlgCover>
  divide(
    const AlgCover& f, ///< [in] 被除数
    const AlgCover& d  ///< [in] 除数
  );

};

END_NAMESPACE_YM_ALG

#endif // WEAKDIVISION_H
