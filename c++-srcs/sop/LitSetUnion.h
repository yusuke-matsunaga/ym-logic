#ifndef LITSETUNION_H
#define LITSETUNION_H

/// @file LitSetUnion.h
/// @brief LitSetUnion のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "SopMgr.h"


BEGIN_NAMESPACE_YM_SOP

//////////////////////////////////////////////////////////////////////
/// @class LitSetUnion LitSetUnion.h "ym/LitSetUnion.h"
/// @brief リテラル集合に対するユニオン演算
//////////////////////////////////////////////////////////////////////
class LitSetUnion :
  public SopMgr
{
public:

  /// @brief コンストラクタ
  LitSetUnion(
    SizeType variable_num ///< [in] 変数の数
  ) : SopMgr{variable_num}
  {
  }

  /// @brief デストラクタ
  ~LitSetUnion() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ユニオン演算
  void
  operator()(
    SopBitVect& dst_bv,      ///< [in] 対象のビットベクタ
    const SopBitVect& src_bv ///< [in] 加えるビットベクタ
  );

};

END_NAMESPACE_YM_SOP

#endif // LITSETUNION_H
