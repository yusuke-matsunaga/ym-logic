#ifndef BOOLDIVISION_H
#define BOOLDIVISION_H

/// @file BoolDivision.h
/// @brief BoolDivision のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/SopCover.h"


BEGIN_NAMESPACE_YM_SOP

//////////////////////////////////////////////////////////////////////
/// @class BoolDivision BoolDivision.h "BoolDivision.h"
/// @brief 'bool_division' を行う Divide 型のクラス
//////////////////////////////////////////////////////////////////////
class BoolDivision
{
public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 除算を行う．
  /// @return 商q と余りr の pair を返す．
  static
  pair<SopCover, SopCover>
  divide(
    const SopCover& f, ///< [in] 被除数
    const SopCover& d  ///< [in] 除数
  );

};

END_NAMESPACE_YM_SOP

#endif // BOOLDIVISION_H
