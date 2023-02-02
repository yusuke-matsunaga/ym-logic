#ifndef WEAKDIVISION_H
#define WEAKDIVISION_H

/// @file WeakDivision.h
/// @brief WeakDivision のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "Divide.h"


BEGIN_NAMESPACE_YM_SOP

//////////////////////////////////////////////////////////////////////
/// @class WeakDivision WeakDivision.h "WeakDivision.h"
/// @brief 'weak_division' を行う Divide の派生クラス
//////////////////////////////////////////////////////////////////////
class WeakDivision :
  public Divide
{
public:

  /// @brief コンストラクタ
  WeakDivision();

  /// @brief デストラクタ
  ~WeakDivision();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 除算を行う．
  /// @return 商q と余りr の pair を返す．
  pair<SopCover, SopCover>
  operator()(
    const SopCover& f, ///< [in] 被除数
    const SopCover& d  ///< [in] 除数
  ) const override;

};

END_NAMESPACE_YM_SOP

#endif // WEAKDIVISION_H
