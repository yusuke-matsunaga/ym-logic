#ifndef LITSETINTERSECT_H
#define LITSETINTERSECT_H

/// @file LitSetIntersect.h
/// @brief LitSetIntersect のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "SopMgr.h"


BEGIN_NAMESPACE_YM_SOP

//////////////////////////////////////////////////////////////////////
/// @class LitSetIntersect LitSetIntersect.h "ym/LitSetIntersect.h"
/// @brief リテラル集合とキューブの交差検査
//////////////////////////////////////////////////////////////////////
class LitSetIntersect :
  public SopMgr
{
public:

  /// @brief コンストラクタ
  LitSetIntersect(
    SizeType variable_num ///< [in] 変数の数
  ) : SopMgr{variable_num}
  {
  }

  /// @brief デストラクタ
  ~LitSetIntersect() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素のチェック
  /// @return cube と共通部分があれば true を返す．
  bool
  operator()(
    SopBitVect* bv,  ///< [in] ビットベクタ
    SopBitVect* cube ///< [in] 対象のキューブ
  );

};

END_NAMESPACE_YM_SOP

#endif // LITSETINTERSECT_H
