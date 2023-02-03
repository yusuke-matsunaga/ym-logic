#ifndef CHECKINTERSECT_H
#define CHECKINTERSECT_H

/// @file CheckIntersect.h
/// @brief CheckIntersect のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "SopMgr.h"


BEGIN_NAMESPACE_YM_SOP

//////////////////////////////////////////////////////////////////////
/// @class CheckIntersect CheckIntersect.h "ym/CheckIntersect.h"
/// @brief キューブに関する演算の実装クラス
//////////////////////////////////////////////////////////////////////
class CheckIntersect :
  public SopMgr
{
public:

  /// @brief コンストラクタ
  CheckIntersect(
    SizeType variable_num ///< [in] 変数の数
  ) : SopMgr{variable_num}
  {
  }

  /// @brief デストラクタ
  ~CheckIntersect() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ２つのキューブに共通なリテラルがあれば true を返す．
  /// @return ２つのキューブに共通なリテラルがあれば true を返す．
  bool
  operator()(
    const SopBitVect* bv1, ///< [in] 1つめのキューブを表すビットベクタ
    const SopBitVect* bv2  ///< [in] 2つめのキューブを表すビットベクタ
  );

};

END_NAMESPACE_YM_SOP

#endif // CHECKINTERSECT_H
