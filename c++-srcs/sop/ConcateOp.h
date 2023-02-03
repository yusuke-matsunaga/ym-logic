#ifndef CONCATEOP_H
#define CONCATEOP_H

/// @file ConcateOp.h
/// @brief ConcateOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "SopMgr.h"
#include "SopBlock.h"


BEGIN_NAMESPACE_YM_SOP

//////////////////////////////////////////////////////////////////////
/// @class ConcateOp ConcateOp.h "ym/ConcateOp.h"
/// @brief カバーの連結を行う演算
//////////////////////////////////////////////////////////////////////
class ConcateOp :
  public SopMgr
{
public:

  /// @brief コンストラクタ
  ConcateOp(
    SizeType variable_num ///< [in] 変数の数
  ) : SopMgr{variable_num}
  {
  }

  /// @brief デストラクタ
  ~ConcateOp() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 2つのカバーを結合する．
  void
  operator()(
    SopBlock& dst,        ///< [in] 結果を格納するブロック
    const SopBlock& cov1, ///< [in] 1つめのカバー
    const SopBlock& cov2  ///< [in] 2つめのカバー
  );

};

END_NAMESPACE_YM_SOP

#endif // CONCATEOP_H
