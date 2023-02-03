#ifndef LITSETCHECK_H
#define LITSETCHECK_H

/// @file LitSetCheck.h
/// @brief LitSetCheck のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "SopMgr.h"


BEGIN_NAMESPACE_YM_SOP

//////////////////////////////////////////////////////////////////////
/// @class LitSetCheck LitSetCheck.h "ym/LitSetCheck.h"
/// @brief リテラル集合に対する要素の検索
//////////////////////////////////////////////////////////////////////
class LitSetCheck :
  public SopMgr
{
public:

  /// @brief コンストラクタ
  LitSetCheck(
    SizeType variable_num ///< [in] 変数の数
  ) : SopMgr{variable_num}
  {
  }

  /// @brief デストラクタ
  ~LitSetCheck() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素のチェック
  /// @return lit を含んでいたら true を返す．
  bool
  operator()(
    SopBitVect* bv, ///< [in] ビットベクタ
    Literal lit     ///< [in] 対象のリテラル
  );

};

END_NAMESPACE_YM_SOP

#endif // LITSETCHECK_H
